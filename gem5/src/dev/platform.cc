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
 *
 * Authors: Ali Saidi
 *          Nathan Binkert
 */

#include "base/misc.hh"
#include "config/the_isa.hh"
#include "dev/platform.hh"
#include "sim/sim_exit.hh"

using namespace std;

Platform::Platform(const Params *p)
    : SimObject(p), intrctrl(p->intrctrl)
{
}

Platform::~Platform()
{
}

void
Platform::postPciInt(int line)
{
   panic("No PCI interrupt support in platform.");
}

void
Platform::clearPciInt(int line)
{
   panic("No PCI interrupt support in platform.");
}

Addr
Platform::pciToDma(Addr pciAddr) const
{
   panic("No PCI dma support in platform.");
   M5_DUMMY_RETURN
}

void
Platform::registerPciDevice(uint8_t bus, uint8_t dev, uint8_t func, uint8_t intr)
{
    uint32_t bdf = bus << 16 | dev << 8 | func << 0;
    if (pciDevices.find(bdf) != pciDevices.end())
        fatal("Two PCI devices have same bus:device:function\n");

    if (intLines.test(intr))
        fatal("Two PCI devices have same interrupt line: %d\n", intr);

    pciDevices.insert(bdf);

    intLines.set(intr);
}
