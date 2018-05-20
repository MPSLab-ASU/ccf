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
 *          Andreas Hansson
 */

#include "mem/mem_object.hh"
#include "mem/tport.hh"

SimpleTimingPort::SimpleTimingPort(const std::string& _name,
                                   MemObject* _owner) :
    QueuedSlavePort(_name, _owner, queueImpl), queueImpl(*_owner, *this)
{
}

void
SimpleTimingPort::recvFunctional(PacketPtr pkt)
{
    if (!queue.checkFunctional(pkt)) {
        // do an atomic access and throw away the returned latency
        recvAtomic(pkt);
    }
}

bool
SimpleTimingPort::recvTimingReq(PacketPtr pkt)
{
    /// @todo temporary hack to deal with memory corruption issue until
    /// 4-phase transactions are complete. Remove me later
    for (int x = 0; x < pendingDelete.size(); x++)
        delete pendingDelete[x];
    pendingDelete.clear();

    // the SimpleTimingPort should not be used anywhere where there is
    // a need to deal with inhibited packets
    if (pkt->memInhibitAsserted())
        panic("SimpleTimingPort should never see an inhibited request\n");

    bool needsResponse = pkt->needsResponse();
    Tick latency = recvAtomic(pkt);
    // turn packet around to go back to requester if response expected
    if (needsResponse) {
        // recvAtomic() should already have turned packet into
        // atomic response
        assert(pkt->isResponse());
        schedTimingResp(pkt, curTick() + latency);
    } else {
        /// @todo nominally we should just delete the packet here.
        /// Until 4-phase stuff we can't because the sending
        /// cache is still relying on it
        pendingDelete.push_back(pkt);
    }

    return true;
}
