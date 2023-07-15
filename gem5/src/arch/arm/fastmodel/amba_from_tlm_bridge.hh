/*
 * Copyright 2019 Google, Inc.
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

#ifndef __ARCH_ARM_FASTMODEL_AMBA_FROM_TLM_BRIDGE_HH__
#define __ARCH_ARM_FASTMODEL_AMBA_FROM_TLM_BRIDGE_HH__

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "amba_pv.h"
#pragma GCC diagnostic pop
#include "arch/arm/fastmodel/amba_ports.hh"
#include "systemc/tlm_port_wrapper.hh"

namespace FastModel
{

// A wrapper around the fast model AMBA -> TLM bridge which provides
// gem5_getPort.
class AmbaFromTlmBridge64 : public amba_pv::amba_pv_from_tlm_bridge<64>
{
  public:
    AmbaFromTlmBridge64(const char *name);

    ::Port &gem5_getPort(const std::string &if_name, int idx=-1) override;

  private:
    AmbaInitiator ambaWrapper;
    sc_gem5::TlmTargetWrapper<64> tlmWrapper;
};

} // namespace FastModel

#endif // __ARCH_ARM_FASTMODEL_AMBA_FROM_TLM_BRIDGE_HH__
