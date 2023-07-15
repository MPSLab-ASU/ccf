/*
 * Copyright (c) 2011-2012,2015,2018,2020 ARM Limited
 * Copyright (c) 2013 Advanced Micro Devices, Inc.
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2002-2005 The Regents of The University of Michigan
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
 */

#ifndef __CPU_SIMPLE_BASE_HH__
#define __CPU_SIMPLE_BASE_HH__

#include "base/statistics.hh"
#include "config/the_isa.hh"
#include "cpu/base.hh"
#include "cpu/checker/cpu.hh"
#include "cpu/exec_context.hh"
#include "cpu/pc_event.hh"
#include "cpu/simple_thread.hh"
#include "cpu/static_inst.hh"
#include "mem/packet.hh"
#include "mem/port.hh"
#include "mem/request.hh"
#include "sim/eventq.hh"
#include "sim/full_system.hh"
#include "sim/system.hh"

// forward declarations
class Checkpoint;
class Process;
class Processor;
class ThreadContext;

namespace TheISA
{
    class DTB;
    class ITB;
}

namespace Trace {
    class InstRecord;
}

struct BaseSimpleCPUParams;
class BPredUnit;
class SimpleExecContext;

class BaseSimpleCPU : public BaseCPU
{
  protected:
    ThreadID curThread;
    BPredUnit *branchPred;

    void checkPcEventQueue();
    void swapActiveThread();

  public:
    BaseSimpleCPU(BaseSimpleCPUParams *params);
    virtual ~BaseSimpleCPU();
    void wakeup(ThreadID tid) override;
    void init() override;
  public:
    Trace::InstRecord *traceData;
    CheckerCPU *checker;

    std::vector<SimpleExecContext*> threadInfo;
    std::list<ThreadID> activeThreads;

    /** Current instruction */
    TheISA::MachInst inst;
    StaticInstPtr curStaticInst;
    StaticInstPtr curMacroStaticInst;

  protected:
    enum Status {
        Idle,
        Running,
        Faulting,
        ITBWaitResponse,
        IcacheRetry,
        IcacheWaitResponse,
        IcacheWaitSwitch,
        DTBWaitResponse,
        DcacheRetry,
        DcacheWaitResponse,
        DcacheWaitSwitch,
    };

    Status _status;

    /**
     * Handler used when encountering a fault; its purpose is to
     * tear down the InstRecord. If a fault is meant to be traced,
     * the handler won't delete the record and it will annotate
     * the record as coming from a faulting instruction.
     */
    void traceFault();

  public:
    void checkForInterrupts();
    void setupFetchRequest(const RequestPtr &req);
    void preExecute();
    void postExecute();
    void advancePC(const Fault &fault);

    void haltContext(ThreadID thread_num) override;

    // statistics
    void regStats() override;
    void resetStats() override;

    virtual Fault readMem(Addr addr, uint8_t* data, unsigned size,
                          Request::Flags flags,
                          const std::vector<bool>& byte_enable =
                              std::vector<bool>())
    { panic("readMem() is not implemented\n"); }

    virtual Fault initiateMemRead(Addr addr, unsigned size,
                                  Request::Flags flags,
                                  const std::vector<bool>& byte_enable =
                                      std::vector<bool>())
    { panic("initiateMemRead() is not implemented\n"); }

    virtual Fault writeMem(uint8_t* data, unsigned size, Addr addr,
                           Request::Flags flags, uint64_t* res,
                           const std::vector<bool>& byte_enable =
                               std::vector<bool>())
    { panic("writeMem() is not implemented\n"); }

    virtual Fault amoMem(Addr addr, uint8_t* data, unsigned size,
                         Request::Flags flags,
                         AtomicOpFunctorPtr amo_op)
    { panic("amoMem() is not implemented\n"); }

    virtual Fault initiateMemAMO(Addr addr, unsigned size,
                                 Request::Flags flags,
                                 AtomicOpFunctorPtr amo_op)
    { panic("initiateMemAMO() is not implemented\n"); }

    void countInst();
    Counter totalInsts() const override;
    Counter totalOps() const override;

    void serializeThread(CheckpointOut &cp, ThreadID tid) const override;
    void unserializeThread(CheckpointIn &cp, ThreadID tid) override;

    /** Hardware transactional memory commands (HtmCmds), e.g. start a
     * transaction and commit a transaction, are memory operations but are
     * neither really (true) loads nor stores. For this reason the interface
     * is extended and initiateHtmCmd() is used to instigate the command. */
    virtual Fault initiateHtmCmd(Request::Flags flags) = 0;

    /** This function is used to instruct the memory subsystem that a
     * transaction should be aborted and the speculative state should be
     * thrown away.  This is called in the transaction's very last breath in
     * the core.  Afterwards, the core throws away its speculative state and
     * resumes execution at the point the transaction started, i.e. reverses
     * time.  When instruction execution resumes, the core expects the
     * memory subsystem to be in a stable, i.e. pre-speculative, state as
     * well. */
    virtual void htmSendAbortSignal(HtmFailureFaultCause cause) = 0;
};

#endif // __CPU_SIMPLE_BASE_HH__
