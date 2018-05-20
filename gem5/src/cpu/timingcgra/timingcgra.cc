/*
 * Copyright (c) 2010-2012 ARM Limited
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
 *
 * Authors: Steve Reinhardt
 */

#include "arch/locked_mem.hh"
#include "arch/mmapped_ipr.hh"
#include "arch/utility.hh"
#include "base/bigint.hh"
#include "config/the_isa.hh"
#include "cpu/timingcgra/timingcgra.hh"
#include "cpu/exetrace.hh"
#include "debug/Config.hh"
#include "debug/Drain.hh"
#include "debug/ExecFaulting.hh"
#include "debug/SimpleCPU.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "params/TimingCGRA.hh"
#include "sim/faults.hh"
#include "sim/full_system.hh"
#include "sim/system.hh"

#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;
using namespace TheISA;

static int y_dim=0; 
 
  void
TimingCGRA::init()
{
  BaseCPU::init();

  // Initialise the ThreadContext's memory proxies
  tcBase()->initMemProxies(tcBase());

  if (FullSystem && !params()->switched_out) {
    for (int i = 0; i < threadContexts.size(); ++i) {
      ThreadContext *tc = threadContexts[i];
      // initialize CPU, including PC
      TheISA::initCPU(tc, _cpuId);
    }
  }
}

/*TimingCGRA::DcachePort_Ptr TimingCGRA:: getDcachePort_ptr_from_addr(Addr vaddr)
{
  DcachePort_Ptr result = NULL;
  int i=0; 
  for (int i =0; i < CGRA_YDim; i++)
  {
//    \//printf("\nIN GET DCACHE PORT PTR %d\n",i);
     if(vaddr==MemAddress[i]) {
                      break; }

  }

  switch(i){
    case 0: result = &dcachePort1;
        printf("DCACHE PORT :-P%d\n",i+1);
            break;
    case 1: result = &dcachePort2;
         printf("DCACHE PORT %d\n",i+1);
            break;
    case 2: result = &dcachePort3;
         printf("DCACHE PORT %d\n",i+1);
            break;
    case 3: result = &dcachePort4;
         printf("DCACHE PORT %d\n",i+1);
            break;
    default: break;
  }

  return result;
}*/ 

/* The selection of dcache port corresponds to the row in which the ld_add or st_add is scheduled. Previously (above code) get the ports by vaddr (addr of the ld_add or st_add) which conflicts when 2 loads corresponds to same addr (ex. c=a;d=a and both loads are scheduled in next rows). So instead the selection of dcache ports can be through the row number from readMem or writeMem (ydim or i). 

Author Mahesh April 15, 2015*/  

TimingCGRA::DcachePort_Ptr TimingCGRA:: getDcachePort_ptr_from_addr(Addr vaddr, int ydim)
{
  DcachePort_Ptr result = NULL;
  int i=0; 
  for (i =0; i < CGRA_YDim; i++)
  {
//    \//printf("\nIN GET DCACHE PORT PTR %d\n",i);
/*     if(vaddr!=MemAddress[i]) { 
		printf("ADDRESS NO MATCH"); 
		break; }*/
 
  }


  switch(ydim){
    case 0: result = &dcachePort1;
 //	 printf("DCACHE PORT 1\n"); 
            break;
    case 1: result = &dcachePort2;
//	 printf("DCACHE PORT 2\n");
            break;
    case 2: result = &dcachePort3;
//	 printf("DCACHE PORT 3\n");
            break;
    case 3: result = &dcachePort4;
//	 printf("DCACHE PORT 4\n");
            break;
    default: break;
  }
 
  return result;
}

TimingCGRA::DcachePort_Ptr TimingCGRA:: getDcachePort_ptr(RequestPtr req)
{
  return getDcachePort_ptr(req,y_dim); 

}

TimingCGRA::DcachePort_Ptr TimingCGRA:: getDcachePort_ptr(RequestPtr req, int ydim)
{
  DcachePort_Ptr result = NULL;
  if(is_CPU())
    result = &dcachePort;
  else
  { 
    Addr virtaddr=req->getVaddr(); 
    result = getDcachePort_ptr_from_addr(virtaddr, ydim);
  }
  return result;

}

PacketPtrPtr TimingCGRA:: getDcache_pkt_ptr(RequestPtr req)
{
  PacketPtrPtr result = NULL;

  DcachePort_Ptr port = getDcachePort_ptr(req);

  result = &(port->dcache_pkt);

  return result;

}


StatusPtr TimingCGRA:: getDcachePort_status_ptr(RequestPtr req)
{
 
return getDcachePort_status_ptr(req, y_dim);  

}

StatusPtr TimingCGRA:: getDcachePort_status_ptr(RequestPtr req, int ydim)
{
  StatusPtr result = NULL;

  DcachePort_Ptr port = getDcachePort_ptr(req, ydim);

  result = &(port->_status);

  y_dim = ydim; 

  return result;

}



  void
TimingCGRA::TimingCPUPort::TickEvent::schedule(PacketPtr _pkt, Tick t)
{
  pkt = _pkt;
  cpu->schedule(this, t);
}

//TODO
TimingCGRA::TimingCGRA(TimingCGRAParams *p)
  : BaseCGRA(p), fetchTranslation(this), icachePort(this),
  dcachePort(this),dcachePort1(this,"1"),dcachePort2(this,"2"),dcachePort3(this,"3"),dcachePort4(this,"4"), ifetch_pkt(NULL), previousCycle(0), fetchEvent(this), drainManager(NULL)
{
  _status = Idle;

  system->totalNumInsts = 0;

  //CGRA RELATED START    
  CGRA_Mode = 0;
  Setup_CGRA();
  printf("CGRA instantiated\n");

  //printf("\nMaster PORT ID %d\n",dataMasterId());
  //printf("\nPORT ID %d\n",dcachePort.getId());
  //printf("\nPORT1 ID %d\n",dcachePort1.getId());
  //printf("\nPORT2 ID %d\n",dcachePort2.getId());
  //printf("\nPORT3 ID %d\n",dcachePort3.getId());
  //printf("\nPORT4 ID %d\n",dcachePort4.getId());

  //CGRA RELATED END
}


TimingCGRA::~TimingCGRA()
{
}

  unsigned int
TimingCGRA::drain(DrainManager *drain_manager)
{
  assert(!drainManager);
  if (switchedOut())
    return 0;

  if (_status == Idle ||
      (_status == BaseCGRA::Running && isDrained())) {
    DPRINTF(Drain, "No need to drain.\n");
    return 0;
  } else {
    drainManager = drain_manager;
    DPRINTF(Drain, "Requesting drain: %s\n", pcState());

    // The fetch event can become descheduled if a drain didn't
    // succeed on the first attempt. We need to reschedule it if
    // the CPU is waiting for a microcode routine to complete.
    if (_status == BaseCGRA::Running && !fetchEvent.scheduled())
      schedule(fetchEvent, clockEdge());

    return 1;
  }
}

  void
TimingCGRA::drainResume()
{
  assert(!fetchEvent.scheduled());
  assert(!drainManager);
  if (switchedOut())
    return;

  DPRINTF(SimpleCPU, "Resume\n");
  verifyMemoryMode();

  assert(!threadContexts.empty());
  if (threadContexts.size() > 1)
    fatal("The timing CPU only supports one thread.\n");

  if (thread->status() == ThreadContext::Active) {
    schedule(fetchEvent, nextCycle());
    _status = BaseCGRA::Running;
    notIdleFraction = 1;
  } else {
    _status = BaseCGRA::Idle;
    notIdleFraction = 0;
  }
}

  bool
TimingCGRA::tryCompleteDrain()
{
  if (!drainManager)
    return false;

  DPRINTF(Drain, "tryCompleteDrain: %s\n", pcState());
  if (!isDrained())
    return false;

  DPRINTF(Drain, "CPU done draining, processing drain event\n");
  drainManager->signalDrainDone();
  drainManager = NULL;

  return true;
}

  void
TimingCGRA::switchOut()
{
  BaseCGRA::switchOut();

  assert(!fetchEvent.scheduled());
  assert(_status == BaseCGRA::Running || _status == Idle);
  assert(!stayAtPC);
  assert(microPC() == 0);

  numCycles += curCycle() - previousCycle;
}


  void
TimingCGRA::takeOverFrom(BaseCPU *oldCPU)
{
  BaseCGRA::takeOverFrom(oldCPU);

  previousCycle = curCycle();
}

void
TimingCGRA::verifyMemoryMode() const
{
  if (!system->isTimingMode()) {
    fatal("The timing CPU requires the memory system to be in "
        "'timing' mode.\n");
  }
}

  void
TimingCGRA::activateContext(ThreadID thread_num, Cycles delay)
{
  DPRINTF(SimpleCPU, "ActivateContext %d (%d cycles)\n", thread_num, delay);

  assert(thread_num == 0);
  assert(thread);

  assert(_status == Idle);

  notIdleFraction = 1;
  _status = BaseCGRA::Running;

  // kick things off by initiating the fetch of the next instruction
  schedule(fetchEvent, clockEdge(delay));
}


  void
TimingCGRA::suspendContext(ThreadID thread_num)
{
  DPRINTF(SimpleCPU, "SuspendContext %d\n", thread_num);

  assert(thread_num == 0);
  assert(thread);

  if (_status == Idle)
    return;

  assert(_status == BaseCGRA::Running);

  // just change status to Idle... if status != Running,
  // completeInst() will not initiate fetch of next instruction.

  notIdleFraction = 0;
  _status = Idle;
}

//TODO
  bool
TimingCGRA::handleReadPacket(PacketPtr pkt)
{
  SplitFragmentSenderState * send_state = NULL;
  PacketPtrPtr dcache_pkt_ptr = NULL;
  RequestPtr req = pkt->req;
  DcachePort_Ptr dcachePort_ptr = NULL;
  
  StatusPtr portStatus = NULL;

  if(pkt->senderState == NULL)
  {
    //not a split access
    dcache_pkt_ptr = getDcache_pkt_ptr(req);
    dcachePort_ptr = getDcachePort_ptr(req);
    portStatus = getDcachePort_status_ptr(req);
  }
  else
  {
    //a split access
    send_state = dynamic_cast<SplitFragmentSenderState *>(pkt->senderState);
    dcache_pkt_ptr = getDcache_pkt_ptr(send_state->bigPkt->req);
    dcachePort_ptr = getDcachePort_ptr(send_state->bigPkt->req);
    portStatus = getDcachePort_status_ptr(send_state->bigPkt->req);
  }

  if (req->isMmappedIpr()) {
    Cycles delay = TheISA::handleIprRead(thread->getTC(), pkt);
    new IprEvent(pkt, this, clockEdge(delay));
   // _status = DcacheWaitResponse;
    *portStatus = DcacheWaitResponse;
    _status = WaitForMemory;
    *dcache_pkt_ptr = NULL;
  } else if (!dcachePort_ptr->sendTimingReq(pkt)) {
    //_status = DcacheRetry;
    *portStatus = DcacheRetry;
    _status = WaitForMemory;
    *dcache_pkt_ptr = pkt;
  } else {
    //_status = DcacheWaitResponse;
    *portStatus = DcacheWaitResponse;
    _status = WaitForMemory;
    // memory system takes ownership of packet
    *dcache_pkt_ptr = NULL;
  }
  return *dcache_pkt_ptr == NULL;
}

//TODO
void TimingCGRA::sendData(RequestPtr req, uint8_t *data, uint64_t *res,bool read)
{
  PacketPtr pkt;
  PacketPtrPtr dcache_pkt_ptr = getDcache_pkt_ptr(req);
  buildPacket(pkt, req, read);
  pkt->dataDynamicArray<uint8_t>(data);
  if (req->getFlags().isSet(Request::NO_ACCESS)) {
    assert(!*dcache_pkt_ptr);
    pkt->makeResponse();
    completeDataAccess(pkt);
  } else if (read) {
    handleReadPacket(pkt);
  } else {
    bool do_access = true;  // flag to suppress cache access

    if (req->isLLSC()) {
      do_access = TheISA::handleLockedWrite(thread, req);
    } else if (req->isCondSwap()) {
      assert(res);
      req->setExtraData(*res);
    }

    if (do_access) {
      *dcache_pkt_ptr = pkt;
      handleWritePacket(pkt);
    } else {
      StatusPtr portStatus =  getDcachePort_status_ptr(req);
     // _status = DcacheWaitResponse;
      *portStatus = DcacheWaitResponse;
      _status = WaitForMemory;
      completeDataAccess(pkt);
    }
  }
}

//TODO
  void
TimingCGRA::sendSplitData(RequestPtr req1, RequestPtr req2,
    RequestPtr req, uint8_t *data, bool read)
{
  PacketPtr pkt1, pkt2;
  PacketPtrPtr dcache_pkt_ptr = getDcache_pkt_ptr(req);
  buildSplitPacket(pkt1, pkt2, req1, req2, req, data, read);
  if (req->getFlags().isSet(Request::NO_ACCESS)) {
    assert(!*dcache_pkt_ptr);
    pkt1->makeResponse();
    completeDataAccess(pkt1);
  } else if (read) {
    SplitFragmentSenderState * send_state =
      dynamic_cast<SplitFragmentSenderState *>(pkt1->senderState);
    if (handleReadPacket(pkt1)) {
      send_state->clearFromParent();
      send_state = dynamic_cast<SplitFragmentSenderState *>(
          pkt2->senderState);
      if (handleReadPacket(pkt2)) {
        send_state->clearFromParent();
      }
    }
  } else {
    *dcache_pkt_ptr = pkt1;
    SplitFragmentSenderState * send_state =
      dynamic_cast<SplitFragmentSenderState *>(pkt1->senderState);
    if (handleWritePacket(pkt1)) {
      send_state->clearFromParent();
      *dcache_pkt_ptr = pkt2;
      send_state = dynamic_cast<SplitFragmentSenderState *>(
          pkt2->senderState);
      if (handleWritePacket(pkt2)) {
        send_state->clearFromParent();
      }
    }
  }
}

  void
TimingCGRA::translationFault(Fault fault)
{
  // fault may be NoFault in cases where a fault is suppressed,
  // for instance prefetches.
  numCycles += curCycle() - previousCycle;
  previousCycle = curCycle();

  if (traceData) {
    // Since there was a fault, we shouldn't trace this instruction.
    delete traceData;
    traceData = NULL;
  }

  postExecute();
  printf("CALL IN TRANSLATION FAULT.\n");
  advanceInst(fault);
}

void TimingCGRA::buildPacket(PacketPtr &pkt, RequestPtr req, bool read)
{
  MemCmd cmd;
  if (read) {
    cmd = MemCmd::ReadReq;
    if (req->isLLSC())
      cmd = MemCmd::LoadLockedReq;
  } else {
    cmd = MemCmd::WriteReq;
    if (req->isLLSC()) {
      cmd = MemCmd::StoreCondReq;
    } else if (req->isSwap()) {
      cmd = MemCmd::SwapReq;
    }
  }
  pkt = new Packet(req, cmd);
}

  void
TimingCGRA::buildSplitPacket(PacketPtr &pkt1, PacketPtr &pkt2,
    RequestPtr req1, RequestPtr req2, RequestPtr req,
    uint8_t *data, bool read)
{
  pkt1 = pkt2 = NULL;

  assert(!req1->isMmappedIpr() && !req2->isMmappedIpr());

  if (req->getFlags().isSet(Request::NO_ACCESS)) {
    buildPacket(pkt1, req, read);
    return;
  }

  buildPacket(pkt1, req1, read);
  buildPacket(pkt2, req2, read);

  req->setPhys(req1->getPaddr(), req->getSize(), req1->getFlags(), dataMasterId());
  PacketPtr pkt = new Packet(req, pkt1->cmd.responseCommand());

  pkt->dataDynamicArray<uint8_t>(data);
  pkt1->dataStatic<uint8_t>(data);
  pkt2->dataStatic<uint8_t>(data + req1->getSize());

  SplitMainSenderState * main_send_state = new SplitMainSenderState;
  pkt->senderState = main_send_state;
  main_send_state->fragments[0] = pkt1;
  main_send_state->fragments[1] = pkt2;
  main_send_state->outstanding = 2;
  pkt1->senderState = new SplitFragmentSenderState(pkt, 0);
  pkt2->senderState = new SplitFragmentSenderState(pkt, 1);
}

Fault TimingCGRA::readMem(Addr addr, uint8_t *data,unsigned size, unsigned flags)
{
  return readMem(addr,data,size,flags,-1);
}
Fault TimingCGRA::readMem(Addr addr, uint8_t *data,unsigned size, unsigned flags, int ydim)
{
  //printf("%s,%s,%d ydim: %d,PC:%x\n",__FILE__,__func__,__LINE__,ydim,(unsigned int) tc->instAddr());
  Fault fault;
  const int asid = 0;
  const ThreadID tid = 0;
  const Addr pc = thread->instAddr();
  unsigned block_size = cacheLineSize();
  BaseTLB::Mode mode = BaseTLB::Read;

  if (traceData) {
    traceData->setAddr(addr);
  }
  //TODO
  RequestPtr req  = new Request(asid, addr, size,
      flags, dataMasterId(), pc, _cpuId, tid);

  Addr split_addr = roundDown(addr + size - 1, block_size);
  assert(split_addr <= addr || split_addr - addr < block_size);

  StatusPtr portStatus = getDcachePort_status_ptr(req,ydim);


  //_status = DTBWaitResponse;
  *portStatus = DTBWaitResponse;
  _status = WaitForMemory;
  if (split_addr > addr) {
    RequestPtr req1, req2;
    assert(!req->isLLSC() && !req->isSwap());
    req->splitOnVaddr(split_addr, req1, req2);

    WholeTranslationState *state =
      new WholeTranslationState(req, req1, req2, new uint8_t[size],
          NULL, mode);
    DataTranslation<TimingCGRA *> *trans1 =
      new DataTranslation<TimingCGRA *>(this, state, 0);
    DataTranslation<TimingCGRA *> *trans2 =
      new DataTranslation<TimingCGRA *>(this, state, 1);

    thread->dtb->translateTiming(req1, tc, trans1, mode);
    thread->dtb->translateTiming(req2, tc, trans2, mode);
  } else {
    WholeTranslationState *state = new WholeTranslationState(req, new uint8_t[size], NULL, mode);

    DataTranslation<TimingCGRA *> *translation = new DataTranslation<TimingCGRA *>(this, state);

    thread->dtb->translateTiming(req, tc, translation, mode);
  }

  return NoFault;
}


//TODO
  bool
TimingCGRA::handleWritePacket(PacketPtr pkt)
{
  SplitFragmentSenderState * send_state = NULL;
  PacketPtrPtr dcache_pkt_ptr = NULL;
  RequestPtr req = pkt->req;
  DcachePort_Ptr dcachePort_ptr = NULL;

  StatusPtr portStatus = NULL;
   

  if(pkt->senderState == NULL)
  {
    //not a split access
    dcache_pkt_ptr = getDcache_pkt_ptr(req);
    dcachePort_ptr = getDcachePort_ptr(req);
    portStatus = getDcachePort_status_ptr(req);
  }
  else
  {
    //a split access
    send_state = dynamic_cast<SplitFragmentSenderState *>(pkt->senderState);
    dcache_pkt_ptr = getDcache_pkt_ptr(send_state->bigPkt->req);
    dcachePort_ptr = getDcachePort_ptr(send_state->bigPkt->req);
    portStatus = getDcachePort_status_ptr(send_state->bigPkt->req);
  }

  if (req->isMmappedIpr()) {
    Cycles delay = TheISA::handleIprWrite(thread->getTC(), *dcache_pkt_ptr);
    new IprEvent(*dcache_pkt_ptr, this, clockEdge(delay));
   // _status = DcacheWaitResponse;
    *portStatus = DcacheWaitResponse;
    _status = WaitForMemory;
    *dcache_pkt_ptr = NULL;
  } else if (!dcachePort_ptr->sendTimingReq(*dcache_pkt_ptr)) {
   // _status = DcacheRetry;
    *portStatus = DcacheRetry;
    _status = WaitForMemory;
  } else {
    //_status = DcacheWaitResponse;
    *portStatus = DcacheWaitResponse;
    _status = WaitForMemory;
    // memory system takes ownership of packet
    *dcache_pkt_ptr = NULL;
  }
  return *dcache_pkt_ptr == NULL;
}

Fault TimingCGRA::writeMem(uint8_t *data, unsigned size,Addr addr, unsigned flags, uint64_t *res)
{
  return writeMem(data,size,addr,flags,res,-1);
}
Fault TimingCGRA::writeMem(uint8_t *data, unsigned size,Addr addr, unsigned flags, uint64_t *res,int ydim)
{
  //printf("%s,%s,%d ydim: %d,PC:%x\n",__FILE__,__func__,__LINE__,ydim,(unsigned int) tc->instAddr());
  uint8_t *newData = new uint8_t[size];
  memcpy(newData, data, size);

  const int asid = 0;
  const ThreadID tid = 0;
  const Addr pc = thread->instAddr();
  unsigned block_size = cacheLineSize();
  BaseTLB::Mode mode = BaseTLB::Write;

  if (traceData) {
    traceData->setAddr(addr);
  }
  //TODO
  RequestPtr req = new Request(asid, addr, size,
      flags, dataMasterId(), pc, _cpuId, tid);

  Addr split_addr = roundDown(addr + size - 1, block_size);
  assert(split_addr <= addr || split_addr - addr < block_size);

  StatusPtr portStatus = getDcachePort_status_ptr(req,ydim);


  //_status = DTBWaitResponse;
  *portStatus = DTBWaitResponse;
  _status = WaitForMemory;
  if (split_addr > addr) {
    RequestPtr req1, req2;
    assert(!req->isLLSC() && !req->isSwap());
    req->splitOnVaddr(split_addr, req1, req2);

    WholeTranslationState *state =
      new WholeTranslationState(req, req1, req2, newData, res, mode);
    DataTranslation<TimingCGRA *> *trans1 =
      new DataTranslation<TimingCGRA *>(this, state, 0);
    DataTranslation<TimingCGRA *> *trans2 =
      new DataTranslation<TimingCGRA *>(this, state, 1);

    thread->dtb->translateTiming(req1, tc, trans1, mode);
    thread->dtb->translateTiming(req2, tc, trans2, mode);
  } else {
    WholeTranslationState *state =
      new WholeTranslationState(req, newData, res, mode);
    DataTranslation<TimingCGRA *> *translation =
      new DataTranslation<TimingCGRA *>(this, state);
    thread->dtb->translateTiming(req, tc, translation, mode);
  }

  // Translation faults will be returned via finishTranslation()
  return NoFault;
}


  void
TimingCGRA::finishTranslation(WholeTranslationState *state)
{
  _status = BaseCGRA::Running;

  if (state->getFault() != NoFault) {
    if (state->isPrefetch()) {
      state->setNoFault();
    }
    delete [] state->data;
    state->deleteReqs();
    translationFault(state->getFault());
  } else {
    if (!state->isSplit) {
      sendData(state->mainReq, state->data, state->res,
          state->mode == BaseTLB::Read);
    } else {
      sendSplitData(state->sreqLow, state->sreqHigh, state->mainReq,
          state->data, state->mode == BaseTLB::Read);
    }
  }

  delete state;
}

  void
TimingCGRA::fetch()
{
  DPRINTF(SimpleCPU, "Fetch\n");

  if (!curStaticInst || !curStaticInst->isDelayedCommit())
    checkForInterrupts();

  checkPcEventQueue();

  // We must have just got suspended by a PC event
  if (_status == Idle)
    return;

  TheISA::PCState pcState = thread->pcState();
  bool needToFetch = !isRomMicroPC(pcState.microPC()) && !curMacroStaticInst;

  //if CGRA, reset memory instructions status  
  if(!is_CPU())
  {
    for (int i = 0; i < CGRA_YDim; i++)
    {
      MemBusStatus[i] = 0;

    }
    MemAccessCount = 0;
    AccessCount = 0;
  }

  if (needToFetch) {
    _status = BaseCGRA::Running;
    Request *ifetch_req = new Request();
    ifetch_req->setThreadContext(_cpuId, /* thread ID */ 0);
    setupFetchRequest(ifetch_req);
    DPRINTF(SimpleCPU, "Translating address %#x\n", ifetch_req->getVaddr());
    thread->itb->translateTiming(ifetch_req, tc, &fetchTranslation,
        BaseTLB::Execute);
  } else {
    _status = IcacheWaitResponse;
    completeIfetch(NULL);
    numCycles += curCycle() - previousCycle;
    previousCycle = curCycle();
  }


}


void TimingCGRA::sendFetch(Fault fault, RequestPtr req, ThreadContext *tc)
{
  if (fault == NoFault) {
    DPRINTF(SimpleCPU, "Sending fetch for addr %#x(pa: %#x)\n",
        req->getVaddr(), req->getPaddr());
    ifetch_pkt = new Packet(req, MemCmd::ReadReq);
    //CGRA INSTRUCTIONS ARE STORED IN CGRA_instructions
    if (is_CPU()) 
      ifetch_pkt->dataStatic(&inst);
    else
      ifetch_pkt->dataStatic(CGRA_instructions);
    //CGRA RELATED END
    DPRINTF(SimpleCPU, " -- pkt addr: %#x\n", ifetch_pkt->getAddr());

    if (!icachePort.sendTimingReq(ifetch_pkt)) {
      // Need to wait for retry
      _status = IcacheRetry;
    } else {
      // Need to wait for cache to respond
      _status = IcacheWaitResponse;
      // ownership of packet transferred to memory system
      ifetch_pkt = NULL;
    }
  } else {
    DPRINTF(SimpleCPU, "Translation of addr %#x faulted\n", req->getVaddr());
    delete req;
    // fetch fault: advance directly to next instruction (fault handler)
    _status = BaseCGRA::Running;
    printf("\nCALL IN SENDFETCH.\n");
    advanceInst(fault);
  }

  numCycles += curCycle() - previousCycle;
  previousCycle = curCycle();
}


  void
TimingCGRA::advanceInst(Fault fault)
{
  
  ThreadContext *tc = threadContexts[0];
  
  if (is_CPU())
  {
    //CGRA execution should start as soon as 15 is written to register 11 of the CPU
    if ((int) tc->readIntReg(11) == 15)
    {
      printf("************************Setting UP the CGRA************\n");
      Setup_CGRA_Execution();
      if(Prolog>0 || KernelCounter>0 || EPILog>0)
      {
        Switch_To_CGRA();
	printf("\nExecuting 1 CGRA Cycle.\n");
        reschedule(fetchEvent, nextCycle(), true);
      }
      else
      {  
        //CGRA execution over
        Prepare_to_Switch_Back_to_CPU();
        Restore_CPU_Execution();
        Switch_To_CPU();
	printf("\nCALL IN ADVANCEINST TO CPU.\n");
        TimingCGRA::advanceInst(fault);

      }

    }
    else
    {
      //      printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
      //NORMAL CPU EXECUTION
      if (_status == Faulting)
        return;

      if (fault != NoFault) {
        advancePC(fault);
        DPRINTF(SimpleCPU, "Fault occured, scheduling fetch event\n");
        reschedule(fetchEvent, clockEdge(), true);
        _status = Faulting;
        return;
      }


      if (!stayAtPC)
        advancePC(fault);

      if (tryCompleteDrain())
        return;

      if (_status == BaseCGRA::Running) {
        // kick off fetch of next instruction... callback from icache
        // response will cause that instruction to be executed,
        // keeping the CPU running.
        fetch();
      }
    }
  }
  else
  {
    //CGRA EXECUTION
    printf("advanceInst CGRA MODE\n");
    if(Len==0)
    {  
      if(state==EPI)
        state=FINISH;
    }

    if(state==FINISH)
    {	
      printf("************************PREPARING TO MOVE TO CPU************\n");
      Prepare_to_Switch_Back_to_CPU();

    }
    //CGRA EXECUTION OVER
    if ((int) tc->readIntReg(8) == 66)
    {
      printf("newPC=%ld, II=%ld, EPILog=%ld, Prolog=%ld, Len=%ld\n", (long) newPC, (long) II, (long) EPILog, (long) Prolog, (long) Len);
      printf("**********************CGRA Execution is over**********************\n");
      Restore_CPU_Execution();
      Switch_To_CPU();
      printf("******REG R11= %d*********\n",(int)tc->readIntReg(11));
      printf("AFTER COMPLETETION PC: %x\n",(unsigned int) tc->instAddr());
      //TimingCGRA::advanceInst(fault);
    }
    else
    {
      printf("\nCALL IN NOW CGRA_ADVANCEINST.\n");	
      CGRA_advanceInst(fault);
    }

  } 
}

//CGRA RELATED START

void TimingCGRA::CGRA_completeIfetch(PacketPtr pkt)
{
  int memAccessFlag=0;
  // received a response from the icache: execute the received
  // instruction
  //FetchedInstuction++;
  Len--;	
  //KernelCount--;
  assert(!pkt || !pkt->isError());
  assert(_status == IcacheWaitResponse);
  printf("size of data is %d\n", pkt->getSize());
  //if (FetchedInstuction == 16)
  {
    _status = BaseCGRA::Running;

    numCycles += curCycle() - previousCycle;
    previousCycle = curCycle();
  }

  if (getDrainState() == Drainable::Draining)
  {
    if (pkt)
    {
      delete pkt->req;
      delete pkt;
    }

    completeDrain();
    return;
  }

  //*********FETCH********************
  for (int i = 0; i < CGRA_YDim; i++)
  {
    for (int j = 0; j < CGRA_XDim; j++)
    {
      CGRA_Instruction *currentIns = new CGRA_Instruction(CGRA_instructions[i * CGRA_XDim + j]);
      cgra_PEs[i * CGRA_YDim + j].Fetch(currentIns);

    }
  }

  //*********DECODE********************
  for (int i = 0; i < CGRA_YDim; i++)
  {
    for (int j = 0; j < CGRA_XDim; j++)
    {
      cgra_PEs[i * CGRA_YDim + j].Decode();

    }
  }

  if((Len == 0) & (state == KERN))
    KernelCounter--;
  cout << "KernelCounter: " << KernelCounter << endl;

  //*********EXECUTE********************
  for (int i = 0; i < CGRA_YDim; i++)
  {
    for (int j = 0; j < CGRA_XDim; j++)
    {
      cgra_PEs[i * CGRA_YDim + j].Execute();
    }
  }
  
  if(KernelCounter < 1)
    *Conditional_Reg = 0;
  

  //*********WRITE BACK********************
  for (int i = 0; i < CGRA_YDim; i++)
  {
    for (int j = 0; j < CGRA_XDim; j++)
    {
      cgra_PEs[i * CGRA_YDim + j].WriteBack();

    }
  }
  //*********PERFORM MEMORY OPERATIONS********************
  for (int i = 0; i < CGRA_YDim; i++)
  {
    if (MemBusStatus[i] == CGRA_MEMORY_READ)
    {
      memAccessFlag=1;
      //printf("In readMem\n"); 
      //printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
      readMem((Addr) MemAddress[i], (uint8_t *) &MemData[i], (unsigned) 4, (unsigned) 163,i);
       //printf("ReadMemData[%d],%d\n",i,MemData[i]);
      MemAccessCount++;
      y_dim=0; 
    }
    else if (MemBusStatus[i] == CGRA_MEMORY_WRITE)
    {
      memAccessFlag=1;
     // printf("IN writeMem\n"); 
      //printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
      writeMem((uint8_t *) &MemData[i], (unsigned) 4, (Addr) MemAddress[i], (unsigned) 131, unknownRes,i);
      //printf("WriteMemData[%d],%d\n",i,MemData[i]);
      MemAccessCount++;
      y_dim=0; 
    }
  }

  // printf("AFTER READMEM MEMDATA IS %d\n",MemData[0]);
  if(!memAccessFlag) {
    printf("\nCALL IN CGRA COMPLETE IFETCH MEMACCESSFLAG.\n");	
    advanceInst(NoFault);
  }
  // printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
  if (pkt)
  {
    delete pkt->req;
    delete pkt;
  }

  /* if (written == 0)
     {
     written = 1;
     printf("CGRA: Writing %d to address %x\n",writeValue,(unsigned int)Address_To_Write);
     writeMem((uint8_t *) &writeValue, (unsigned) 4, (Addr) Address_To_Write, (unsigned) 131, unknownRes);
     printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());

     }*/
  /* writeValue=42;
     if (written == 0)
     {
     written = 1;
     printf("CGRA: Reading from address %x\n",(unsigned int)Address_To_Write);
     readMem((Addr) Address_To_Write,(uint8_t *) &writeValue, (unsigned) 4,  (unsigned) 163);
     printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
     printf("Value read :%d \n",writeValue);

     }*/
  /*preExecute();
    if (curStaticInst && curStaticInst->isMemRef())
    {
  // load or store: just send to dcache
  Fault fault = curStaticInst->initiateAcc(this, traceData);

  // If we're not running now the instruction will complete in a dcache
  // response callback or the instruction faulted and has started an
  // ifetch
  if (_status == BaseCGRA::Running)
  {
  if (fault != NoFault && traceData)
  {
  // If there was a fault, we shouldn't trace this instruction.
  delete traceData;
  traceData = NULL;
  }

  postExecute();
  // @todo remove me after debugging with legion done
  if (curStaticInst && (!curStaticInst->isMicroop() || curStaticInst->isFirstMicroop()))
  instCnt++;
  printf("FETCHEDINST=%d\n",FetchedInstuction);
  //			if (FetchedInstuction == 16)
  //			{
  advanceInst(fault);
  //			}

  }
  }
  else if (curStaticInst)
  {
  printf("******1 COMPLETEIFETCH REG R11= %d*********\n",(int)tc->readIntReg(11));
  // non-memory instruction: execute completely now
  Fault fault = curStaticInst->execute(this, traceData);

  printf("******2 COMPLETEIFETCH REG R11= %d*********\n",(int)tc->readIntReg(11));
  // keep an instruction count
  if (fault == NoFault)
  countInst();
  else if (traceData && !DTRACE(ExecFaulting))
  {
  delete traceData;
  traceData = NULL;
  }

  postExecute();
  // @todo remove me after debugging with legion done
  printf("******3 COMPLETEIFETCH REG R11= %d*********\n",(int)tc->readIntReg(11));
  if (curStaticInst && (!curStaticInst->isMicroop() || curStaticInst->isFirstMicroop()))
  instCnt++;
  printf("FETCHEDINST=%d\n",FetchedInstuction);
  //		if (FetchedInstuction == 16)
  //		{
  advanceInst(fault);
  printf("******4 COMPLETEIFETCH REG R11= %d*********\n",(int)tc->readIntReg(11));
  //		}
  }
  else
  {
  //		if (FetchedInstuction == 16)
  //		{
  advanceInst(NoFault);
  //		}

  }

  printf("******5 COMPLETEIFETCH REG R11= %d*********\n",(int)tc->readIntReg(11));
  */
  // printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());

  // printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
  //printf("******COMPLETEIFETCH REG R11= %d*********\n",(int)tc->readIntReg(11));

}



//CGRA RELATED END




  void
TimingCGRA::completeIfetch(PacketPtr pkt)
{

  //printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
  if (is_CPU())
  {

    DPRINTF(SimpleCPU, "Complete ICache Fetch for addr %#x\n", pkt ?
        pkt->getAddr() : 0);

    // received a response from the icache: execute the received
    // instruction

    assert(!pkt || !pkt->isError());
    assert(_status == IcacheWaitResponse);

    _status = BaseCGRA::Running;

    numCycles += curCycle() - previousCycle;
    previousCycle = curCycle();

    preExecute();
    if (curStaticInst && curStaticInst->isMemRef()) {
      // load or store: just send to dcache
      Fault fault = curStaticInst->initiateAcc(this, traceData);

      // If we're not running now the instruction will complete in a dcache
      // response callback or the instruction faulted and has started an
      // ifetch
      if (_status == BaseCGRA::Running) {
        if (fault != NoFault && traceData) {
          // If there was a fault, we shouldn't trace this instruction.
          delete traceData;
          traceData = NULL;
        }

        postExecute();
        // @todo remove me after debugging with legion done
        if (curStaticInst && (!curStaticInst->isMicroop() ||
              curStaticInst->isFirstMicroop()))
          instCnt++;
	printf("\nCALL IN COMPLETEIFETCH MAIN IF.\n");
        advanceInst(fault);
      }
    } else if (curStaticInst) {
      // non-memory instruction: execute completely now
      Fault fault = curStaticInst->execute(this, traceData);

      // keep an instruction count
      if (fault == NoFault)
        countInst();
      else if (traceData && !DTRACE(ExecFaulting)) {
        delete traceData;
        traceData = NULL;
      }

      postExecute();
      // @todo remove me after debugging with legion done
      if (curStaticInst && (!curStaticInst->isMicroop() ||
            curStaticInst->isFirstMicroop()))
        instCnt++;
      printf("\nCALL IN COMPLETEIFETCH SECOND IF.\n");
      advanceInst(fault);
    } else {
      printf("\nCALL IN COMPLETEIFETCH SECOND ELSE.\n");	
      advanceInst(NoFault);
    }

    if (pkt) {
      delete pkt->req;
      delete pkt;
    }
  }
  else
  {
    CGRA_completeIfetch(pkt);
  }





}

  void
TimingCGRA::IcachePort::ITickEvent::process()
{
  cpu->completeIfetch(pkt);
}

  bool
TimingCGRA::IcachePort::recvTimingResp(PacketPtr pkt)
{
  DPRINTF(SimpleCPU, "Received timing response %#x\n", pkt->getAddr());
  // delay processing of returned data until next CPU clock edge

  if(cpu->CGRA_Mode == 0)
  {
    Tick next_tick = cpu->clockEdge();

    if (next_tick == curTick())
      cpu->completeIfetch(pkt);
    else
      tickEvent.schedule(pkt, next_tick);
  }
  else
  {
    cpu->completeIfetch(pkt);
  }

  return true;
}

void TimingCGRA::IcachePort::recvRetry()
{
  // we shouldn't get a retry unless we have a packet that we're
  // waiting to transmit
  assert(cpu->ifetch_pkt != NULL);
  assert(cpu->_status == IcacheRetry);
  PacketPtr tmp = cpu->ifetch_pkt;
  if (sendTimingReq(tmp)) {
    cpu->_status = IcacheWaitResponse;
    cpu->ifetch_pkt = NULL;
  }
}

//CGRA RELATED START

void TimingCGRA::CGRA_completeDataAccess(PacketPtr pkt)
{

  StatusPtr portStatus = NULL;
   
  SplitFragmentSenderState * send_state = NULL;

  if(pkt->senderState == NULL)
  {
    //not a split access
    portStatus = getDcachePort_status_ptr(pkt->req);
  }
  else
  {
    //a split access
    send_state = dynamic_cast<SplitFragmentSenderState *>(pkt->senderState);
    portStatus = getDcachePort_status_ptr(send_state->bigPkt->req);
  }
  printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
  // received a response from the dcache: complete the load or store
  // instruction
  assert(!pkt->isError());
  //assert(_status == DcacheWaitResponse || _status == DTBWaitResponse || pkt->req->getFlags().isSet(Request::NO_ACCESS));
  //assert(*portStatus == DcacheWaitResponse || *portStatus == DTBWaitResponse || pkt->req->getFlags().isSet(Request::NO_ACCESS));
  AccessCount++;

  if (pkt->senderState)
  {
    SplitFragmentSenderState * send_state = dynamic_cast<SplitFragmentSenderState *>(pkt->senderState);
    assert(send_state);
    delete pkt->req;
    delete pkt;
    PacketPtr big_pkt = send_state->bigPkt;
    delete send_state;

    SplitMainSenderState * main_send_state = dynamic_cast<SplitMainSenderState *>(big_pkt->senderState);
    assert(main_send_state);
    // Record the fact that this packet is no longer outstanding.
    assert(main_send_state->outstanding != 0);
    main_send_state->outstanding--;

    if (main_send_state->outstanding)
    {
      return;
    }
    else
    {
      delete main_send_state;
      big_pkt->senderState = NULL;
      pkt = big_pkt;
    }
  }

  if (AccessCount == MemAccessCount)
  {
    //		//printf("Setting _status 7\n");
    //printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
    _status = BaseCGRA::Running;
    *portStatus = Idle;
    numCycles += curCycle() - previousCycle;
    previousCycle = curCycle();
  }
  for (int i = 0; i < CGRA_YDim; i++)
  {
    Addr virtaddr=pkt->req->getVaddr();
    int *tempdata=(int*)pkt->data;
    if (MemBusStatus[i] == CGRA_MEMORY_READ)
      if(virtaddr==MemAddress[i])
        MemData[i]=*tempdata;
  }
  delete pkt->req;
  //printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
  delete pkt;

  numCycles += curCycle() - previousCycle;
  previousCycle = curCycle();

  if (AccessCount == MemAccessCount)
  {
    //printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
    printf("\nCALL IN DATA ACCESS COUNT MEMACCESS.\n");
    advanceInst (NoFault);
  }


}
//CGRA RELATED END

  void
TimingCGRA::completeDataAccess(PacketPtr pkt)
{
  if (is_CPU())
  {

  StatusPtr portStatus = NULL;
   
  SplitFragmentSenderState * send_state = NULL;

  if(pkt->senderState == NULL)
  {
    //not a split access
    portStatus = getDcachePort_status_ptr(pkt->req);
  }
  else
  {
    //a split access
    send_state = dynamic_cast<SplitFragmentSenderState *>(pkt->senderState);
    portStatus = getDcachePort_status_ptr(send_state->bigPkt->req);
  }
    // received a response from the dcache: complete the load or store
    // instruction
    assert(!pkt->isError());
    //assert(_status == DcacheWaitResponse || _status == DTBWaitResponse || pkt->req->getFlags().isSet(Request::NO_ACCESS));
    assert(*portStatus == DcacheWaitResponse || *portStatus == DTBWaitResponse || pkt->req->getFlags().isSet(Request::NO_ACCESS));

    numCycles += curCycle() - previousCycle;
    previousCycle = curCycle();

    if (pkt->senderState) {
      SplitFragmentSenderState * send_state =
        dynamic_cast<SplitFragmentSenderState *>(pkt->senderState);
      assert(send_state);
      delete pkt->req;
      delete pkt;
      PacketPtr big_pkt = send_state->bigPkt;
      delete send_state;

      SplitMainSenderState * main_send_state =
        dynamic_cast<SplitMainSenderState *>(big_pkt->senderState);
      assert(main_send_state);
      // Record the fact that this packet is no longer outstanding.
      assert(main_send_state->outstanding != 0);
      main_send_state->outstanding--;

      if (main_send_state->outstanding) {
        return;
      } else {
        delete main_send_state;
        big_pkt->senderState = NULL;
        pkt = big_pkt;
      }
    }

    _status = BaseCGRA::Running;
    *portStatus = Idle;

    Fault fault = curStaticInst->completeAcc(pkt, this, traceData);

    // keep an instruction count
    if (fault == NoFault)
      countInst();
    else if (traceData) {
      // If there was a fault, we shouldn't trace this instruction.
      delete traceData;
      traceData = NULL;
    }

    // the locked flag may be cleared on the response packet, so check
    // pkt->req and not pkt to see if it was a load-locked
    if (pkt->isRead() && pkt->req->isLLSC()) {
      TheISA::handleLockedRead(thread, pkt->req);
    }

    delete pkt->req;
    delete pkt;

    postExecute();
    printf("\nCALL IN COMPLETE DATA ACESSS MAIN IF.\n");
    advanceInst(fault);
  }
  else
  {
    //		printf("CGRA: completeDataAccess\n");
    CGRA_completeDataAccess(pkt);
  }

}

  bool
TimingCGRA::DcachePort::recvTimingResp(PacketPtr pkt)
{
  // delay processing of returned data until next CPU clock edge
  Tick next_tick = cpu->clockEdge();

  if (next_tick == curTick()) {
    cpu->completeDataAccess(pkt);
  } else {
    if (!tickEvent.scheduled()) {
      tickEvent.schedule(pkt, next_tick);
    } else {
      // In the case of a split transaction and a cache that is
      // faster than a CPU we could get two responses before
      // next_tick expires
      if (!retryEvent.scheduled())
        cpu->schedule(retryEvent, next_tick);
      return false;
    }
  }

  return true;
}

  void
TimingCGRA::DcachePort::DTickEvent::process()
{
  cpu->completeDataAccess(pkt);
}

//TODO
  void
TimingCGRA::DcachePort::recvRetry()
{
  // we shouldn't get a retry unless we have a packet that we're
  // waiting to transmit
  // assert(cpu->dcache_pkt != NULL);
  // assert(cpu->_status == DcacheRetry);
  //printf("\nPort Name %s\n",name().c_str());
  assert(dcache_pkt != NULL);
  assert(_status == DcacheRetry);
  assert(cpu->_status == WaitForMemory);
  //PacketPtr tmp = cpu->dcache_pkt;
  PacketPtr tmp = dcache_pkt;
  if (tmp->senderState) {
    // This is a packet from a split access.
    SplitFragmentSenderState * send_state =
      dynamic_cast<SplitFragmentSenderState *>(tmp->senderState);
    assert(send_state);
    PacketPtr big_pkt = send_state->bigPkt;

    SplitMainSenderState * main_send_state =
      dynamic_cast<SplitMainSenderState *>(big_pkt->senderState);
    assert(main_send_state);

    if (sendTimingReq(tmp)) {
      // If we were able to send without retrying, record that fact
      // and try sending the other fragment.
      send_state->clearFromParent();
      int other_index = main_send_state->getPendingFragment();
      if (other_index > 0) {
        tmp = main_send_state->fragments[other_index];
        //cpu->dcache_pkt = tmp;
        dcache_pkt = tmp;
        if ((big_pkt->isRead() && cpu->handleReadPacket(tmp)) ||
            (big_pkt->isWrite() && cpu->handleWritePacket(tmp))) {
          main_send_state->fragments[other_index] = NULL;
        }
      } else {
        //cpu->_status = DcacheWaitResponse;
        cpu->_status = WaitForMemory;
        _status = DcacheWaitResponse;
        // memory system takes ownership of packet
        //cpu->dcache_pkt = NULL;
        dcache_pkt = NULL;
      }
    }
  } else if (sendTimingReq(tmp)) {
    //cpu->_status = DcacheWaitResponse;
    _status = DcacheWaitResponse;
    cpu->_status = WaitForMemory;
    // memory system takes ownership of packet
    //cpu->dcache_pkt = NULL;
    dcache_pkt = NULL;
  }
}

TimingCGRA::IprEvent::IprEvent(Packet *_pkt, TimingCGRA *_cpu,
    Tick t)
: pkt(_pkt), cpu(_cpu)
{
  cpu->schedule(this, t);
}

  void
TimingCGRA::IprEvent::process()
{
  cpu->completeDataAccess(pkt);
}

const char *
TimingCGRA::IprEvent::description() const
{
  return "Timing Simple CPU Delay IPR event";
}


  void
TimingCGRA::printAddr(Addr a)
{
  dcachePort.printAddr(a);
}


////////////////////////////////////////////////////////////////////////
//
//  TimingCGRA Simulation Object
//
  TimingCGRA *
TimingCGRAParams::create()
{
  numThreads = 1;
  if (!FullSystem && workload.size() != 1)
    panic("only one workload allowed");
  return new TimingCGRA(this);
}

//CGRA RELATED START

void TimingCGRA::Setup_CGRA()
{

  Conditional_Reg=new int;
  CGRA_instructions = new uint32_t[16];
  unknownRes = new uint64_t[16];
  written = 0;

  cgra_PEs = new CGRA_PE[CGRA_YDim * CGRA_XDim];
  /*for (int i = 0; i < CGRA_YDim * CGRA_XDim; i++)
    {
    CGRA_PE[i] = new CGRA_PE();
    }*/
  cgra_decoder = new CGRADecoder();
  //fetch.Setup_CGRA_Decoder(cgra_decoder);

  //CGRA_ControlUnit = new CGRA_Controller();

  CGRA_DMem = new CGRA_DataMemory();
  //CGRA_ControlUnit->SetLen(CGRA_YDim * CGRA_YDim);

  MemPort = new CGRA_MemoryPort*[CGRA_YDim];
  for (int i = 0; i < CGRA_YDim; i++)
  {
    MemPort[i] = new CGRA_MemoryPort(&MemBusStatus[i], &MemData[i], &MemAddress[i]);
  }

  //Setting Neighbors - PopulatOutputing structure
  for (int i = 0; i < CGRA_YDim; i++)
  {
    for (int j = 0; j < CGRA_XDim; j++)
    {
      int iIndex;
      int jIndex;
      if (i == 0)
      {
        iIndex = 4;
      }
      else
      {
        iIndex = i;
      }
      if (j == 0)
      {
        jIndex = 4;
      }
      else
      {
        jIndex = j;
      }

      cgra_PEs[i * CGRA_YDim + j].SetNeighbours(&cgra_PEs[i * CGRA_YDim + ((jIndex - 1) % CGRA_XDim)],	//Left
          &cgra_PEs[i * CGRA_YDim + ((jIndex + 1) % CGRA_XDim)],	//Right
          &cgra_PEs[(((iIndex - 1) % CGRA_YDim) * CGRA_YDim) + j],	//Up
          &cgra_PEs[(((iIndex + 1) % CGRA_YDim) * CGRA_YDim) + j]);	//Down

      cgra_PEs[i * CGRA_YDim + j].SetCGRA_MemoryPort(MemPort[i]);
      cgra_PEs[i * CGRA_YDim + j].SetController_Reg(Conditional_Reg);
    }
  }

}

/*
 *Use of registers to initialize CGRA execution
 Reg Num: purpose
5: kenerl PC
12: prolog PC
10: Epilog PC
6: II
1: Epilog Length
2: Prolog Length
8: Kernel Counter

*/

void TimingCGRA::Setup_CGRA_Execution()
{

  printf("CGRA: OLD PC: %x\n",(unsigned int) tc->instAddr());
  TheISA::PCState pcState = thread->pcState();
  TheISA::advancePC(pcState, curStaticInst);
  thread->pcState(pcState);

  /*
  Code Below Reads Value For Compare Functions Performed In Prolog And Epilog
  To Caluclate KernelCounter Value
  KernelCounter = n - Prolog_Count - Epilog_Count; where n are loop iterations
  */
  /*ifstream countfile1;
  countfile1.open("prolog_cnt.txt");
  int prolog_count=13;
  countfile1 >> prolog_count; 
  countfile1.close();
  cout<<"Prolog is  "<<prolog_count<<endl;

  ifstream countfile2;
  countfile2.open("epilog_cnt.txt");
  int epilog_count=2;
  countfile2 >> epilog_count; 
  cout<<"Epilog is  "<<epilog_count<<endl;
*/
  KernelPC = (long) tc->readIntReg(5);
  PROLogPC = (long) tc->readIntReg(12);
  EPILogPC = (long) tc->readIntReg(10);
  KernelCounter= (int) tc->readIntReg(8); //+1;

  II = (int) tc->readIntReg(6);
  EPILog = (int) tc->readIntReg(1);
  Prolog = (int) tc->readIntReg(2);
  
  //LiveVar_St_Epilog = (int) tc->readIntReg(4);     
  
  //config= (int) tc->readIntReg(7); 

  cout<<"Loop count is  "<<KernelCounter<<endl;
  cout<<"Prolog Length is " << Prolog << endl;
  cout<<"Epilog Length is " << EPILog << endl;
  //cout<<"LiveVar_St_Epilog Length is " << LiveVar_St_Epilog << endl;
  //cout<<"Initial Configuration Length is " << config << endl;	
  cout<<"II is " << II << endl;
  //EPILog = EPILog - LiveVar_St_Epilog;

/*
  int new_epilog_length = (EPILog-LiveVar_St_Epilog);
  int new_prolog_length = Prolog - config;
 
  float epilog_count_by_II = ((float) new_epilog_length)/II; 
  float epilog_count_ceil = ceil(epilog_count_by_II);
  int epilog_count = (int) epilog_count_ceil;

  cout << "Epilog Count is: " << epilog_count << endl;
//  int prolog_count = (Prolog - new_epilog_length - config)/II;
  cout<<"Actual Epilog length is  "<<new_epilog_length<<endl;
  cout<<"Actual Prolog length is  "<<new_prolog_length<<endl;
//  KernelCounter = KernelCounter - ((new_epilog_length + new_prolog_length)/II); //Previously Shail made it as epilog_count
  KernelCounter = KernelCounter - (2*epilog_count);  */
  cout<<"Kernel Counter is  "<<KernelCounter<<endl;

  for(int i=0;i<CGRA_YDim;++i)
    for(int j=0;j<CGRA_XDim;++j) {
        cgra_PEs[i * CGRA_YDim + j].setII(II);
	cgra_PEs[i * CGRA_YDim + j].SetKernelCount(KernelCounter);
    }

  if(Prolog>0)
  { 
    printf("CGRA!!! PROLOG\n");
    newPC=PROLogPC;
    Len=Prolog;
    state=PRO;
  }
  else if(KernelCounter>0)
  { 
    printf("CGRA!!! KERNEL\n");
    newPC=KernelPC;
    Len=II;
    state=KERN;
  }
  else if(EPILog>0)
  {
    printf("CGRA!!! EPILOG\n");
    newPC=EPILogPC;
    Len=EPILog;
    state=EPI;
  }

  printf("CGRA PARAMETERS: PROLOG= %d, EPILOG=%d, II=%d, KernelCounter=%d\n",Prolog,EPILog,II,KernelCounter);
  printf("CGRA PARAMETERS: PROLOGPC= %x, EPILOGPC=%x,  KernelPC=%x\n",(unsigned int)PROLogPC,(unsigned int)EPILogPC,(unsigned int)KernelPC);
  printf("CGRA PC : %x\n",(unsigned int)newPC);
  printf("**********************CGRA Execution is started**************************** \n");
  backPC = tc->pcState();
  tc->pcState((Addr) newPC);
}

void TimingCGRA::Prepare_to_Switch_Back_to_CPU()
{
  tc->setIntReg(11, 16);
  //CGRA_Mode=0;
  //make sure to go back to actual  processor state
  tc->setIntReg(8, 66);
  //make sure that you reset the state of the core to make sure it will fetch afterward
  printf("Setting _status 9\n");
  _status = BaseCGRA::Running;
}

void TimingCGRA::Restore_CPU_Execution()
{
  tc->pcState(backPC);
}

void TimingCGRA::Switch_To_CPU()
{
  CGRA_Mode = 0;
}
void TimingCGRA::Switch_To_CGRA()
{
  CGRA_Mode = 1;
}
bool TimingCGRA::is_CPU()
{
  return (CGRA_Mode==0);
}


void TimingCGRA::setupFetchRequest(Request *req)
{
  BaseCGRA::setupFetchRequest(req);
  //IF CGRA, FETCH 64 BYTES OF INSTRUCTIONS
  if (!is_CPU())
  {
    req->setSize(64);
  }

}

void TimingCGRA::CGRA_advancePC()
{

  printf("*******IN ADVANCE PC******\n");

  printf("%s,%s,%d,PC:%x\n",__FILE__,__func__,__LINE__,(unsigned int) tc->instAddr());
  if(Len==0)
  { 
    if(state==PRO)
    {
      printf("\nPROLOG->KERNEL\n");
      newPC=KernelPC;
      state=KERN;
      Len=II;
      *Conditional_Reg=1;
    }
    if(state==KERN)
    {    
      if(*Conditional_Reg)
      {
        Len=II;
        newPC=KernelPC;
        *Conditional_Reg=0;
      }
      else
      {
        printf("\nKERNEL->EPILOG\n");
        state=EPI;
        newPC=EPILogPC;
        Len=EPILog;
      }
   
//     if(KernelCounter < 1)
//         KernelCounter--;
//     else
//         *Conditional_Reg = 0;
    }
  }
  else
    newPC+=64;
  tc->pcState((Addr) newPC);
}

void TimingCGRA::CGRA_advanceInst(Fault fault)
{

  if (_status == Faulting)
    return;

  if (fault != NoFault)
  {
    CGRA_advancePC();
    reschedule(fetchEvent, nextCycle(), true);
    _status = Faulting;
    return;
  }

  if (!stayAtPC)
  {
    CGRA_advancePC();
  }

  if (_status == BaseCGRA::Running)
  {
    // kick off fetch of next instruction... callback from icache
    // response will cause that instruction to be executed,
    //keeping the CPU running.
    // printf("%s,%d,PC:%x\n",__func__,__LINE__,(unsigned int) tc->instAddr());
    fetch();
  }

}

void TimingCGRA::completeDrain()
{
  DPRINTF(Drain, "CPU done draining, processing drain event\n");
  setDrainState(Drainable::Drained);
  drainManager->signalDrainDone();
}

//CGRA RELATED END







