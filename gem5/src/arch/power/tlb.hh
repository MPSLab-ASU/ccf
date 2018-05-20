/*
 * Copyright (c) 2001-2005 The Regents of The University of Michigan
 * Copyright (c) 2007 MIPS Technologies, Inc.
 * Copyright (c) 2007-2008 The Florida State University
 * Copyright (c) 2009 The University of Edinburgh
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Nathan Binkert
 *          Steve Reinhardt
 *          Stephen Hines
 *          Timothy M. Jones
 */

#ifndef __ARCH_POWER_TLB_HH__
#define __ARCH_POWER_TLB_HH__

#include <map>

#include "arch/power/isa_traits.hh"
#include "arch/power/pagetable.hh"
#include "arch/power/utility.hh"
#include "arch/power/vtophys.hh"
#include "base/statistics.hh"
#include "mem/request.hh"
#include "params/PowerTLB.hh"
#include "sim/fault_fwd.hh"
#include "sim/tlb.hh"

class ThreadContext;

namespace PowerISA {

// This is copied from the ARM ISA and has not been checked against the
// Power at all.
struct TlbEntry
{
    Addr _pageStart;

    TlbEntry()
    {
    }

    TlbEntry(Addr asn, Addr vaddr, Addr paddr)
        : _pageStart(paddr)
    {
    }

    void
    updateVaddr(Addr new_vaddr)
    {
        panic("unimplemented");
    }

    Addr
    pageStart()
    {
        return _pageStart;
    }

    void
    serialize(std::ostream &os)
    {
        SERIALIZE_SCALAR(_pageStart);
    }

    void
    unserialize(Checkpoint *cp, const std::string &section)
    {
        UNSERIALIZE_SCALAR(_pageStart);
    }
};

class TLB : public BaseTLB
{
  protected:
    typedef std::multimap<Addr, int> PageTable;
    PageTable lookupTable;      // Quick lookup into page table

    PowerISA::PTE *table;       // the Page Table
    int size;                   // TLB Size
    int nlu;                    // not last used entry (for replacement)

    void
    nextnlu()
    {
        if (++nlu >= size) {
            nlu = 0;
        }
    }

    PowerISA::PTE *lookup(Addr vpn, uint8_t asn) const;

    mutable Stats::Scalar read_hits;
    mutable Stats::Scalar read_misses;
    mutable Stats::Scalar read_acv;
    mutable Stats::Scalar read_accesses;
    mutable Stats::Scalar write_hits;
    mutable Stats::Scalar write_misses;
    mutable Stats::Scalar write_acv;
    mutable Stats::Scalar write_accesses;
    Stats::Formula hits;
    Stats::Formula misses;
    Stats::Formula accesses;

  public:
    typedef PowerTLBParams Params;
    TLB(const Params *p);
    virtual ~TLB();

    int probeEntry(Addr vpn,uint8_t) const;
    PowerISA::PTE *getEntry(unsigned) const;

    int smallPages;

    int
    getsize() const
    {
        return size;
    }

    PowerISA::PTE &index(bool advance = true);
    void insert(Addr vaddr, PowerISA::PTE &pte);
    void insertAt(PowerISA::PTE &pte, unsigned Index, int _smallPages);
    void flushAll();

    void
    demapPage(Addr vaddr, uint64_t asn)
    {
        panic("demapPage unimplemented.\n");
    }

    // static helper functions... really
    static bool validVirtualAddress(Addr vaddr);
    static Fault checkCacheability(RequestPtr &req);
    Fault translateInst(RequestPtr req, ThreadContext *tc);
    Fault translateData(RequestPtr req, ThreadContext *tc, bool write);
    Fault translateAtomic(RequestPtr req, ThreadContext *tc, Mode mode);
    void translateTiming(RequestPtr req, ThreadContext *tc,
                         Translation *translation, Mode mode);
    /** Stub function for CheckerCPU compilation support.  Power ISA not
     *  supported by Checker at the moment
     */
    Fault translateFunctional(RequestPtr req, ThreadContext *tc, Mode mode);
    Fault finalizePhysical(RequestPtr req, ThreadContext *tc, Mode mode) const;

    // Checkpointing
    void serialize(std::ostream &os);
    void unserialize(Checkpoint *cp, const std::string &section);
    void regStats();
};

} // namespace PowerISA

#endif // __ARCH_POWER_TLB_HH__
