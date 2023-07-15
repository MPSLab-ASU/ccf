/*
 * Copyright (c) 2010, 2012-2013, 2015-2019 ARM Limited
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
 */

#ifndef __ARCH_ARM_FS_WORKLOAD_HH__
#define __ARCH_ARM_FS_WORKLOAD_HH__

#include <memory>
#include <vector>

#include "kern/linux/events.hh"
#include "params/ArmFsWorkload.hh"
#include "sim/kernel_workload.hh"
#include "sim/sim_object.hh"

namespace ArmISA
{

class SkipFunc : public SkipFuncBase
{
  public:
    using SkipFuncBase::SkipFuncBase;
    void returnFromFuncIn(ThreadContext *tc) override;
};

class FsWorkload : public KernelWorkload
{
  protected:
    /** Bootloaders */
    std::vector<std::unique_ptr<Loader::ObjectFile>> bootLoaders;

    /**
     * Pointer to the bootloader object
     */
    Loader::ObjectFile *bootldr = nullptr;

    /**
     * This differs from entry since it takes into account where
     * the kernel is loaded in memory (with loadAddrMask and
     * loadAddrOffset).
     */
    Addr kernelEntry = 0;

    /**
     * Get a boot loader that matches the kernel.
     *
     * @param obj Kernel binary
     * @return Pointer to boot loader ObjectFile or nullptr if there
     *         is no matching boot loader.
     */
    Loader::ObjectFile *getBootLoader(Loader::ObjectFile *const obj);

  public:
    typedef ArmFsWorkloadParams Params;
    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(&_params);
    }

    Addr
    getEntry() const override
    {
        if (bootldr)
            return bootldr->entryPoint();
        else
            return kernelEntry;
    }

    Loader::Arch
    getArch() const override
    {
        if (bootldr)
            return bootldr->getArch();
        else if (kernelObj)
            return kernelObj->getArch();
        else
            return Loader::Arm64;
    }

    FsWorkload(Params *p);

    void initState() override;

    Addr
    fixFuncEventAddr(Addr addr) const override
    {
        // Remove the low bit that thumb symbols have set
        // but that aren't actually odd aligned
        return addr & ~1;
    }
};

} // namespace ArmISA

#endif // __ARCH_ARM_FS_WORKLOAD_HH__
