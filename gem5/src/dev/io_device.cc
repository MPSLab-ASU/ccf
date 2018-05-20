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
 * Copyright (c) 2006 The Regents of The University of Michigan
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
 *          Nathan Binkert
 */

#include "base/trace.hh"
#include "debug/BusAddrRanges.hh"
#include "dev/io_device.hh"
#include "sim/system.hh"

PioPort::PioPort(PioDevice *dev)
    : SimpleTimingPort(dev->name() + ".pio", dev), device(dev)
{
}

Tick
PioPort::recvAtomic(PacketPtr pkt)
{
    // @todo: We need to pay for this and not just zero it out
    pkt->busFirstWordDelay = pkt->busLastWordDelay = 0;

    return pkt->isRead() ? device->read(pkt) : device->write(pkt);
}

AddrRangeList
PioPort::getAddrRanges() const
{
    return device->getAddrRanges();
}

PioDevice::PioDevice(const Params *p)
    : MemObject(p), sys(p->system), pioPort(this)
{}

PioDevice::~PioDevice()
{
}

void
PioDevice::init()
{
    if (!pioPort.isConnected())
        panic("Pio port of %s not connected to anything!", name());
    pioPort.sendRangeChange();
}

BaseSlavePort &
PioDevice::getSlavePort(const std::string &if_name, PortID idx)
{
    if (if_name == "pio") {
        return pioPort;
    }
    return MemObject::getSlavePort(if_name, idx);
}

unsigned int
PioDevice::drain(DrainManager *dm)
{
    unsigned int count;
    count = pioPort.drain(dm);
    if (count)
        setDrainState(Drainable::Draining);
    else
        setDrainState(Drainable::Drained);
    return count;
}

BasicPioDevice::BasicPioDevice(const Params *p, Addr size)
    : PioDevice(p), pioAddr(p->pio_addr), pioSize(size),
      pioDelay(p->pio_latency)
{}

AddrRangeList
BasicPioDevice::getAddrRanges() const
{
    assert(pioSize != 0);
    AddrRangeList ranges;
    DPRINTF(BusAddrRanges, "registering range: %#x-%#x\n", pioAddr, pioSize);
    ranges.push_back(RangeSize(pioAddr, pioSize));
    return ranges;
}
