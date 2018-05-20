/*
 * Copyright (c) 2008 Mark D. Hill and David A. Wood
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

#ifndef __MEM_RUBY_SYSTEM_DMASEQUENCER_HH__
#define __MEM_RUBY_SYSTEM_DMASEQUENCER_HH__

#include <ostream>

#include "mem/protocol/DMASequencerRequestType.hh"
#include "mem/ruby/common/DataBlock.hh"
#include "mem/ruby/system/RubyPort.hh"
#include "params/DMASequencer.hh"

struct DMARequest
{
    uint64_t start_paddr;
    int len;
    bool write;
    int bytes_completed;
    int bytes_issued;
    uint8_t *data;
    PacketPtr pkt;
};

class DMASequencer : public RubyPort
{
  public:
    typedef DMASequencerParams Params;
    DMASequencer(const Params *);
    void init();
    /* external interface */
    RequestStatus makeRequest(PacketPtr pkt);
    bool busy() { return m_is_busy;}
    int outstandingCount() const { return (m_is_busy ? 1 : 0); }
    bool isDeadlockEventScheduled() const { return false; }
    void descheduleDeadlockEvent() {}

    /* SLICC callback */
    void dataCallback(const DataBlock & dblk);
    void ackCallback();

    void recordRequestType(DMASequencerRequestType requestType);

  private:
    void issueNext();

  private:
    bool m_is_busy;
    uint64_t m_data_block_mask;
    DMARequest active_request;
};

#endif // __MEM_RUBY_SYSTEM_DMASEQUENCER_HH__
