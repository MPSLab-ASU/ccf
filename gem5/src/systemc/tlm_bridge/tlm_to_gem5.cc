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
 *
 * Copyright (c) 2016, Dresden University of Technology (TU Dresden)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "systemc/tlm_bridge/tlm_to_gem5.hh"

#include "params/TlmToGem5Bridge32.hh"
#include "params/TlmToGem5Bridge64.hh"
#include "sim/system.hh"
#include "systemc/ext/core/sc_module_name.hh"
#include "systemc/ext/core/sc_time.hh"

namespace sc_gem5
{

PacketPtr
payload2packet(RequestorID _id, tlm::tlm_generic_payload &trans)
{
    MemCmd cmd;

    switch (trans.get_command()) {
        case tlm::TLM_READ_COMMAND:
            cmd = MemCmd::ReadReq;
            break;
        case tlm::TLM_WRITE_COMMAND:
            cmd = MemCmd::WriteReq;
            break;
        case tlm::TLM_IGNORE_COMMAND:
            return nullptr;
        default:
            SC_REPORT_FATAL("TlmToGem5Bridge",
                            "received transaction with unsupported command");
    }

    Request::Flags flags;
    auto req = std::make_shared<Request>(
        trans.get_address(), trans.get_data_length(), flags, _id);

    /*
     * Allocate a new Packet. The packet will be deleted when it returns from
     * the gem5 world as a response.
     */
    auto pkt = new Packet(req, cmd);
    pkt->dataStatic(trans.get_data_ptr());

    return pkt;
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::sendEndReq(tlm::tlm_generic_payload &trans)
{
    tlm::tlm_phase phase = tlm::END_REQ;
    auto delay = sc_core::SC_ZERO_TIME;

    auto status = socket->nb_transport_bw(trans, phase, delay);
    panic_if(status != tlm::TLM_ACCEPTED,
             "Unexpected status after sending END_REQ");
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::sendBeginResp(tlm::tlm_generic_payload &trans,
                                         sc_core::sc_time &delay)
{
    tlm::tlm_phase phase = tlm::BEGIN_RESP;

    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    auto status = socket->nb_transport_bw(trans, phase, delay);

    if (status == tlm::TLM_COMPLETED ||
        (status == tlm::TLM_UPDATED && phase == tlm::END_RESP)) {
        // transaction completed -> no need to wait for tlm::END_RESP
        responseInProgress = false;
    } else if (status == tlm::TLM_ACCEPTED) {
        // we need to wait for tlm::END_RESP
        responseInProgress = true;
    } else {
        panic("Unexpected status after sending BEGIN_RESP");
    }
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::handleBeginReq(tlm::tlm_generic_payload &trans)
{
    sc_assert(!waitForRetry);
    sc_assert(pendingRequest == nullptr);
    sc_assert(pendingPacket == nullptr);

    trans.acquire();

    PacketPtr pkt = nullptr;

    Gem5SystemC::Gem5Extension *extension = nullptr;
    trans.get_extension(extension);

    // If there is an extension, this transaction was initiated by the gem5
    // world and we can pipe through the original packet. Otherwise, we
    // generate a new packet based on the transaction.
    if (extension != nullptr) {
        extension->setPipeThrough();
        pkt = extension->getPacket();
    } else {
        pkt = payload2packet(_id, trans);
    }

    auto tlmSenderState = new TlmSenderState(trans);
    pkt->pushSenderState(tlmSenderState);

    // If the packet doesn't need a response, we should send BEGIN_RESP by
    // ourselves.
    bool needsResponse = pkt->needsResponse();
    if (bmp.sendTimingReq(pkt)) { // port is free -> send END_REQ immediately
        sendEndReq(trans);
        if (!needsResponse) {
            auto delay = sc_core::SC_ZERO_TIME;
            sendBeginResp(trans, delay);
        }
        trans.release();
    } else { // port is blocked -> wait for retry before sending END_REQ
        waitForRetry = true;
        pendingRequest = &trans;
        pendingPacket = pkt;
    }
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::handleEndResp(tlm::tlm_generic_payload &trans)
{
    sc_assert(responseInProgress);

    responseInProgress = false;

    checkTransaction(trans);

    if (needToSendRetry) {
        bmp.sendRetryResp();
        needToSendRetry = false;
    }
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::destroyPacket(PacketPtr pkt)
{
    delete pkt;
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::checkTransaction(tlm::tlm_generic_payload &trans)
{
    if (trans.is_response_error()) {
        std::stringstream ss;
        ss << "Transaction returned with error, response status = "
           << trans.get_response_string();
        SC_REPORT_ERROR("TLM-2", ss.str().c_str());
    }
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::invalidateDmi(const ::MemBackdoor &backdoor)
{
    socket->invalidate_direct_mem_ptr(
            backdoor.range().start(), backdoor.range().end());
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::peq_cb(tlm::tlm_generic_payload &trans,
                                  const tlm::tlm_phase &phase)
{
    switch (phase) {
        case tlm::BEGIN_REQ:
            handleBeginReq(trans);
            break;
        case tlm::END_RESP:
            handleEndResp(trans);
            break;
        default:
            panic("unimplemented phase in callback");
    }
}

template <unsigned int BITWIDTH>
tlm::tlm_sync_enum
TlmToGem5Bridge<BITWIDTH>::nb_transport_fw(
        tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase,
        sc_core::sc_time &delay)
{
    unsigned len = trans.get_data_length();
    unsigned char *byteEnable = trans.get_byte_enable_ptr();
    unsigned width = trans.get_streaming_width();

    // check the transaction attributes for unsupported features ...
    if (byteEnable != 0) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return tlm::TLM_COMPLETED;
    }
    if (width < len) { // is this a burst request?
        trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
        return tlm::TLM_COMPLETED;
    }

    // ... and queue the valid transaction
    trans.acquire();
    peq.notify(trans, phase, delay);
    return tlm::TLM_ACCEPTED;
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::b_transport(tlm::tlm_generic_payload &trans,
                                       sc_core::sc_time &t)
{
    Gem5SystemC::Gem5Extension *extension = nullptr;
    trans.get_extension(extension);

    PacketPtr pkt = nullptr;

    // If there is an extension, this transaction was initiated by the gem5
    // world and we can pipe through the original packet.
    if (extension != nullptr) {
        extension->setPipeThrough();
        pkt = extension->getPacket();
    } else {
        pkt = payload2packet(_id, trans);
    }

    MemBackdoorPtr backdoor = nullptr;
    Tick ticks = bmp.sendAtomicBackdoor(pkt, backdoor);
    if (backdoor)
        trans.set_dmi_allowed(true);

    // send an atomic request to gem5
    panic_if(pkt->needsResponse() && !pkt->isResponse(),
             "Packet sending failed!\n");

    auto delay =
      sc_core::sc_time((double)(ticks / SimClock::Int::ps), sc_core::SC_PS);

    // update time
    t += delay;

    if (extension == nullptr)
        destroyPacket(pkt);

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

template <unsigned int BITWIDTH>
unsigned int
TlmToGem5Bridge<BITWIDTH>::transport_dbg(tlm::tlm_generic_payload &trans)
{
    Gem5SystemC::Gem5Extension *extension = nullptr;
    trans.get_extension(extension);

    // If there is an extension, this transaction was initiated by the gem5
    // world and we can pipe through the original packet.
    if (extension != nullptr) {
        extension->setPipeThrough();
        bmp.sendFunctional(extension->getPacket());
    } else {
        auto pkt = payload2packet(_id, trans);
        if (pkt) {
            bmp.sendFunctional(pkt);
            destroyPacket(pkt);
        }
    }

    return trans.get_data_length();
}

template <unsigned int BITWIDTH>
bool
TlmToGem5Bridge<BITWIDTH>::get_direct_mem_ptr(tlm::tlm_generic_payload &trans,
                                              tlm::tlm_dmi &dmi_data)
{
    Gem5SystemC::Gem5Extension *extension = nullptr;
    trans.get_extension(extension);

    PacketPtr pkt = nullptr;

    // If there is an extension, this transaction was initiated by the gem5
    // world and we can pipe through the original packet.
    if (extension != nullptr) {
        extension->setPipeThrough();
        pkt = extension->getPacket();
    } else {
        pkt = payload2packet(_id, trans);
        pkt->req->setFlags(Request::NO_ACCESS);
    }

    MemBackdoorPtr backdoor = nullptr;
    bmp.sendAtomicBackdoor(pkt, backdoor);
    if (backdoor) {
        trans.set_dmi_allowed(true);
        dmi_data.set_dmi_ptr(backdoor->ptr());
        dmi_data.set_start_address(backdoor->range().start());
        dmi_data.set_end_address(backdoor->range().end());

        typedef tlm::tlm_dmi::dmi_access_e access_t;
        access_t access = tlm::tlm_dmi::DMI_ACCESS_NONE;
        if (backdoor->readable())
            access = (access_t)(access | tlm::tlm_dmi::DMI_ACCESS_READ);
        if (backdoor->writeable())
            access = (access_t)(access | tlm::tlm_dmi::DMI_ACCESS_WRITE);
        dmi_data.set_granted_access(access);

        backdoor->addInvalidationCallback(
            [this](const MemBackdoor &backdoor)
            {
                invalidateDmi(backdoor);
            }
        );
    }

    if (extension == nullptr)
        destroyPacket(pkt);

    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    return backdoor != nullptr;
}

template <unsigned int BITWIDTH>
bool
TlmToGem5Bridge<BITWIDTH>::recvTimingResp(PacketPtr pkt)
{
    // exclusion rule
    // We need to Wait for END_RESP before sending next BEGIN_RESP
    if (responseInProgress) {
        sc_assert(!needToSendRetry);
        needToSendRetry = true;
        return false;
    }

    sc_assert(pkt->isResponse());

    /*
     * Pay for annotated transport delays.
     *
     * See recvTimingReq in sc_slave_port.cc for a detailed description.
     */
    auto delay = sc_core::sc_time::from_value(pkt->payloadDelay);
    // reset the delays
    pkt->payloadDelay = 0;
    pkt->headerDelay = 0;

    auto tlmSenderState = dynamic_cast<TlmSenderState*>(pkt->popSenderState());
    sc_assert(tlmSenderState != nullptr);

    auto &trans = tlmSenderState->trans;

    Gem5SystemC::Gem5Extension *extension = nullptr;
    trans.get_extension(extension);

    // clean up
    delete tlmSenderState;

    // If there is an extension the packet was piped through and we must not
    // delete it. The packet travels back with the transaction.
    if (extension == nullptr)
        destroyPacket(pkt);
    else
        sc_assert(extension->isPipeThrough());

    sendBeginResp(trans, delay);
    trans.release();

    return true;
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::recvReqRetry()
{
    sc_assert(waitForRetry);
    sc_assert(pendingRequest != nullptr);
    sc_assert(pendingPacket != nullptr);

    // If the packet doesn't need a response, we should send BEGIN_RESP by
    // ourselves.
    bool needsResponse = pendingPacket->needsResponse();
    if (bmp.sendTimingReq(pendingPacket)) {
        waitForRetry = false;
        pendingPacket = nullptr;

        auto &trans = *pendingRequest;
        sendEndReq(trans);
        if (!needsResponse) {
            auto delay = sc_core::SC_ZERO_TIME;
            sendBeginResp(trans, delay);
        }
        trans.release();

        pendingRequest = nullptr;
    }
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::recvRangeChange()
{
    SC_REPORT_WARNING("TlmToGem5Bridge",
                      "received address range change but ignored it");
}

template <unsigned int BITWIDTH>
::Port &
TlmToGem5Bridge<BITWIDTH>::gem5_getPort(const std::string &if_name, int idx)
{
    if (if_name == "gem5")
        return bmp;
    else if (if_name == "tlm")
        return wrapper;

    return sc_core::sc_module::gem5_getPort(if_name, idx);
}

template <unsigned int BITWIDTH>
TlmToGem5Bridge<BITWIDTH>::TlmToGem5Bridge(
        Params *params, const sc_core::sc_module_name &mn) :
    TlmToGem5BridgeBase(mn), peq(this, &TlmToGem5Bridge<BITWIDTH>::peq_cb),
    waitForRetry(false), pendingRequest(nullptr), pendingPacket(nullptr),
    needToSendRetry(false), responseInProgress(false),
    bmp(std::string(name()) + "master", *this), socket("tlm_socket"),
    wrapper(socket, std::string(name()) + ".tlm", InvalidPortID),
    system(params->system),
    _id(params->system->getGlobalRequestorId(
                std::string("[systemc].") + name()))
{
}

template <unsigned int BITWIDTH>
void
TlmToGem5Bridge<BITWIDTH>::before_end_of_elaboration()
{
    /*
     * Register the TLM non-blocking interface when using gem5 Timing mode and
     * the TLM blocking interface when using the gem5 Atomic mode.
     * Then the magic (TM) in simple_target_socket automatically transforms
     * non-blocking in blocking transactions and vice versa.
     *
     * NOTE: The mode may change during execution.
     */
    if (system->isTimingMode()) {
        SC_REPORT_INFO("TlmToGem5Bridge", "register non-blocking interface");
        socket.register_nb_transport_fw(
                this, &TlmToGem5Bridge<BITWIDTH>::nb_transport_fw);
    } else if (system->isAtomicMode()) {
        SC_REPORT_INFO("TlmToGem5Bridge", "register blocking interface");
        socket.register_b_transport(
                this, &TlmToGem5Bridge<BITWIDTH>::b_transport);
        socket.register_get_direct_mem_ptr(
                this, &TlmToGem5Bridge<BITWIDTH>::get_direct_mem_ptr);
    } else {
        panic("gem5 operates neither in Timing nor in Atomic mode");
    }

    socket.register_transport_dbg(
            this, &TlmToGem5Bridge<BITWIDTH>::transport_dbg);

    sc_core::sc_module::before_end_of_elaboration();
}

} // namespace sc_gem5

sc_gem5::TlmToGem5Bridge<32> *
TlmToGem5Bridge32Params::create()
{
    return new sc_gem5::TlmToGem5Bridge<32>(
            this, sc_core::sc_module_name(name.c_str()));
}

sc_gem5::TlmToGem5Bridge<64> *
TlmToGem5Bridge64Params::create()
{
    return new sc_gem5::TlmToGem5Bridge<64>(
            this, sc_core::sc_module_name(name.c_str()));
}
