/*
 * Copyright (c) 2008 Princeton University
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
 * Authors: Niket Agarwal
 */

#include "mem/ruby/network/garnet/flexible-pipeline/flit.hh"

flit::flit(int id, int  vc, int vnet, int size, MsgPtr msg_ptr, Cycles curTime)
{
    m_size = size;
    m_msg_ptr = msg_ptr;
    m_enqueue_time = curTime;
    m_time = curTime;
    m_id = id;
    m_vnet = vnet;
    m_vc = vc;

    if (size == 1) {
        m_type = HEAD_TAIL_;
        return;
    }
    if (id == 0)
        m_type = HEAD_;
    else if (id == (size - 1))
        m_type = TAIL_;
    else
        m_type = BODY_;
}

int
flit::get_size()
{
    return m_size;
}

int
flit::get_id()
{
    return m_id;
}

Cycles
flit::get_time()
{
    return m_time;
}

Cycles
flit::get_enqueue_time()
{
    return m_enqueue_time;
}

void
flit::set_time(Cycles time)
{
    m_time = time;
}

int
flit::get_vnet()
{
    return m_vnet;
}

int
flit::get_vc()
{
    return m_vc;
}

void
flit::set_vc(int vc)
{
    m_vc = vc;
}

MsgPtr&
flit::get_msg_ptr()
{
    return m_msg_ptr;
}

flit_type
flit::get_type()
{
    return m_type;
}

void
flit::set_delay(Cycles delay)
{
    src_delay = delay;
}

Cycles
flit::get_delay()
{
    return src_delay;
}

void
flit::print(std::ostream& out) const
{
    out << "[flit:: ";
    out << "Id=" << m_id << " ";
    out << "Type=" << m_type << " ";
    out << "Vnet=" << m_vnet << " ";
    out << "VC=" << m_vc << " ";
    out << "Enqueue Time=" << m_enqueue_time << " ";
    out << "]";
}

bool
flit::functionalRead(Packet *pkt)
{
    Message *msg = m_msg_ptr.get();
    return msg->functionalRead(pkt);
}

bool
flit::functionalWrite(Packet *pkt)
{
    Message *msg = m_msg_ptr.get();
    return msg->functionalWrite(pkt);
}
