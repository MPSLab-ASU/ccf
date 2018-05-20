//===- SyntheticSection.h ---------------------------------------*- C++ -*-===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Synthetic sections represent chunks of linker-created data. If you
// need to create a chunk of data that to be included in some section
// in the result, you probably want to create that as a synthetic section.
//
// Synthetic sections are designed as input sections as opposed to
// output sections because we want to allow them to be manipulated
// using linker scripts just like other input sections from regular
// files.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_ELF_SYNTHETIC_SECTION_H
#define LLD_ELF_SYNTHETIC_SECTION_H

#include "EhFrame.h"
#include "GdbIndex.h"
#include "InputSection.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/MC/StringTableBuilder.h"

namespace lld {
namespace elf {

class SyntheticSection : public InputSection {
public:
  SyntheticSection(uint64_t Flags, uint32_t Type, uint64_t Addralign,
                   StringRef Name)
      : InputSection(Flags, Type, Addralign, {}, Name,
                     InputSectionBase::Synthetic) {
    this->Live = true;
  }

  virtual ~SyntheticSection() = default;
  virtual void writeTo(uint8_t *Buf) = 0;
  virtual size_t getSize() const = 0;
  virtual void finalizeContents() {}
  virtual bool empty() const { return false; }
  uint64_t getVA() const;

  static bool classof(const InputSectionBase *D) {
    return D->kind() == InputSectionBase::Synthetic;
  }
};

struct CieRecord {
  EhSectionPiece *Piece = nullptr;
  std::vector<EhSectionPiece *> FdePieces;
};

// Section for .eh_frame.
template <class ELFT> class EhFrameSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;
  typedef typename ELFT::Shdr Elf_Shdr;
  typedef typename ELFT::Rel Elf_Rel;
  typedef typename ELFT::Rela Elf_Rela;

  void updateAlignment(uint64_t Val) {
    if (Val > this->Alignment)
      this->Alignment = Val;
  }

public:
  EhFrameSection();
  void writeTo(uint8_t *Buf) override;
  void finalizeContents() override;
  bool empty() const override { return Sections.empty(); }
  size_t getSize() const override { return Size; }

  void addSection(InputSectionBase *S);

  size_t NumFdes = 0;

private:
  uint64_t Size = 0;
  template <class RelTy>
  void addSectionAux(EhInputSection<ELFT> *S, llvm::ArrayRef<RelTy> Rels);

  template <class RelTy>
  CieRecord *addCie(EhSectionPiece &Piece, ArrayRef<RelTy> Rels);

  template <class RelTy>
  bool isFdeLive(EhSectionPiece &Piece, ArrayRef<RelTy> Rels);

  uintX_t getFdePc(uint8_t *Buf, size_t Off, uint8_t Enc);

  std::vector<EhInputSection<ELFT> *> Sections;
  std::vector<CieRecord *> Cies;

  // CIE records are uniquified by their contents and personality functions.
  llvm::DenseMap<std::pair<ArrayRef<uint8_t>, SymbolBody *>, CieRecord> CieMap;
};

template <class ELFT> class GotSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  GotSection();
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return Size; }
  void finalizeContents() override;
  bool empty() const override;

  void addEntry(SymbolBody &Sym);
  bool addDynTlsEntry(SymbolBody &Sym);
  bool addTlsIndex();
  uintX_t getGlobalDynAddr(const SymbolBody &B) const;
  uintX_t getGlobalDynOffset(const SymbolBody &B) const;

  uintX_t getTlsIndexVA() { return this->getVA() + TlsIndexOff; }
  uint32_t getTlsIndexOff() const { return TlsIndexOff; }

  // Flag to force GOT to be in output if we have relocations
  // that relies on its address.
  bool HasGotOffRel = false;

private:
  size_t NumEntries = 0;
  uint32_t TlsIndexOff = -1;
  uintX_t Size = 0;
};

// .note.gnu.build-id section.
template <class ELFT> class BuildIdSection : public SyntheticSection {
  // First 16 bytes are a header.
  static const unsigned HeaderSize = 16;

public:
  BuildIdSection();
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return HeaderSize + HashSize; }
  void writeBuildId(llvm::ArrayRef<uint8_t> Buf);

private:
  void computeHash(llvm::ArrayRef<uint8_t> Buf,
                   std::function<void(uint8_t *, ArrayRef<uint8_t>)> Hash);

  size_t HashSize;
  uint8_t *HashBuf;
};

// For each copy relocation, we create an instance of this class to
// reserve space in .bss or .bss.rel.ro.
template <class ELFT> class CopyRelSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  CopyRelSection(bool ReadOnly, uintX_t AddrAlign, size_t Size);
  void writeTo(uint8_t *) override {}
  size_t getSize() const override { return Size; }
  size_t Size;
};

template <class ELFT> class MipsGotSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  MipsGotSection();
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return Size; }
  void finalizeContents() override;
  bool empty() const override;
  void addEntry(SymbolBody &Sym, int64_t Addend, RelExpr Expr);
  bool addDynTlsEntry(SymbolBody &Sym);
  bool addTlsIndex();
  uintX_t getPageEntryOffset(const SymbolBody &B, int64_t Addend) const;
  uintX_t getBodyEntryOffset(const SymbolBody &B, int64_t Addend) const;
  uintX_t getGlobalDynOffset(const SymbolBody &B) const;

  // Returns the symbol which corresponds to the first entry of the global part
  // of GOT on MIPS platform. It is required to fill up MIPS-specific dynamic
  // table properties.
  // Returns nullptr if the global part is empty.
  const SymbolBody *getFirstGlobalEntry() const;

  // Returns the number of entries in the local part of GOT including
  // the number of reserved entries.
  unsigned getLocalEntriesNum() const;

  // Returns offset of TLS part of the MIPS GOT table. This part goes
  // after 'local' and 'global' entries.
  uintX_t getTlsOffset() const;

  uint32_t getTlsIndexOff() const { return TlsIndexOff; }

  uintX_t getGp() const;

private:
  // MIPS GOT consists of three parts: local, global and tls. Each part
  // contains different types of entries. Here is a layout of GOT:
  // - Header entries                |
  // - Page entries                  |   Local part
  // - Local entries (16-bit access) |
  // - Local entries (32-bit access) |
  // - Normal global entries         ||  Global part
  // - Reloc-only global entries     ||
  // - TLS entries                   ||| TLS part
  //
  // Header:
  //   Two entries hold predefined value 0x0 and 0x80000000.
  // Page entries:
  //   These entries created by R_MIPS_GOT_PAGE relocation and R_MIPS_GOT16
  //   relocation against local symbols. They are initialized by higher 16-bit
  //   of the corresponding symbol's value. So each 64kb of address space
  //   requires a single GOT entry.
  // Local entries (16-bit access):
  //   These entries created by GOT relocations against global non-preemptible
  //   symbols so dynamic linker is not necessary to resolve the symbol's
  //   values. "16-bit access" means that corresponding relocations address
  //   GOT using 16-bit index. Each unique Symbol-Addend pair has its own
  //   GOT entry.
  // Local entries (32-bit access):
  //   These entries are the same as above but created by relocations which
  //   address GOT using 32-bit index (R_MIPS_GOT_HI16/LO16 etc).
  // Normal global entries:
  //   These entries created by GOT relocations against preemptible global
  //   symbols. They need to be initialized by dynamic linker and they ordered
  //   exactly as the corresponding entries in the dynamic symbols table.
  // Reloc-only global entries:
  //   These entries created for symbols that are referenced by dynamic
  //   relocations R_MIPS_REL32. These entries are not accessed with gp-relative
  //   addressing, but MIPS ABI requires that these entries be present in GOT.
  // TLS entries:
  //   Entries created by TLS relocations.

  // Number of "Header" entries.
  static const unsigned HeaderEntriesNum = 2;
  // Number of allocated "Page" entries.
  uint32_t PageEntriesNum = 0;
  // Map output sections referenced by MIPS GOT relocations
  // to the first index of "Page" entries allocated for this section.
  llvm::SmallMapVector<const OutputSection *, size_t, 16> PageIndexMap;

  typedef std::pair<const SymbolBody *, uintX_t> GotEntry;
  typedef std::vector<GotEntry> GotEntries;
  // Map from Symbol-Addend pair to the GOT index.
  llvm::DenseMap<GotEntry, size_t> EntryIndexMap;
  // Local entries (16-bit access).
  GotEntries LocalEntries;
  // Local entries (32-bit access).
  GotEntries LocalEntries32;

  // Normal and reloc-only global entries.
  GotEntries GlobalEntries;

  // TLS entries.
  std::vector<const SymbolBody *> TlsEntries;

  uint32_t TlsIndexOff = -1;
  uintX_t Size = 0;
};

template <class ELFT> class GotPltSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  GotPltSection();
  void addEntry(SymbolBody &Sym);
  size_t getSize() const override;
  void writeTo(uint8_t *Buf) override;
  bool empty() const override { return Entries.empty(); }

private:
  std::vector<const SymbolBody *> Entries;
};

// The IgotPltSection is a Got associated with the PltSection for GNU Ifunc
// Symbols that will be relocated by Target->IRelativeRel.
// On most Targets the IgotPltSection will immediately follow the GotPltSection
// on ARM the IgotPltSection will immediately follow the GotSection.
template <class ELFT> class IgotPltSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  IgotPltSection();
  void addEntry(SymbolBody &Sym);
  size_t getSize() const override;
  void writeTo(uint8_t *Buf) override;
  bool empty() const override { return Entries.empty(); }

private:
  std::vector<const SymbolBody *> Entries;
};

template <class ELFT> class StringTableSection final : public SyntheticSection {
public:
  typedef typename ELFT::uint uintX_t;
  StringTableSection(StringRef Name, bool Dynamic);
  unsigned addString(StringRef S, bool HashIt = true);
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return Size; }
  bool isDynamic() const { return Dynamic; }

private:
  const bool Dynamic;

  uintX_t Size = 0;

  llvm::DenseMap<StringRef, unsigned> StringMap;
  std::vector<StringRef> Strings;
};

template <class ELFT> class DynamicReloc {
  typedef typename ELFT::uint uintX_t;

public:
  DynamicReloc(uint32_t Type, const InputSectionBase *InputSec,
               uintX_t OffsetInSec, bool UseSymVA, SymbolBody *Sym,
               int64_t Addend)
      : Type(Type), Sym(Sym), InputSec(InputSec), OffsetInSec(OffsetInSec),
        UseSymVA(UseSymVA), Addend(Addend) {}

  uintX_t getOffset() const;
  int64_t getAddend() const;
  uint32_t getSymIndex() const;
  const InputSectionBase *getInputSec() const { return InputSec; }

  uint32_t Type;

private:
  SymbolBody *Sym;
  const InputSectionBase *InputSec = nullptr;
  uintX_t OffsetInSec;
  bool UseSymVA;
  int64_t Addend;
};

template <class ELFT> class DynamicSection final : public SyntheticSection {
  typedef typename ELFT::Dyn Elf_Dyn;
  typedef typename ELFT::Rel Elf_Rel;
  typedef typename ELFT::Rela Elf_Rela;
  typedef typename ELFT::Shdr Elf_Shdr;
  typedef typename ELFT::Sym Elf_Sym;
  typedef typename ELFT::uint uintX_t;

  // The .dynamic section contains information for the dynamic linker.
  // The section consists of fixed size entries, which consist of
  // type and value fields. Value are one of plain integers, symbol
  // addresses, or section addresses. This struct represents the entry.
  struct Entry {
    int32_t Tag;
    union {
      OutputSection *OutSec;
      InputSection *InSec;
      uint64_t Val;
      const SymbolBody *Sym;
    };
    enum KindT { SecAddr, SecSize, SymAddr, PlainInt, InSecAddr } Kind;
    Entry(int32_t Tag, OutputSection *OutSec, KindT Kind = SecAddr)
        : Tag(Tag), OutSec(OutSec), Kind(Kind) {}
    Entry(int32_t Tag, InputSection *Sec)
        : Tag(Tag), InSec(Sec), Kind(InSecAddr) {}
    Entry(int32_t Tag, uint64_t Val) : Tag(Tag), Val(Val), Kind(PlainInt) {}
    Entry(int32_t Tag, const SymbolBody *Sym)
        : Tag(Tag), Sym(Sym), Kind(SymAddr) {}
  };

  // finalizeContents() fills this vector with the section contents.
  // finalizeContents()cannot directly create final section contents because
  // when the function is called, symbol or section addresses are not fixed yet.
  std::vector<Entry> Entries;

public:
  DynamicSection();
  void finalizeContents() override;
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return Size; }

private:
  void addEntries();
  void add(Entry E) { Entries.push_back(E); }
  uintX_t Size = 0;
};

template <class ELFT> class RelocationSection final : public SyntheticSection {
  typedef typename ELFT::Rel Elf_Rel;
  typedef typename ELFT::Rela Elf_Rela;
  typedef typename ELFT::uint uintX_t;

public:
  RelocationSection(StringRef Name, bool Sort);
  void addReloc(const DynamicReloc<ELFT> &Reloc);
  unsigned getRelocOffset();
  void finalizeContents() override;
  void writeTo(uint8_t *Buf) override;
  bool empty() const override { return Relocs.empty(); }
  size_t getSize() const override { return Relocs.size() * this->Entsize; }
  size_t getRelativeRelocCount() const { return NumRelativeRelocs; }

private:
  bool Sort;
  size_t NumRelativeRelocs = 0;
  std::vector<DynamicReloc<ELFT>> Relocs;
};

struct SymbolTableEntry {
  SymbolBody *Symbol;
  size_t StrTabOffset;
};

template <class ELFT> class SymbolTableSection final : public SyntheticSection {
public:
  typedef typename ELFT::Sym Elf_Sym;
  typedef typename ELFT::uint uintX_t;

  SymbolTableSection(StringTableSection<ELFT> &StrTabSec);

  void finalizeContents() override;
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return getNumSymbols() * sizeof(Elf_Sym); }
  void addSymbol(SymbolBody *Body);
  unsigned getNumSymbols() const { return Symbols.size() + 1; }
  size_t getSymbolIndex(SymbolBody *Body);
  ArrayRef<SymbolTableEntry> getSymbols() const { return Symbols; }

private:
  // A vector of symbols and their string table offsets.
  std::vector<SymbolTableEntry> Symbols;

  StringTableSection<ELFT> &StrTabSec;
};

// Outputs GNU Hash section. For detailed explanation see:
// https://blogs.oracle.com/ali/entry/gnu_hash_elf_sections
template <class ELFT>
class GnuHashTableSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  GnuHashTableSection();
  void finalizeContents() override;
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return Size; }

  // Adds symbols to the hash table.
  // Sorts the input to satisfy GNU hash section requirements.
  void addSymbols(std::vector<SymbolTableEntry> &Symbols);

private:
  size_t getShift2() const { return ELFT::Is64Bits ? 6 : 5; }

  void writeBloomFilter(uint8_t *Buf);
  void writeHashTable(uint8_t *Buf);

  struct Entry {
    SymbolBody *Body;
    size_t StrTabOffset;
    uint32_t Hash;
  };

  std::vector<Entry> Symbols;
  size_t MaskWords;
  size_t NBuckets = 0;
  size_t Size = 0;
};

template <class ELFT> class HashTableSection final : public SyntheticSection {
public:
  HashTableSection();
  void finalizeContents() override;
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override { return Size; }

private:
  size_t Size = 0;
};

// The PltSection is used for both the Plt and Iplt. The former always has a
// header as its first entry that is used at run-time to resolve lazy binding.
// The latter is used for GNU Ifunc symbols, that will be subject to a
// Target->IRelativeRel.
template <class ELFT> class PltSection : public SyntheticSection {
public:
  PltSection(size_t HeaderSize);
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override;
  void addEntry(SymbolBody &Sym);
  bool empty() const override { return Entries.empty(); }
  void addSymbols();

private:
  void writeHeader(uint8_t *Buf){};
  void addHeaderSymbols(){};
  unsigned getPltRelocOff() const;
  std::vector<std::pair<const SymbolBody *, unsigned>> Entries;
  // Iplt always has HeaderSize of 0, the Plt HeaderSize is always non-zero
  size_t HeaderSize;
};

template <class ELFT> class GdbIndexSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

  const unsigned OffsetTypeSize = 4;
  const unsigned CuListOffset = 6 * OffsetTypeSize;
  const unsigned CompilationUnitSize = 16;
  const unsigned AddressEntrySize = 16 + OffsetTypeSize;
  const unsigned SymTabEntrySize = 2 * OffsetTypeSize;

public:
  GdbIndexSection();
  void finalizeContents() override;
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override;
  bool empty() const override;

  // Pairs of [CU Offset, CU length].
  std::vector<std::pair<uintX_t, uintX_t>> CompilationUnits;

  llvm::StringTableBuilder StringPool;

  GdbHashTab SymbolTable;

  // The CU vector portion of the constant pool.
  std::vector<std::vector<std::pair<uint32_t, uint8_t>>> CuVectors;

  std::vector<AddressEntry> AddressArea;

private:
  void readDwarf(InputSection *Sec);

  uint32_t CuTypesOffset;
  uint32_t SymTabOffset;
  uint32_t ConstantPoolOffset;
  uint32_t StringPoolOffset;

  size_t CuVectorsSize = 0;
  std::vector<size_t> CuVectorsOffset;

  bool Finalized = false;
};

// --eh-frame-hdr option tells linker to construct a header for all the
// .eh_frame sections. This header is placed to a section named .eh_frame_hdr
// and also to a PT_GNU_EH_FRAME segment.
// At runtime the unwinder then can find all the PT_GNU_EH_FRAME segments by
// calling dl_iterate_phdr.
// This section contains a lookup table for quick binary search of FDEs.
// Detailed info about internals can be found in Ian Lance Taylor's blog:
// http://www.airs.com/blog/archives/460 (".eh_frame")
// http://www.airs.com/blog/archives/462 (".eh_frame_hdr")
template <class ELFT> class EhFrameHeader final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  EhFrameHeader();
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override;
  void addFde(uint32_t Pc, uint32_t FdeVA);
  bool empty() const override;

private:
  struct FdeData {
    uint32_t Pc;
    uint32_t FdeVA;
  };

  std::vector<FdeData> Fdes;
};

// For more information about .gnu.version and .gnu.version_r see:
// https://www.akkadia.org/drepper/symbol-versioning

// The .gnu.version_d section which has a section type of SHT_GNU_verdef shall
// contain symbol version definitions. The number of entries in this section
// shall be contained in the DT_VERDEFNUM entry of the .dynamic section.
// The section shall contain an array of Elf_Verdef structures, optionally
// followed by an array of Elf_Verdaux structures.
template <class ELFT>
class VersionDefinitionSection final : public SyntheticSection {
  typedef typename ELFT::Verdef Elf_Verdef;
  typedef typename ELFT::Verdaux Elf_Verdaux;

public:
  VersionDefinitionSection();
  void finalizeContents() override;
  size_t getSize() const override;
  void writeTo(uint8_t *Buf) override;

private:
  void writeOne(uint8_t *Buf, uint32_t Index, StringRef Name, size_t NameOff);

  unsigned FileDefNameOff;
};

// The .gnu.version section specifies the required version of each symbol in the
// dynamic symbol table. It contains one Elf_Versym for each dynamic symbol
// table entry. An Elf_Versym is just a 16-bit integer that refers to a version
// identifier defined in the either .gnu.version_r or .gnu.version_d section.
// The values 0 and 1 are reserved. All other values are used for versions in
// the own object or in any of the dependencies.
template <class ELFT>
class VersionTableSection final : public SyntheticSection {
  typedef typename ELFT::Versym Elf_Versym;

public:
  VersionTableSection();
  void finalizeContents() override;
  size_t getSize() const override;
  void writeTo(uint8_t *Buf) override;
  bool empty() const override;
};

// The .gnu.version_r section defines the version identifiers used by
// .gnu.version. It contains a linked list of Elf_Verneed data structures. Each
// Elf_Verneed specifies the version requirements for a single DSO, and contains
// a reference to a linked list of Elf_Vernaux data structures which define the
// mapping from version identifiers to version names.
template <class ELFT> class VersionNeedSection final : public SyntheticSection {
  typedef typename ELFT::Verneed Elf_Verneed;
  typedef typename ELFT::Vernaux Elf_Vernaux;

  // A vector of shared files that need Elf_Verneed data structures and the
  // string table offsets of their sonames.
  std::vector<std::pair<SharedFile<ELFT> *, size_t>> Needed;

  // The next available version identifier.
  unsigned NextIndex;

public:
  VersionNeedSection();
  void addSymbol(SharedSymbol *SS);
  void finalizeContents() override;
  void writeTo(uint8_t *Buf) override;
  size_t getSize() const override;
  size_t getNeedNum() const { return Needed.size(); }
  bool empty() const override;
};

// MergeSyntheticSection is a class that allows us to put mergeable sections
// with different attributes in a single output sections. To do that
// we put them into MergeSyntheticSection synthetic input sections which are
// attached to regular output sections.
template <class ELFT>
class MergeSyntheticSection final : public SyntheticSection {
  typedef typename ELFT::uint uintX_t;

public:
  MergeSyntheticSection(StringRef Name, uint32_t Type, uintX_t Flags,
                        uintX_t Alignment);
  void addSection(MergeInputSection<ELFT> *MS);
  void writeTo(uint8_t *Buf) override;
  void finalizeContents() override;
  bool shouldTailMerge() const;
  size_t getSize() const override;

private:
  void finalizeTailMerge();
  void finalizeNoTailMerge();

  bool Finalized = false;
  llvm::StringTableBuilder Builder;
  std::vector<MergeInputSection<ELFT> *> Sections;
};

// .MIPS.abiflags section.
template <class ELFT>
class MipsAbiFlagsSection final : public SyntheticSection {
  typedef llvm::object::Elf_Mips_ABIFlags<ELFT> Elf_Mips_ABIFlags;

public:
  static MipsAbiFlagsSection *create();

  MipsAbiFlagsSection(Elf_Mips_ABIFlags Flags);
  size_t getSize() const override { return sizeof(Elf_Mips_ABIFlags); }
  void writeTo(uint8_t *Buf) override;

private:
  Elf_Mips_ABIFlags Flags;
};

// .MIPS.options section.
template <class ELFT> class MipsOptionsSection final : public SyntheticSection {
  typedef llvm::object::Elf_Mips_Options<ELFT> Elf_Mips_Options;
  typedef llvm::object::Elf_Mips_RegInfo<ELFT> Elf_Mips_RegInfo;

public:
  static MipsOptionsSection *create();

  MipsOptionsSection(Elf_Mips_RegInfo Reginfo);
  void writeTo(uint8_t *Buf) override;

  size_t getSize() const override {
    return sizeof(Elf_Mips_Options) + sizeof(Elf_Mips_RegInfo);
  }

private:
  Elf_Mips_RegInfo Reginfo;
};

// MIPS .reginfo section.
template <class ELFT> class MipsReginfoSection final : public SyntheticSection {
  typedef llvm::object::Elf_Mips_RegInfo<ELFT> Elf_Mips_RegInfo;

public:
  static MipsReginfoSection *create();

  MipsReginfoSection(Elf_Mips_RegInfo Reginfo);
  size_t getSize() const override { return sizeof(Elf_Mips_RegInfo); }
  void writeTo(uint8_t *Buf) override;

private:
  Elf_Mips_RegInfo Reginfo;
};

// This is a MIPS specific section to hold a space within the data segment
// of executable file which is pointed to by the DT_MIPS_RLD_MAP entry.
// See "Dynamic section" in Chapter 5 in the following document:
// ftp://www.linux-mips.org/pub/linux/mips/doc/ABI/mipsabi.pdf
template <class ELFT> class MipsRldMapSection : public SyntheticSection {
public:
  MipsRldMapSection();
  size_t getSize() const override { return sizeof(typename ELFT::uint); }
  void writeTo(uint8_t *Buf) override;
};

template <class ELFT> class ARMExidxSentinelSection : public SyntheticSection {
public:
  ARMExidxSentinelSection();
  size_t getSize() const override { return 8; }
  void writeTo(uint8_t *Buf) override;
};

// A container for one or more linker generated thunks. Instances of these
// thunks including ARM interworking and Mips LA25 PI to non-PI thunks.
template <class ELFT> class ThunkSection : public SyntheticSection {
public:
  // ThunkSection in OS, with desired OutSecOff of Off
  ThunkSection(OutputSection *OS, uint64_t Off);

  // Add a newly created Thunk to this container:
  // Thunk is given offset from start of this InputSection
  // Thunk defines a symbol in this InputSection that can be used as target
  // of a relocation
  void addThunk(Thunk<ELFT> *T);
  size_t getSize() const override { return Size; }
  void writeTo(uint8_t *Buf) override;
  InputSection *getTargetInputSection() const;

private:
  std::vector<const Thunk<ELFT> *> Thunks;
  size_t Size = 0;
};

template <class ELFT> InputSection *createCommonSection();
InputSection *createInterpSection();
template <class ELFT> MergeInputSection<ELFT> *createCommentSection();
template <class ELFT>
SymbolBody *addSyntheticLocal(StringRef Name, uint8_t Type, uint64_t Value,
                              uint64_t Size, InputSectionBase *Section);

// Linker generated sections which can be used as inputs.
template <class ELFT> struct In {
  static InputSection *ARMAttributes;
  static BuildIdSection<ELFT> *BuildId;
  static InputSection *Common;
  static DynamicSection<ELFT> *Dynamic;
  static StringTableSection<ELFT> *DynStrTab;
  static SymbolTableSection<ELFT> *DynSymTab;
  static EhFrameHeader<ELFT> *EhFrameHdr;
  static GnuHashTableSection<ELFT> *GnuHashTab;
  static GdbIndexSection<ELFT> *GdbIndex;
  static GotSection<ELFT> *Got;
  static EhFrameSection<ELFT> *EhFrame;
  static MipsGotSection<ELFT> *MipsGot;
  static GotPltSection<ELFT> *GotPlt;
  static IgotPltSection<ELFT> *IgotPlt;
  static HashTableSection<ELFT> *HashTab;
  static InputSection *Interp;
  static MipsRldMapSection<ELFT> *MipsRldMap;
  static PltSection<ELFT> *Plt;
  static PltSection<ELFT> *Iplt;
  static RelocationSection<ELFT> *RelaDyn;
  static RelocationSection<ELFT> *RelaPlt;
  static RelocationSection<ELFT> *RelaIplt;
  static StringTableSection<ELFT> *ShStrTab;
  static StringTableSection<ELFT> *StrTab;
  static SymbolTableSection<ELFT> *SymTab;
  static VersionDefinitionSection<ELFT> *VerDef;
  static VersionTableSection<ELFT> *VerSym;
  static VersionNeedSection<ELFT> *VerNeed;
};

template <class ELFT> InputSection *In<ELFT>::ARMAttributes;
template <class ELFT> BuildIdSection<ELFT> *In<ELFT>::BuildId;
template <class ELFT> InputSection *In<ELFT>::Common;
template <class ELFT> DynamicSection<ELFT> *In<ELFT>::Dynamic;
template <class ELFT> StringTableSection<ELFT> *In<ELFT>::DynStrTab;
template <class ELFT> SymbolTableSection<ELFT> *In<ELFT>::DynSymTab;
template <class ELFT> EhFrameHeader<ELFT> *In<ELFT>::EhFrameHdr;
template <class ELFT> GdbIndexSection<ELFT> *In<ELFT>::GdbIndex;
template <class ELFT> GnuHashTableSection<ELFT> *In<ELFT>::GnuHashTab;
template <class ELFT> GotSection<ELFT> *In<ELFT>::Got;
template <class ELFT> EhFrameSection<ELFT> *In<ELFT>::EhFrame;
template <class ELFT> MipsGotSection<ELFT> *In<ELFT>::MipsGot;
template <class ELFT> GotPltSection<ELFT> *In<ELFT>::GotPlt;
template <class ELFT> IgotPltSection<ELFT> *In<ELFT>::IgotPlt;
template <class ELFT> HashTableSection<ELFT> *In<ELFT>::HashTab;
template <class ELFT> InputSection *In<ELFT>::Interp;
template <class ELFT> MipsRldMapSection<ELFT> *In<ELFT>::MipsRldMap;
template <class ELFT> PltSection<ELFT> *In<ELFT>::Plt;
template <class ELFT> PltSection<ELFT> *In<ELFT>::Iplt;
template <class ELFT> RelocationSection<ELFT> *In<ELFT>::RelaDyn;
template <class ELFT> RelocationSection<ELFT> *In<ELFT>::RelaPlt;
template <class ELFT> RelocationSection<ELFT> *In<ELFT>::RelaIplt;
template <class ELFT> StringTableSection<ELFT> *In<ELFT>::ShStrTab;
template <class ELFT> StringTableSection<ELFT> *In<ELFT>::StrTab;
template <class ELFT> SymbolTableSection<ELFT> *In<ELFT>::SymTab;
template <class ELFT> VersionDefinitionSection<ELFT> *In<ELFT>::VerDef;
template <class ELFT> VersionTableSection<ELFT> *In<ELFT>::VerSym;
template <class ELFT> VersionNeedSection<ELFT> *In<ELFT>::VerNeed;
} // namespace elf
} // namespace lld

#endif
