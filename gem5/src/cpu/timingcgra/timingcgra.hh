/*
 * Copyright (c) 2012-2013 ARM Limited
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

#ifndef __CGRA_TIMING_HH__
#define __CGRA_TIMING_HH__

#include "cpu/timingcgra/base.hh"
#include "cpu/translation.hh"
#include "params/TimingCGRA.hh"

/*CGRA RELATED INCLUDES AND DEFINES STARTED*/
#include "cpu/timingcgra/CGRAPE.h"
#include "cpu/timingcgra/CGRA_MemoryPort.h"
#include "cpu/timingcgra/CGRAController.h"
#include "cpu/timingcgra/CGRADecoder.h"
#define PRO 1
#define KERN 2
#define EPI 3
#define FINISH 4
/*CGRA RELATED INCLUDES AND DEFINES END*/
typedef PacketPtr* PacketPtrPtr;
typedef BaseCGRA::Status* StatusPtr;

class TimingCGRA : public BaseCGRA
{
  public:

    TimingCGRA(TimingCGRAParams * params);
    virtual ~TimingCGRA();

    virtual void init();

    /*CGRA DEFINITIONS START*/
    int CGRA_Mode;

    uint32_t *CGRA_instructions;
    int FetchedInstuction;

    void CGRA_fetch();	//copied
    void CGRA_completeIfetch(PacketPtr pkt);	//copied
    void sendFetchCGRA(Fault fault, RequestPtr req, ThreadContext *tc);	//done
    void CGRA_completeDataAccess(PacketPtr pkt);	//COPIED
    void CGRA_advanceInst(Fault fault);	//copied
    void CGRA_advancePC();	//done


    void Setup_CGRA();	//done

    void Prepare_to_Switch_Back_to_CPU();	//done
    void Setup_CGRA_Execution();	//done
    void Restore_CPU_Execution();	//done

    void Switch_To_CPU();	//done
    void Switch_To_CGRA();	//done
    bool is_CPU();	//done

    int MemBusStatus[CGRA_YDim];
    int MemData[CGRA_YDim];
    uint64_t MemAddress[CGRA_YDim];
    int MemAccessCount;
    int AccessCount;

    uint64_t* unknownRes;

    void completeDrain();
    void setupFetchRequest(Request *req);

  private:
    long newPC;
    long EPILogPC;
    long PROLogPC;
    long KernelPC;
    int II;
    int EPILog;
    int Prolog;
    int Len;
    int originalLen;
    int KernelCounter;
    int LiveVar_St_Epilog;
    int config;

    int state;

    bool written;
    int writeValue;
    int operand1;

    int* Conditional_Reg;

    CGRA_PE* cgra_PEs;

    CGRA_MemoryPort **MemPort;
    CGRA_DataMemory* CGRA_DMem;

    CGRA_Controller* CGRA_ControlUnit;
    CGRADecoder* cgra_decoder;

    TheISA::PCState backPC;

    /*CGRA DEFINITIONS END*/		

  private:

    /*
     * If an access needs to be broken into fragments, currently at most two,
     * the the following two classes are used as the sender state of the
     * packets so the CPU can keep track of everything. In the main packet
     * sender state, there's an array with a spot for each fragment. If a
     * fragment has already been accepted by the CPU, aka isn't waiting for
     * a retry, it's pointer is NULL. After each fragment has successfully
     * been processed, the "outstanding" counter is decremented. Once the
     * count is zero, the entire larger access is complete.
     */
    class SplitMainSenderState : public Packet::SenderState
  {
    public:
      int outstanding;
      PacketPtr fragments[2];

      int
        getPendingFragment()
        {
          if (fragments[0]) {
            return 0;
          } else if (fragments[1]) {
            return 1;
          } else {
            return -1;
          }
        }
  };

    class SplitFragmentSenderState : public Packet::SenderState
  {
    public:
      SplitFragmentSenderState(PacketPtr _bigPkt, int _index) :
        bigPkt(_bigPkt), index(_index)
    {}
      PacketPtr bigPkt;
      int index;

      void
        clearFromParent()
        {
          SplitMainSenderState * main_send_state =
            dynamic_cast<SplitMainSenderState *>(bigPkt->senderState);
          main_send_state->fragments[index] = NULL;
        }
  };

    class FetchTranslation : public BaseTLB::Translation
  {
    protected:
      TimingCGRA *cpu;

    public:
      FetchTranslation(TimingCGRA *_cpu)
        : cpu(_cpu)
      {}

      void
        markDelayed()
        {
          assert(cpu->_status == BaseCGRA::Running);
          cpu->_status = ITBWaitResponse;
        }

      void
        finish(Fault fault, RequestPtr req, ThreadContext *tc,
            BaseTLB::Mode mode)
        {
          cpu->sendFetch(fault, req, tc);
        }
  };
    FetchTranslation fetchTranslation;

    void sendData(RequestPtr req, uint8_t *data, uint64_t *res, bool read);
    void sendSplitData(RequestPtr req1, RequestPtr req2, RequestPtr req,
        uint8_t *data, bool read);

    void translationFault(Fault fault);

    void buildPacket(PacketPtr &pkt, RequestPtr req, bool read);
    void buildSplitPacket(PacketPtr &pkt1, PacketPtr &pkt2,
        RequestPtr req1, RequestPtr req2, RequestPtr req,
        uint8_t *data, bool read);

    bool handleReadPacket(PacketPtr pkt);
    // This function always implicitly uses dcache_pkt.
    bool handleWritePacket(PacketPtr req);

    /**
     * A TimingCPUPort overrides the default behaviour of the
     * recvTiming and recvRetry and implements events for the
     * scheduling of handling of incoming packets in the following
     * cycle.
     */
    class TimingCPUPort : public MasterPort
  {
    public:

      TimingCPUPort(const std::string& _name, TimingCGRA* _cpu)
        : MasterPort(_name, _cpu), cpu(_cpu), retryEvent(this)
      { }

    protected:

      /**
       * Snooping a coherence request, do nothing.
       */
      virtual void recvTimingSnoopReq(PacketPtr pkt) { }

      TimingCGRA* cpu;

      struct TickEvent : public Event
    {
      PacketPtr pkt;
      TimingCGRA *cpu;

      TickEvent(TimingCGRA *_cpu) : pkt(NULL), cpu(_cpu) {}
      const char *description() const { return "Timing CPU tick"; }
      void schedule(PacketPtr _pkt, Tick t);
    };

      EventWrapper<MasterPort, &MasterPort::sendRetry> retryEvent;
  };

    class IcachePort : public TimingCPUPort
  {
    public:

      IcachePort(TimingCGRA *_cpu)
        : TimingCPUPort(_cpu->name() + ".icache_port", _cpu),
        tickEvent(_cpu)
    { }

    protected:

      virtual bool recvTimingResp(PacketPtr pkt);

      virtual void recvRetry();

      struct ITickEvent : public TickEvent
    {

      ITickEvent(TimingCGRA *_cpu)
        : TickEvent(_cpu) {}
      void process();
      const char *description() const { return "Timing CPU icache tick"; }
    };

      ITickEvent tickEvent;

  };

    class DcachePort : public TimingCPUPort
  {
    public:

      DcachePort(TimingCGRA *_cpu)
        : TimingCPUPort(_cpu->name() + ".dcache_port", _cpu),dcache_pkt(NULL),_status(Idle),tickEvent(_cpu)
    { }

      //for CGRA
      DcachePort(TimingCGRA *_cpu,std::string num)
        : TimingCPUPort(_cpu->name() + ".dcache_port"+num, _cpu),dcache_pkt(NULL),_status(Idle),tickEvent(_cpu)
    { }

    PacketPtr dcache_pkt;
    Status _status;
    protected:

      virtual bool recvTimingResp(PacketPtr pkt);

      virtual void recvRetry();

      struct DTickEvent : public TickEvent
    {
      DTickEvent(TimingCGRA *_cpu)
        : TickEvent(_cpu) {}
      void process();
      const char *description() const { return "Timing CPU dcache tick"; }
    };

      DTickEvent tickEvent;

  };

    typedef DcachePort* DcachePort_Ptr;
    //TODO
    IcachePort icachePort;
    DcachePort dcachePort;

    //CGRA PORTS
    DcachePort dcachePort1;
    DcachePort dcachePort2;
    DcachePort dcachePort3;
    DcachePort dcachePort4; 
   
    //TODO
    PacketPtr ifetch_pkt;

    //CGRA port packets
/*    PacketPtr dcache_pkt1;
    PacketPtr dcache_pkt2;
    PacketPtr dcache_pkt3;
    PacketPtr dcache_pkt4;
*/
    PacketPtrPtr  getDcache_pkt_ptr(RequestPtr req);

    DcachePort_Ptr getDcachePort_ptr(RequestPtr req);
    DcachePort_Ptr getDcachePort_ptr_from_addr(Addr vaddr);

    StatusPtr getDcachePort_status_ptr(RequestPtr req);
//Mahesh

    DcachePort_Ptr getDcachePort_ptr(RequestPtr req, int ydim);
    DcachePort_Ptr getDcachePort_ptr_from_addr(Addr vaddr, int ydim);

    StatusPtr getDcachePort_status_ptr(RequestPtr req, int ydim);
    Tick previousCycle;

  protected:

    /** Return a reference to the data port. */
    virtual MasterPort &getDataPort() { return dcachePort; }

    /** Return a reference to the CGRA data port. */
    virtual MasterPort &getDataPort1() { return dcachePort1; }
    virtual MasterPort &getDataPort2() { return dcachePort2; }
    virtual MasterPort &getDataPort3() { return dcachePort3; }
    virtual MasterPort &getDataPort4() { return dcachePort4; }

    /** Return a reference to the instruction port. */
    virtual MasterPort &getInstPort() { return icachePort; }

  public:

    unsigned int drain(DrainManager *drain_manager);
    void drainResume();

    void switchOut();
    void takeOverFrom(BaseCPU *oldCPU);

    void verifyMemoryMode() const;

    virtual void activateContext(ThreadID thread_num, Cycles delay);
    virtual void suspendContext(ThreadID thread_num);

    Fault readMem(Addr addr, uint8_t *data, unsigned size, unsigned flags,int ydim);
    Fault readMem(Addr addr, uint8_t *data, unsigned size, unsigned flags);

    Fault writeMem(uint8_t *data, unsigned size,Addr addr, unsigned flags, uint64_t *res);
    Fault writeMem(uint8_t *data, unsigned size,Addr addr, unsigned flags, uint64_t *res,int ydim);

    void fetch();
    void sendFetch(Fault fault, RequestPtr req, ThreadContext *tc);
    void completeIfetch(PacketPtr );
    void completeDataAccess(PacketPtr pkt);
    void advanceInst(Fault fault);

    /** This function is used by the page table walker to determine if it could
     * translate the a pending request or if the underlying request has been
     * squashed. This always returns false for the simple timing CPU as it never
     * executes any instructions speculatively.
     * @ return Is the current instruction squashed?
     */
    bool isSquashed() const { return false; }

    /**
     * Print state of address in memory system via PrintReq (for
     * debugging).
     */
    void printAddr(Addr a);

    /**
     * Finish a DTB translation.
     * @param state The DTB translation state.
     */
    void finishTranslation(WholeTranslationState *state);

  private:

    typedef EventWrapper<TimingCGRA, &TimingCGRA::fetch> FetchEvent;
    FetchEvent fetchEvent;

    struct IprEvent : Event {
      Packet *pkt;
      TimingCGRA *cpu;
      IprEvent(Packet *_pkt, TimingCGRA *_cpu, Tick t);
      virtual void process();
      virtual const char *description() const;
    };

    /**
     * Check if a system is in a drained state.
     *
     * We need to drain if:
     * <ul>
     * <li>We are in the middle of a microcode sequence as some CPUs
     *     (e.g., HW accelerated CPUs) can't be started in the middle
     *     of a gem5 microcode sequence.
     *
     * <li>Stay at PC is true.
     *
     * <li>A fetch event is scheduled. Normally this would never be the
     *     case with microPC() == 0, but right after a context is
     *     activated it can happen.
     * </ul>
     */
    bool isDrained() {
      return microPC() == 0 && !stayAtPC && !fetchEvent.scheduled();
    }

    /**
     * Try to complete a drain request.
     *
     * @returns true if the CPU is drained, false otherwise.
     */
    bool tryCompleteDrain();

    /**
     * Drain manager to use when signaling drain completion
     *
     * This pointer is non-NULL when draining and NULL otherwise.
     */
    DrainManager *drainManager;
};

#endif // __CGRA_TIMING_HH__
