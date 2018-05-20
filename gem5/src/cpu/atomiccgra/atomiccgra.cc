/*
* Copyright (c) 2012 ARM Limited
* All rights reserved.
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
*
* Authors: Steve Reinhardt
*/

/* Contributors for CGRA model:
* Created on: May 24, 2011
* Author: mahdi
*
* edited on: Nov 29, 2016
* Author: Mahesh Balasubramanian
*
* Last edited on: May 22 2017
* Author: Shail Dave
*/

#include "arch/locked_mem.hh"
#include "arch/mmapped_ipr.hh"
#include "arch/utility.hh"
#include "base/bigint.hh"
#include "base/output.hh"
#include "config/the_isa.hh"
#include "cpu/atomiccgra/atomiccgra.hh"
#include "cpu/exetrace.hh"
#include "debug/Drain.hh"
#include "debug/ExecFaulting.hh"
#include "debug/SimpleCPU.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "mem/physical.hh"
#include "params/AtomicCGRA.hh"
#include "sim/faults.hh"
#include "sim/system.hh"
#include "sim/full_system.hh"

#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;
using namespace TheISA;

AtomicCGRA::TickEvent::TickEvent(AtomicCGRA *c)
: Event(CPU_Tick_Pri), cpu(c)
{
}

void AtomicCGRA::TickEvent::process()
{
  cpu->tick();
}

const char * AtomicCGRA::TickEvent::description() const
{
  return "AtomicCGRA tick";
}

void AtomicCGRA::init()
{
  BaseCPU::init();

  // Initialise the ThreadContext's memory proxies
  tcBase()->initMemProxies(tcBase());

  if (FullSystem && !params()->switched_out) {
    ThreadID size = threadContexts.size();
    for (ThreadID i = 0; i < size; ++i) {
      ThreadContext *tc = threadContexts[i];
      // initialize CPU, including PC
      TheISA::initCPU(tc, tc->contextId());
    }
  }

  // Atomic doesn't do MT right now, so contextId == threadId
  ifetch_req.setThreadContext(_cpuId, 0); // Add thread ID if we add MT
  data_read_req.setThreadContext(_cpuId, 0); // Add thread ID here too
  data_write_req.setThreadContext(_cpuId, 0); // Add thread ID here too
}


AtomicCGRA::DcachePort_Ptr AtomicCGRA:: getDcachePort_ptr_from_addr(Addr vaddr, int xdim)
{
  DcachePort_Ptr result = NULL;

  switch(xdim){
    case 0: result = &dcachePort1;
    break;
    case 1: result = &dcachePort2;
    break;
    case 2: result = &dcachePort3;
    break;
    case 3: result = &dcachePort4;
    break;
    default: break;
  }
  return result;
}

AtomicCGRA::DcachePort_Ptr AtomicCGRA:: getDcachePort_ptr(Request *req)
{
  return getDcachePort_ptr(req,x_dim);
}

AtomicCGRA::DcachePort_Ptr AtomicCGRA:: getDcachePort_ptr(Request *req, int xdim)
{
  DcachePort_Ptr result = NULL;
  if(is_CPU())
    result = &dcachePort;
  else
  {
    Addr virtaddr=req->getVaddr();
    result = getDcachePort_ptr_from_addr(virtaddr, xdim);
  }
  return result;
}

PacketPtrPtr AtomicCGRA:: getDcache_pkt_ptr(Request *req)
{
  PacketPtrPtr result = NULL;
  DcachePort_Ptr port = getDcachePort_ptr(req);
  result = &(port->dcache_pkt);
  return result;
}

StatusPtr AtomicCGRA:: getDcachePort_status_ptr(Request* req)
{
  return getDcachePort_status_ptr(req, x_dim);
}

StatusPtr AtomicCGRA:: getDcachePort_status_ptr(Request* req, int xdim)
{
  StatusPtr result = NULL;
  DcachePort_Ptr port = getDcachePort_ptr(req, xdim);
  result = &(port->_status);
  x_dim = xdim;
  return result;
}

AtomicCGRA::AtomicCGRA(AtomicCGRAParams *p)
: BaseCGRA(p), tickEvent(this), width(p->width), locked(false),
simulate_data_stalls(p->simulate_data_stalls),
simulate_inst_stalls(p->simulate_inst_stalls),
drain_manager(NULL),
icachePort(name() + ".icache_port", this),
dcachePort(name() + ".dcache_port", this),
dcachePort1(name() + ".dcache_port1", this),
dcachePort2(name() + ".dcache_port2", this),
dcachePort3(name() + ".dcache_port3", this),
dcachePort4(name() + ".dcache_port4", this),
fastmem(p->fastmem),
simpoint(p->simpoint_profile),
intervalSize(p->simpoint_interval),
intervalCount(0),
intervalDrift(0),
simpointStream(NULL),
currentBBV(0, 0),
currentBBVInstCount(0)
{
  _status = Idle;

  //CGRA RELATED START
  DPRINTF(CGRA,"Setting Up CGRA\n");
  CGRA_Mode = 0;
  Setup_CGRA();
  DPRINTF(CGRA,"CGRA instantiated\n");

  if (simpoint) {
    simpointStream = simout.create(p->simpoint_profile_file, false);
  }
}


AtomicCGRA::~AtomicCGRA()
{
  if (tickEvent.scheduled()) {
    deschedule(tickEvent);
  }
  if (simpointStream) {
    simout.close(simpointStream);
  }
}

unsigned int AtomicCGRA::drain(DrainManager *dm)
{
  assert(!drain_manager);
  if (switchedOut())
  return 0;

  if (!isDrained()) {
    DPRINTF(Drain, "Requesting drain: %s\n", pcState());
    drain_manager = dm;
    return 1;
  }
  else {
    if (tickEvent.scheduled())
    deschedule(tickEvent);

    DPRINTF(Drain, "Not executing microcode, no need to drain.\n");
    return 0;
  }
}

void AtomicCGRA::drainResume()
{
  assert(!tickEvent.scheduled());
  assert(!drain_manager);
  if (switchedOut())
    return;

  DPRINTF(SimpleCPU, "Resume\n");
  verifyMemoryMode();

  assert(!threadContexts.empty());
  if (threadContexts.size() > 1)
  fatal("The atomic CPU only supports one thread.\n");

  if (thread->status() == ThreadContext::Active) {
    schedule(tickEvent, nextCycle());
    _status = BaseCGRA::Running;
    notIdleFraction = 1;
  } else {
    _status = BaseCGRA::Idle;
    notIdleFraction = 0;
  }

  system->totalNumInsts = 0;
}

bool AtomicCGRA::tryCompleteDrain()
{
  if (!drain_manager)
  return false;

  DPRINTF(Drain, "tryCompleteDrain: %s\n", pcState());
  if (!isDrained())
  return false;

  DPRINTF(Drain, "CPU done draining, processing drain event\n");
  drain_manager->signalDrainDone();
  drain_manager = NULL;

  return true;
}


void AtomicCGRA::switchOut()
{
  BaseCGRA::switchOut();

  assert(!tickEvent.scheduled());
  assert(_status == BaseCGRA::Running || _status == Idle);
  assert(isDrained());
}


void
AtomicCGRA::takeOverFrom(BaseCPU *oldCPU)
{
  BaseCGRA::takeOverFrom(oldCPU);

  // The tick event should have been descheduled by drain()
  assert(!tickEvent.scheduled());

  ifetch_req.setThreadContext(_cpuId, 0); // Add thread ID if we add MT
  data_read_req.setThreadContext(_cpuId, 0); // Add thread ID here too
  data_write_req.setThreadContext(_cpuId, 0); // Add thread ID here too
}

void AtomicCGRA::verifyMemoryMode() const
{
  if (!system->isAtomicMode()) {
    fatal("The atomic CPU requires the memory system to be in "
    "'atomic' mode.\n");
  }
}

void AtomicCGRA::activateContext(ThreadID thread_num, Cycles delay)
{
  DPRINTF(SimpleCPU, "ActivateContext %d (%d cycles)\n", thread_num, delay);

  assert(thread_num == 0);
  assert(thread);

  assert(_status == Idle);
  assert(!tickEvent.scheduled());

  notIdleFraction = 1;
  numCycles += ticksToCycles(thread->lastActivate - thread->lastSuspend);

  //Make sure ticks are still on multiples of cycles
  schedule(tickEvent, clockEdge(delay));
  _status = BaseCGRA::Running;
}

void AtomicCGRA::suspendContext(ThreadID thread_num)
{
  DPRINTF(SimpleCPU, "SuspendContext %d\n", thread_num);

  assert(thread_num == 0);
  assert(thread);

  if (_status == Idle)
    return;

  assert(_status == BaseCGRA::Running);

  // tick event may not be scheduled if this gets called from inside
  // an instruction's execution, e.g. "quiesce"
  if (tickEvent.scheduled())
    deschedule(tickEvent);

  notIdleFraction = 0;
  _status = Idle;
}

Fault AtomicCGRA::readMem(Addr addr, uint8_t * data, unsigned size, unsigned flags)
{
  return readMem(addr, data, size, flags, -1);
}


Fault AtomicCGRA::readMem(Addr addr, uint8_t * data,
    unsigned size, unsigned flags, int xdim)
{
  Request *req = &data_read_req;
  // use the CPU's statically allocated read request and packet objects

  if (traceData) {
    traceData->setAddr(addr);
  }

  //The size of the data we're trying to read.
  int fullSize = size;

  //The address of the second part of this access if it needs to be split
  //across a cache line boundary.
  Addr secondAddr = roundDown(addr + size - 1, cacheLineSize());

  if (secondAddr > addr)
    size = secondAddr - addr;

  StatusPtr portStatus = getDcachePort_status_ptr(req,xdim);

  *portStatus = DTBWaitResponse;
  dcache_latency = 0;

  while (1) {
    req->setVirt(0, addr, size, flags, dataMasterId(), thread->pcState().instAddr());

    // translate to physical address
    Fault fault = thread->dtb->translateAtomic(req, tc, BaseTLB::Read);

    // Now do the access.
    if (fault == NoFault && !req->getFlags().isSet(Request::NO_ACCESS))
    {
      Packet pkt = Packet(req, req->isLLSC() ? MemCmd::LoadLockedReq : MemCmd::ReadReq);
      pkt.dataStatic(data);

      if (req->isMmappedIpr())
        dcache_latency += TheISA::handleIprRead(thread->getTC(), &pkt);
      else {
          if (fastmem && system->isMemAddr(pkt.getAddr()))
            system->getPhysMem().access(&pkt);
          else
            dcache_latency += dcachePort.sendAtomic(&pkt);
      }
      dcache_access = true;

      assert(!pkt.isError());

        if (req->isLLSC())
          TheISA::handleLockedRead(thread, req);
    }

    //If there's a fault, return it
    if (fault != NoFault)
    {
      if (req->isPrefetch()) {
        return NoFault;
      }
      else {
        return fault;
      }
    }

    //If we don't need to access a second cache line, stop now.
    if (secondAddr <= addr)
    {
      if (req->isLocked() && fault == NoFault) {
        assert(!locked);
        locked = true;
      }
      return fault;
    }

    // Set up for accessing the second cache line.
    //Move the pointer we're reading into to the correct location.
    data += size;
    //Adjust the size to get the remaining bytes.
    size = addr + fullSize - secondAddr;
    //And access the right address.
    addr = secondAddr;
  }
}

Fault AtomicCGRA::writeMem(uint8_t *data, unsigned size, Addr addr, unsigned flags, uint64_t *res)
{
  return writeMem(data, size, addr, flags, res, -1);
}

Fault AtomicCGRA::writeMem(uint8_t *data, unsigned size,
    Addr addr, unsigned flags, uint64_t *res, int xdim)
{
  // use the CPU's statically allocated write request and packet objects
  Request *req = &data_write_req;

  if (traceData) {
    traceData->setAddr(addr);
  }

  //The size of the data we're trying to read.
  int fullSize = size;

  //The address of the second part of this access if it needs to be split
  //across a cache line boundary.
  Addr secondAddr = roundDown(addr + size - 1, cacheLineSize());

  if(secondAddr > addr)
    size = secondAddr - addr;

  StatusPtr portStatus = getDcachePort_status_ptr(req,xdim);

  *portStatus = DTBWaitResponse;
  dcache_latency = 0;

  while(1) {
    req->setVirt(0, addr, size, flags, dataMasterId(), thread->pcState().instAddr());

    // translate to physical address
    Fault fault = thread->dtb->translateAtomic(req, tc, BaseTLB::Write);

    // Now do the access.
    if (fault == NoFault) {
      MemCmd cmd = MemCmd::WriteReq; // default
      bool do_access = true;  // flag to suppress cache access

      if (req->isLLSC()) {
        cmd = MemCmd::StoreCondReq;
        do_access = TheISA::handleLockedWrite(thread, req);
      } else if (req->isSwap()) {
        cmd = MemCmd::SwapReq;
        if (req->isCondSwap()) {
          assert(res);
          req->setExtraData(*res);
        }
      }

      if (do_access && !req->getFlags().isSet(Request::NO_ACCESS)) {
        Packet pkt = Packet(req, cmd);
        pkt.dataStatic(data);

        if (req->isMmappedIpr()) {
          dcache_latency +=
          TheISA::handleIprWrite(thread->getTC(), &pkt);
        } else {
          if (fastmem && system->isMemAddr(pkt.getAddr()))
          system->getPhysMem().access(&pkt);
          else
          dcache_latency += dcachePort.sendAtomic(&pkt);
        }
        dcache_access = true;
        assert(!pkt.isError());

        if (req->isSwap()) {
          assert(res);
          memcpy(res, pkt.getPtr<uint8_t>(), fullSize);
        }
      }

  }

  //If there's a fault or we don't need to access a second cache line,
  //stop now.
  if (fault != NoFault || secondAddr <= addr)
  {
    if (req->isLocked() && fault == NoFault) {
      assert(locked);
      locked = false;
    }
    if (fault != NoFault && req->isPrefetch()) {
      return NoFault;
    } else {
      return fault;
    }
  }

  /*
  * Set up for accessing the second cache line.
  */
  //Move the pointer we're reading into to the correct location.
  data += size;
  //Adjust the size to get the remaining bytes.
  size = addr + fullSize - secondAddr;
  //And access the right address.
  addr = secondAddr;
}
}

void AtomicCGRA::CGRA_Execution()
{
  DPRINTF(SimpleCPU, "CGRA Pipeline\n");
  Fault fault = NoFault;

  // received a response from the icache: execute the received instruction
  Len--;
  {
    _status = BaseCGRA::Running;
    numCycles++;
  }

  if (getDrainState() == Drainable::Draining)
  {
    completeDrain();
    return;
  }

  //*********FETCH********************
  for (unsigned i = 0; i < CGRA_XDim; i++)
  {
    for (unsigned j = 0; j < CGRA_YDim; j++)
    {
      // little modification compared to hardware implementation
      CGRA_Instruction *currentIns = new CGRA_Instruction(CGRA_instructions[i*CGRA_YDim + j]);
      cgra_PEs[i * CGRA_YDim + j].Fetch(currentIns);
    }
  }

  //*********DECODE********************
  for (unsigned i = 0; i < CGRA_XDim; i++)
    for (unsigned j = 0; j < CGRA_YDim; j++)
      cgra_PEs[i * CGRA_YDim + j].Decode();

  if(!isTCdynamic)
  {
    if((Len == 0) & (state == KERN))
    KernelCounter--;
  }

  //*********EXECUTE********************
  for (int i = 0; i < CGRA_XDim; i++)
  {
    for (int j = 0; j < CGRA_YDim; j++)
    {
      cgra_PEs[i * CGRA_YDim + j].Execute();
      Conditional_Reg = (Conditional_Reg & cgra_PEs[i * CGRA_YDim + j].getController_Reg() );
    }
  }

  /*
  If TC is statically known then, decrement KernelCounter till 0.
  Set Conditional_Reg as 0 to exit the kernel, moving to epilogue.
  If TC is not known statically,
  CGRA PE evaluating exit operation would set Conditional_Reg to 0.
  */
  if(!isTCdynamic)
  {
    if(KernelCounter < 1)
      Conditional_Reg = 0;
    else
      Conditional_Reg = 1;
  }
  else
  {
    ;
  }

  //*********WRITE BACK********************
  for (int i = 0; i < CGRA_XDim; i++)
    for (int j = 0; j < CGRA_YDim; j++)
      cgra_PEs[i * CGRA_YDim + j].WriteBack();

  //*********PERFORM MEMORY OPERATIONS********************
  for (int i = 0; i < CGRA_XDim; i++)
  {
    if (MemBusStatus[i] == CGRA_MEMORY_READ)
    {
      readMem((Addr) MemAddress[i], (uint8_t *) &MemData[i], (unsigned)MemAccessAlignment[i], (unsigned) 163, i);
      MemAccessCount++;
      x_dim=0;
    }
    else if (MemBusStatus[i] == CGRA_MEMORY_WRITE)
    {
      writeMem((uint8_t *) &MemData[i], (unsigned) MemAccessAlignment[i], (Addr) MemAddress[i], (unsigned) 131, unknownRes, i);
      MemAccessCount++;
      x_dim=0;
    }
  }

  CGRA_advanceTime();

  if(fault != NoFault || !stayAtPC)
    CGRA_advancePC();

  if (tryCompleteDrain())
    return;

  // instruction takes at least one cycle
  if (latency < clockPeriod())
    latency = clockPeriod();

  if (_status != Idle)
    schedule(tickEvent, curTick() + latency);
}

void AtomicCGRA::CGRA_advanceTime()
{
  CycleCounter++;

  if (CycleCounter%II==0)
  {
    CycleCounter = 0;
    for (unsigned i = 0; i < CGRA_XDim; i++)
      for (unsigned j = 0; j < CGRA_YDim; j++)
        cgra_PEs[i * CGRA_YDim + j].advanceTime();
  }
}

void AtomicCGRA::tick()
{
  DPRINTF(SimpleCPU, "Tick\n");
  Fault fault;

  for (int i = 0; i < width || locked; ++i)
  {
    numCycles++;

    if (!curStaticInst || !curStaticInst->isDelayedCommit())
      checkForInterrupts();

    checkPcEventQueue();
    // We must have just got suspended by a PC event
    if (_status == Idle) {
      tryCompleteDrain();
      return;
    }

    TheISA::PCState pcState = thread->pcState();

    bool needToFetch = !isRomMicroPC(pcState.microPC()) &&
    !curMacroStaticInst;

    //if CGRA, reset memory instructions status
    if(!is_CPU())
    {
      for (int i = 0; i < CGRA_XDim; i++)
        MemBusStatus[i] = 0;

      MemAccessCount = 0;
      AccessCount = 0;
    }

    if (needToFetch) {
      setupFetchRequest(&ifetch_req);
      fault = thread->itb->translateAtomic(&ifetch_req, tc, BaseTLB::Execute);
    }

    if (fault == NoFault)
    {
      Tick icache_latency = 0;
      bool icache_access = false;
      dcache_access = false; // assume no dcache access

      if (needToFetch) {
        // This is commented out because the decoder would act like
        // a tiny cache otherwise. It wouldn't be flushed when needed
        // like the I cache. It should be flushed, and when that works
        // this code should be uncommented.
        //Fetch more instruction memory if necessary
        icache_access = true;
        Packet ifetch_pkt = Packet(&ifetch_req, MemCmd::ReadReq);

        //CGRA INSTRUCTIONS ARE STORED IN CGRA_instructions
        if (is_CPU())
          ifetch_pkt.dataStatic(&inst);
        else
          ifetch_pkt.dataStatic(CGRA_instructions);


        if (fastmem && system->isMemAddr(ifetch_pkt.getAddr()))
          system->getPhysMem().access(&ifetch_pkt);
        else
          icache_latency = icachePort.sendAtomic(&ifetch_pkt);

        assert(!ifetch_pkt.isError());
      }

      if (is_CPU())
      {
        preExecute();

        fault = curStaticInst->execute(this, traceData);
        if (curStaticInst)
        {
          // keep an instruction count
          if (fault == NoFault)
            countInst();
          else if (traceData && !DTRACE(ExecFaulting))
          {
            delete traceData;
            traceData = NULL;
          }
          postExecute();
        }

        if (curStaticInst && (!curStaticInst->isMicroop() ||
        curStaticInst->isFirstMicroop()))
          instCnt++;

        // profile for SimPoints if enabled and macro inst is finished
        if (simpoint && curStaticInst && (fault == NoFault) &&
        (!curStaticInst->isMicroop() ||
        curStaticInst->isLastMicroop())) {
          profileSimPoint();
        }
        Tick stall_ticks = 0;
        if (simulate_inst_stalls && icache_access)
          stall_ticks += icache_latency;

        if (simulate_data_stalls && dcache_access)
          stall_ticks += dcache_latency;

        if (stall_ticks) {
          // the atomic cpu does its accounting in ticks, so
          // keep counting in ticks but round to the clock period
          latency += divCeil(stall_ticks, clockPeriod()) *
          clockPeriod();
        }

        if ((int) tc->readIntReg(11) == 17)
        {
          DPRINTF(CGRA,"********Deleting CGRA********\n");
          thread->setStatus(ThreadContext::Halted);
          suspendContext(0);
        }

        if ((int) tc->readIntReg(11) == 15)
        {
          DPRINTF(CGRA,"************************Setting UP the CGRA************\n");
          Setup_CGRA_Execution();

          if(Prolog>0 || KernelCounter>0 || EPILog>0)
          {
            Switch_To_CGRA();
            DPRINTF(CGRA,"\nExecuting 1 CGRA Cycle.\n");
            schedule(tickEvent, nextCycle());
          }
          else
          {
            //CGRA execution over
            Prepare_to_Switch_Back_to_CPU();
            Restore_CPU_Execution();
            tc->setIntReg(11, 16);
            Switch_To_CPU();
            DPRINTF(CGRA,"\nCALL IN ADVANCE INST TO CPU.\n");

            if(fault != NoFault || !stayAtPC)
              CGRA_advancePC();

            if (tryCompleteDrain())
              return;

            // instruction takes at least one cycle
            if (latency < clockPeriod())
              latency = clockPeriod();

            if (_status != Idle)
              schedule(tickEvent, curTick() + latency);
          }
        }
        else
        {
          //NORMAL CPU EXECUTION
          if(fault != NoFault || !stayAtPC)
            advancePC(fault);

          if (tryCompleteDrain())
            return;

          // instruction takes at least one cycle
          if (latency < clockPeriod())
            latency = clockPeriod();

          if (_status != Idle)
            schedule(tickEvent, curTick() + latency);
        }
      }
      else
      {
        //CGRA EXECUTION
        if(Len==0)
        {
          if(state==EPI)
          state=FINISH;
        }

        if(state==FINISH)
        {
          DPRINTF(CGRA,"************************PREPARING TO MOVE TO CPU************\n");
          Prepare_to_Switch_Back_to_CPU();
        }
        //CGRA EXECUTION OVER
        if ((int) tc->readIntReg(8) == 66)
        {
          DPRINTF(CGRA,"newPC=%ld, II=%ld, EPILog=%ld, Prolog=%ld, Len=%ld\n", (long) newPC, (long) II, (long) EPILog, (long) Prolog, (long) Len);
          DPRINTF(CGRA,"**********************CGRA Execution is over**********************\n");
          Restore_CPU_Execution();
          Switch_To_CPU();
          tc->setIntReg(11, 16);
          tc->setIntReg(8,  60);
          DPRINTF(CGRA,"******REG R11= %d*********\n",(int)tc->readIntReg(11));
          DPRINTF(CGRA,"AFTER COMPLETETION PC: %x\n",(unsigned int) tc->instAddr());
          for (int i = 0; i < CGRA_XDim; i++)
            for (int j = 0; j < CGRA_YDim; j++)
              cgra_PEs[i * CGRA_YDim + j].ClearRegfile();

          //If thread is not joined after CGRA Execution
          //and we need to continue from old PC of CGRA controller
          if(fault != NoFault || !stayAtPC)
            advancePC(fault);

          if (tryCompleteDrain())
            return;

          // instruction takes at least one cycle
          if (latency < clockPeriod())
            latency = clockPeriod();

          if (_status != Idle)
            schedule(tickEvent, curTick() + latency);
        }
        else
        {
          DPRINTF(CGRA_Detailed,"\nCALL IN NOW CGRA_Execution.\n");
          CGRA_Execution();
        }
      }
    } // end of if(fault == NoFault)
  } //end of for loop
} // end of tick()

void AtomicCGRA::printAddr(Addr a)
{
  dcachePort.printAddr(a);
}

void AtomicCGRA::profileSimPoint()
{
  if (!currentBBVInstCount)
    currentBBV.first = thread->pcState().instAddr();

  ++intervalCount;
  ++currentBBVInstCount;

  // If inst is control inst, assume end of basic block.
  if (curStaticInst->isControl()) {
    currentBBV.second = thread->pcState().instAddr();

    auto map_itr = bbMap.find(currentBBV);
    if (map_itr == bbMap.end()){
      // If a new (previously unseen) basic block is found,
      // add a new unique id, record num of insts and insert into bbMap.
      BBInfo info;
      info.id = bbMap.size() + 1;
      info.insts = currentBBVInstCount;
      info.count = currentBBVInstCount;
      bbMap.insert(std::make_pair(currentBBV, info));
    } else {
      // If basic block is seen before, just increment the count by the
      // number of insts in basic block.
      BBInfo& info = map_itr->second;
      assert(info.insts == currentBBVInstCount);
      info.count += currentBBVInstCount;
    }
    currentBBVInstCount = 0;

    // Reached end of interval if the sum of the current inst count
    // (intervalCount) and the excessive inst count from the previous
    // interval (intervalDrift) is greater than/equal to the interval size.
    if (intervalCount + intervalDrift >= intervalSize) {
      // summarize interval and display BBV info
      std::vector<pair<uint64_t, uint64_t> > counts;
      for (auto map_itr = bbMap.begin(); map_itr != bbMap.end();
      ++map_itr) {
        BBInfo& info = map_itr->second;
        if (info.count != 0) {
          counts.push_back(std::make_pair(info.id, info.count));
          info.count = 0;
        }
      }
      std::sort(counts.begin(), counts.end());

      // Print output BBV info
      *simpointStream << "T";
      for (auto cnt_itr = counts.begin(); cnt_itr != counts.end();
      ++cnt_itr) {
        *simpointStream << ":" << cnt_itr->first
        << ":" << cnt_itr->second << " ";
      }
      *simpointStream << "\n";

      intervalDrift = (intervalCount + intervalDrift) - intervalSize;
      intervalCount = 0;
    }
  }
}

//  AtomicCGRA Simulation Object
AtomicCGRA * AtomicCGRAParams::create()
{
  numThreads = 1;
  if (!FullSystem && workload.size() != 1)
  panic("only one workload allowed");
  return new AtomicCGRA(this);
}

void AtomicCGRA::Setup_CGRA()
{
  CGRA_instructions = new uint32_t[16]; //Shail: TODO Parameterize this
  unknownRes = new uint64_t[16]; // Shail: TODO Parameterize this
  written = 0;

  cgra_PEs = new CGRA_PE[CGRA_XDim * CGRA_YDim];

  //Setting Neighbors - PopulatOutputing structure
  for (int i = 0; i < CGRA_XDim; i++)
  {
    for (int j = 0; j < CGRA_YDim; j++)
    {
      int iIndex;
      int jIndex;
      if (i == 0)
      {
        iIndex = CGRA_XDim;
      }
      else
      {
        iIndex = i;
      }
      if (j == 0)
      {
        jIndex = CGRA_YDim;
      }
      else
      {
        jIndex = j;
      }

      cgra_PEs[i * CGRA_YDim + j].SetNeighbours(
        cgra_PEs[i * CGRA_YDim + ((jIndex - 1) % CGRA_YDim)].getOutputPtr(), //Left
        cgra_PEs[i * CGRA_YDim + ((jIndex + 1) % CGRA_YDim)].getOutputPtr(),	//Right
        cgra_PEs[(((iIndex - 1) % CGRA_XDim) * CGRA_YDim) + j].getOutputPtr(),	//Up
        cgra_PEs[(((iIndex + 1) % CGRA_XDim) * CGRA_YDim) + j].getOutputPtr());	//Down

      cgra_PEs[i * CGRA_YDim + j].setDataBus((int*)(&MemData[i]));
      cgra_PEs[i * CGRA_YDim + j].setAddressBus((uint64_t *)(&MemAddress[i]));
      cgra_PEs[i * CGRA_YDim + j].setRWStatusBus((int*)(&MemBusStatus[i]));
      cgra_PEs[i * CGRA_YDim + j].setAlignmentBus((unsigned*)(&MemAccessAlignment[i]));
      cgra_PEs[i * CGRA_YDim + j].SetController_Reg();//
      cgra_PEs[i * CGRA_YDim + j].ClearRegfile();
    }
  }
}

void AtomicCGRA::Setup_CGRA_Parameters()
{
  // Setting up controller
  std::ostringstream osLoopID;
  osLoopID << TotalLoops;

  // Need to modify this for multiple performance critical loops
  std::string directoryPath = "./CGRAExec/L1/initCGRA.txt";

  unsigned long temp;
  ifstream initCGRAFile;
  initCGRAFile.open(directoryPath.c_str());

  initCGRAFile >> temp;
  initCGRAFile >> temp;

  initCGRAFile >> II;
  initCGRAFile >> EPILog;
  initCGRAFile >> Prolog;
  initCGRAFile >> KernelCounter;
  initCGRAFile >> LiveVar_St_Epilog;

  initCGRAFile >> EPILogPC;
  initCGRAFile >> PROLogPC;
  initCGRAFile >> KernelPC;
  initCGRAFile.close();

  DPRINTF(CGRA,"Loop count is %d\n",KernelCounter);
  DPRINTF(CGRA,"Epilog Length is %d\n",EPILog);
  DPRINTF(CGRA,"LiveVar_St_Epilog Length is %d\n",LiveVar_St_Epilog);
  DPRINTF(CGRA,"II is %d\n",II);
  int epilog_count = (int) ceil((float) ((EPILog-LiveVar_St_Epilog)/II));
  int prolog_count = 1;
  DPRINTF(CGRA,"Epilog count is  %d\n",epilog_count);
  DPRINTF(CGRA,"Prolog count is  %d\n",prolog_count);
  DPRINTF(CGRA,"Kernel Counter is  %d\n",KernelCounter);

  newPC=PROLogPC;
  Len=Prolog;
  state=PRO;
  CycleCounter = 0;
  Conditional_Reg = true;

  if(KernelCounter <= 0) isTCdynamic = true;

  DPRINTF(CGRA,"CGRA PARAMETERS: PROLOG= %d, EPILOG=%d, II=%d, KernelCounter=%d\n",Prolog,EPILog,II,KernelCounter);
  DPRINTF(CGRA,"CGRA PARAMETERS: PROLOGPC= %x, EPILOGPC=%x,  KernelPC=%x\n",(unsigned int)PROLogPC,(unsigned int)EPILogPC,(unsigned int)KernelPC);
}

void AtomicCGRA::Setup_CGRA_Execution()
{
  DPRINTF(CGRA,"Inside Setup_CGRA_Execution\n");
  DPRINTF(CGRA,"CGRA: OLD PC: %x\n",(unsigned int) tc->instAddr());
  Setup_CGRA_Parameters();
  DPRINTF(CGRA,"CGRA PC : %x\n",(unsigned int)newPC);
  DPRINTF(CGRA,"**********************CGRA Execution is started**************************** \n");
  backPC = tc->pcState();
  tc->pcState((Addr) newPC);
}

void AtomicCGRA::Prepare_to_Switch_Back_to_CPU()
{
  tc->setIntReg(11, 16);
  //make sure to go back to actual  processor state
  tc->setIntReg(8, 66);
  //make sure that you reset the state of the core to make sure it will fetch afterward
  _status = BaseCGRA::Running;
}

void AtomicCGRA::Restore_CPU_Execution()
{
  tc->pcState(backPC);
}

void AtomicCGRA::Switch_To_CPU()
{
  CGRA_Mode = 0;
}

void AtomicCGRA::Switch_To_CGRA()
{
  CGRA_Mode = 1;
}

bool AtomicCGRA::is_CPU()
{
  return (CGRA_Mode==0);
}


void AtomicCGRA::setupFetchRequest(Request *req)
{
  BaseCGRA::setupFetchRequest(req);
  //IF CGRA, FETCH 64 BYTES OF INSTRUCTIONS
  if (!is_CPU())
  { //Shail: Parameterize this
    req->setSize(4*(CGRA_XDim*CGRA_YDim));
  }
}

void AtomicCGRA::CGRA_advancePC()
{
  DPRINTF(CGRA_Detailed,"*******IN ADVANCE PC******\n");
  DPRINTF(CGRA_Detailed,"%s,%s,%d,PC:%x\n",
  __FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
  if(Len==0)
  {
    if(state==PRO)
    {
      DPRINTF(CGRA_Detailed,"\nPROLOG->KERNEL\n");
      state=KERN;
      newPC=KernelPC;
      Len=II;
      Conditional_Reg=1;
    }
    else if(state==KERN)
    {
      if(Conditional_Reg)
      {
        Len=II;
        newPC=KernelPC;
      }
      else
      {
        DPRINTF(CGRA_Detailed,"\nKERNEL->EPILOG\n");
        state=EPI;
        newPC=EPILogPC;
        Len=EPILog;
      }
    }
  }
  else
    newPC+=4*(CGRA_XDim*CGRA_YDim); //Shail: Parameterize this

  tc->pcState((Addr) newPC);
}


void AtomicCGRA::completeDrain()
{
  DPRINTF(Drain, "CPU done draining, processing drain event\n");
  setDrainState(Drainable::Drained);
  drain_manager->signalDrainDone();
}
