/*
 * Copyright (c) 2003-2004 The Regents of The University of Michigan
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
 * Authors: Gabe Black
 *          Ali Saidi
 */

#include "arch/alpha/isa_traits.hh"
#include "arch/alpha/process.hh"
#include "base/loader/elf_object.hh"
#include "base/loader/object_file.hh"
#include "base/misc.hh"
#include "cpu/thread_context.hh"
#include "debug/Loader.hh"
#include "mem/page_table.hh"
#include "sim/byteswap.hh"
#include "sim/process_impl.hh"
#include "sim/system.hh"

using namespace AlphaISA;
using namespace std;

AlphaLiveProcess::AlphaLiveProcess(LiveProcessParams *params,
                                   ObjectFile *objFile)
    : LiveProcess(params, objFile)
{
    brk_point = objFile->dataBase() + objFile->dataSize() + objFile->bssSize();
    brk_point = roundUp(brk_point, VMPageSize);

    // Set up stack.  On Alpha, stack goes below text section.  This
    // code should get moved to some architecture-specific spot.
    stack_base = objFile->textBase() - (409600+4096);

    // Set up region for mmaps.  Tru64 seems to start just above 0 and
    // grow up from there.
    mmap_start = mmap_end = 0x10000;

    // Set pointer for next thread stack.  Reserve 8M for main stack.
    next_thread_stack_base = stack_base - (8 * 1024 * 1024);

}

void
AlphaLiveProcess::argsInit(int intSize, int pageSize)
{
    objFile->loadSections(initVirtMem);

    typedef AuxVector<uint64_t> auxv_t;
    std::vector<auxv_t>  auxv;

    ElfObject * elfObject = dynamic_cast<ElfObject *>(objFile);
    if(elfObject)
    {
        // modern glibc uses a bunch of auxiliary vectors to set up 
        // TLS as well as do a bunch of other stuff
        // these vectors go on the bottom of the stack, below argc/argv/envp
        // pointers but above actual arg strings
        // I don't have all the ones glibc looks at here, but so far it doesn't
        // seem to be a problem.
        // check out _dl_aux_init() in glibc/elf/dl-support.c for details
        // --Lisa
        auxv.push_back(auxv_t(M5_AT_PAGESZ, AlphaISA::VMPageSize));
        auxv.push_back(auxv_t(M5_AT_CLKTCK, 100));
        auxv.push_back(auxv_t(M5_AT_PHDR, elfObject->programHeaderTable()));
        DPRINTF(Loader, "auxv at PHDR %08p\n", elfObject->programHeaderTable());
        auxv.push_back(auxv_t(M5_AT_PHNUM, elfObject->programHeaderCount()));
        auxv.push_back(auxv_t(M5_AT_ENTRY, objFile->entryPoint()));
        auxv.push_back(auxv_t(M5_AT_UID, uid()));
        auxv.push_back(auxv_t(M5_AT_EUID, euid()));
        auxv.push_back(auxv_t(M5_AT_GID, gid()));
        auxv.push_back(auxv_t(M5_AT_EGID, egid()));

    }

    // Calculate how much space we need for arg & env & auxv arrays.
    int argv_array_size = intSize * (argv.size() + 1);
    int envp_array_size = intSize * (envp.size() + 1);
    int auxv_array_size = intSize * 2 * (auxv.size() + 1);

    int arg_data_size = 0;
    for (vector<string>::size_type i = 0; i < argv.size(); ++i) {
        arg_data_size += argv[i].size() + 1;
    }
    int env_data_size = 0;
    for (vector<string>::size_type i = 0; i < envp.size(); ++i) {
        env_data_size += envp[i].size() + 1;
    }

    int space_needed =
        argv_array_size + 
        envp_array_size + 
        auxv_array_size +
        arg_data_size + 
        env_data_size;

    if (space_needed < 32*1024)
        space_needed = 32*1024;

    // set bottom of stack
    stack_min = stack_base - space_needed;
    // align it
    stack_min = roundDown(stack_min, pageSize);
    stack_size = stack_base - stack_min;
    // map memory
    allocateMem(stack_min, roundUp(stack_size, pageSize));

    // map out initial stack contents
    Addr argv_array_base = stack_min + intSize; // room for argc
    Addr envp_array_base = argv_array_base + argv_array_size;
    Addr auxv_array_base = envp_array_base + envp_array_size;
    Addr arg_data_base = auxv_array_base + auxv_array_size;
    Addr env_data_base = arg_data_base + arg_data_size;

    // write contents to stack
    uint64_t argc = argv.size();
    if (intSize == 8)
        argc = htog((uint64_t)argc);
    else if (intSize == 4)
        argc = htog((uint32_t)argc);
    else
        panic("Unknown int size");

    initVirtMem.writeBlob(stack_min, (uint8_t*)&argc, intSize);

    copyStringArray(argv, argv_array_base, arg_data_base, initVirtMem);
    copyStringArray(envp, envp_array_base, env_data_base, initVirtMem);

    //Copy the aux stuff
    for (vector<auxv_t>::size_type x = 0; x < auxv.size(); x++) {
        initVirtMem.writeBlob(auxv_array_base + x * 2 * intSize,
                (uint8_t*)&(auxv[x].a_type), intSize);
        initVirtMem.writeBlob(auxv_array_base + (x * 2 + 1) * intSize,
                (uint8_t*)&(auxv[x].a_val), intSize);
    }

    ThreadContext *tc = system->getThreadContext(contextIds[0]);

    setSyscallArg(tc, 0, argc);
    setSyscallArg(tc, 1, argv_array_base);
    tc->setIntReg(StackPointerReg, stack_min);

    tc->pcState(objFile->entryPoint());
}

void
AlphaLiveProcess::setupASNReg()
{
    ThreadContext *tc = system->getThreadContext(contextIds[0]);
    tc->setMiscRegNoEffect(IPR_DTB_ASN, M5_pid << 57);
}


void
AlphaLiveProcess::loadState(Checkpoint *cp)
{
    LiveProcess::loadState(cp);
    // need to set up ASN after unserialization since M5_pid value may
    // come from checkpoint
    setupASNReg();
}


void
AlphaLiveProcess::initState()
{
    // need to set up ASN before further initialization since init
    // will involve writing to virtual memory addresses
    setupASNReg();

    LiveProcess::initState();

    argsInit(MachineBytes, VMPageSize);

    ThreadContext *tc = system->getThreadContext(contextIds[0]);
    tc->setIntReg(GlobalPointerReg, objFile->globalPointer());
    //Operate in user mode
    tc->setMiscRegNoEffect(IPR_ICM, mode_user << 3);
    tc->setMiscRegNoEffect(IPR_DTB_CM, mode_user << 3);
    //No super page mapping
    tc->setMiscRegNoEffect(IPR_MCSR, 0);
}

AlphaISA::IntReg
AlphaLiveProcess::getSyscallArg(ThreadContext *tc, int &i)
{
    assert(i < 6);
    return tc->readIntReg(FirstArgumentReg + i++);
}

void
AlphaLiveProcess::setSyscallArg(ThreadContext *tc,
        int i, AlphaISA::IntReg val)
{
    assert(i < 6);
    tc->setIntReg(FirstArgumentReg + i, val);
}

void
AlphaLiveProcess::setSyscallReturn(ThreadContext *tc,
        SyscallReturn return_value)
{
    // check for error condition.  Alpha syscall convention is to
    // indicate success/failure in reg a3 (r19) and put the
    // return value itself in the standard return value reg (v0).
    if (return_value.successful()) {
        // no error
        tc->setIntReg(SyscallSuccessReg, 0);
        tc->setIntReg(ReturnValueReg, return_value.value());
    } else {
        // got an error, return details
        tc->setIntReg(SyscallSuccessReg, (IntReg)-1);
        tc->setIntReg(ReturnValueReg, -return_value.value());
    }
}
