/*
 * Copyright (c) 2010, 2013 ARM Limited
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
 * Copyright (c) 2005 The Regents of The University of Michigan
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
 * Authors: Ali Saidi
 */


/** @file
 * Implementiation of a PL390 GIC
 */

#ifndef __DEV_ARM_GIC_PL390_H__
#define __DEV_ARM_GIC_PL390_H__

#include "base/bitunion.hh"
#include "cpu/intr_control.hh"
#include "dev/arm/base_gic.hh"
#include "dev/io_device.hh"
#include "dev/platform.hh"
#include "params/Pl390.hh"

/** @todo this code only assumes one processor for now. Low word
 * of intEnabled and pendingInt need to be replicated per CPU.
 * bottom 31 interrupts (7 words) need to be replicated for
 * for interrupt priority register, processor target registers
 * interrupt config registers  */

class Pl390 : public BaseGic
{
  protected:
    // distributor memory addresses
    static const int ICDDCR     = 0x000; // control register
    static const int ICDICTR    = 0x004; // controller type
    static const int ICDIIDR    = 0x008; // implementer id
    static const int ICDISER_ST = 0x100; // interrupt set enable
    static const int ICDISER_ED = 0x17c;
    static const int ICDICER_ST = 0x180; // interrupt clear enable
    static const int ICDICER_ED = 0x1fc;
    static const int ICDISPR_ST = 0x200; // set pending interrupt
    static const int ICDISPR_ED = 0x27c;
    static const int ICDICPR_ST = 0x280; // clear pending interrupt
    static const int ICDICPR_ED = 0x2fc;
    static const int ICDABR_ST  = 0x300; // active bit registers
    static const int ICDABR_ED  = 0x37c;
    static const int ICDIPR_ST  = 0x400; // interrupt priority registers
    static const int ICDIPR_ED  = 0x7f8;
    static const int ICDIPTR_ST = 0x800; // processor target registers
    static const int ICDIPTR_ED = 0xbf8;
    static const int ICDICFR_ST = 0xc00; // interrupt config registers
    static const int ICDICFR_ED = 0xcfc;
    static const int ICDSGIR    = 0xf00; // software generated interrupt
    static const int DIST_SIZE  = 0xfff;

    // cpu memory addressesa
    static const int ICCICR  = 0x00; // CPU control register
    static const int ICCPMR  = 0x04; // Interrupt priority mask
    static const int ICCBPR  = 0x08; // binary point register
    static const int ICCIAR  = 0x0C; // interrupt ack register
    static const int ICCEOIR = 0x10; // end of interrupt
    static const int ICCRPR  = 0x14; // runing priority
    static const int ICCHPIR = 0x18; // highest pending interrupt
    static const int ICCABPR = 0x1c; // aliased binary point
    static const int ICCIIDR = 0xfc; // cpu interface id register
    static const int CPU_SIZE  = 0xff;

    static const int SGI_MAX = 16;  // Number of Software Gen Interrupts
    static const int PPI_MAX = 16;  // Number of Private Peripheral Interrupts

    /** Mask off SGI's when setting/clearing pending bits */
    static const int SGI_MASK = 0xFFFF0000;

    /** Mask for bits that config N:N mode in ICDICFR's */
    static const int NN_CONFIG_MASK = 0x55555555;

    static const int CPU_MAX = 8;   // Max number of supported CPU interfaces
    static const int SPURIOUS_INT = 1023;
    static const int INT_BITS_MAX = 32;
    static const int INT_LINES_MAX = 1020;

    /** MSI-X register offset and size */
    static const int MSIX_SR = 0x0; // MSI register devices will write to
    static const int MSIX_SIZE = 0x4; // Size of MSI-X register space

    BitUnion32(SWI)
        Bitfield<3,0> sgi_id;
        Bitfield<23,16> cpu_list;
        Bitfield<25,24> list_type;
    EndBitUnion(SWI)

    BitUnion32(IAR)
        Bitfield<9,0> ack_id;
        Bitfield<12,10> cpu_id;
    EndBitUnion(IAR)

    /** Distributor address GIC listens at */
    Addr distAddr;

    /** CPU address GIC listens at */
    /** @todo is this one per cpu? */
    Addr cpuAddr;

    /** Latency for a distributor operation */
    Tick distPioDelay;

    /** Latency for a cpu operation */
    Tick cpuPioDelay;

    /** Latency for a interrupt to get to CPU */
    Tick intLatency;

    /** Gic enabled */
    bool enabled;

    /** Number of itLines enabled */
    uint32_t itLines;

    uint32_t itLinesLog2;

    /** interrupt enable bits for all possible 1020 interupts.
     * one bit per interrupt, 32 bit per word = 32 words */
    uint32_t intEnabled[INT_BITS_MAX];

    /** interrupt pending bits for all possible 1020 interupts.
     * one bit per interrupt, 32 bit per word = 32 words */
    uint32_t pendingInt[INT_BITS_MAX];

    /** interrupt active bits for all possible 1020 interupts.
     * one bit per interrupt, 32 bit per word = 32 words */
    uint32_t activeInt[INT_BITS_MAX];

    /** read only running priroity register, 1 per cpu*/
    uint32_t iccrpr[CPU_MAX];

    /** an 8 bit priority (lower is higher priority) for each
     * of the 1020 possible supported interrupts.
     */
    uint8_t intPriority[INT_LINES_MAX];

    /** an 8 bit cpu target id for each shared peripheral interrupt
     * of the 1020 possible supported interrupts.
     */
    uint8_t cpuTarget[INT_LINES_MAX];

    /** 2 bit per interrupt signaling if it's level or edge sensitive
     * and if it is 1:N or N:N */
    uint32_t intConfig[INT_BITS_MAX*2];

    /** CPU enabled */
    bool cpuEnabled[CPU_MAX];

    /** CPU priority */
    uint8_t cpuPriority[CPU_MAX];

    /** Binary point registers */
    uint8_t cpuBpr[CPU_MAX];

    /** highest interrupt that is interrupting CPU */
    uint32_t cpuHighestInt[CPU_MAX];

    /** One bit per cpu per software interrupt that is pending for each possible
     * sgi source. Indexed by SGI number. Each byte in generating cpu id and
     * bits in position is destination id. e.g. 0x4 = CPU 0 generated interrupt
     * for CPU 2. */
    uint64_t cpuSgiPending[SGI_MAX];
    uint64_t cpuSgiActive[SGI_MAX];

    /** One bit per private peripheral interrupt. Only upper 16 bits
     * will be used since PPI interrupts are numberred from 16 to 32 */
    uint32_t cpuPpiPending[CPU_MAX];
    uint32_t cpuPpiActive[CPU_MAX];

    /** Banked interrupt prioirty registers for SGIs and PPIs */
    uint8_t bankedIntPriority[CPU_MAX][SGI_MAX + PPI_MAX];

    /** IRQ Enable Used for debug */
    bool irqEnable;

    /** MSIX Register */
    Addr msixRegAddr;
    uint32_t msixReg;

    /** software generated interrupt
     * @param data data to decode that indicates which cpus to interrupt
     */
    void softInt(int ctx_id, SWI swi);

    /** See if some processor interrupt flags need to be enabled/disabled
     * @param hint which set of interrupts needs to be checked
     */
    void updateIntState(int hint);

    /** Update the register that records priority of the highest priority
     *  active interrupt*/
    void updateRunPri();

    /** generate a bit mask to check cpuSgi for an interrupt. */
    uint64_t genSwiMask(int cpu);

    int intNumToWord(int num) const { return num >> 5; }
    int intNumToBit(int num) const { return num % 32; }

    /** Post an interrupt to a CPU
     */
    void postInt(uint32_t cpu, Tick when);

    /** Event definition to post interrupt to CPU after a delay
    */
    class PostIntEvent : public Event
    {
      private:
        uint32_t cpu;
        Platform *platform;
      public:
        PostIntEvent( uint32_t c, Platform* p)
            : cpu(c), platform(p)
        { }
        void process() { platform->intrctrl->post(cpu, ArmISA::INT_IRQ, 0);}
        const char *description() const { return "Post Interrupt to CPU"; }
    };
    PostIntEvent *postIntEvent[CPU_MAX];

  public:
    typedef Pl390Params Params;
    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(_params);
    }
    Pl390(const Params *p);

    /** @{ */
    /** Return the address ranges used by the Gic
     * This is the distributor address + all cpu addresses
     */
    virtual AddrRangeList getAddrRanges() const;

    /** A PIO read to the device, immediately split up into
     * readDistributor() or readCpu()
     */
    virtual Tick read(PacketPtr pkt);

    /** A PIO read to the device, immediately split up into
     * writeDistributor() or writeCpu()
     */
    virtual Tick write(PacketPtr pkt);
    /** @} */

    /** @{ */
    /** Post an interrupt from a device that is connected to the Gic.
     * Depending on the configuration, the gic will pass this interrupt
     * on through to a CPU.
     * @param number number of interrupt to send */
    void sendInt(uint32_t number);

    /** Interface call for private peripheral interrupts  */
    void sendPPInt(uint32_t num, uint32_t cpu);

    /** Clear an interrupt from a device that is connected to the Gic
     * Depending on the configuration, the gic may de-assert it's cpu line
     * @param number number of interrupt to send */
    void clearInt(uint32_t number);

    /** Clear a (level-sensitive) PPI */
    void clearPPInt(uint32_t num, uint32_t cpu);
    /** @} */

    /** @{ */
    /* Various functions fer testing and debugging */
    void driveSPI(uint32_t spi);
    void driveLegIRQ(bool state);
    void driveLegFIQ(bool state);
    void driveIrqEn(bool state);
    /** @} */

    virtual void serialize(std::ostream &os);
    virtual void unserialize(Checkpoint *cp, const std::string &section);

  protected:
    /** Handle a read to the distributor poriton of the GIC
     * @param pkt packet to respond to
     */
    Tick readDistributor(PacketPtr pkt);

    /** Handle a read to the cpu poriton of the GIC
     * @param pkt packet to respond to
     */
    Tick readCpu(PacketPtr pkt);

    /** Handle a read to the MSI-X register on the GIC
     *  @param pkt packet to respond to
     */
    Tick readMsix(PacketPtr pkt);

    /** Handle a write to the distributor poriton of the GIC
     * @param pkt packet to respond to
     */
    Tick writeDistributor(PacketPtr pkt);

    /** Handle a write to the cpu poriton of the GIC
     * @param pkt packet to respond to
     */
    Tick writeCpu(PacketPtr pkt);

    /** Handle a write to the MSI-X register on the GIC
     *  @param pkt packet to process
     */
    Tick writeMsix(PacketPtr pkt);
};

#endif //__DEV_ARM_GIC_H__
