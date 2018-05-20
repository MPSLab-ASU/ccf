//===- SyntheticSections.cpp ----------------------------------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains linker-synthesized sections. Currently,
// synthetic sections are created either output sections or input sections,
// but we are rewriting code so that all synthetic sections are created as
// input sections.
//
//===----------------------------------------------------------------------===//

#include "SyntheticSections.h"
#include "Config.h"
#include "Error.h"
#include "InputFiles.h"
#include "LinkerScript.h"
#include "Memory.h"
#include "OutputSections.h"
#include "Strings.h"
#include "SymbolTable.h"
#include "Target.h"
#include "Threads.h"
#include "Writer.h"
#include "lld/Config/Version.h"
#include "llvm/DebugInfo/DWARF/DWARFDebugPubTable.h"
#include "llvm/Object/ELFObjectFile.h"
#include "llvm/Support/Dwarf.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/MD5.h"
#include "llvm/Support/RandomNumberGenerator.h"
#include "llvm/Support/SHA1.h"
#include "llvm/Support/xxhash.h"
#include <cstdlib>

using namespace llvm;
using namespace llvm::dwarf;
using namespace llvm::ELF;
using namespace llvm::object;
using namespace llvm::support;
using namespace llvm::support::endian;

using namespace lld;
using namespace lld::elf;

uint64_t SyntheticSection::getVA() const {
  if (this->OutSec)
    return this->OutSec->Addr + this->OutSecOff;
  return 0;
}

template <class ELFT> static std::vector<DefinedCommon *> getCommonSymbols() {
  std::vector<DefinedCommon *> V;
  for (Symbol *S : Symtab<ELFT>::X->getSymbols())
    if (auto *B = dyn_cast<DefinedCommon>(S->body()))
      V.push_back(B);
  return V;
}

// Find all common symbols and allocate space for them.
template <class ELFT> InputSection *elf::createCommonSection() {
  auto *Ret = make<InputSection>(SHF_ALLOC | SHF_WRITE, SHT_NOBITS, 1,
                                 ArrayRef<uint8_t>(), "COMMON");
  Ret->Live = true;

  if (!Config->DefineCommon)
    return Ret;

  // Sort the common symbols by alignment as an heuristic to pack them better.
  std::vector<DefinedCommon *> Syms = getCommonSymbols<ELFT>();
  std::stable_sort(Syms.begin(), Syms.end(),
                   [](const DefinedCommon *A, const DefinedCommon *B) {
                     return A->Alignment > B->Alignment;
                   });

  // Assign offsets to symbols.
  size_t Size = 0;
  size_t Alignment = 1;
  for (DefinedCommon *Sym : Syms) {
    Alignment = std::max<size_t>(Alignment, Sym->Alignment);
    Size = alignTo(Size, Sym->Alignment);

    // Compute symbol offset relative to beginning of input section.
    Sym->Offset = Size;
    Size += Sym->Size;
  }
  Ret->Alignment = Alignment;
  Ret->Data = makeArrayRef<uint8_t>(nullptr, Size);
  return Ret;
}

// Returns an LLD version string.
static ArrayRef<uint8_t> getVersion() {
  // Check LLD_VERSION first for ease of testing.
  // You can get consitent output by using the environment variable.
  // This is only for testing.
  StringRef S = getenv("LLD_VERSION");
  if (S.empty())
    S = Saver.save(Twine("Linker: ") + getLLDVersion());

  // +1 to include the terminating '\0'.
  return {(const uint8_t *)S.data(), S.size() + 1};
}

// Creates a .comment section containing LLD version info.
// With this feature, you can identify LLD-generated binaries easily
// by "objdump -s -j .comment <file>".
// The returned object is a mergeable string section.
template <class ELFT> MergeInputSection<ELFT> *elf::createCommentSection() {
  typename ELFT::Shdr Hdr = {};
  Hdr.sh_flags = SHF_MERGE | SHF_STRINGS;
  Hdr.sh_type = SHT_PROGBITS;
  Hdr.sh_entsize = 1;
  Hdr.sh_addralign = 1;

  auto *Ret = make<MergeInputSection<ELFT>>(/*file=*/nullptr, &Hdr, ".comment");
  Ret->Data = getVersion();
  Ret->splitIntoPieces();
  return Ret;
}

// .MIPS.abiflags section.
template <class ELFT>
MipsAbiFlagsSection<ELFT>::MipsAbiFlagsSection(Elf_Mips_ABIFlags Flags)
    : SyntheticSection(SHF_ALLOC, SHT_MIPS_ABIFLAGS, 8, ".MIPS.abiflags"),
      Flags(Flags) {
  this->Entsize = sizeof(Elf_Mips_ABIFlags);
}

template <class ELFT> void MipsAbiFlagsSection<ELFT>::writeTo(uint8_t *Buf) {
  memcpy(Buf, &Flags, sizeof(Flags));
}

template <class ELFT>
MipsAbiFlagsSection<ELFT> *MipsAbiFlagsSection<ELFT>::create() {
  Elf_Mips_ABIFlags Flags = {};
  bool Create = false;

  for (InputSectionBase *Sec : InputSections) {
    if (!Sec->Live || Sec->Type != SHT_MIPS_ABIFLAGS)
      continue;
    Sec->Live = false;
    Create = true;

    std::string Filename = toString(Sec->getFile<ELFT>());
    const size_t Size = Sec->Data.size();
    // Older version of BFD (such as the default FreeBSD linker) concatenate
    // .MIPS.abiflags instead of merging. To allow for this case (or potential
    // zero padding) we ignore everything after the first Elf_Mips_ABIFlags
    if (Size < sizeof(Elf_Mips_ABIFlags)) {
      error(Filename + ": invalid size of .MIPS.abiflags section: got " +
            Twine(Size) + " instead of " + Twine(sizeof(Elf_Mips_ABIFlags)));
      return nullptr;
    }
    auto *S = reinterpret_cast<const Elf_Mips_ABIFlags *>(Sec->Data.data());
    if (S->version != 0) {
      error(Filename + ": unexpected .MIPS.abiflags version " +
            Twine(S->version));
      return nullptr;
    }

    // LLD checks ISA compatibility in getMipsEFlags(). Here we just
    // select the highest number of ISA/Rev/Ext.
    Flags.isa_level = std::max(Flags.isa_level, S->isa_level);
    Flags.isa_rev = std::max(Flags.isa_rev, S->isa_rev);
    Flags.isa_ext = std::max(Flags.isa_ext, S->isa_ext);
    Flags.gpr_size = std::max(Flags.gpr_size, S->gpr_size);
    Flags.cpr1_size = std::max(Flags.cpr1_size, S->cpr1_size);
    Flags.cpr2_size = std::max(Flags.cpr2_size, S->cpr2_size);
    Flags.ases |= S->ases;
    Flags.flags1 |= S->flags1;
    Flags.flags2 |= S->flags2;
    Flags.fp_abi = elf::getMipsFpAbiFlag(Flags.fp_abi, S->fp_abi, Filename);
  };

  if (Create)
    return make<MipsAbiFlagsSection<ELFT>>(Flags);
  return nullptr;
}

// .MIPS.options section.
template <class ELFT>
MipsOptionsSection<ELFT>::MipsOptionsSection(Elf_Mips_RegInfo Reginfo)
    : SyntheticSection(SHF_ALLOC, SHT_MIPS_OPTIONS, 8, ".MIPS.options"),
      Reginfo(Reginfo) {
  this->Entsize = sizeof(Elf_Mips_Options) + sizeof(Elf_Mips_RegInfo);
}

template <class ELFT> void MipsOptionsSection<ELFT>::writeTo(uint8_t *Buf) {
  auto *Options = reinterpret_cast<Elf_Mips_Options *>(Buf);
  Options->kind = ODK_REGINFO;
  Options->size = getSize();

  if (!Config->Relocatable)
    Reginfo.ri_gp_value = In<ELFT>::MipsGot->getGp();
  memcpy(Buf + sizeof(Elf_Mips_Options), &Reginfo, sizeof(Reginfo));
}

template <class ELFT>
MipsOptionsSection<ELFT> *MipsOptionsSection<ELFT>::create() {
  // N64 ABI only.
  if (!ELFT::Is64Bits)
    return nullptr;

  Elf_Mips_RegInfo Reginfo = {};
  bool Create = false;

  for (InputSectionBase *Sec : InputSections) {
    if (!Sec->Live || Sec->Type != SHT_MIPS_OPTIONS)
      continue;
    Sec->Live = false;
    Create = true;

    std::string Filename = toString(Sec->getFile<ELFT>());
    ArrayRef<uint8_t> D = Sec->Data;

    while (!D.empty()) {
      if (D.size() < sizeof(Elf_Mips_Options)) {
        error(Filename + ": invalid size of .MIPS.options section");
        break;
      }

      auto *Opt = reinterpret_cast<const Elf_Mips_Options *>(D.data());
      if (Opt->kind == ODK_REGINFO) {
        if (Config->Relocatable && Opt->getRegInfo().ri_gp_value)
          error(Filename + ": unsupported non-zero ri_gp_value");
        Reginfo.ri_gprmask |= Opt->getRegInfo().ri_gprmask;
        Sec->getFile<ELFT>()->MipsGp0 = Opt->getRegInfo().ri_gp_value;
        break;
      }

      if (!Opt->size)
        fatal(Filename + ": zero option descriptor size");
      D = D.slice(Opt->size);
    }
  };

  if (Create)
    return make<MipsOptionsSection<ELFT>>(Reginfo);
  return nullptr;
}

// MIPS .reginfo section.
template <class ELFT>
MipsReginfoSection<ELFT>::MipsReginfoSection(Elf_Mips_RegInfo Reginfo)
    : SyntheticSection(SHF_ALLOC, SHT_MIPS_REGINFO, 4, ".reginfo"),
      Reginfo(Reginfo) {
  this->Entsize = sizeof(Elf_Mips_RegInfo);
}

template <class ELFT> void MipsReginfoSection<ELFT>::writeTo(uint8_t *Buf) {
  if (!Config->Relocatable)
    Reginfo.ri_gp_value = In<ELFT>::MipsGot->getGp();
  memcpy(Buf, &Reginfo, sizeof(Reginfo));
}

template <class ELFT>
MipsReginfoSection<ELFT> *MipsReginfoSection<ELFT>::create() {
  // Section should be alive for O32 and N32 ABIs only.
  if (ELFT::Is64Bits)
    return nullptr;

  Elf_Mips_RegInfo Reginfo = {};
  bool Create = false;

  for (InputSectionBase *Sec : InputSections) {
    if (!Sec->Live || Sec->Type != SHT_MIPS_REGINFO)
      continue;
    Sec->Live = false;
    Create = true;

    if (Sec->Data.size() != sizeof(Elf_Mips_RegInfo)) {
      error(toString(Sec->getFile<ELFT>()) +
            ": invalid size of .reginfo section");
      return nullptr;
    }
    auto *R = reinterpret_cast<const Elf_Mips_RegInfo *>(Sec->Data.data());
    if (Config->Relocatable && R->ri_gp_value)
      error(toString(Sec->getFile<ELFT>()) +
            ": unsupported non-zero ri_gp_value");

    Reginfo.ri_gprmask |= R->ri_gprmask;
    Sec->getFile<ELFT>()->MipsGp0 = R->ri_gp_value;
  };

  if (Create)
    return make<MipsReginfoSection<ELFT>>(Reginfo);
  return nullptr;
}

InputSection *elf::createInterpSection() {
  // StringSaver guarantees that the returned string ends with '\0'.
  StringRef S = Saver.save(Config->DynamicLinker);
  ArrayRef<uint8_t> Contents = {(const uint8_t *)S.data(), S.size() + 1};

  auto *Sec =
      make<InputSection>(SHF_ALLOC, SHT_PROGBITS, 1, Contents, ".interp");
  Sec->Live = true;
  return Sec;
}

template <class ELFT>
SymbolBody *elf::addSyntheticLocal(StringRef Name, uint8_t Type, uint64_t Value,
                                   uint64_t Size, InputSectionBase *Section) {
  auto *S = make<DefinedRegular>(Name, /*IsLocal*/ true, STV_DEFAULT, Type,
                                 Value, Size, Section, nullptr);
  if (In<ELFT>::SymTab)
    In<ELFT>::SymTab->addSymbol(S);
  return S;
}

static size_t getHashSize() {
  switch (Config->BuildId) {
  case BuildIdKind::Fast:
    return 8;
  case BuildIdKind::Md5:
  case BuildIdKind::Uuid:
    return 16;
  case BuildIdKind::Sha1:
    return 20;
  case BuildIdKind::Hexstring:
    return Config->BuildIdVector.size();
  default:
    llvm_unreachable("unknown BuildIdKind");
  }
}

template <class ELFT>
BuildIdSection<ELFT>::BuildIdSection()
    : SyntheticSection(SHF_ALLOC, SHT_NOTE, 1, ".note.gnu.build-id"),
      HashSize(getHashSize()) {}

template <class ELFT> void BuildIdSection<ELFT>::writeTo(uint8_t *Buf) {
  const endianness E = ELFT::TargetEndianness;
  write32<E>(Buf, 4);                   // Name size
  write32<E>(Buf + 4, HashSize);        // Content size
  write32<E>(Buf + 8, NT_GNU_BUILD_ID); // Type
  memcpy(Buf + 12, "GNU", 4);           // Name string
  HashBuf = Buf + 16;
}

// Split one uint8 array into small pieces of uint8 arrays.
static std::vector<ArrayRef<uint8_t>> split(ArrayRef<uint8_t> Arr,
                                            size_t ChunkSize) {
  std::vector<ArrayRef<uint8_t>> Ret;
  while (Arr.size() > ChunkSize) {
    Ret.push_back(Arr.take_front(ChunkSize));
    Arr = Arr.drop_front(ChunkSize);
  }
  if (!Arr.empty())
    Ret.push_back(Arr);
  return Ret;
}

// Computes a hash value of Data using a given hash function.
// In order to utilize multiple cores, we first split data into 1MB
// chunks, compute a hash for each chunk, and then compute a hash value
// of the hash values.
template <class ELFT>
void BuildIdSection<ELFT>::computeHash(
    llvm::ArrayRef<uint8_t> Data,
    std::function<void(uint8_t *Dest, ArrayRef<uint8_t> Arr)> HashFn) {
  std::vector<ArrayRef<uint8_t>> Chunks = split(Data, 1024 * 1024);
  std::vector<uint8_t> Hashes(Chunks.size() * HashSize);

  // Compute hash values.
  forLoop(0, Chunks.size(),
          [&](size_t I) { HashFn(Hashes.data() + I * HashSize, Chunks[I]); });

  // Write to the final output buffer.
  HashFn(HashBuf, Hashes);
}

template <class ELFT>
CopyRelSection<ELFT>::CopyRelSection(bool ReadOnly, uintX_t AddrAlign, size_t S)
    : SyntheticSection(SHF_ALLOC, SHT_NOBITS, AddrAlign,
                       ReadOnly ? ".bss.rel.ro" : ".bss"),
      Size(S) {}

template <class ELFT>
void BuildIdSection<ELFT>::writeBuildId(ArrayRef<uint8_t> Buf) {
  switch (Config->BuildId) {
  case BuildIdKind::Fast:
    computeHash(Buf, [](uint8_t *Dest, ArrayRef<uint8_t> Arr) {
      write64le(Dest, xxHash64(toStringRef(Arr)));
    });
    break;
  case BuildIdKind::Md5:
    computeHash(Buf, [](uint8_t *Dest, ArrayRef<uint8_t> Arr) {
      memcpy(Dest, MD5::hash(Arr).data(), 16);
    });
    break;
  case BuildIdKind::Sha1:
    computeHash(Buf, [](uint8_t *Dest, ArrayRef<uint8_t> Arr) {
      memcpy(Dest, SHA1::hash(Arr).data(), 20);
    });
    break;
  case BuildIdKind::Uuid:
    if (getRandomBytes(HashBuf, HashSize))
      error("entropy source failure");
    break;
  case BuildIdKind::Hexstring:
    memcpy(HashBuf, Config->BuildIdVector.data(), Config->BuildIdVector.size());
    break;
  default:
    llvm_unreachable("unknown BuildIdKind");
  }
}

template <class ELFT>
EhFrameSection<ELFT>::EhFrameSection()
    : SyntheticSection(SHF_ALLOC, SHT_PROGBITS, 1, ".eh_frame") {}

// Search for an existing CIE record or create a new one.
// CIE records from input object files are uniquified by their contents
// and where their relocations point to.
template <class ELFT>
template <class RelTy>
CieRecord *EhFrameSection<ELFT>::addCie(EhSectionPiece &Piece,
                                        ArrayRef<RelTy> Rels) {
  auto *Sec = cast<EhInputSection<ELFT>>(Piece.ID);
  const endianness E = ELFT::TargetEndianness;
  if (read32<E>(Piece.data().data() + 4) != 0)
    fatal(toString(Sec) + ": CIE expected at beginning of .eh_frame");

  SymbolBody *Personality = nullptr;
  unsigned FirstRelI = Piece.FirstRelocation;
  if (FirstRelI != (unsigned)-1)
    Personality =
        &Sec->template getFile<ELFT>()->getRelocTargetSym(Rels[FirstRelI]);

  // Search for an existing CIE by CIE contents/relocation target pair.
  CieRecord *Cie = &CieMap[{Piece.data(), Personality}];

  // If not found, create a new one.
  if (Cie->Piece == nullptr) {
    Cie->Piece = &Piece;
    Cies.push_back(Cie);
  }
  return Cie;
}

// There is one FDE per function. Returns true if a given FDE
// points to a live function.
template <class ELFT>
template <class RelTy>
bool EhFrameSection<ELFT>::isFdeLive(EhSectionPiece &Piece,
                                     ArrayRef<RelTy> Rels) {
  auto *Sec = cast<EhInputSection<ELFT>>(Piece.ID);
  unsigned FirstRelI = Piece.FirstRelocation;
  if (FirstRelI == (unsigned)-1)
    return false;
  const RelTy &Rel = Rels[FirstRelI];
  SymbolBody &B = Sec->template getFile<ELFT>()->getRelocTargetSym(Rel);
  auto *D = dyn_cast<DefinedRegular>(&B);
  if (!D || !D->Section)
    return false;
  InputSectionBase *Target = D->Section->Repl;
  return Target && Target->Live;
}

// .eh_frame is a sequence of CIE or FDE records. In general, there
// is one CIE record per input object file which is followed by
// a list of FDEs. This function searches an existing CIE or create a new
// one and associates FDEs to the CIE.
template <class ELFT>
template <class RelTy>
void EhFrameSection<ELFT>::addSectionAux(EhInputSection<ELFT> *Sec,
                                         ArrayRef<RelTy> Rels) {
  const endianness E = ELFT::TargetEndianness;

  DenseMap<size_t, CieRecord *> OffsetToCie;
  for (EhSectionPiece &Piece : Sec->Pieces) {
    // The empty record is the end marker.
    if (Piece.size() == 4)
      return;

    size_t Offset = Piece.InputOff;
    uint32_t ID = read32<E>(Piece.data().data() + 4);
    if (ID == 0) {
      OffsetToCie[Offset] = addCie(Piece, Rels);
      continue;
    }

    uint32_t CieOffset = Offset + 4 - ID;
    CieRecord *Cie = OffsetToCie[CieOffset];
    if (!Cie)
      fatal(toString(Sec) + ": invalid CIE reference");

    if (!isFdeLive(Piece, Rels))
      continue;
    Cie->FdePieces.push_back(&Piece);
    NumFdes++;
  }
}

template <class ELFT>
void EhFrameSection<ELFT>::addSection(InputSectionBase *C) {
  auto *Sec = cast<EhInputSection<ELFT>>(C);
  Sec->EHSec = this;
  updateAlignment(Sec->Alignment);
  Sections.push_back(Sec);

  // .eh_frame is a sequence of CIE or FDE records. This function
  // splits it into pieces so that we can call
  // SplitInputSection::getSectionPiece on the section.
  Sec->split();
  if (Sec->Pieces.empty())
    return;

  if (Sec->NumRelocations) {
    if (Sec->AreRelocsRela)
      addSectionAux(Sec, Sec->template relas<ELFT>());
    else
      addSectionAux(Sec, Sec->template rels<ELFT>());
    return;
  }
  addSectionAux(Sec, makeArrayRef<Elf_Rela>(nullptr, nullptr));
}

template <class ELFT>
static void writeCieFde(uint8_t *Buf, ArrayRef<uint8_t> D) {
  memcpy(Buf, D.data(), D.size());

  // Fix the size field. -4 since size does not include the size field itself.
  const endianness E = ELFT::TargetEndianness;
  write32<E>(Buf, alignTo(D.size(), sizeof(typename ELFT::uint)) - 4);
}

template <class ELFT> void EhFrameSection<ELFT>::finalizeContents() {
  if (this->Size)
    return; // Already finalized.

  size_t Off = 0;
  for (CieRecord *Cie : Cies) {
    Cie->Piece->OutputOff = Off;
    Off += alignTo(Cie->Piece->size(), sizeof(uintX_t));

    for (EhSectionPiece *Fde : Cie->FdePieces) {
      Fde->OutputOff = Off;
      Off += alignTo(Fde->size(), sizeof(uintX_t));
    }
  }
  this->Size = Off;
}

template <class ELFT> static uint64_t readFdeAddr(uint8_t *Buf, int Size) {
  const endianness E = ELFT::TargetEndianness;
  switch (Size) {
  case DW_EH_PE_udata2:
    return read16<E>(Buf);
  case DW_EH_PE_udata4:
    return read32<E>(Buf);
  case DW_EH_PE_udata8:
    return read64<E>(Buf);
  case DW_EH_PE_absptr:
    if (ELFT::Is64Bits)
      return read64<E>(Buf);
    return read32<E>(Buf);
  }
  fatal("unknown FDE size encoding");
}

// Returns the VA to which a given FDE (on a mmap'ed buffer) is applied to.
// We need it to create .eh_frame_hdr section.
template <class ELFT>
typename ELFT::uint EhFrameSection<ELFT>::getFdePc(uint8_t *Buf, size_t FdeOff,
                                                   uint8_t Enc) {
  // The starting address to which this FDE applies is
  // stored at FDE + 8 byte.
  size_t Off = FdeOff + 8;
  uint64_t Addr = readFdeAddr<ELFT>(Buf + Off, Enc & 0x7);
  if ((Enc & 0x70) == DW_EH_PE_absptr)
    return Addr;
  if ((Enc & 0x70) == DW_EH_PE_pcrel)
    return Addr + this->OutSec->Addr + Off;
  fatal("unknown FDE size relative encoding");
}

template <class ELFT> void EhFrameSection<ELFT>::writeTo(uint8_t *Buf) {
  const endianness E = ELFT::TargetEndianness;
  for (CieRecord *Cie : Cies) {
    size_t CieOffset = Cie->Piece->OutputOff;
    writeCieFde<ELFT>(Buf + CieOffset, Cie->Piece->data());

    for (EhSectionPiece *Fde : Cie->FdePieces) {
      size_t Off = Fde->OutputOff;
      writeCieFde<ELFT>(Buf + Off, Fde->data());

      // FDE's second word should have the offset to an associated CIE.
      // Write it.
      write32<E>(Buf + Off + 4, Off + 4 - CieOffset);
    }
  }

  for (EhInputSection<ELFT> *S : Sections)
    S->template relocate<ELFT>(Buf, nullptr);

  // Construct .eh_frame_hdr. .eh_frame_hdr is a binary search table
  // to get a FDE from an address to which FDE is applied. So here
  // we obtain two addresses and pass them to EhFrameHdr object.
  if (In<ELFT>::EhFrameHdr) {
    for (CieRecord *Cie : Cies) {
      uint8_t Enc = getFdeEncoding<ELFT>(Cie->Piece);
      for (SectionPiece *Fde : Cie->FdePieces) {
        uintX_t Pc = getFdePc(Buf, Fde->OutputOff, Enc);
        uintX_t FdeVA = this->OutSec->Addr + Fde->OutputOff;
        In<ELFT>::EhFrameHdr->addFde(Pc, FdeVA);
      }
    }
  }
}

template <class ELFT>
GotSection<ELFT>::GotSection()
    : SyntheticSection(SHF_ALLOC | SHF_WRITE, SHT_PROGBITS,
                       Target->GotEntrySize, ".got") {}

template <class ELFT> void GotSection<ELFT>::addEntry(SymbolBody &Sym) {
  Sym.GotIndex = NumEntries;
  ++NumEntries;
}

template <class ELFT> bool GotSection<ELFT>::addDynTlsEntry(SymbolBody &Sym) {
  if (Sym.GlobalDynIndex != -1U)
    return false;
  Sym.GlobalDynIndex = NumEntries;
  // Global Dynamic TLS entries take two GOT slots.
  NumEntries += 2;
  return true;
}

// Reserves TLS entries for a TLS module ID and a TLS block offset.
// In total it takes two GOT slots.
template <class ELFT> bool GotSection<ELFT>::addTlsIndex() {
  if (TlsIndexOff != uint32_t(-1))
    return false;
  TlsIndexOff = NumEntries * sizeof(uintX_t);
  NumEntries += 2;
  return true;
}

template <class ELFT>
typename GotSection<ELFT>::uintX_t
GotSection<ELFT>::getGlobalDynAddr(const SymbolBody &B) const {
  return this->getVA() + B.GlobalDynIndex * sizeof(uintX_t);
}

template <class ELFT>
typename GotSection<ELFT>::uintX_t
GotSection<ELFT>::getGlobalDynOffset(const SymbolBody &B) const {
  return B.GlobalDynIndex * sizeof(uintX_t);
}

template <class ELFT> void GotSection<ELFT>::finalizeContents() {
  Size = NumEntries * sizeof(uintX_t);
}

template <class ELFT> bool GotSection<ELFT>::empty() const {
  // If we have a relocation that is relative to GOT (such as GOTOFFREL),
  // we need to emit a GOT even if it's empty.
  return NumEntries == 0 && !HasGotOffRel;
}

template <class ELFT> void GotSection<ELFT>::writeTo(uint8_t *Buf) {
  this->template relocate<ELFT>(Buf, Buf + Size);
}

template <class ELFT>
MipsGotSection<ELFT>::MipsGotSection()
    : SyntheticSection(SHF_ALLOC | SHF_WRITE | SHF_MIPS_GPREL, SHT_PROGBITS, 16,
                       ".got") {}

template <class ELFT>
void MipsGotSection<ELFT>::addEntry(SymbolBody &Sym, int64_t Addend,
                                    RelExpr Expr) {
  // For "true" local symbols which can be referenced from the same module
  // only compiler creates two instructions for address loading:
  //
  // lw   $8, 0($gp) # R_MIPS_GOT16
  // addi $8, $8, 0  # R_MIPS_LO16
  //
  // The first instruction loads high 16 bits of the symbol address while
  // the second adds an offset. That allows to reduce number of required
  // GOT entries because only one global offset table entry is necessary
  // for every 64 KBytes of local data. So for local symbols we need to
  // allocate number of GOT entries to hold all required "page" addresses.
  //
  // All global symbols (hidden and regular) considered by compiler uniformly.
  // It always generates a single `lw` instruction and R_MIPS_GOT16 relocation
  // to load address of the symbol. So for each such symbol we need to
  // allocate dedicated GOT entry to store its address.
  //
  // If a symbol is preemptible we need help of dynamic linker to get its
  // final address. The corresponding GOT entries are allocated in the
  // "global" part of GOT. Entries for non preemptible global symbol allocated
  // in the "local" part of GOT.
  //
  // See "Global Offset Table" in Chapter 5:
  // ftp://www.linux-mips.org/pub/linux/mips/doc/ABI/mipsabi.pdf
  if (Expr == R_MIPS_GOT_LOCAL_PAGE) {
    // At this point we do not know final symbol value so to reduce number
    // of allocated GOT entries do the following trick. Save all output
    // sections referenced by GOT relocations. Then later in the `finalize`
    // method calculate number of "pages" required to cover all saved output
    // section and allocate appropriate number of GOT entries.
    auto *DefSym = cast<DefinedRegular>(&Sym);
    PageIndexMap.insert(
        {DefSym->Section->template getOutputSection<ELFT>(), 0});
    return;
  }
  if (Sym.isTls()) {
    // GOT entries created for MIPS TLS relocations behave like
    // almost GOT entries from other ABIs. They go to the end
    // of the global offset table.
    Sym.GotIndex = TlsEntries.size();
    TlsEntries.push_back(&Sym);
    return;
  }
  auto AddEntry = [&](SymbolBody &S, uintX_t A, GotEntries &Items) {
    if (S.isInGot() && !A)
      return;
    size_t NewIndex = Items.size();
    if (!EntryIndexMap.insert({{&S, A}, NewIndex}).second)
      return;
    Items.emplace_back(&S, A);
    if (!A)
      S.GotIndex = NewIndex;
  };
  if (Sym.isPreemptible()) {
    // Ignore addends for preemptible symbols. They got single GOT entry anyway.
    AddEntry(Sym, 0, GlobalEntries);
    Sym.IsInGlobalMipsGot = true;
  } else if (Expr == R_MIPS_GOT_OFF32) {
    AddEntry(Sym, Addend, LocalEntries32);
    Sym.Is32BitMipsGot = true;
  } else {
    // Hold local GOT entries accessed via a 16-bit index separately.
    // That allows to write them in the beginning of the GOT and keep
    // their indexes as less as possible to escape relocation's overflow.
    AddEntry(Sym, Addend, LocalEntries);
  }
}

template <class ELFT>
bool MipsGotSection<ELFT>::addDynTlsEntry(SymbolBody &Sym) {
  if (Sym.GlobalDynIndex != -1U)
    return false;
  Sym.GlobalDynIndex = TlsEntries.size();
  // Global Dynamic TLS entries take two GOT slots.
  TlsEntries.push_back(nullptr);
  TlsEntries.push_back(&Sym);
  return true;
}

// Reserves TLS entries for a TLS module ID and a TLS block offset.
// In total it takes two GOT slots.
template <class ELFT> bool MipsGotSection<ELFT>::addTlsIndex() {
  if (TlsIndexOff != uint32_t(-1))
    return false;
  TlsIndexOff = TlsEntries.size() * sizeof(uintX_t);
  TlsEntries.push_back(nullptr);
  TlsEntries.push_back(nullptr);
  return true;
}

static uint64_t getMipsPageAddr(uint64_t Addr) {
  return (Addr + 0x8000) & ~0xffff;
}

static uint64_t getMipsPageCount(uint64_t Size) {
  return (Size + 0xfffe) / 0xffff + 1;
}

template <class ELFT>
typename MipsGotSection<ELFT>::uintX_t
MipsGotSection<ELFT>::getPageEntryOffset(const SymbolBody &B,
                                         int64_t Addend) const {
  const OutputSection *OutSec =
      cast<DefinedRegular>(&B)->Section->template getOutputSection<ELFT>();
  uintX_t SecAddr = getMipsPageAddr(OutSec->Addr);
  uintX_t SymAddr = getMipsPageAddr(B.getVA<ELFT>(Addend));
  uintX_t Index = PageIndexMap.lookup(OutSec) + (SymAddr - SecAddr) / 0xffff;
  assert(Index < PageEntriesNum);
  return (HeaderEntriesNum + Index) * sizeof(uintX_t);
}

template <class ELFT>
typename MipsGotSection<ELFT>::uintX_t
MipsGotSection<ELFT>::getBodyEntryOffset(const SymbolBody &B,
                                         int64_t Addend) const {
  // Calculate offset of the GOT entries block: TLS, global, local.
  uintX_t Index = HeaderEntriesNum + PageEntriesNum;
  if (B.isTls())
    Index += LocalEntries.size() + LocalEntries32.size() + GlobalEntries.size();
  else if (B.IsInGlobalMipsGot)
    Index += LocalEntries.size() + LocalEntries32.size();
  else if (B.Is32BitMipsGot)
    Index += LocalEntries.size();
  // Calculate offset of the GOT entry in the block.
  if (B.isInGot())
    Index += B.GotIndex;
  else {
    auto It = EntryIndexMap.find({&B, Addend});
    assert(It != EntryIndexMap.end());
    Index += It->second;
  }
  return Index * sizeof(uintX_t);
}

template <class ELFT>
typename MipsGotSection<ELFT>::uintX_t
MipsGotSection<ELFT>::getTlsOffset() const {
  return (getLocalEntriesNum() + GlobalEntries.size()) * sizeof(uintX_t);
}

template <class ELFT>
typename MipsGotSection<ELFT>::uintX_t
MipsGotSection<ELFT>::getGlobalDynOffset(const SymbolBody &B) const {
  return B.GlobalDynIndex * sizeof(uintX_t);
}

template <class ELFT>
const SymbolBody *MipsGotSection<ELFT>::getFirstGlobalEntry() const {
  return GlobalEntries.empty() ? nullptr : GlobalEntries.front().first;
}

template <class ELFT>
unsigned MipsGotSection<ELFT>::getLocalEntriesNum() const {
  return HeaderEntriesNum + PageEntriesNum + LocalEntries.size() +
         LocalEntries32.size();
}

template <class ELFT> void MipsGotSection<ELFT>::finalizeContents() {
  PageEntriesNum = 0;
  for (std::pair<const OutputSection *, size_t> &P : PageIndexMap) {
    // For each output section referenced by GOT page relocations calculate
    // and save into PageIndexMap an upper bound of MIPS GOT entries required
    // to store page addresses of local symbols. We assume the worst case -
    // each 64kb page of the output section has at least one GOT relocation
    // against it. And take in account the case when the section intersects
    // page boundaries.
    P.second = PageEntriesNum;
    PageEntriesNum += getMipsPageCount(P.first->Size);
  }
  Size = (getLocalEntriesNum() + GlobalEntries.size() + TlsEntries.size()) *
         sizeof(uintX_t);
}

template <class ELFT> bool MipsGotSection<ELFT>::empty() const {
  // We add the .got section to the result for dynamic MIPS target because
  // its address and properties are mentioned in the .dynamic section.
  return Config->Relocatable;
}

template <class ELFT>
typename MipsGotSection<ELFT>::uintX_t MipsGotSection<ELFT>::getGp() const {
  return ElfSym::MipsGp->template getVA<ELFT>(0);
}

template <class ELFT>
static void writeUint(uint8_t *Buf, typename ELFT::uint Val) {
  typedef typename ELFT::uint uintX_t;
  write<uintX_t, ELFT::TargetEndianness, sizeof(uintX_t)>(Buf, Val);
}

template <class ELFT> void MipsGotSection<ELFT>::writeTo(uint8_t *Buf) {
  // Set the MSB of the second GOT slot. This is not required by any
  // MIPS ABI documentation, though.
  //
  // There is a comment in glibc saying that "The MSB of got[1] of a
  // gnu object is set to identify gnu objects," and in GNU gold it
  // says "the second entry will be used by some runtime loaders".
  // But how this field is being used is unclear.
  //
  // We are not really willing to mimic other linkers behaviors
  // without understanding why they do that, but because all files
  // generated by GNU tools have this special GOT value, and because
  // we've been doing this for years, it is probably a safe bet to
  // keep doing this for now. We really need to revisit this to see
  // if we had to do this.
  auto *P = reinterpret_cast<typename ELFT::Off *>(Buf);
  P[1] = uintX_t(1) << (ELFT::Is64Bits ? 63 : 31);
  Buf += HeaderEntriesNum * sizeof(uintX_t);
  // Write 'page address' entries to the local part of the GOT.
  for (std::pair<const OutputSection *, size_t> &L : PageIndexMap) {
    size_t PageCount = getMipsPageCount(L.first->Size);
    uintX_t FirstPageAddr = getMipsPageAddr(L.first->Addr);
    for (size_t PI = 0; PI < PageCount; ++PI) {
      uint8_t *Entry = Buf + (L.second + PI) * sizeof(uintX_t);
      writeUint<ELFT>(Entry, FirstPageAddr + PI * 0x10000);
    }
  }
  Buf += PageEntriesNum * sizeof(uintX_t);
  auto AddEntry = [&](const GotEntry &SA) {
    uint8_t *Entry = Buf;
    Buf += sizeof(uintX_t);
    const SymbolBody *Body = SA.first;
    uintX_t VA = Body->template getVA<ELFT>(SA.second);
    writeUint<ELFT>(Entry, VA);
  };
  std::for_each(std::begin(LocalEntries), std::end(LocalEntries), AddEntry);
  std::for_each(std::begin(LocalEntries32), std::end(LocalEntries32), AddEntry);
  std::for_each(std::begin(GlobalEntries), std::end(GlobalEntries), AddEntry);
  // Initialize TLS-related GOT entries. If the entry has a corresponding
  // dynamic relocations, leave it initialized by zero. Write down adjusted
  // TLS symbol's values otherwise. To calculate the adjustments use offsets
  // for thread-local storage.
  // https://www.linux-mips.org/wiki/NPTL
  if (TlsIndexOff != -1U && !Config->pic())
    writeUint<ELFT>(Buf + TlsIndexOff, 1);
  for (const SymbolBody *B : TlsEntries) {
    if (!B || B->isPreemptible())
      continue;
    uintX_t VA = B->getVA<ELFT>();
    if (B->GotIndex != -1U) {
      uint8_t *Entry = Buf + B->GotIndex * sizeof(uintX_t);
      writeUint<ELFT>(Entry, VA - 0x7000);
    }
    if (B->GlobalDynIndex != -1U) {
      uint8_t *Entry = Buf + B->GlobalDynIndex * sizeof(uintX_t);
      writeUint<ELFT>(Entry, 1);
      Entry += sizeof(uintX_t);
      writeUint<ELFT>(Entry, VA - 0x8000);
    }
  }
}

template <class ELFT>
GotPltSection<ELFT>::GotPltSection()
    : SyntheticSection(SHF_ALLOC | SHF_WRITE, SHT_PROGBITS,
                       Target->GotPltEntrySize, ".got.plt") {}

template <class ELFT> void GotPltSection<ELFT>::addEntry(SymbolBody &Sym) {
  Sym.GotPltIndex = Target->GotPltHeaderEntriesNum + Entries.size();
  Entries.push_back(&Sym);
}

template <class ELFT> size_t GotPltSection<ELFT>::getSize() const {
  return (Target->GotPltHeaderEntriesNum + Entries.size()) *
         Target->GotPltEntrySize;
}

template <class ELFT> void GotPltSection<ELFT>::writeTo(uint8_t *Buf) {
  Target->writeGotPltHeader(Buf);
  Buf += Target->GotPltHeaderEntriesNum * Target->GotPltEntrySize;
  for (const SymbolBody *B : Entries) {
    Target->writeGotPlt(Buf, *B);
    Buf += sizeof(uintX_t);
  }
}

// On ARM the IgotPltSection is part of the GotSection, on other Targets it is
// part of the .got.plt
template <class ELFT>
IgotPltSection<ELFT>::IgotPltSection()
    : SyntheticSection(SHF_ALLOC | SHF_WRITE, SHT_PROGBITS,
                       Target->GotPltEntrySize,
                       Config->EMachine == EM_ARM ? ".got" : ".got.plt") {}

template <class ELFT> void IgotPltSection<ELFT>::addEntry(SymbolBody &Sym) {
  Sym.IsInIgot = true;
  Sym.GotPltIndex = Entries.size();
  Entries.push_back(&Sym);
}

template <class ELFT> size_t IgotPltSection<ELFT>::getSize() const {
  return Entries.size() * Target->GotPltEntrySize;
}

template <class ELFT> void IgotPltSection<ELFT>::writeTo(uint8_t *Buf) {
  for (const SymbolBody *B : Entries) {
    Target->writeIgotPlt(Buf, *B);
    Buf += sizeof(uintX_t);
  }
}

template <class ELFT>
StringTableSection<ELFT>::StringTableSection(StringRef Name, bool Dynamic)
    : SyntheticSection(Dynamic ? (uintX_t)SHF_ALLOC : 0, SHT_STRTAB, 1, Name),
      Dynamic(Dynamic) {
  // ELF string tables start with a NUL byte.
  addString("");
}

// Adds a string to the string table. If HashIt is true we hash and check for
// duplicates. It is optional because the name of global symbols are already
// uniqued and hashing them again has a big cost for a small value: uniquing
// them with some other string that happens to be the same.
template <class ELFT>
unsigned StringTableSection<ELFT>::addString(StringRef S, bool HashIt) {
  if (HashIt) {
    auto R = StringMap.insert(std::make_pair(S, this->Size));
    if (!R.second)
      return R.first->second;
  }
  unsigned Ret = this->Size;
  this->Size = this->Size + S.size() + 1;
  Strings.push_back(S);
  return Ret;
}

template <class ELFT> void StringTableSection<ELFT>::writeTo(uint8_t *Buf) {
  for (StringRef S : Strings) {
    memcpy(Buf, S.data(), S.size());
    Buf += S.size() + 1;
  }
}

// Returns the number of version definition entries. Because the first entry
// is for the version definition itself, it is the number of versioned symbols
// plus one. Note that we don't support multiple versions yet.
static unsigned getVerDefNum() { return Config->VersionDefinitions.size() + 1; }

template <class ELFT>
DynamicSection<ELFT>::DynamicSection()
    : SyntheticSection(SHF_ALLOC | SHF_WRITE, SHT_DYNAMIC, sizeof(uintX_t),
                       ".dynamic") {
  this->Entsize = ELFT::Is64Bits ? 16 : 8;

  // .dynamic section is not writable on MIPS.
  // See "Special Section" in Chapter 4 in the following document:
  // ftp://www.linux-mips.org/pub/linux/mips/doc/ABI/mipsabi.pdf
  if (Config->EMachine == EM_MIPS)
    this->Flags = SHF_ALLOC;

  addEntries();
}

// There are some dynamic entries that don't depend on other sections.
// Such entries can be set early.
template <class ELFT> void DynamicSection<ELFT>::addEntries() {
  // Add strings to .dynstr early so that .dynstr's size will be
  // fixed early.
  for (StringRef S : Config->AuxiliaryList)
    add({DT_AUXILIARY, In<ELFT>::DynStrTab->addString(S)});
  if (!Config->RPath.empty())
    add({Config->EnableNewDtags ? DT_RUNPATH : DT_RPATH,
         In<ELFT>::DynStrTab->addString(Config->RPath)});
  for (SharedFile<ELFT> *F : Symtab<ELFT>::X->getSharedFiles())
    if (F->isNeeded())
      add({DT_NEEDED, In<ELFT>::DynStrTab->addString(F->getSoName())});
  if (!Config->SoName.empty())
    add({DT_SONAME, In<ELFT>::DynStrTab->addString(Config->SoName)});

  // Set DT_FLAGS and DT_FLAGS_1.
  uint32_t DtFlags = 0;
  uint32_t DtFlags1 = 0;
  if (Config->Bsymbolic)
    DtFlags |= DF_SYMBOLIC;
  if (Config->ZNodelete)
    DtFlags1 |= DF_1_NODELETE;
  if (Config->ZNow) {
    DtFlags |= DF_BIND_NOW;
    DtFlags1 |= DF_1_NOW;
  }
  if (Config->ZOrigin) {
    DtFlags |= DF_ORIGIN;
    DtFlags1 |= DF_1_ORIGIN;
  }

  if (DtFlags)
    add({DT_FLAGS, DtFlags});
  if (DtFlags1)
    add({DT_FLAGS_1, DtFlags1});

  if (!Config->Shared && !Config->Relocatable)
    add({DT_DEBUG, (uint64_t)0});
}

// Add remaining entries to complete .dynamic contents.
template <class ELFT> void DynamicSection<ELFT>::finalizeContents() {
  if (this->Size)
    return; // Already finalized.

  this->Link = In<ELFT>::DynStrTab->OutSec->SectionIndex;
  if (In<ELFT>::RelaDyn->OutSec->Size > 0) {
    bool IsRela = Config->Rela;
    add({IsRela ? DT_RELA : DT_REL, In<ELFT>::RelaDyn});
    add({IsRela ? DT_RELASZ : DT_RELSZ, In<ELFT>::RelaDyn->OutSec->Size});
    add({IsRela ? DT_RELAENT : DT_RELENT,
         uintX_t(IsRela ? sizeof(Elf_Rela) : sizeof(Elf_Rel))});

    // MIPS dynamic loader does not support RELCOUNT tag.
    // The problem is in the tight relation between dynamic
    // relocations and GOT. So do not emit this tag on MIPS.
    if (Config->EMachine != EM_MIPS) {
      size_t NumRelativeRels = In<ELFT>::RelaDyn->getRelativeRelocCount();
      if (Config->ZCombreloc && NumRelativeRels)
        add({IsRela ? DT_RELACOUNT : DT_RELCOUNT, NumRelativeRels});
    }
  }
  if (In<ELFT>::RelaPlt->OutSec->Size > 0) {
    add({DT_JMPREL, In<ELFT>::RelaPlt});
    add({DT_PLTRELSZ, In<ELFT>::RelaPlt->OutSec->Size});
    add({Config->EMachine == EM_MIPS ? DT_MIPS_PLTGOT : DT_PLTGOT,
         In<ELFT>::GotPlt});
    add({DT_PLTREL, uint64_t(Config->Rela ? DT_RELA : DT_REL)});
  }

  add({DT_SYMTAB, In<ELFT>::DynSymTab});
  add({DT_SYMENT, sizeof(Elf_Sym)});
  add({DT_STRTAB, In<ELFT>::DynStrTab});
  add({DT_STRSZ, In<ELFT>::DynStrTab->getSize()});
  if (In<ELFT>::GnuHashTab)
    add({DT_GNU_HASH, In<ELFT>::GnuHashTab});
  if (In<ELFT>::HashTab)
    add({DT_HASH, In<ELFT>::HashTab});

  if (Out::PreinitArray) {
    add({DT_PREINIT_ARRAY, Out::PreinitArray});
    add({DT_PREINIT_ARRAYSZ, Out::PreinitArray, Entry::SecSize});
  }
  if (Out::InitArray) {
    add({DT_INIT_ARRAY, Out::InitArray});
    add({DT_INIT_ARRAYSZ, Out::InitArray, Entry::SecSize});
  }
  if (Out::FiniArray) {
    add({DT_FINI_ARRAY, Out::FiniArray});
    add({DT_FINI_ARRAYSZ, Out::FiniArray, Entry::SecSize});
  }

  if (SymbolBody *B = Symtab<ELFT>::X->findInCurrentDSO(Config->Init))
    add({DT_INIT, B});
  if (SymbolBody *B = Symtab<ELFT>::X->findInCurrentDSO(Config->Fini))
    add({DT_FINI, B});

  bool HasVerNeed = In<ELFT>::VerNeed->getNeedNum() != 0;
  if (HasVerNeed || In<ELFT>::VerDef)
    add({DT_VERSYM, In<ELFT>::VerSym});
  if (In<ELFT>::VerDef) {
    add({DT_VERDEF, In<ELFT>::VerDef});
    add({DT_VERDEFNUM, getVerDefNum()});
  }
  if (HasVerNeed) {
    add({DT_VERNEED, In<ELFT>::VerNeed});
    add({DT_VERNEEDNUM, In<ELFT>::VerNeed->getNeedNum()});
  }

  if (Config->EMachine == EM_MIPS) {
    add({DT_MIPS_RLD_VERSION, 1});
    add({DT_MIPS_FLAGS, RHF_NOTPOT});
    add({DT_MIPS_BASE_ADDRESS, Config->ImageBase});
    add({DT_MIPS_SYMTABNO, In<ELFT>::DynSymTab->getNumSymbols()});
    add({DT_MIPS_LOCAL_GOTNO, In<ELFT>::MipsGot->getLocalEntriesNum()});
    if (const SymbolBody *B = In<ELFT>::MipsGot->getFirstGlobalEntry())
      add({DT_MIPS_GOTSYM, B->DynsymIndex});
    else
      add({DT_MIPS_GOTSYM, In<ELFT>::DynSymTab->getNumSymbols()});
    add({DT_PLTGOT, In<ELFT>::MipsGot});
    if (In<ELFT>::MipsRldMap)
      add({DT_MIPS_RLD_MAP, In<ELFT>::MipsRldMap});
  }

  this->OutSec->Link = this->Link;

  // +1 for DT_NULL
  this->Size = (Entries.size() + 1) * this->Entsize;
}

template <class ELFT> void DynamicSection<ELFT>::writeTo(uint8_t *Buf) {
  auto *P = reinterpret_cast<Elf_Dyn *>(Buf);

  for (const Entry &E : Entries) {
    P->d_tag = E.Tag;
    switch (E.Kind) {
    case Entry::SecAddr:
      P->d_un.d_ptr = E.OutSec->Addr;
      break;
    case Entry::InSecAddr:
      P->d_un.d_ptr = E.InSec->OutSec->Addr + E.InSec->OutSecOff;
      break;
    case Entry::SecSize:
      P->d_un.d_val = E.OutSec->Size;
      break;
    case Entry::SymAddr:
      P->d_un.d_ptr = E.Sym->template getVA<ELFT>();
      break;
    case Entry::PlainInt:
      P->d_un.d_val = E.Val;
      break;
    }
    ++P;
  }
}

template <class ELFT>
typename ELFT::uint DynamicReloc<ELFT>::getOffset() const {
  return InputSec->OutSec->Addr + InputSec->getOffset<ELFT>(OffsetInSec);
}

template <class ELFT> int64_t DynamicReloc<ELFT>::getAddend() const {
  if (UseSymVA)
    return Sym->getVA<ELFT>(Addend);
  return Addend;
}

template <class ELFT> uint32_t DynamicReloc<ELFT>::getSymIndex() const {
  if (Sym && !UseSymVA)
    return Sym->DynsymIndex;
  return 0;
}

template <class ELFT>
RelocationSection<ELFT>::RelocationSection(StringRef Name, bool Sort)
    : SyntheticSection(SHF_ALLOC, Config->Rela ? SHT_RELA : SHT_REL,
                       sizeof(uintX_t), Name),
      Sort(Sort) {
  this->Entsize = Config->Rela ? sizeof(Elf_Rela) : sizeof(Elf_Rel);
}

template <class ELFT>
void RelocationSection<ELFT>::addReloc(const DynamicReloc<ELFT> &Reloc) {
  if (Reloc.Type == Target->RelativeRel)
    ++NumRelativeRelocs;
  Relocs.push_back(Reloc);
}

template <class ELFT, class RelTy>
static bool compRelocations(const RelTy &A, const RelTy &B) {
  bool AIsRel = A.getType(Config->Mips64EL) == Target->RelativeRel;
  bool BIsRel = B.getType(Config->Mips64EL) == Target->RelativeRel;
  if (AIsRel != BIsRel)
    return AIsRel;

  return A.getSymbol(Config->Mips64EL) < B.getSymbol(Config->Mips64EL);
}

template <class ELFT> void RelocationSection<ELFT>::writeTo(uint8_t *Buf) {
  uint8_t *BufBegin = Buf;
  for (const DynamicReloc<ELFT> &Rel : Relocs) {
    auto *P = reinterpret_cast<Elf_Rela *>(Buf);
    Buf += Config->Rela ? sizeof(Elf_Rela) : sizeof(Elf_Rel);

    if (Config->Rela)
      P->r_addend = Rel.getAddend();
    P->r_offset = Rel.getOffset();
    if (Config->EMachine == EM_MIPS && Rel.getInputSec() == In<ELFT>::MipsGot)
      // Dynamic relocation against MIPS GOT section make deal TLS entries
      // allocated in the end of the GOT. We need to adjust the offset to take
      // in account 'local' and 'global' GOT entries.
      P->r_offset += In<ELFT>::MipsGot->getTlsOffset();
    P->setSymbolAndType(Rel.getSymIndex(), Rel.Type, Config->Mips64EL);
  }

  if (Sort) {
    if (Config->Rela)
      std::stable_sort((Elf_Rela *)BufBegin,
                       (Elf_Rela *)BufBegin + Relocs.size(),
                       compRelocations<ELFT, Elf_Rela>);
    else
      std::stable_sort((Elf_Rel *)BufBegin, (Elf_Rel *)BufBegin + Relocs.size(),
                       compRelocations<ELFT, Elf_Rel>);
  }
}

template <class ELFT> unsigned RelocationSection<ELFT>::getRelocOffset() {
  return this->Entsize * Relocs.size();
}

template <class ELFT> void RelocationSection<ELFT>::finalizeContents() {
  this->Link = In<ELFT>::DynSymTab ? In<ELFT>::DynSymTab->OutSec->SectionIndex
                                   : In<ELFT>::SymTab->OutSec->SectionIndex;

  // Set required output section properties.
  this->OutSec->Link = this->Link;
}

template <class ELFT>
SymbolTableSection<ELFT>::SymbolTableSection(
    StringTableSection<ELFT> &StrTabSec)
    : SyntheticSection(StrTabSec.isDynamic() ? (uintX_t)SHF_ALLOC : 0,
                       StrTabSec.isDynamic() ? SHT_DYNSYM : SHT_SYMTAB,
                       sizeof(uintX_t),
                       StrTabSec.isDynamic() ? ".dynsym" : ".symtab"),
      StrTabSec(StrTabSec) {
  this->Entsize = sizeof(Elf_Sym);
}

// Orders symbols according to their positions in the GOT,
// in compliance with MIPS ABI rules.
// See "Global Offset Table" in Chapter 5 in the following document
// for detailed description:
// ftp://www.linux-mips.org/pub/linux/mips/doc/ABI/mipsabi.pdf
static bool sortMipsSymbols(const SymbolTableEntry &L, const SymbolTableEntry &R) {
  // Sort entries related to non-local preemptible symbols by GOT indexes.
  // All other entries go to the first part of GOT in arbitrary order.
  bool LIsInLocalGot = !L.Symbol->IsInGlobalMipsGot;
  bool RIsInLocalGot = !R.Symbol->IsInGlobalMipsGot;
  if (LIsInLocalGot || RIsInLocalGot)
    return !RIsInLocalGot;
  return L.Symbol->GotIndex < R.Symbol->GotIndex;
}

// Finalize a symbol table. The ELF spec requires that all local
// symbols precede global symbols, so we sort symbol entries in this
// function. (For .dynsym, we don't do that because symbols for
// dynamic linking are inherently all globals.)
template <class ELFT> void SymbolTableSection<ELFT>::finalizeContents() {
  this->OutSec->Link = StrTabSec.OutSec->SectionIndex;

  // If it is a .dynsym, there should be no local symbols, but we need
  // to do a few things for the dynamic linker.
  if (this->Type == SHT_DYNSYM) {
    // Section's Info field has the index of the first non-local symbol.
    // Because the first symbol entry is a null entry, 1 is the first.
    this->OutSec->Info = 1;

    if (In<ELFT>::GnuHashTab) {
      // NB: It also sorts Symbols to meet the GNU hash table requirements.
      In<ELFT>::GnuHashTab->addSymbols(Symbols);
    } else if (Config->EMachine == EM_MIPS) {
      std::stable_sort(Symbols.begin(), Symbols.end(), sortMipsSymbols);
    }

    size_t I = 0;
    for (const SymbolTableEntry &S : Symbols)
      S.Symbol->DynsymIndex = ++I;
    return;
  }

  // If it is a .symtab, move all local symbols before global symbols.
  auto It = std::stable_partition(
      Symbols.begin(), Symbols.end(), [](const SymbolTableEntry &S) {
        return S.Symbol->isLocal() ||
               S.Symbol->symbol()->computeBinding() == STB_LOCAL;
      });
  size_t NumLocals = It - Symbols.begin();
  this->OutSec->Info = NumLocals + 1;
}

template <class ELFT> void SymbolTableSection<ELFT>::addSymbol(SymbolBody *B) {
  // Adding a local symbol to a .dynsym is a bug.
  assert(this->Type != SHT_DYNSYM || !B->isLocal());

  bool HashIt = B->isLocal();
  Symbols.push_back({B, StrTabSec.addString(B->getName(), HashIt)});
}

template <class ELFT>
size_t SymbolTableSection<ELFT>::getSymbolIndex(SymbolBody *Body) {
  auto I = llvm::find_if(Symbols, [&](const SymbolTableEntry &E) {
    if (E.Symbol == Body)
      return true;
    // This is used for -r, so we have to handle multiple section
    // symbols being combined.
    if (Body->Type == STT_SECTION && E.Symbol->Type == STT_SECTION)
      return cast<DefinedRegular>(Body)->Section->OutSec ==
             cast<DefinedRegular>(E.Symbol)->Section->OutSec;
    return false;
  });
  if (I == Symbols.end())
    return 0;
  return I - Symbols.begin() + 1;
}

// Write the internal symbol table contents to the output symbol table.
template <class ELFT> void SymbolTableSection<ELFT>::writeTo(uint8_t *Buf) {
  // The first entry is a null entry as per the ELF spec.
  Buf += sizeof(Elf_Sym);

  auto *ESym = reinterpret_cast<Elf_Sym *>(Buf);

  for (SymbolTableEntry &Ent : Symbols) {
    SymbolBody *Body = Ent.Symbol;

    // Set st_info and st_other.
    if (Body->isLocal()) {
      ESym->setBindingAndType(STB_LOCAL, Body->Type);
    } else {
      ESym->setBindingAndType(Body->symbol()->computeBinding(), Body->Type);
      ESym->setVisibility(Body->symbol()->Visibility);
    }

    ESym->st_name = Ent.StrTabOffset;
    ESym->st_size = Body->getSize<ELFT>();

    // Set a section index.
    if (const OutputSection *OutSec = Body->getOutputSection<ELFT>())
      ESym->st_shndx = OutSec->SectionIndex;
    else if (isa<DefinedRegular>(Body))
      ESym->st_shndx = SHN_ABS;
    else if (isa<DefinedCommon>(Body))
      ESym->st_shndx = SHN_COMMON;

    // st_value is usually an address of a symbol, but that has a
    // special meaining for uninstantiated common symbols (this can
    // occur if -r is given).
    if (!Config->DefineCommon && isa<DefinedCommon>(Body))
      ESym->st_value = cast<DefinedCommon>(Body)->Alignment;
    else
      ESym->st_value = Body->getVA<ELFT>();

    ++ESym;
  }

  // On MIPS we need to mark symbol which has a PLT entry and requires
  // pointer equality by STO_MIPS_PLT flag. That is necessary to help
  // dynamic linker distinguish such symbols and MIPS lazy-binding stubs.
  // https://sourceware.org/ml/binutils/2008-07/txt00000.txt
  if (Config->EMachine == EM_MIPS) {
    auto *ESym = reinterpret_cast<Elf_Sym *>(Buf);

    for (SymbolTableEntry &Ent : Symbols) {
      SymbolBody *Body = Ent.Symbol;
      if (Body->isInPlt() && Body->NeedsPltAddr)
        ESym->st_other |= STO_MIPS_PLT;

      if (Config->Relocatable)
        if (auto *D = dyn_cast<DefinedRegular>(Body))
          if (D->isMipsPIC<ELFT>())
            ESym->st_other |= STO_MIPS_PIC;
      ++ESym;
    }
  }
}

// .hash and .gnu.hash sections contain on-disk hash tables that map
// symbol names to their dynamic symbol table indices. Their purpose
// is to help the dynamic linker resolve symbols quickly. If ELF files
// don't have them, the dynamic linker has to do linear search on all
// dynamic symbols, which makes programs slower. Therefore, a .hash
// section is added to a DSO by default. A .gnu.hash is added if you
// give the -hash-style=gnu or -hash-style=both option.
//
// The Unix semantics of resolving dynamic symbols is somewhat expensive.
// Each ELF file has a list of DSOs that the ELF file depends on and a
// list of dynamic symbols that need to be resolved from any of the
// DSOs. That means resolving all dynamic symbols takes O(m)*O(n)
// where m is the number of DSOs and n is the number of dynamic
// symbols. For modern large programs, both m and n are large.  So
// making each step faster by using hash tables substiantially
// improves time to load programs.
//
// (Note that this is not the only way to design the shared library.
// For instance, the Windows DLL takes a different approach. On
// Windows, each dynamic symbol has a name of DLL from which the symbol
// has to be resolved. That makes the cost of symbol resolution O(n).
// This disables some hacky techniques you can use on Unix such as
// LD_PRELOAD, but this is arguably better semantics than the Unix ones.)
//
// Due to historical reasons, we have two different hash tables, .hash
// and .gnu.hash. They are for the same purpose, and .gnu.hash is a new
// and better version of .hash. .hash is just an on-disk hash table, but
// .gnu.hash has a bloom filter in addition to a hash table to skip
// DSOs very quickly. If you are sure that your dynamic linker knows
// about .gnu.hash, you want to specify -hash-style=gnu. Otherwise, a
// safe bet is to specify -hash-style=both for backward compatibilty.
template <class ELFT>
GnuHashTableSection<ELFT>::GnuHashTableSection()
    : SyntheticSection(SHF_ALLOC, SHT_GNU_HASH, sizeof(uintX_t), ".gnu.hash") {
  this->Entsize = ELFT::Is64Bits ? 0 : 4;
}

template <class ELFT> void GnuHashTableSection<ELFT>::finalizeContents() {
  this->OutSec->Link = In<ELFT>::DynSymTab->OutSec->SectionIndex;

  // Computes bloom filter size in word size. We want to allocate 8
  // bits for each symbol. It must be a power of two.
  if (Symbols.empty())
    MaskWords = 1;
  else
    MaskWords = NextPowerOf2((Symbols.size() - 1) / sizeof(uintX_t));

  Size = 16;                           // Header
  Size += sizeof(uintX_t) * MaskWords; // Bloom filter
  Size += NBuckets * 4;                // Hash buckets
  Size += Symbols.size() * 4;          // Hash values
}

template <class ELFT> void GnuHashTableSection<ELFT>::writeTo(uint8_t *Buf) {
  // Write a header.
  const endianness E = ELFT::TargetEndianness;
  write32<E>(Buf, NBuckets);
  write32<E>(Buf + 4, In<ELFT>::DynSymTab->getNumSymbols() - Symbols.size());
  write32<E>(Buf + 8, MaskWords);
  write32<E>(Buf + 12, getShift2());
  Buf += 16;

  // Write a bloom filter and a hash table.
  writeBloomFilter(Buf);
  Buf += sizeof(uintX_t) * MaskWords;
  writeHashTable(Buf);
}

// This function writes a 2-bit bloom filter. This bloom filter alone
// usually filters out 80% or more of all symbol lookups [1].
// The dynamic linker uses the hash table only when a symbol is not
// filtered out by a bloom filter.
//
// [1] Ulrich Drepper (2011), "How To Write Shared Libraries" (Ver. 4.1.2),
//     p.9, https://www.akkadia.org/drepper/dsohowto.pdf
template <class ELFT>
void GnuHashTableSection<ELFT>::writeBloomFilter(uint8_t *Buf) {
  typedef typename ELFT::Off Elf_Off;
  const unsigned C = sizeof(uintX_t) * 8;

  auto *Filter = reinterpret_cast<Elf_Off *>(Buf);
  for (const Entry &Sym : Symbols) {
    size_t I = (Sym.Hash / C) & (MaskWords - 1);
    Filter[I] |= uintX_t(1) << (Sym.Hash % C);
    Filter[I] |= uintX_t(1) << ((Sym.Hash >> getShift2()) % C);
  }
}

template <class ELFT>
void GnuHashTableSection<ELFT>::writeHashTable(uint8_t *Buf) {
  // A 32-bit integer type in the target endianness.
  typedef typename ELFT::Word Elf_Word;

  // Group symbols by hash value.
  std::vector<std::vector<Entry>> Syms(NBuckets);
  for (const Entry &Ent : Symbols)
    Syms[Ent.Hash % NBuckets].push_back(Ent);

  // Write hash buckets. Hash buckets contain indices in the following
  // hash value table.
  Elf_Word *Buckets = reinterpret_cast<Elf_Word *>(Buf);
  for (size_t I = 0; I < NBuckets; ++I)
    if (!Syms[I].empty())
      Buckets[I] = Syms[I][0].Body->DynsymIndex;

  // Write a hash value table. It represents a sequence of chains that
  // share the same hash modulo value. The last element of each chain
  // is terminated by LSB 1.
  Elf_Word *Values = Buckets + NBuckets;
  size_t I = 0;
  for (std::vector<Entry> &Vec : Syms) {
    if (Vec.empty())
      continue;
    for (const Entry &Ent : makeArrayRef(Vec).drop_back())
      Values[I++] = Ent.Hash & ~1;
    Values[I++] = Vec.back().Hash | 1;
  }
}

static uint32_t hashGnu(StringRef Name) {
  uint32_t H = 5381;
  for (uint8_t C : Name)
    H = (H << 5) + H + C;
  return H;
}

// Returns a number of hash buckets to accomodate given number of elements.
// We want to choose a moderate number that is not too small (which
// causes too many hash collisions) and not too large (which wastes
// disk space.)
//
// We return a prime number because it (is believed to) achieve good
// hash distribution.
static size_t getBucketSize(size_t NumSymbols) {
  // List of largest prime numbers that are not greater than 2^n + 1.
  for (size_t N : {131071, 65521, 32749, 16381, 8191, 4093, 2039, 1021, 509,
                   251, 127, 61, 31, 13, 7, 3, 1})
    if (N <= NumSymbols)
      return N;
  return 0;
}

// Add symbols to this symbol hash table. Note that this function
// destructively sort a given vector -- which is needed because
// GNU-style hash table places some sorting requirements.
template <class ELFT>
void GnuHashTableSection<ELFT>::addSymbols(std::vector<SymbolTableEntry> &V) {
  // We cannot use 'auto' for Mid because GCC 6.1 cannot deduce
  // its type correctly.
  std::vector<SymbolTableEntry>::iterator Mid =
      std::stable_partition(V.begin(), V.end(), [](const SymbolTableEntry &S) {
        return S.Symbol->isUndefined();
      });
  if (Mid == V.end())
    return;

  for (SymbolTableEntry &Ent : llvm::make_range(Mid, V.end())) {
    SymbolBody *B = Ent.Symbol;
    Symbols.push_back({B, Ent.StrTabOffset, hashGnu(B->getName())});
  }

  NBuckets = getBucketSize(Symbols.size());
  std::stable_sort(Symbols.begin(), Symbols.end(),
                   [&](const Entry &L, const Entry &R) {
                     return L.Hash % NBuckets < R.Hash % NBuckets;
                   });

  V.erase(Mid, V.end());
  for (const Entry &Ent : Symbols)
    V.push_back({Ent.Body, Ent.StrTabOffset});
}

template <class ELFT>
HashTableSection<ELFT>::HashTableSection()
    : SyntheticSection(SHF_ALLOC, SHT_HASH, 4, ".hash") {
  this->Entsize = 4;
}

template <class ELFT> void HashTableSection<ELFT>::finalizeContents() {
  this->OutSec->Link = In<ELFT>::DynSymTab->OutSec->SectionIndex;

  unsigned NumEntries = 2;                            // nbucket and nchain.
  NumEntries += In<ELFT>::DynSymTab->getNumSymbols(); // The chain entries.

  // Create as many buckets as there are symbols.
  // FIXME: This is simplistic. We can try to optimize it, but implementing
  // support for SHT_GNU_HASH is probably even more profitable.
  NumEntries += In<ELFT>::DynSymTab->getNumSymbols();
  this->Size = NumEntries * 4;
}

template <class ELFT> void HashTableSection<ELFT>::writeTo(uint8_t *Buf) {
  // A 32-bit integer type in the target endianness.
  typedef typename ELFT::Word Elf_Word;

  unsigned NumSymbols = In<ELFT>::DynSymTab->getNumSymbols();

  auto *P = reinterpret_cast<Elf_Word *>(Buf);
  *P++ = NumSymbols; // nbucket
  *P++ = NumSymbols; // nchain

  Elf_Word *Buckets = P;
  Elf_Word *Chains = P + NumSymbols;

  for (const SymbolTableEntry &S : In<ELFT>::DynSymTab->getSymbols()) {
    SymbolBody *Body = S.Symbol;
    StringRef Name = Body->getName();
    unsigned I = Body->DynsymIndex;
    uint32_t Hash = hashSysV(Name) % NumSymbols;
    Chains[I] = Buckets[Hash];
    Buckets[Hash] = I;
  }
}

template <class ELFT>
PltSection<ELFT>::PltSection(size_t S)
    : SyntheticSection(SHF_ALLOC | SHF_EXECINSTR, SHT_PROGBITS, 16, ".plt"),
      HeaderSize(S) {}

template <class ELFT> void PltSection<ELFT>::writeTo(uint8_t *Buf) {
  // At beginning of PLT but not the IPLT, we have code to call the dynamic
  // linker to resolve dynsyms at runtime. Write such code.
  if (HeaderSize != 0)
    Target->writePltHeader(Buf);
  size_t Off = HeaderSize;
  // The IPlt is immediately after the Plt, account for this in RelOff
  unsigned PltOff = getPltRelocOff();

  for (auto &I : Entries) {
    const SymbolBody *B = I.first;
    unsigned RelOff = I.second + PltOff;
    uint64_t Got = B->getGotPltVA<ELFT>();
    uint64_t Plt = this->getVA() + Off;
    Target->writePlt(Buf + Off, Got, Plt, B->PltIndex, RelOff);
    Off += Target->PltEntrySize;
  }
}

template <class ELFT> void PltSection<ELFT>::addEntry(SymbolBody &Sym) {
  Sym.PltIndex = Entries.size();
  RelocationSection<ELFT> *PltRelocSection = In<ELFT>::RelaPlt;
  if (HeaderSize == 0) {
    PltRelocSection = In<ELFT>::RelaIplt;
    Sym.IsInIplt = true;
  }
  unsigned RelOff = PltRelocSection->getRelocOffset();
  Entries.push_back(std::make_pair(&Sym, RelOff));
}

template <class ELFT> size_t PltSection<ELFT>::getSize() const {
  return HeaderSize + Entries.size() * Target->PltEntrySize;
}

// Some architectures such as additional symbols in the PLT section. For
// example ARM uses mapping symbols to aid disassembly
template <class ELFT> void PltSection<ELFT>::addSymbols() {
  // The PLT may have symbols defined for the Header, the IPLT has no header
  if (HeaderSize != 0)
    Target->addPltHeaderSymbols(this);
  size_t Off = HeaderSize;
  for (size_t I = 0; I < Entries.size(); ++I) {
    Target->addPltSymbols(this, Off);
    Off += Target->PltEntrySize;
  }
}

template <class ELFT> unsigned PltSection<ELFT>::getPltRelocOff() const {
  return (HeaderSize == 0) ? In<ELFT>::Plt->getSize() : 0;
}

template <class ELFT>
GdbIndexSection<ELFT>::GdbIndexSection()
    : SyntheticSection(0, SHT_PROGBITS, 1, ".gdb_index"),
      StringPool(llvm::StringTableBuilder::ELF) {}

// Iterative hash function for symbol's name is described in .gdb_index format
// specification. Note that we use one for version 5 to 7 here, it is different
// for version 4.
static uint32_t hash(StringRef Str) {
  uint32_t R = 0;
  for (uint8_t C : Str)
    R = R * 67 + tolower(C) - 113;
  return R;
}

static std::vector<std::pair<uint64_t, uint64_t>>
readCuList(DWARFContext &Dwarf, InputSection *Sec) {
  std::vector<std::pair<uint64_t, uint64_t>> Ret;
  for (std::unique_ptr<DWARFCompileUnit> &CU : Dwarf.compile_units())
    Ret.push_back({Sec->OutSecOff + CU->getOffset(), CU->getLength() + 4});
  return Ret;
}

template <class ELFT>
static InputSectionBase *findSection(ArrayRef<InputSectionBase *> Arr,
                                     uint64_t Offset) {
  for (InputSectionBase *S : Arr)
    if (S && S != &InputSection::Discarded)
      if (Offset >= S->Offset && Offset < S->Offset + S->getSize<ELFT>())
        return S;
  return nullptr;
}

template <class ELFT>
static std::vector<AddressEntry>
readAddressArea(DWARFContext &Dwarf, InputSection *Sec, size_t CurrentCU) {
  std::vector<AddressEntry> Ret;

  for (std::unique_ptr<DWARFCompileUnit> &CU : Dwarf.compile_units()) {
    DWARFAddressRangesVector Ranges;
    CU->collectAddressRanges(Ranges);

    ArrayRef<InputSectionBase *> Sections =
        Sec->template getFile<ELFT>()->getSections();

    for (std::pair<uint64_t, uint64_t> &R : Ranges)
      if (InputSectionBase *S = findSection<ELFT>(Sections, R.first))
        Ret.push_back(
            {S, R.first - S->Offset, R.second - S->Offset, CurrentCU});
    ++CurrentCU;
  }
  return Ret;
}

static std::vector<std::pair<StringRef, uint8_t>>
readPubNamesAndTypes(DWARFContext &Dwarf, bool IsLE) {
  StringRef Data[] = {Dwarf.getGnuPubNamesSection(),
                      Dwarf.getGnuPubTypesSection()};

  std::vector<std::pair<StringRef, uint8_t>> Ret;
  for (StringRef D : Data) {
    DWARFDebugPubTable PubTable(D, IsLE, true);
    for (const DWARFDebugPubTable::Set &Set : PubTable.getData())
      for (const DWARFDebugPubTable::Entry &Ent : Set.Entries)
        Ret.push_back({Ent.Name, Ent.Descriptor.toBits()});
  }
  return Ret;
}

class ObjInfoTy : public llvm::LoadedObjectInfo {
  uint64_t getSectionLoadAddress(const object::SectionRef &Sec) const override {
    auto &S = static_cast<const object::ELFSectionRef &>(Sec);
    if (S.getFlags() & ELF::SHF_ALLOC)
      return S.getOffset();
    return 0;
  }

  std::unique_ptr<llvm::LoadedObjectInfo> clone() const override { return {}; }
};

template <class ELFT> void GdbIndexSection<ELFT>::readDwarf(InputSection *Sec) {
  elf::ObjectFile<ELFT> *File = Sec->template getFile<ELFT>();

  Expected<std::unique_ptr<object::ObjectFile>> Obj =
      object::ObjectFile::createObjectFile(File->MB);
  if (!Obj) {
    error(toString(File) + ": error creating DWARF context");
    return;
  }

  ObjInfoTy ObjInfo;
  DWARFContextInMemory Dwarf(*Obj.get(), &ObjInfo);

  size_t CuId = CompilationUnits.size();
  for (std::pair<uint64_t, uint64_t> &P : readCuList(Dwarf, Sec))
    CompilationUnits.push_back(P);

  for (AddressEntry &Ent : readAddressArea<ELFT>(Dwarf, Sec, CuId))
    AddressArea.push_back(Ent);

  std::vector<std::pair<StringRef, uint8_t>> NamesAndTypes =
      readPubNamesAndTypes(Dwarf, ELFT::TargetEndianness == support::little);

  for (std::pair<StringRef, uint8_t> &Pair : NamesAndTypes) {
    uint32_t Hash = hash(Pair.first);
    size_t Offset = StringPool.add(Pair.first);

    bool IsNew;
    GdbSymbol *Sym;
    std::tie(IsNew, Sym) = SymbolTable.add(Hash, Offset);
    if (IsNew) {
      Sym->CuVectorIndex = CuVectors.size();
      CuVectors.push_back({{CuId, Pair.second}});
      continue;
    }

    CuVectors[Sym->CuVectorIndex].push_back({CuId, Pair.second});
  }
}

template <class ELFT> void GdbIndexSection<ELFT>::finalizeContents() {
  if (Finalized)
    return;
  Finalized = true;

  for (InputSectionBase *S : InputSections)
    if (InputSection *IS = dyn_cast<InputSection>(S))
      if (IS->OutSec && IS->Name == ".debug_info")
        readDwarf(IS);

  SymbolTable.finalizeContents();

  // GdbIndex header consist from version fields
  // and 5 more fields with different kinds of offsets.
  CuTypesOffset = CuListOffset + CompilationUnits.size() * CompilationUnitSize;
  SymTabOffset = CuTypesOffset + AddressArea.size() * AddressEntrySize;

  ConstantPoolOffset =
      SymTabOffset + SymbolTable.getCapacity() * SymTabEntrySize;

  for (std::vector<std::pair<uint32_t, uint8_t>> &CuVec : CuVectors) {
    CuVectorsOffset.push_back(CuVectorsSize);
    CuVectorsSize += OffsetTypeSize * (CuVec.size() + 1);
  }
  StringPoolOffset = ConstantPoolOffset + CuVectorsSize;

  StringPool.finalizeInOrder();
}

template <class ELFT> size_t GdbIndexSection<ELFT>::getSize() const {
  const_cast<GdbIndexSection<ELFT> *>(this)->finalizeContents();
  return StringPoolOffset + StringPool.getSize();
}

template <class ELFT> void GdbIndexSection<ELFT>::writeTo(uint8_t *Buf) {
  write32le(Buf, 7);                       // Write version.
  write32le(Buf + 4, CuListOffset);        // CU list offset.
  write32le(Buf + 8, CuTypesOffset);       // Types CU list offset.
  write32le(Buf + 12, CuTypesOffset);      // Address area offset.
  write32le(Buf + 16, SymTabOffset);       // Symbol table offset.
  write32le(Buf + 20, ConstantPoolOffset); // Constant pool offset.
  Buf += 24;

  // Write the CU list.
  for (std::pair<uintX_t, uintX_t> CU : CompilationUnits) {
    write64le(Buf, CU.first);
    write64le(Buf + 8, CU.second);
    Buf += 16;
  }

  // Write the address area.
  for (AddressEntry &E : AddressArea) {
    uintX_t BaseAddr =
        E.Section->OutSec->Addr + E.Section->template getOffset<ELFT>(0);
    write64le(Buf, BaseAddr + E.LowAddress);
    write64le(Buf + 8, BaseAddr + E.HighAddress);
    write32le(Buf + 16, E.CuIndex);
    Buf += 20;
  }

  // Write the symbol table.
  for (size_t I = 0; I < SymbolTable.getCapacity(); ++I) {
    GdbSymbol *Sym = SymbolTable.getSymbol(I);
    if (Sym) {
      size_t NameOffset =
          Sym->NameOffset + StringPoolOffset - ConstantPoolOffset;
      size_t CuVectorOffset = CuVectorsOffset[Sym->CuVectorIndex];
      write32le(Buf, NameOffset);
      write32le(Buf + 4, CuVectorOffset);
    }
    Buf += 8;
  }

  // Write the CU vectors into the constant pool.
  for (std::vector<std::pair<uint32_t, uint8_t>> &CuVec : CuVectors) {
    write32le(Buf, CuVec.size());
    Buf += 4;
    for (std::pair<uint32_t, uint8_t> &P : CuVec) {
      uint32_t Index = P.first;
      uint8_t Flags = P.second;
      Index |= Flags << 24;
      write32le(Buf, Index);
      Buf += 4;
    }
  }

  StringPool.write(Buf);
}

template <class ELFT> bool GdbIndexSection<ELFT>::empty() const {
  return !Out::DebugInfo;
}

template <class ELFT>
EhFrameHeader<ELFT>::EhFrameHeader()
    : SyntheticSection(SHF_ALLOC, SHT_PROGBITS, 1, ".eh_frame_hdr") {}

// .eh_frame_hdr contains a binary search table of pointers to FDEs.
// Each entry of the search table consists of two values,
// the starting PC from where FDEs covers, and the FDE's address.
// It is sorted by PC.
template <class ELFT> void EhFrameHeader<ELFT>::writeTo(uint8_t *Buf) {
  const endianness E = ELFT::TargetEndianness;

  // Sort the FDE list by their PC and uniqueify. Usually there is only
  // one FDE for a PC (i.e. function), but if ICF merges two functions
  // into one, there can be more than one FDEs pointing to the address.
  auto Less = [](const FdeData &A, const FdeData &B) { return A.Pc < B.Pc; };
  std::stable_sort(Fdes.begin(), Fdes.end(), Less);
  auto Eq = [](const FdeData &A, const FdeData &B) { return A.Pc == B.Pc; };
  Fdes.erase(std::unique(Fdes.begin(), Fdes.end(), Eq), Fdes.end());

  Buf[0] = 1;
  Buf[1] = DW_EH_PE_pcrel | DW_EH_PE_sdata4;
  Buf[2] = DW_EH_PE_udata4;
  Buf[3] = DW_EH_PE_datarel | DW_EH_PE_sdata4;
  write32<E>(Buf + 4, In<ELFT>::EhFrame->OutSec->Addr - this->getVA() - 4);
  write32<E>(Buf + 8, Fdes.size());
  Buf += 12;

  uintX_t VA = this->getVA();
  for (FdeData &Fde : Fdes) {
    write32<E>(Buf, Fde.Pc - VA);
    write32<E>(Buf + 4, Fde.FdeVA - VA);
    Buf += 8;
  }
}

template <class ELFT> size_t EhFrameHeader<ELFT>::getSize() const {
  // .eh_frame_hdr has a 12 bytes header followed by an array of FDEs.
  return 12 + In<ELFT>::EhFrame->NumFdes * 8;
}

template <class ELFT>
void EhFrameHeader<ELFT>::addFde(uint32_t Pc, uint32_t FdeVA) {
  Fdes.push_back({Pc, FdeVA});
}

template <class ELFT> bool EhFrameHeader<ELFT>::empty() const {
  return In<ELFT>::EhFrame->empty();
}

template <class ELFT>
VersionDefinitionSection<ELFT>::VersionDefinitionSection()
    : SyntheticSection(SHF_ALLOC, SHT_GNU_verdef, sizeof(uint32_t),
                       ".gnu.version_d") {}

static StringRef getFileDefName() {
  if (!Config->SoName.empty())
    return Config->SoName;
  return Config->OutputFile;
}

template <class ELFT> void VersionDefinitionSection<ELFT>::finalizeContents() {
  FileDefNameOff = In<ELFT>::DynStrTab->addString(getFileDefName());
  for (VersionDefinition &V : Config->VersionDefinitions)
    V.NameOff = In<ELFT>::DynStrTab->addString(V.Name);

  this->OutSec->Link = In<ELFT>::DynStrTab->OutSec->SectionIndex;

  // sh_info should be set to the number of definitions. This fact is missed in
  // documentation, but confirmed by binutils community:
  // https://sourceware.org/ml/binutils/2014-11/msg00355.html
  this->OutSec->Info = getVerDefNum();
}

template <class ELFT>
void VersionDefinitionSection<ELFT>::writeOne(uint8_t *Buf, uint32_t Index,
                                              StringRef Name, size_t NameOff) {
  auto *Verdef = reinterpret_cast<Elf_Verdef *>(Buf);
  Verdef->vd_version = 1;
  Verdef->vd_cnt = 1;
  Verdef->vd_aux = sizeof(Elf_Verdef);
  Verdef->vd_next = sizeof(Elf_Verdef) + sizeof(Elf_Verdaux);
  Verdef->vd_flags = (Index == 1 ? VER_FLG_BASE : 0);
  Verdef->vd_ndx = Index;
  Verdef->vd_hash = hashSysV(Name);

  auto *Verdaux = reinterpret_cast<Elf_Verdaux *>(Buf + sizeof(Elf_Verdef));
  Verdaux->vda_name = NameOff;
  Verdaux->vda_next = 0;
}

template <class ELFT>
void VersionDefinitionSection<ELFT>::writeTo(uint8_t *Buf) {
  writeOne(Buf, 1, getFileDefName(), FileDefNameOff);

  for (VersionDefinition &V : Config->VersionDefinitions) {
    Buf += sizeof(Elf_Verdef) + sizeof(Elf_Verdaux);
    writeOne(Buf, V.Id, V.Name, V.NameOff);
  }

  // Need to terminate the last version definition.
  Elf_Verdef *Verdef = reinterpret_cast<Elf_Verdef *>(Buf);
  Verdef->vd_next = 0;
}

template <class ELFT> size_t VersionDefinitionSection<ELFT>::getSize() const {
  return (sizeof(Elf_Verdef) + sizeof(Elf_Verdaux)) * getVerDefNum();
}

template <class ELFT>
VersionTableSection<ELFT>::VersionTableSection()
    : SyntheticSection(SHF_ALLOC, SHT_GNU_versym, sizeof(uint16_t),
                       ".gnu.version") {
  this->Entsize = sizeof(Elf_Versym);
}

template <class ELFT> void VersionTableSection<ELFT>::finalizeContents() {
  // At the moment of june 2016 GNU docs does not mention that sh_link field
  // should be set, but Sun docs do. Also readelf relies on this field.
  this->OutSec->Link = In<ELFT>::DynSymTab->OutSec->SectionIndex;
}

template <class ELFT> size_t VersionTableSection<ELFT>::getSize() const {
  return sizeof(Elf_Versym) * (In<ELFT>::DynSymTab->getSymbols().size() + 1);
}

template <class ELFT> void VersionTableSection<ELFT>::writeTo(uint8_t *Buf) {
  auto *OutVersym = reinterpret_cast<Elf_Versym *>(Buf) + 1;
  for (const SymbolTableEntry &S : In<ELFT>::DynSymTab->getSymbols()) {
    OutVersym->vs_index = S.Symbol->symbol()->VersionId;
    ++OutVersym;
  }
}

template <class ELFT> bool VersionTableSection<ELFT>::empty() const {
  return !In<ELFT>::VerDef && In<ELFT>::VerNeed->empty();
}

template <class ELFT>
VersionNeedSection<ELFT>::VersionNeedSection()
    : SyntheticSection(SHF_ALLOC, SHT_GNU_verneed, sizeof(uint32_t),
                       ".gnu.version_r") {
  // Identifiers in verneed section start at 2 because 0 and 1 are reserved
  // for VER_NDX_LOCAL and VER_NDX_GLOBAL.
  // First identifiers are reserved by verdef section if it exist.
  NextIndex = getVerDefNum() + 1;
}

template <class ELFT>
void VersionNeedSection<ELFT>::addSymbol(SharedSymbol *SS) {
  auto *Ver = reinterpret_cast<const typename ELFT::Verdef *>(SS->Verdef);
  if (!Ver) {
    SS->symbol()->VersionId = VER_NDX_GLOBAL;
    return;
  }

  auto *File = cast<SharedFile<ELFT>>(SS->File);

  // If we don't already know that we need an Elf_Verneed for this DSO, prepare
  // to create one by adding it to our needed list and creating a dynstr entry
  // for the soname.
  if (File->VerdefMap.empty())
    Needed.push_back({File, In<ELFT>::DynStrTab->addString(File->getSoName())});
  typename SharedFile<ELFT>::NeededVer &NV = File->VerdefMap[Ver];
  // If we don't already know that we need an Elf_Vernaux for this Elf_Verdef,
  // prepare to create one by allocating a version identifier and creating a
  // dynstr entry for the version name.
  if (NV.Index == 0) {
    NV.StrTab = In<ELFT>::DynStrTab->addString(File->getStringTable().data() +
                                               Ver->getAux()->vda_name);
    NV.Index = NextIndex++;
  }
  SS->symbol()->VersionId = NV.Index;
}

template <class ELFT> void VersionNeedSection<ELFT>::writeTo(uint8_t *Buf) {
  // The Elf_Verneeds need to appear first, followed by the Elf_Vernauxs.
  auto *Verneed = reinterpret_cast<Elf_Verneed *>(Buf);
  auto *Vernaux = reinterpret_cast<Elf_Vernaux *>(Verneed + Needed.size());

  for (std::pair<SharedFile<ELFT> *, size_t> &P : Needed) {
    // Create an Elf_Verneed for this DSO.
    Verneed->vn_version = 1;
    Verneed->vn_cnt = P.first->VerdefMap.size();
    Verneed->vn_file = P.second;
    Verneed->vn_aux =
        reinterpret_cast<char *>(Vernaux) - reinterpret_cast<char *>(Verneed);
    Verneed->vn_next = sizeof(Elf_Verneed);
    ++Verneed;

    // Create the Elf_Vernauxs for this Elf_Verneed. The loop iterates over
    // VerdefMap, which will only contain references to needed version
    // definitions. Each Elf_Vernaux is based on the information contained in
    // the Elf_Verdef in the source DSO. This loop iterates over a std::map of
    // pointers, but is deterministic because the pointers refer to Elf_Verdef
    // data structures within a single input file.
    for (auto &NV : P.first->VerdefMap) {
      Vernaux->vna_hash = NV.first->vd_hash;
      Vernaux->vna_flags = 0;
      Vernaux->vna_other = NV.second.Index;
      Vernaux->vna_name = NV.second.StrTab;
      Vernaux->vna_next = sizeof(Elf_Vernaux);
      ++Vernaux;
    }

    Vernaux[-1].vna_next = 0;
  }
  Verneed[-1].vn_next = 0;
}

template <class ELFT> void VersionNeedSection<ELFT>::finalizeContents() {
  this->OutSec->Link = In<ELFT>::DynStrTab->OutSec->SectionIndex;
  this->OutSec->Info = Needed.size();
}

template <class ELFT> size_t VersionNeedSection<ELFT>::getSize() const {
  unsigned Size = Needed.size() * sizeof(Elf_Verneed);
  for (const std::pair<SharedFile<ELFT> *, size_t> &P : Needed)
    Size += P.first->VerdefMap.size() * sizeof(Elf_Vernaux);
  return Size;
}

template <class ELFT> bool VersionNeedSection<ELFT>::empty() const {
  return getNeedNum() == 0;
}

template <class ELFT>
MergeSyntheticSection<ELFT>::MergeSyntheticSection(StringRef Name,
                                                   uint32_t Type, uintX_t Flags,
                                                   uintX_t Alignment)
    : SyntheticSection(Flags, Type, Alignment, Name),
      Builder(StringTableBuilder::RAW, Alignment) {}

template <class ELFT>
void MergeSyntheticSection<ELFT>::addSection(MergeInputSection<ELFT> *MS) {
  assert(!Finalized);
  MS->MergeSec = this;
  Sections.push_back(MS);
}

template <class ELFT> void MergeSyntheticSection<ELFT>::writeTo(uint8_t *Buf) {
  Builder.write(Buf);
}

template <class ELFT>
bool MergeSyntheticSection<ELFT>::shouldTailMerge() const {
  return (this->Flags & SHF_STRINGS) && Config->Optimize >= 2;
}

template <class ELFT> void MergeSyntheticSection<ELFT>::finalizeTailMerge() {
  // Add all string pieces to the string table builder to create section
  // contents.
  for (MergeInputSection<ELFT> *Sec : Sections)
    for (size_t I = 0, E = Sec->Pieces.size(); I != E; ++I)
      if (Sec->Pieces[I].Live)
        Builder.add(Sec->getData(I));

  // Fix the string table content. After this, the contents will never change.
  Builder.finalize();

  // finalize() fixed tail-optimized strings, so we can now get
  // offsets of strings. Get an offset for each string and save it
  // to a corresponding StringPiece for easy access.
  for (MergeInputSection<ELFT> *Sec : Sections)
    for (size_t I = 0, E = Sec->Pieces.size(); I != E; ++I)
      if (Sec->Pieces[I].Live)
        Sec->Pieces[I].OutputOff = Builder.getOffset(Sec->getData(I));
}

template <class ELFT> void MergeSyntheticSection<ELFT>::finalizeNoTailMerge() {
  // Add all string pieces to the string table builder to create section
  // contents. Because we are not tail-optimizing, offsets of strings are
  // fixed when they are added to the builder (string table builder contains
  // a hash table from strings to offsets).
  for (MergeInputSection<ELFT> *Sec : Sections)
    for (size_t I = 0, E = Sec->Pieces.size(); I != E; ++I)
      if (Sec->Pieces[I].Live)
        Sec->Pieces[I].OutputOff = Builder.add(Sec->getData(I));

  Builder.finalizeInOrder();
}

template <class ELFT> void MergeSyntheticSection<ELFT>::finalizeContents() {
  if (Finalized)
    return;
  Finalized = true;
  if (shouldTailMerge())
    finalizeTailMerge();
  else
    finalizeNoTailMerge();
}

template <class ELFT> size_t MergeSyntheticSection<ELFT>::getSize() const {
  // We should finalize string builder to know the size.
  const_cast<MergeSyntheticSection<ELFT> *>(this)->finalizeContents();
  return Builder.getSize();
}

template <class ELFT>
MipsRldMapSection<ELFT>::MipsRldMapSection()
    : SyntheticSection(SHF_ALLOC | SHF_WRITE, SHT_PROGBITS,
                       sizeof(typename ELFT::uint), ".rld_map") {}

template <class ELFT> void MipsRldMapSection<ELFT>::writeTo(uint8_t *Buf) {
  // Apply filler from linker script.
  uint64_t Filler = Script<ELFT>::X->getFiller(this->Name);
  Filler = (Filler << 32) | Filler;
  memcpy(Buf, &Filler, getSize());
}

template <class ELFT>
ARMExidxSentinelSection<ELFT>::ARMExidxSentinelSection()
    : SyntheticSection(SHF_ALLOC | SHF_LINK_ORDER, SHT_ARM_EXIDX,
                       sizeof(typename ELFT::uint), ".ARM.exidx") {}

// Write a terminating sentinel entry to the end of the .ARM.exidx table.
// This section will have been sorted last in the .ARM.exidx table.
// This table entry will have the form:
// | PREL31 upper bound of code that has exception tables | EXIDX_CANTUNWIND |
template <class ELFT>
void ARMExidxSentinelSection<ELFT>::writeTo(uint8_t *Buf) {
  // Get the InputSection before us, we are by definition last
  auto RI = cast<OutputSection>(this->OutSec)->Sections.rbegin();
  InputSection *LE = *(++RI);
  InputSection *LC = cast<InputSection>(LE->template getLinkOrderDep<ELFT>());
  uint64_t S = LC->OutSec->Addr +
               LC->template getOffset<ELFT>(LC->template getSize<ELFT>());
  uint64_t P = this->getVA();
  Target->relocateOne(Buf, R_ARM_PREL31, S - P);
  write32le(Buf + 4, 0x1);
}

template <class ELFT>
ThunkSection<ELFT>::ThunkSection(OutputSection *OS, uint64_t Off)
    : SyntheticSection(SHF_ALLOC | SHF_EXECINSTR, SHT_PROGBITS,
                       sizeof(typename ELFT::uint), ".text.thunk") {
  this->OutSec = OS;
  this->OutSecOff = Off;
}

template <class ELFT> void ThunkSection<ELFT>::addThunk(Thunk<ELFT> *T) {
  uint64_t Off = alignTo(Size, T->alignment);
  T->Offset = Off;
  Thunks.push_back(T);
  T->addSymbols(*this);
  Size = Off + T->size();
}

template <class ELFT> void ThunkSection<ELFT>::writeTo(uint8_t *Buf) {
  for (const Thunk<ELFT> *T : Thunks)
    T->writeTo(Buf + T->Offset, *this);
}

template <class ELFT>
InputSection *ThunkSection<ELFT>::getTargetInputSection() const {
  const Thunk<ELFT> *T = Thunks.front();
  return T->getTargetInputSection();
}

template InputSection *elf::createCommonSection<ELF32LE>();
template InputSection *elf::createCommonSection<ELF32BE>();
template InputSection *elf::createCommonSection<ELF64LE>();
template InputSection *elf::createCommonSection<ELF64BE>();

template MergeInputSection<ELF32LE> *elf::createCommentSection();
template MergeInputSection<ELF32BE> *elf::createCommentSection();
template MergeInputSection<ELF64LE> *elf::createCommentSection();
template MergeInputSection<ELF64BE> *elf::createCommentSection();

template SymbolBody *elf::addSyntheticLocal<ELF32LE>(StringRef, uint8_t,
                                                     uint64_t, uint64_t,
                                                     InputSectionBase *);
template SymbolBody *elf::addSyntheticLocal<ELF32BE>(StringRef, uint8_t,
                                                     uint64_t, uint64_t,
                                                     InputSectionBase *);
template SymbolBody *elf::addSyntheticLocal<ELF64LE>(StringRef, uint8_t,
                                                     uint64_t, uint64_t,
                                                     InputSectionBase *);
template SymbolBody *elf::addSyntheticLocal<ELF64BE>(StringRef, uint8_t,
                                                     uint64_t, uint64_t,
                                                     InputSectionBase *);

template class elf::MipsAbiFlagsSection<ELF32LE>;
template class elf::MipsAbiFlagsSection<ELF32BE>;
template class elf::MipsAbiFlagsSection<ELF64LE>;
template class elf::MipsAbiFlagsSection<ELF64BE>;

template class elf::MipsOptionsSection<ELF32LE>;
template class elf::MipsOptionsSection<ELF32BE>;
template class elf::MipsOptionsSection<ELF64LE>;
template class elf::MipsOptionsSection<ELF64BE>;

template class elf::MipsReginfoSection<ELF32LE>;
template class elf::MipsReginfoSection<ELF32BE>;
template class elf::MipsReginfoSection<ELF64LE>;
template class elf::MipsReginfoSection<ELF64BE>;

template class elf::BuildIdSection<ELF32LE>;
template class elf::BuildIdSection<ELF32BE>;
template class elf::BuildIdSection<ELF64LE>;
template class elf::BuildIdSection<ELF64BE>;

template class elf::CopyRelSection<ELF32LE>;
template class elf::CopyRelSection<ELF32BE>;
template class elf::CopyRelSection<ELF64LE>;
template class elf::CopyRelSection<ELF64BE>;

template class elf::GotSection<ELF32LE>;
template class elf::GotSection<ELF32BE>;
template class elf::GotSection<ELF64LE>;
template class elf::GotSection<ELF64BE>;

template class elf::MipsGotSection<ELF32LE>;
template class elf::MipsGotSection<ELF32BE>;
template class elf::MipsGotSection<ELF64LE>;
template class elf::MipsGotSection<ELF64BE>;

template class elf::GotPltSection<ELF32LE>;
template class elf::GotPltSection<ELF32BE>;
template class elf::GotPltSection<ELF64LE>;
template class elf::GotPltSection<ELF64BE>;

template class elf::IgotPltSection<ELF32LE>;
template class elf::IgotPltSection<ELF32BE>;
template class elf::IgotPltSection<ELF64LE>;
template class elf::IgotPltSection<ELF64BE>;

template class elf::StringTableSection<ELF32LE>;
template class elf::StringTableSection<ELF32BE>;
template class elf::StringTableSection<ELF64LE>;
template class elf::StringTableSection<ELF64BE>;

template class elf::DynamicSection<ELF32LE>;
template class elf::DynamicSection<ELF32BE>;
template class elf::DynamicSection<ELF64LE>;
template class elf::DynamicSection<ELF64BE>;

template class elf::RelocationSection<ELF32LE>;
template class elf::RelocationSection<ELF32BE>;
template class elf::RelocationSection<ELF64LE>;
template class elf::RelocationSection<ELF64BE>;

template class elf::SymbolTableSection<ELF32LE>;
template class elf::SymbolTableSection<ELF32BE>;
template class elf::SymbolTableSection<ELF64LE>;
template class elf::SymbolTableSection<ELF64BE>;

template class elf::GnuHashTableSection<ELF32LE>;
template class elf::GnuHashTableSection<ELF32BE>;
template class elf::GnuHashTableSection<ELF64LE>;
template class elf::GnuHashTableSection<ELF64BE>;

template class elf::HashTableSection<ELF32LE>;
template class elf::HashTableSection<ELF32BE>;
template class elf::HashTableSection<ELF64LE>;
template class elf::HashTableSection<ELF64BE>;

template class elf::PltSection<ELF32LE>;
template class elf::PltSection<ELF32BE>;
template class elf::PltSection<ELF64LE>;
template class elf::PltSection<ELF64BE>;

template class elf::GdbIndexSection<ELF32LE>;
template class elf::GdbIndexSection<ELF32BE>;
template class elf::GdbIndexSection<ELF64LE>;
template class elf::GdbIndexSection<ELF64BE>;

template class elf::EhFrameHeader<ELF32LE>;
template class elf::EhFrameHeader<ELF32BE>;
template class elf::EhFrameHeader<ELF64LE>;
template class elf::EhFrameHeader<ELF64BE>;

template class elf::VersionTableSection<ELF32LE>;
template class elf::VersionTableSection<ELF32BE>;
template class elf::VersionTableSection<ELF64LE>;
template class elf::VersionTableSection<ELF64BE>;

template class elf::VersionNeedSection<ELF32LE>;
template class elf::VersionNeedSection<ELF32BE>;
template class elf::VersionNeedSection<ELF64LE>;
template class elf::VersionNeedSection<ELF64BE>;

template class elf::VersionDefinitionSection<ELF32LE>;
template class elf::VersionDefinitionSection<ELF32BE>;
template class elf::VersionDefinitionSection<ELF64LE>;
template class elf::VersionDefinitionSection<ELF64BE>;

template class elf::MergeSyntheticSection<ELF32LE>;
template class elf::MergeSyntheticSection<ELF32BE>;
template class elf::MergeSyntheticSection<ELF64LE>;
template class elf::MergeSyntheticSection<ELF64BE>;

template class elf::MipsRldMapSection<ELF32LE>;
template class elf::MipsRldMapSection<ELF32BE>;
template class elf::MipsRldMapSection<ELF64LE>;
template class elf::MipsRldMapSection<ELF64BE>;

template class elf::ARMExidxSentinelSection<ELF32LE>;
template class elf::ARMExidxSentinelSection<ELF32BE>;
template class elf::ARMExidxSentinelSection<ELF64LE>;
template class elf::ARMExidxSentinelSection<ELF64BE>;

template class elf::ThunkSection<ELF32LE>;
template class elf::ThunkSection<ELF32BE>;
template class elf::ThunkSection<ELF64LE>;
template class elf::ThunkSection<ELF64BE>;

template class elf::EhFrameSection<ELF32LE>;
template class elf::EhFrameSection<ELF32BE>;
template class elf::EhFrameSection<ELF64LE>;
template class elf::EhFrameSection<ELF64BE>;
