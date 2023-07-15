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

#ifndef __ARCH_ARM_FASTMODEL_CORTEXA76_EVS_HH__
#define __ARCH_ARM_FASTMODEL_CORTEXA76_EVS_HH__

#include <memory>

#include "arch/arm/fastmodel/amba_ports.hh"
#include "arch/arm/fastmodel/common/signal_receiver.hh"
#include "arch/arm/fastmodel/protocol/exported_clock_rate_control.hh"
#include "mem/port_proxy.hh"
#include "params/FastModelScxEvsCortexA76x1.hh"
#include "params/FastModelScxEvsCortexA76x2.hh"
#include "params/FastModelScxEvsCortexA76x3.hh"
#include "params/FastModelScxEvsCortexA76x4.hh"
#include "scx_evs_CortexA76x1.h"
#include "scx_evs_CortexA76x2.h"
#include "scx_evs_CortexA76x3.h"
#include "scx_evs_CortexA76x4.h"
#include "systemc/ext/core/sc_event.hh"
#include "systemc/ext/core/sc_module.hh"
#include "systemc/tlm_port_wrapper.hh"

namespace FastModel
{

class CortexA76Cluster;

template <class Types>
class ScxEvsCortexA76 : public Types::Base
{
  private:
    static const int CoreCount = Types::CoreCount;
    using Base = typename Types::Base;
    using Params = typename Types::Params;

    SC_HAS_PROCESS(ScxEvsCortexA76);

    ClockRateControlInitiatorSocket clockRateControl;

    typedef sc_gem5::TlmTargetBaseWrapper<
        64, svp_gicv3_comms::gicv3_comms_fw_if,
        svp_gicv3_comms::gicv3_comms_bw_if, 1,
        sc_core::SC_ONE_OR_MORE_BOUND> TlmGicTarget;

    AmbaInitiator amba;
    std::vector<std::unique_ptr<TlmGicTarget>> redist;

    std::vector<std::unique_ptr<SignalReceiver>> cnthpirq;
    std::vector<std::unique_ptr<SignalReceiver>> cnthvirq;
    std::vector<std::unique_ptr<SignalReceiver>> cntpsirq;
    std::vector<std::unique_ptr<SignalReceiver>> cntvirq;
    std::vector<std::unique_ptr<SignalReceiver>> commirq;
    std::vector<std::unique_ptr<SignalReceiver>> ctidbgirq;
    std::vector<std::unique_ptr<SignalReceiver>> pmuirq;
    std::vector<std::unique_ptr<SignalReceiver>> vcpumntirq;
    std::vector<std::unique_ptr<SignalReceiver>> cntpnsirq;

    sc_core::sc_event clockChanged;
    sc_core::sc_attribute<Tick> clockPeriod;
    sc_core::sc_attribute<CortexA76Cluster *> gem5CpuCluster;
    sc_core::sc_attribute<PortProxy::SendFunctionalFunc> sendFunctional;

    void sendFunc(PacketPtr pkt);

    void clockChangeHandler();

    const Params &params;

  public:
    ScxEvsCortexA76(const sc_core::sc_module_name &mod_name, const Params &p);

    void before_end_of_elaboration() override;
    Port &gem5_getPort(const std::string &if_name, int idx) override;

    void
    end_of_elaboration() override
    {
        Base::end_of_elaboration();
        Base::start_of_simulation();
    }
    void start_of_simulation() override {}
};

struct ScxEvsCortexA76x1Types
{
    using Base = scx_evs_CortexA76x1;
    using Params = FastModelScxEvsCortexA76x1Params;
    static const int CoreCount = 1;
};
using ScxEvsCortexA76x1 = ScxEvsCortexA76<ScxEvsCortexA76x1Types>;
extern template class ScxEvsCortexA76<ScxEvsCortexA76x1Types>;

struct ScxEvsCortexA76x2Types
{
    using Base = scx_evs_CortexA76x2;
    using Params = FastModelScxEvsCortexA76x2Params;
    static const int CoreCount = 2;
};
using ScxEvsCortexA76x2 = ScxEvsCortexA76<ScxEvsCortexA76x2Types>;
extern template class ScxEvsCortexA76<ScxEvsCortexA76x2Types>;

struct ScxEvsCortexA76x3Types
{
    using Base = scx_evs_CortexA76x3;
    using Params = FastModelScxEvsCortexA76x3Params;
    static const int CoreCount = 3;
};
using ScxEvsCortexA76x3 = ScxEvsCortexA76<ScxEvsCortexA76x3Types>;
extern template class ScxEvsCortexA76<ScxEvsCortexA76x3Types>;

struct ScxEvsCortexA76x4Types
{
    using Base = scx_evs_CortexA76x4;
    using Params = FastModelScxEvsCortexA76x4Params;
    static const int CoreCount = 4;
};
using ScxEvsCortexA76x4 = ScxEvsCortexA76<ScxEvsCortexA76x4Types>;
extern template class ScxEvsCortexA76<ScxEvsCortexA76x4Types>;

} // namespace FastModel

#endif // __ARCH_ARM_FASTMODEL_CORTEXA76_EVS_HH__
