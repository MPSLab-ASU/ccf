/*
 * Copyright (c) 2012-2013, 2015, 2018, 2020 ARM Limited
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
 */

#ifndef __CGRA_ATOMIC_HH__
#define __CGRA_ATOMIC_HH__

#include "cpu/atomiccgra/base.hh"
#include "cpu/atomiccgra/exec_context.hh"
#include "mem/request.hh"
#include "params/AtomicCGRA.hh"
#include "sim/probe/probe.hh"

/*CGRA RELATED INCLUDES AND DEFINES STARTED*/
#include "cpu/atomiccgra/CGRAPE.h"

#define PRO 1
#define KERN 2
#define EPI 3
#define FINISH 4

#define CGRA_STATE_REG 0
#define CPU_STATE_REG 12
#define SIM_CLOCK_REG 12
#define CGRA_LOOPID_REG 11

#define CGRA_ACTIVATE 0xefefefef  // Previously 15
#define CGRA_DEACTIVATE 0xdfffffff  // Previously 17
#define CGRA_SWITCH 0xcfcfcfcf  // Previously 16
#define CGRA_EXEC_OVER 0xbfffffff  // Previously 66
#define SYS_CLOCK 0xafffffff


//#define DEBUG_BINARY  // Print binary inst and PC for CPU exec

//#define PC_DEBUG  // Print set of 14 regiters at selected PC range
#ifdef PC_DEBUG
  #define PC_DEBUG_BASE 0x36a48
  #define PC_DEBUG_TOP  0x377b8
#endif

#define MAX_INSTRUCTION_SIZE 32000
/*CGRA RELATED INCLUDES AND DEFINES END*/

enum ConnectionType {Simple_Connection, Diagonal_Connection, Hop_Connection, Only_Hop_Connection};
enum POSITION
    {
      topleftcorner=0,
      toprow,
      leftcol,
      toprightcorner,
      rightcol,
      bottomleftcorner,
      bottomrightcorner,
      bottomrow,
      regular
    };

class AtomicCGRA: public BaseCGRA 
{
  public:
  
    AtomicCGRA(AtomicCGRAParams *params);
    virtual ~AtomicCGRA();

    //void init() override;
    void init();

    /*CGRA DEFINITIONS START*/
    bool CGRA_Mode;

    //uint32_t *CGRA_instructions;
    uint64_t *CGRA_instructions;
    int loopID;
    void fetchInstructions(unsigned int *InstMem);
  //void fetch_CGRA_inst(long, uint64_t*);

    /*AtomicCGRA related CGRA functions. This is replaced by tick() in atomic mode
       due to the functionality of atomic mode in gem5.*/

    Tick latency = 0;

    //int x_dim=0;

    void CGRA_advancePC(SimpleThread*);
    void CGRA_advanceTime();
    
    int Position(int current, int x, int y);
    void Setup_CGRA();

    void Prepare_to_Switch_Back_to_CPU(SimpleThread*);
    void Setup_CGRA_Execution(SimpleThread*);
    void Setup_CGRA_Parameters();
    void Restore_CPU_Execution(SimpleThread*);

    void Switch_To_CPU();
    void Switch_To_CGRA();
    bool is_CPU();


    //int MemBusStatus[CGRA_XDim];	// Bus is shared per row
    //int MemData[CGRA_XDim];
    //uint64_t MemAddress[CGRA_XDim];
    //unsigned MemAccessAlignment[CGRA_XDim];
    
    int *MemBusDatatype;
    std::vector<int> MemBusStatus;
    int *MemData;
    float *FMemData;  
    //uint64_t *MemAddress;   
    std::vector<uint64_t> MemAddress;
    //unsigned *MemAccessAlignment; 
    std::vector<unsigned> MemAccessAlignment; 

    int MemAccessCount;
    int AccessCount;

    uint64_t* unknownRes;

    void completeDrain();
    void setupFetchRequest(const RequestPtr &req);
    
  protected:

    /*CGRA DEFINITIONS START*/
    long newPC;
    long EPILogPC;
    long PROLogPC;
    long KernelPC;
    unsigned II;
    unsigned EPILog;
    unsigned Prolog;
    unsigned Prolog_extension_cycle;
    unsigned Prolog_version_cycle;
    unsigned Len;
    int originalLen;
    int KernelCounter;
    unsigned CycleCounter;
    int LiveVar_St_Epilog;
    unsigned long long TotalLoops = 0;

    unsigned callback_reg;

    unsigned short state;

    bool written;
    int writeValue;
    int operand1;

    bool Conditional_Reg;
    unsigned Prolog_Branch_Cycle;
    //bool * PE_Conditional_Reg[CGRA_XDim*CGRA_YDim];
    bool ** PE_Conditional_Reg;
    bool isTCdynamic = false;
    //std::vector<CGRA_PE> cgra_PEs;    
    CGRA_PE* cgra_PEs;
    uint64_t fetched_instructions[MAX_INSTRUCTION_SIZE];

    TheISA::PCState backPC;
    /*CGRA DEFINITIONS END*/

    EventFunctionWrapper tickEvent;

    const int width;
    bool locked;
    const bool simulate_data_stalls;
    const bool simulate_inst_stalls;

    // main simulation loop (one cycle)
    void tick();

    void CGRA_Execution(SimpleExecContext& t_info);
    /**
     * Check if a system is in a drained state.
     *
     * We need to drain if:
     * <ul>
     * <li>We are in the middle of a microcode sequence as some CPUs
     *     (e.g., HW accelerated CPUs) can't be started in the middle
     *     of a gem5 microcode sequence.
     *
     * <li>The CPU is in a LLSC region. This shouldn't normally happen
     *     as these are executed atomically within a single tick()
     *     call. The only way this can happen at the moment is if
     *     there is an event in the PC event queue that affects the
     *     CPU state while it is in an LLSC region.
     *
     * <li>Stay at PC is true.
     * </ul>
     */
    bool isCpuDrained() const {
        SimpleExecContext &t_info = *threadInfo[curThread];

        return t_info.thread->microPC() == 0 &&
            !locked &&
            !t_info.stayAtPC;
    }

    /**
     * Try to complete a drain request.
     *
     * @returns true if the CPU is drained, false otherwise.
     */
    bool tryCompleteDrain();

    virtual Tick sendPacket(RequestPort &port, const PacketPtr &pkt);

    /**
     * An AtomicCGRAPort overrides the default behaviour of the
     * recvAtomicSnoop and ignores the packet instead of panicking. It
     * also provides an implementation for the purely virtual timing
     * functions and panics on either of these.
     */
    class AtomicCGRAPort : public RequestPort
    {

      public:

      AtomicCGRAPort(const std::string &_name, BaseCGRA* _cpu, std::string index)
            : RequestPort(_name+index, _cpu)
        { }

      protected:

        bool recvTimingResp(PacketPtr pkt)
        {
            panic("Atomic CPU doesn't expect recvTimingResp!\n");
            return true;
        }

        void recvReqRetry()
        {
            panic("Atomic CPU doesn't expect recvRetry!\n");
        }

    };

    class AtomicCGRADPort : public AtomicCGRAPort
    {

      public:
      AtomicCGRADPort(const std::string &_name, BaseCGRA* _cpu, std::string index) // index = row number
	: AtomicCGRAPort(_name, _cpu, index), cpu(_cpu), _status(Idle)
        {
            cacheBlockMask = ~(cpu->cacheLineSize() - 1);
        }

        bool isSnooping() const { return true; }

        Addr cacheBlockMask;
        Status _status;
      protected:
        BaseCGRA *cpu;

        virtual Tick recvAtomicSnoop(PacketPtr pkt);
        virtual void recvFunctionalSnoop(PacketPtr pkt);
    };

    typedef AtomicCGRADPort* DcachePort_Ptr;
    typedef BaseCGRA::Status* StatusPtr;
  
    AtomicCGRAPort icachePort;
    AtomicCGRADPort dcachePort;

    int CGRA_XDim;
    int CGRA_YDim;
    int RFSize;
    int regsize;
    int connection_type;

    std::vector<AtomicCGRADPort> dcache_CGRA_ports;
    typedef std::vector<AtomicCGRAPort>::iterator RequestPortIter;

    DcachePort_Ptr getDcachePort_ptr(RequestPtr req, int xdim);
    DcachePort_Ptr getDcachePort_ptr_from_addr(Addr vaddr, int xdim);

    StatusPtr getDcachePort_status_ptr(RequestPtr req, int xdim);

    RequestPtr ifetch_req;
    RequestPtr data_read_req;
    RequestPtr data_write_req;
    RequestPtr data_amo_req;

    bool dcache_access;
    bool icache_access;
    Tick icache_latency;
    Tick dcache_latency;

    /** Probe Points. */
    ProbePointArg<std::pair<SimpleThread*, const StaticInstPtr>> *ppCommit;

  protected:

    /** Return a reference to the data port. */
    //Port &getDataPort() override { return dcachePort; }
    Port &getDataPort() { return dcachePort; }

    /** Return a reference to the instruction port. */
    //Port &getInstPort() override { return icachePort; }
    Port &getInstPort() { return icachePort; }


    /** Perform snoop for other cpu-local thread contexts. */
    void threadSnoop(PacketPtr pkt, ThreadID sender);

  public:

    //DrainState drain() override;
    DrainState drain(); // override;

  //void drainResume() override;
    void drainResume();

  //    void switchOut();
    void switchOut();
    //void takeOverFrom(BaseCPU *oldCPU) override;
    void takeOverFrom(BaseCPU *oldCPU);

  //void verifyMemoryMode() const override;
    void verifyMemoryMode() const;

  //void activateContext(ThreadID thread_num) override;
    void activateContext(ThreadID thread_num);
  //void suspendContext(ThreadID thread_num) override;
    void suspendContext(ThreadID thread_num);

    /**
     * Helper function used to set up the request for a single fragment of a
     * memory access.
     *
     * Takes care of setting up the appropriate byte-enable mask for the
     * fragment, given the mask for the entire memory access.
     *
     * @param req Pointer to the Request object to populate.
     * @param frag_addr Start address of the fragment.
     * @param size Total size of the memory access in bytes.
     * @param flags Request flags.
     * @param byte_enable Byte-enable mask for the entire memory access.
     * @param[out] frag_size Fragment size.
     * @param[in,out] size_left Size left to be processed in the memory access.
     * @return True if the byte-enable mask for the fragment is not all-false.
     */
    bool genMemFragmentRequest(const RequestPtr& req, Addr frag_addr,
                               int size, Request::Flags flags,
                               const std::vector<bool>& byte_enable,
                               int& frag_size, int& size_left) const;

    Fault readMem(Addr addr, uint8_t *data, unsigned size,
                  Request::Flags flags,
                  const std::vector<bool>& byte_enable = std::vector<bool>());  //override;

    /* CGRA readMem*/
    Fault readMem(Addr addr, uint8_t *data, unsigned size,
                  Request::Flags flags,
                  const std::vector<bool>& byte_enable = std::vector<bool>(),int xdim=-1);

    Fault initiateHtmCmd(Request::Flags flags) //override
    {
        panic("initiateHtmCmd() is for timing accesses, and should "
              "never be called on AtomicSimpleCPU.\n");
    }

    void htmSendAbortSignal(HtmFailureFaultCause cause) //override
    {
        panic("htmSendAbortSignal() is for timing accesses, and should "
              "never be called on AtomicSimpleCPU.\n");
    }

    Fault writeMem(uint8_t *data, unsigned size,
                   Addr addr, Request::Flags flags, uint64_t *res,
                   const std::vector<bool>& byte_enable = std::vector<bool>()); // override;
    /* CGRA writeMem */
    Fault writeMem(uint8_t *data, unsigned size,
                   Addr addr, Request::Flags flags, uint64_t *res,
                   const std::vector<bool>& byte_enable = std::vector<bool>(),int xdim=-1);

    Fault amoMem(Addr addr, uint8_t* data, unsigned size,
                 Request::Flags flags, AtomicOpFunctorPtr amo_op); // override;

    void regProbePoints(); //override;

    /**
     * Print state of address in memory system via PrintReq (for
     * debugging).
     */
    void printAddr(Addr a);
};

#endif // __CGRA_ATOMIC_HH__
