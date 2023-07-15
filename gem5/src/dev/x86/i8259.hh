/*
 * Copyright (c) 2004-2005 The Regents of The University of Michigan
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

#ifndef __DEV_X86_I8259_HH__
#define __DEV_X86_I8259_HH__

#include "dev/intpin.hh"
#include "dev/io_device.hh"
#include "enums/X86I8259CascadeMode.hh"
#include "params/I8259.hh"

namespace X86ISA
{

class I8259 : public BasicPioDevice
{
  protected:
    static const int NumLines = 8;
    bool pinStates[NumLines];

    void init() override;

    Tick latency;
    std::vector<IntSourcePin<I8259> *> output;
    std::vector<IntSinkPin<I8259> *> inputs;
    Enums::X86I8259CascadeMode mode;
    I8259 *slave;

    // Interrupt Request Register
    uint8_t IRR;
    // In Service Register
    uint8_t ISR;
    // Interrupt Mask Register
    uint8_t IMR;

    // The higher order bits of the vector to return
    uint8_t vectorOffset;

    bool cascadeMode;
    // A bit vector of lines with responders attached, or the
    // responder id, depending
    // on if this is a requestor or responder PIC.
    uint8_t cascadeBits;

    bool edgeTriggered;
    bool readIRR;

    // State machine information for reading in initialization control words.
    bool expectICW4;
    int initControlWord;

    // Whether or not the PIC is in auto EOI mode.
    bool autoEOI;

    void requestInterrupt(int line);
    void handleEOI(int line);

  public:
    typedef I8259Params Params;

    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(_params);
    }

    I8259(Params * p);

    Port &
    getPort(const std::string &if_name, PortID idx=InvalidPortID) override
    {
        if (if_name == "inputs")
            return *inputs.at(idx);
        else if (if_name == "output")
            return *output.at(idx);
        else
            return BasicPioDevice::getPort(if_name, idx);
    }

    Tick read(PacketPtr pkt) override;
    Tick write(PacketPtr pkt) override;

    void
    maskAll()
    {
        IMR = 0xFF;
    }

    void
    unmaskAll()
    {
        IMR = 0x00;
    }

    void signalInterrupt(int line);
    void raiseInterruptPin(int number);
    void lowerInterruptPin(int number);
    int getVector();

    void serialize(CheckpointOut &cp) const override;
    void unserialize(CheckpointIn &cp) override;
};

} // namespace X86ISA

#endif //__DEV_X86_I8259_HH__
