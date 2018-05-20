/*
 * Copyright (c) 2010 ARM Limited
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
 * Copyright (c) 2002-2006 The Regents of The University of Michigan
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

#include <iostream>

#include "arch/arm/system.hh"
#include "base/loader/object_file.hh"
#include "base/loader/symtab.hh"
#include "cpu/thread_context.hh"
#include "mem/physical.hh"
#include "mem/fs_translating_port_proxy.hh"

using namespace std;
using namespace Linux;

ArmSystem::ArmSystem(Params *p)
    : System(p), bootldr(NULL), multiProc(p->multi_proc)
{
    if (p->boot_loader != "") {
        bootldr = createObjectFile(p->boot_loader);

        if (!bootldr)
            fatal("Could not read bootloader: %s\n", p->boot_loader);

        bootldr->loadGlobalSymbols(debugSymbolTable);

    }
    debugPrintkEvent = addKernelFuncEvent<DebugPrintkEvent>("dprintk");
}

void
ArmSystem::initState()
{
    // Moved from the constructor to here since it relies on the
    // address map being resolved in the interconnect

    // Call the initialisation of the super class
    System::initState();

    const Params* p = params();

    if (bootldr) {
        bootldr->loadSections(physProxy);

        uint8_t jump_to_bl[] =
        {
            0x07, 0xf0, 0xa0, 0xe1  // branch to r7
        };
        physProxy.writeBlob(0x0, jump_to_bl, sizeof(jump_to_bl));

        inform("Using bootloader at address %#x\n", bootldr->entryPoint());

        // Put the address of the boot loader into r7 so we know
        // where to branch to after the reset fault
        // All other values needed by the boot loader to know what to do
        if (!p->gic_cpu_addr || !p->flags_addr)
            fatal("gic_cpu_addr && flags_addr must be set with bootloader\n");

        for (int i = 0; i < threadContexts.size(); i++) {
            threadContexts[i]->setIntReg(3, kernelEntry & loadAddrMask);
            threadContexts[i]->setIntReg(4, params()->gic_cpu_addr);
            threadContexts[i]->setIntReg(5, params()->flags_addr);
            threadContexts[i]->setIntReg(7, bootldr->entryPoint());
        }
    } else {
        // Set the initial PC to be at start of the kernel code
        threadContexts[0]->pcState(kernelEntry & loadAddrMask);
    }
}

ArmSystem::~ArmSystem()
{
    if (debugPrintkEvent)
        delete debugPrintkEvent;
}


ArmSystem *
ArmSystemParams::create()
{
    return new ArmSystem(this);
}
