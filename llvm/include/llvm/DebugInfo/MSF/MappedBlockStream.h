//===- MappedBlockStream.h - Discontiguous stream data in an MSF -*- C++
//-*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_DEBUGINFO_MSF_MAPPEDBLOCKSTREAM_H
#define LLVM_DEBUGINFO_MSF_MAPPEDBLOCKSTREAM_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/DebugInfo/MSF/MSFStreamLayout.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/BinaryStream.h"
#include "llvm/Support/BinaryStream.h"
#include "llvm/Support/BinaryStreamRef.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/Error.h"
#include <cstdint>
#include <vector>

namespace llvm {
namespace msf {

struct MSFLayout;

/// MappedBlockStream represents data stored in an MSF file into chunks of a
/// particular size (called the Block Size), and whose chunks may not be
/// necessarily contiguous.  The arrangement of these chunks MSF the file
/// is described by some other metadata contained within the MSF file.  In
/// the case of a standard MSF Stream, the layout of the stream's blocks
/// is described by the MSF "directory", but in the case of the directory
/// itself, the layout is described by an array at a fixed location within
/// the MSF.  MappedBlockStream provides methods for reading from and writing
/// to one of these streams transparently, as if it were a contiguous sequence
/// of bytes.
class MappedBlockStream : public BinaryStream {
  friend class WritableMappedBlockStream;
public:
  static std::unique_ptr<MappedBlockStream>
  createStream(uint32_t BlockSize, uint32_t NumBlocks,
               const MSFStreamLayout &Layout, BinaryStreamRef MsfData);

  static std::unique_ptr<MappedBlockStream>
  createIndexedStream(const MSFLayout &Layout, BinaryStreamRef MsfData,
                      uint32_t StreamIndex);

  static std::unique_ptr<MappedBlockStream>
  createFpmStream(const MSFLayout &Layout, BinaryStreamRef MsfData);

  static std::unique_ptr<MappedBlockStream>
  createDirectoryStream(const MSFLayout &Layout, BinaryStreamRef MsfData);

  llvm::support::endianness getEndian() const override {
    return llvm::support::little;
  }

  Error readBytes(uint32_t Offset, uint32_t Size,
                  ArrayRef<uint8_t> &Buffer) override;
  Error readLongestContiguousChunk(uint32_t Offset,
                                   ArrayRef<uint8_t> &Buffer) override;

  uint32_t getLength() override;

  uint32_t getNumBytesCopied() const;

  llvm::BumpPtrAllocator &getAllocator() { return Pool; }

  void invalidateCache();

  uint32_t getBlockSize() const { return BlockSize; }
  uint32_t getNumBlocks() const { return NumBlocks; }
  uint32_t getStreamLength() const { return StreamLayout.Length; }

protected:
  MappedBlockStream(uint32_t BlockSize, uint32_t NumBlocks,
                    const MSFStreamLayout &StreamLayout,
                    BinaryStreamRef MsfData);

private:
  const MSFStreamLayout &getStreamLayout() const { return StreamLayout; }
  void fixCacheAfterWrite(uint32_t Offset, ArrayRef<uint8_t> Data) const;

  Error readBytes(uint32_t Offset, MutableArrayRef<uint8_t> Buffer);
  bool tryReadContiguously(uint32_t Offset, uint32_t Size,
                           ArrayRef<uint8_t> &Buffer);

  const uint32_t BlockSize;
  const uint32_t NumBlocks;
  const MSFStreamLayout StreamLayout;
  BinaryStreamRef MsfData;

  typedef MutableArrayRef<uint8_t> CacheEntry;
  llvm::BumpPtrAllocator Pool;
  DenseMap<uint32_t, std::vector<CacheEntry>> CacheMap;
};

class WritableMappedBlockStream : public WritableBinaryStream {
public:
  static std::unique_ptr<WritableMappedBlockStream>
  createStream(uint32_t BlockSize, uint32_t NumBlocks,
               const MSFStreamLayout &Layout, WritableBinaryStreamRef MsfData);

  static std::unique_ptr<WritableMappedBlockStream>
  createIndexedStream(const MSFLayout &Layout, WritableBinaryStreamRef MsfData,
                      uint32_t StreamIndex);

  static std::unique_ptr<WritableMappedBlockStream>
  createDirectoryStream(const MSFLayout &Layout,
                        WritableBinaryStreamRef MsfData);

  static std::unique_ptr<WritableMappedBlockStream>
  createFpmStream(const MSFLayout &Layout, WritableBinaryStreamRef MsfData);

  llvm::support::endianness getEndian() const override {
    return llvm::support::little;
  }

  Error readBytes(uint32_t Offset, uint32_t Size,
                  ArrayRef<uint8_t> &Buffer) override;
  Error readLongestContiguousChunk(uint32_t Offset,
                                   ArrayRef<uint8_t> &Buffer) override;
  uint32_t getLength() override;

  Error writeBytes(uint32_t Offset, ArrayRef<uint8_t> Buffer) override;

  Error commit() override;

  const MSFStreamLayout &getStreamLayout() const {
    return ReadInterface.getStreamLayout();
  }
  uint32_t getBlockSize() const { return ReadInterface.getBlockSize(); }
  uint32_t getNumBlocks() const { return ReadInterface.getNumBlocks(); }
  uint32_t getStreamLength() const { return ReadInterface.getStreamLength(); }

protected:
  WritableMappedBlockStream(uint32_t BlockSize, uint32_t NumBlocks,
                            const MSFStreamLayout &StreamLayout,
                            WritableBinaryStreamRef MsfData);

private:
  MappedBlockStream ReadInterface;

  WritableBinaryStreamRef WriteInterface;
};

} // end namespace pdb
} // end namespace llvm

#endif // LLVM_DEBUGINFO_MSF_MAPPEDBLOCKSTREAM_H
