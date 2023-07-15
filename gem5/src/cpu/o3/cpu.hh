/*
 * Copyright (c) 2011-2013, 2016-2020 ARM Limited
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
 * Copyright (c) 2004-2005 The Regents of The University of Michigan
 * Copyright (c) 2011 Regents of the University of California
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

#ifndef __CPU_O3_CPU_HH__
#define __CPU_O3_CPU_HH__

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <vector>

#include "arch/generic/types.hh"
#include "arch/types.hh"
#include "base/statistics.hh"
#include "config/the_isa.hh"
#include "cpu/o3/comm.hh"
#include "cpu/o3/cpu_policy.hh"
#include "cpu/o3/scoreboard.hh"
#include "cpu/o3/thread_state.hh"
#include "cpu/activity.hh"
#include "cpu/base.hh"
#include "cpu/simple_thread.hh"
#include "cpu/timebuf.hh"
#include "params/DerivO3CPU.hh"
#include "sim/process.hh"

template <class>
class Checker;
class ThreadContext;
template <class>
class O3ThreadContext;

class Checkpoint;
class Process;

struct BaseCPUParams;

class BaseO3CPU : public BaseCPU
{
    //Stuff that's pretty ISA independent will go here.
  public:
    BaseO3CPU(BaseCPUParams *params);

    void regStats();
};

/**
 * FullO3CPU class, has each of the stages (fetch through commit)
 * within it, as well as all of the time buffers between stages.  The
 * tick() function for the CPU is defined here.
 */
template <class Impl>
class FullO3CPU : public BaseO3CPU
{
  public:
    // Typedefs from the Impl here.
    typedef typename Impl::CPUPol CPUPolicy;
    typedef typename Impl::DynInstPtr DynInstPtr;
    typedef typename Impl::O3CPU O3CPU;

    using VecElem =  TheISA::VecElem;
    using VecRegContainer =  TheISA::VecRegContainer;

    using VecPredRegContainer = TheISA::VecPredRegContainer;

    typedef O3ThreadState<Impl> ImplState;
    typedef O3ThreadState<Impl> Thread;

    typedef typename std::list<DynInstPtr>::iterator ListIt;

    friend class O3ThreadContext<Impl>;

  public:
    enum Status {
        Running,
        Idle,
        Halted,
        Blocked,
        SwitchedOut
    };

    BaseTLB *itb;
    BaseTLB *dtb;
    using LSQRequest = typename LSQ<Impl>::LSQRequest;

    /** Overall CPU status. */
    Status _status;

  private:

    /** The tick event used for scheduling CPU ticks. */
    EventFunctionWrapper tickEvent;

    /** The exit event used for terminating all ready-to-exit threads */
    EventFunctionWrapper threadExitEvent;

    /** Schedule tick event, regardless of its current state. */
    void scheduleTickEvent(Cycles delay)
    {
        if (tickEvent.squashed())
            reschedule(tickEvent, clockEdge(delay));
        else if (!tickEvent.scheduled())
            schedule(tickEvent, clockEdge(delay));
    }

    /** Unschedule tick event, regardless of its current state. */
    void unscheduleTickEvent()
    {
        if (tickEvent.scheduled())
            tickEvent.squash();
    }

    /**
     * Check if the pipeline has drained and signal drain done.
     *
     * This method checks if a drain has been requested and if the CPU
     * has drained successfully (i.e., there are no instructions in
     * the pipeline). If the CPU has drained, it deschedules the tick
     * event and signals the drain manager.
     *
     * @return False if a drain hasn't been requested or the CPU
     * hasn't drained, true otherwise.
     */
    bool tryDrain();

    /**
     * Perform sanity checks after a drain.
     *
     * This method is called from drain() when it has determined that
     * the CPU is fully drained when gem5 is compiled with the NDEBUG
     * macro undefined. The intention of this method is to do more
     * extensive tests than the isDrained() method to weed out any
     * draining bugs.
     */
    void drainSanityCheck() const;

    /** Check if a system is in a drained state. */
    bool isCpuDrained() const;

  public:
    /** Constructs a CPU with the given parameters. */
    FullO3CPU(DerivO3CPUParams *params);
    /** Destructor. */
    ~FullO3CPU();

    /** Registers statistics. */
    void regStats() override;

    ProbePointArg<PacketPtr> *ppInstAccessComplete;
    ProbePointArg<std::pair<DynInstPtr, PacketPtr> > *ppDataAccessComplete;

    /** Register probe points. */
    void regProbePoints() override;

    void demapPage(Addr vaddr, uint64_t asn)
    {
        this->itb->demapPage(vaddr, asn);
        this->dtb->demapPage(vaddr, asn);
    }

    void demapInstPage(Addr vaddr, uint64_t asn)
    {
        this->itb->demapPage(vaddr, asn);
    }

    void demapDataPage(Addr vaddr, uint64_t asn)
    {
        this->dtb->demapPage(vaddr, asn);
    }

    /** Ticks CPU, calling tick() on each stage, and checking the overall
     *  activity to see if the CPU should deschedule itself.
     */
    void tick();

    /** Initialize the CPU */
    void init() override;

    void startup() override;

    /** Returns the Number of Active Threads in the CPU */
    int numActiveThreads()
    { return activeThreads.size(); }

    /** Add Thread to Active Threads List */
    void activateThread(ThreadID tid);

    /** Remove Thread from Active Threads List */
    void deactivateThread(ThreadID tid);

    /** Setup CPU to insert a thread's context */
    void insertThread(ThreadID tid);

    /** Remove all of a thread's context from CPU */
    void removeThread(ThreadID tid);

    /** Count the Total Instructions Committed in the CPU. */
    Counter totalInsts() const override;

    /** Count the Total Ops (including micro ops) committed in the CPU. */
    Counter totalOps() const override;

    /** Add Thread to Active Threads List. */
    void activateContext(ThreadID tid) override;

    /** Remove Thread from Active Threads List */
    void suspendContext(ThreadID tid) override;

    /** Remove Thread from Active Threads List &&
     *  Remove Thread Context from CPU.
     */
    void haltContext(ThreadID tid) override;

    /** Update The Order In Which We Process Threads. */
    void updateThreadPriority();

    /** Is the CPU draining? */
    bool isDraining() const { return drainState() == DrainState::Draining; }

    void serializeThread(CheckpointOut &cp, ThreadID tid) const override;
    void unserializeThread(CheckpointIn &cp, ThreadID tid) override;

    /** Insert tid to the list of threads trying to exit */
    void addThreadToExitingList(ThreadID tid);

    /** Is the thread trying to exit? */
    bool isThreadExiting(ThreadID tid) const;

    /**
     *  If a thread is trying to exit and its corresponding trap event
     *  has been completed, schedule an event to terminate the thread.
     */
    void scheduleThreadExitEvent(ThreadID tid);

    /** Terminate all threads that are ready to exit */
    void exitThreads();

  public:
    /** Executes a syscall.
     * @todo: Determine if this needs to be virtual.
     */
    void syscall(ThreadID tid);

    /** Starts draining the CPU's pipeline of all instructions in
     * order to stop all memory accesses. */
    DrainState drain() override;

    /** Resumes execution after a drain. */
    void drainResume() override;

    /**
     * Commit has reached a safe point to drain a thread.
     *
     * Commit calls this method to inform the pipeline that it has
     * reached a point where it is not executed microcode and is about
     * to squash uncommitted instructions to fully drain the pipeline.
     */
    void commitDrained(ThreadID tid);

    /** Switches out this CPU. */
    void switchOut() override;

    /** Takes over from another CPU. */
    void takeOverFrom(BaseCPU *oldCPU) override;

    void verifyMemoryMode() const override;

    /** Get the current instruction sequence number, and increment it. */
    InstSeqNum getAndIncrementInstSeq()
    { return globalSeqNum++; }

    /** Traps to handle given fault. */
    void trap(const Fault &fault, ThreadID tid, const StaticInstPtr &inst);

    /**
     * Mark vector fields in scoreboard as ready right after switching
     * vector mode, since software may read vectors at this time.
     */
    void setVectorsAsReady(ThreadID tid);

    /** Check if a change in renaming is needed for vector registers.
     * The vecMode variable is updated and propagated to rename maps.
     *
     * @param tid ThreadID
     * @param freelist list of free registers
     */
    void switchRenameMode(ThreadID tid, UnifiedFreeList* freelist);

    /** Returns the Fault for any valid interrupt. */
    Fault getInterrupts();

    /** Processes any an interrupt fault. */
    void processInterrupts(const Fault &interrupt);

    /** Halts the CPU. */
    void halt() { panic("Halt not implemented!\n"); }

    /** Register accessors.  Index refers to the physical register index. */

    /** Reads a miscellaneous register. */
    RegVal readMiscRegNoEffect(int misc_reg, ThreadID tid) const;

    /** Reads a misc. register, including any side effects the read
     * might have as defined by the architecture.
     */
    RegVal readMiscReg(int misc_reg, ThreadID tid);

    /** Sets a miscellaneous register. */
    void setMiscRegNoEffect(int misc_reg, RegVal val, ThreadID tid);

    /** Sets a misc. register, including any side effects the write
     * might have as defined by the architecture.
     */
    void setMiscReg(int misc_reg, RegVal val, ThreadID tid);

    RegVal readIntReg(PhysRegIdPtr phys_reg);

    RegVal readFloatReg(PhysRegIdPtr phys_reg);

    const VecRegContainer& readVecReg(PhysRegIdPtr reg_idx) const;

    /**
     * Read physical vector register for modification.
     */
    VecRegContainer& getWritableVecReg(PhysRegIdPtr reg_idx);

    /** Returns current vector renaming mode */
    Enums::VecRegRenameMode vecRenameMode() const { return vecMode; }

    /** Sets the current vector renaming mode */
    void vecRenameMode(Enums::VecRegRenameMode vec_mode)
    { vecMode = vec_mode; }

    /**
     * Read physical vector register lane
     */
    template<typename VecElem, int LaneIdx>
    VecLaneT<VecElem, true>
    readVecLane(PhysRegIdPtr phys_reg) const
    {
        vecRegfileReads++;
        return regFile.readVecLane<VecElem, LaneIdx>(phys_reg);
    }

    /**
     * Read physical vector register lane
     */
    template<typename VecElem>
    VecLaneT<VecElem, true>
    readVecLane(PhysRegIdPtr phys_reg) const
    {
        vecRegfileReads++;
        return regFile.readVecLane<VecElem>(phys_reg);
    }

    /** Write a lane of the destination vector register. */
    template<typename LD>
    void
    setVecLane(PhysRegIdPtr phys_reg, const LD& val)
    {
        vecRegfileWrites++;
        return regFile.setVecLane(phys_reg, val);
    }

    const VecElem& readVecElem(PhysRegIdPtr reg_idx) const;

    const VecPredRegContainer& readVecPredReg(PhysRegIdPtr reg_idx) const;

    VecPredRegContainer& getWritableVecPredReg(PhysRegIdPtr reg_idx);

    RegVal readCCReg(PhysRegIdPtr phys_reg);

    void setIntReg(PhysRegIdPtr phys_reg, RegVal val);

    void setFloatReg(PhysRegIdPtr phys_reg, RegVal val);

    void setVecReg(PhysRegIdPtr reg_idx, const VecRegContainer& val);

    void setVecElem(PhysRegIdPtr reg_idx, const VecElem& val);

    void setVecPredReg(PhysRegIdPtr reg_idx, const VecPredRegContainer& val);

    void setCCReg(PhysRegIdPtr phys_reg, RegVal val);

    RegVal readArchIntReg(int reg_idx, ThreadID tid);

    RegVal readArchFloatReg(int reg_idx, ThreadID tid);

    const VecRegContainer& readArchVecReg(int reg_idx, ThreadID tid) const;
    /** Read architectural vector register for modification. */
    VecRegContainer& getWritableArchVecReg(int reg_idx, ThreadID tid);

    /** Read architectural vector register lane. */
    template<typename VecElem>
    VecLaneT<VecElem, true>
    readArchVecLane(int reg_idx, int lId, ThreadID tid) const
    {
        PhysRegIdPtr phys_reg = commitRenameMap[tid].lookup(
                    RegId(VecRegClass, reg_idx));
        return readVecLane<VecElem>(phys_reg);
    }


    /** Write a lane of the destination vector register. */
    template<typename LD>
    void
    setArchVecLane(int reg_idx, int lId, ThreadID tid, const LD& val)
    {
        PhysRegIdPtr phys_reg = commitRenameMap[tid].lookup(
                    RegId(VecRegClass, reg_idx));
        setVecLane(phys_reg, val);
    }

    const VecElem& readArchVecElem(const RegIndex& reg_idx,
                                   const ElemIndex& ldx, ThreadID tid) const;

    const VecPredRegContainer& readArchVecPredReg(int reg_idx,
                                                  ThreadID tid) const;

    VecPredRegContainer& getWritableArchVecPredReg(int reg_idx, ThreadID tid);

    RegVal readArchCCReg(int reg_idx, ThreadID tid);

    /** Architectural register accessors.  Looks up in the commit
     * rename table to obtain the true physical index of the
     * architected register first, then accesses that physical
     * register.
     */
    void setArchIntReg(int reg_idx, RegVal val, ThreadID tid);

    void setArchFloatReg(int reg_idx, RegVal val, ThreadID tid);

    void setArchVecPredReg(int reg_idx, const VecPredRegContainer& val,
                           ThreadID tid);

    void setArchVecReg(int reg_idx, const VecRegContainer& val, ThreadID tid);

    void setArchVecElem(const RegIndex& reg_idx, const ElemIndex& ldx,
                        const VecElem& val, ThreadID tid);

    void setArchCCReg(int reg_idx, RegVal val, ThreadID tid);

    /** Sets the commit PC state of a specific thread. */
    void pcState(const TheISA::PCState &newPCState, ThreadID tid);

    /** Reads the commit PC state of a specific thread. */
    TheISA::PCState pcState(ThreadID tid);

    /** Reads the commit PC of a specific thread. */
    Addr instAddr(ThreadID tid);

    /** Reads the commit micro PC of a specific thread. */
    MicroPC microPC(ThreadID tid);

    /** Reads the next PC of a specific thread. */
    Addr nextInstAddr(ThreadID tid);

    /** Initiates a squash of all in-flight instructions for a given
     * thread.  The source of the squash is an external update of
     * state through the TC.
     */
    void squashFromTC(ThreadID tid);

    /** Function to add instruction onto the head of the list of the
     *  instructions.  Used when new instructions are fetched.
     */
    ListIt addInst(const DynInstPtr &inst);

    /** Function to tell the CPU that an instruction has completed. */
    void instDone(ThreadID tid, const DynInstPtr &inst);

    /** Remove an instruction from the front end of the list.  There's
     *  no restriction on location of the instruction.
     */
    void removeFrontInst(const DynInstPtr &inst);

    /** Remove all instructions that are not currently in the ROB.
     *  There's also an option to not squash delay slot instructions.*/
    void removeInstsNotInROB(ThreadID tid);

    /** Remove all instructions younger than the given sequence number. */
    void removeInstsUntil(const InstSeqNum &seq_num, ThreadID tid);

    /** Removes the instruction pointed to by the iterator. */
    inline void squashInstIt(const ListIt &instIt, ThreadID tid);

    /** Cleans up all instructions on the remove list. */
    void cleanUpRemovedInsts();

    /** Debug function to print all instructions on the list. */
    void dumpInsts();

  public:
#ifndef NDEBUG
    /** Count of total number of dynamic instructions in flight. */
    int instcount;
#endif

    /** List of all the instructions in flight. */
    std::list<DynInstPtr> instList;

    /** List of all the instructions that will be removed at the end of this
     *  cycle.
     */
    std::queue<ListIt> removeList;

#ifdef DEBUG
    /** Debug structure to keep track of the sequence numbers still in
     * flight.
     */
    std::set<InstSeqNum> snList;
#endif

    /** Records if instructions need to be removed this cycle due to
     *  being retired or squashed.
     */
    bool removeInstsThisCycle;

  protected:
    /** The fetch stage. */
    typename CPUPolicy::Fetch fetch;

    /** The decode stage. */
    typename CPUPolicy::Decode decode;

    /** The dispatch stage. */
    typename CPUPolicy::Rename rename;

    /** The issue/execute/writeback stages. */
    typename CPUPolicy::IEW iew;

    /** The commit stage. */
    typename CPUPolicy::Commit commit;

    /** The rename mode of the vector registers */
    Enums::VecRegRenameMode vecMode;

    /** The register file. */
    PhysRegFile regFile;

    /** The free list. */
    typename CPUPolicy::FreeList freeList;

    /** The rename map. */
    typename CPUPolicy::RenameMap renameMap[Impl::MaxThreads];

    /** The commit rename map. */
    typename CPUPolicy::RenameMap commitRenameMap[Impl::MaxThreads];

    /** The re-order buffer. */
    typename CPUPolicy::ROB rob;

    /** Active Threads List */
    std::list<ThreadID> activeThreads;

    /**
     *  This is a list of threads that are trying to exit. Each thread id
     *  is mapped to a boolean value denoting whether the thread is ready
     *  to exit.
     */
    std::unordered_map<ThreadID, bool> exitingThreads;

    /** Integer Register Scoreboard */
    Scoreboard scoreboard;

    std::vector<TheISA::ISA *> isa;

  public:
    /** Enum to give each stage a specific index, so when calling
     *  activateStage() or deactivateStage(), they can specify which stage
     *  is being activated/deactivated.
     */
    enum StageIdx {
        FetchIdx,
        DecodeIdx,
        RenameIdx,
        IEWIdx,
        CommitIdx,
        NumStages };

    /** Typedefs from the Impl to get the structs that each of the
     *  time buffers should use.
     */
    typedef typename CPUPolicy::TimeStruct TimeStruct;

    typedef typename CPUPolicy::FetchStruct FetchStruct;

    typedef typename CPUPolicy::DecodeStruct DecodeStruct;

    typedef typename CPUPolicy::RenameStruct RenameStruct;

    typedef typename CPUPolicy::IEWStruct IEWStruct;

    /** The main time buffer to do backwards communication. */
    TimeBuffer<TimeStruct> timeBuffer;

    /** The fetch stage's instruction queue. */
    TimeBuffer<FetchStruct> fetchQueue;

    /** The decode stage's instruction queue. */
    TimeBuffer<DecodeStruct> decodeQueue;

    /** The rename stage's instruction queue. */
    TimeBuffer<RenameStruct> renameQueue;

    /** The IEW stage's instruction queue. */
    TimeBuffer<IEWStruct> iewQueue;

  private:
    /** The activity recorder; used to tell if the CPU has any
     * activity remaining or if it can go to idle and deschedule
     * itself.
     */
    ActivityRecorder activityRec;

  public:
    /** Records that there was time buffer activity this cycle. */
    void activityThisCycle() { activityRec.activity(); }

    /** Changes a stage's status to active within the activity recorder. */
    void activateStage(const StageIdx idx)
    { activityRec.activateStage(idx); }

    /** Changes a stage's status to inactive within the activity recorder. */
    void deactivateStage(const StageIdx idx)
    { activityRec.deactivateStage(idx); }

    /** Wakes the CPU, rescheduling the CPU if it's not already active. */
    void wakeCPU();

    virtual void wakeup(ThreadID tid) override;

    /** Gets a free thread id. Use if thread ids change across system. */
    ThreadID getFreeTid();

  public:
    /** Returns a pointer to a thread context. */
    ThreadContext *
    tcBase(ThreadID tid)
    {
        return thread[tid]->getTC();
    }

    /** The global sequence number counter. */
    InstSeqNum globalSeqNum;//[Impl::MaxThreads];

    /** Pointer to the checker, which can dynamically verify
     * instruction results at run time.  This can be set to NULL if it
     * is not being used.
     */
    Checker<Impl> *checker;

    /** Pointer to the system. */
    System *system;

    /** Pointers to all of the threads in the CPU. */
    std::vector<Thread *> thread;

    /** Threads Scheduled to Enter CPU */
    std::list<int> cpuWaitList;

    /** The cycle that the CPU was last running, used for statistics. */
    Cycles lastRunningCycle;

    /** The cycle that the CPU was last activated by a new thread*/
    Tick lastActivatedCycle;

    /** Mapping for system thread id to cpu id */
    std::map<ThreadID, unsigned> threadMap;

    /** Available thread ids in the cpu*/
    std::vector<ThreadID> tids;

    /** CPU pushRequest function, forwards request to LSQ. */
    Fault pushRequest(const DynInstPtr& inst, bool isLoad, uint8_t *data,
                      unsigned int size, Addr addr, Request::Flags flags,
                      uint64_t *res, AtomicOpFunctorPtr amo_op = nullptr,
                      const std::vector<bool>& byte_enable =
                          std::vector<bool>())

    {
        return iew.ldstQueue.pushRequest(inst, isLoad, data, size, addr,
                flags, res, std::move(amo_op), byte_enable);
    }

    /** CPU read function, forwards read to LSQ. */
    Fault read(LSQRequest* req, int load_idx)
    {
        return this->iew.ldstQueue.read(req, load_idx);
    }

    /** CPU write function, forwards write to LSQ. */
    Fault write(LSQRequest* req, uint8_t *data, int store_idx)
    {
        return this->iew.ldstQueue.write(req, data, store_idx);
    }

    /** Used by the fetch unit to get a hold of the instruction port. */
    Port &
    getInstPort() override
    {
        return this->fetch.getInstPort();
    }

    /** Get the dcache port (used to find block size for translations). */
    Port &
    getDataPort() override
    {
        return this->iew.ldstQueue.getDataPort();
    }

    /** Stat for total number of times the CPU is descheduled. */
    Stats::Scalar timesIdled;
    /** Stat for total number of cycles the CPU spends descheduled. */
    Stats::Scalar idleCycles;
    /** Stat for total number of cycles the CPU spends descheduled due to a
     * quiesce operation or waiting for an interrupt. */
    Stats::Scalar quiesceCycles;
    /** Stat for the number of committed instructions per thread. */
    Stats::Vector committedInsts;
    /** Stat for the number of committed ops (including micro ops) per thread. */
    Stats::Vector committedOps;
    /** Stat for the CPI per thread. */
    Stats::Formula cpi;
    /** Stat for the total CPI. */
    Stats::Formula totalCpi;
    /** Stat for the IPC per thread. */
    Stats::Formula ipc;
    /** Stat for the total IPC. */
    Stats::Formula totalIpc;

    //number of integer register file accesses
    Stats::Scalar intRegfileReads;
    Stats::Scalar intRegfileWrites;
    //number of float register file accesses
    Stats::Scalar fpRegfileReads;
    Stats::Scalar fpRegfileWrites;
    //number of vector register file accesses
    mutable Stats::Scalar vecRegfileReads;
    Stats::Scalar vecRegfileWrites;
    //number of predicate register file accesses
    mutable Stats::Scalar vecPredRegfileReads;
    Stats::Scalar vecPredRegfileWrites;
    //number of CC register file accesses
    Stats::Scalar ccRegfileReads;
    Stats::Scalar ccRegfileWrites;
    //number of misc
    Stats::Scalar miscRegfileReads;
    Stats::Scalar miscRegfileWrites;

  public:
    // hardware transactional memory
    void htmSendAbortSignal(ThreadID tid, uint64_t htm_uid,
                            HtmFailureFaultCause cause);
};

#endif // __CPU_O3_CPU_HH__
