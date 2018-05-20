/*
 * Copyright (c) 1999-2008 Mark D. Hill and David A. Wood
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

#include <cassert>

#include "base/cprintf.hh"
#include "base/misc.hh"
#include "base/stl_helpers.hh"
#include "debug/RubyQueue.hh"
#include "mem/ruby/buffers/MessageBuffer.hh"
#include "mem/ruby/system/System.hh"

using namespace std;
using m5::stl_helpers::operator<<;

MessageBuffer::MessageBuffer(const string &name)
    : m_time_last_time_size_checked(0), m_time_last_time_enqueue(0),
    m_time_last_time_pop(0), m_last_arrival_time(0)
{
    m_msg_counter = 0;
    m_consumer = NULL;
    m_sender = NULL;
    m_receiver = NULL;

    m_ordering_set = false;
    m_strict_fifo = true;
    m_max_size = -1;
    m_randomization = true;
    m_size_last_time_size_checked = 0;
    m_size_at_cycle_start = 0;
    m_msgs_this_cycle = 0;
    m_not_avail_count = 0;
    m_priority_rank = 0;
    m_name = name;

    m_stall_msg_map.clear();
    m_input_link_id = 0;
    m_vnet_id = 0;
}

int
MessageBuffer::getSize()
{
    if (m_time_last_time_size_checked != m_receiver->curCycle()) {
        m_time_last_time_size_checked = m_receiver->curCycle();
        m_size_last_time_size_checked = m_prio_heap.size();
    }

    return m_size_last_time_size_checked;
}

bool
MessageBuffer::areNSlotsAvailable(int n)
{

    // fast path when message buffers have infinite size
    if (m_max_size == -1) {
        return true;
    }

    // determine the correct size for the current cycle
    // pop operations shouldn't effect the network's visible size
    // until next cycle, but enqueue operations effect the visible
    // size immediately
    unsigned int current_size = 0;

    if (m_time_last_time_pop < m_receiver->curCycle()) {
        // no pops this cycle - heap size is correct
        current_size = m_prio_heap.size();
    } else {
        if (m_time_last_time_enqueue < m_receiver->curCycle()) {
            // no enqueues this cycle - m_size_at_cycle_start is correct
            current_size = m_size_at_cycle_start;
        } else {
            // both pops and enqueues occured this cycle - add new
            // enqueued msgs to m_size_at_cycle_start
            current_size = m_size_at_cycle_start + m_msgs_this_cycle;
        }
    }

    // now compare the new size with our max size
    if (current_size + n <= m_max_size) {
        return true;
    } else {
        DPRINTF(RubyQueue, "n: %d, current_size: %d, heap size: %d, "
                "m_max_size: %d\n",
                n, current_size, m_prio_heap.size(), m_max_size);
        m_not_avail_count++;
        return false;
    }
}

const MsgPtr
MessageBuffer::getMsgPtrCopy() const
{
    assert(isReady());

    return m_prio_heap.front().m_msgptr->clone();
}

const Message*
MessageBuffer::peekAtHeadOfQueue() const
{
    DPRINTF(RubyQueue, "Peeking at head of queue.\n");
    assert(isReady());

    const Message* msg_ptr = m_prio_heap.front().m_msgptr.get();
    assert(msg_ptr);

    DPRINTF(RubyQueue, "Message: %s\n", (*msg_ptr));
    return msg_ptr;
}

// FIXME - move me somewhere else
Cycles
random_time()
{
    Cycles time(1);
    time += Cycles(random() & 0x3);  // [0...3]
    if ((random() & 0x7) == 0) {  // 1 in 8 chance
        time += Cycles(100 + (random() % 0xf)); // 100 + [1...15]
    }
    return time;
}

void
MessageBuffer::enqueue(MsgPtr message, Cycles delta)
{
    m_msg_counter++;

    // record current time incase we have a pop that also adjusts my size
    if (m_time_last_time_enqueue < m_sender->curCycle()) {
        m_msgs_this_cycle = 0;  // first msg this cycle
        m_time_last_time_enqueue = m_sender->curCycle();
    }
    m_msgs_this_cycle++;

    if (!m_ordering_set) {
        panic("Ordering property of %s has not been set", m_name);
    }

    // Calculate the arrival time of the message, that is, the first
    // cycle the message can be dequeued.
    assert(delta > 0);
    Tick current_time = m_sender->clockEdge();
    Tick arrival_time = 0;

    if (!RubySystem::getRandomization() || (m_randomization == false)) {
        // No randomization
        arrival_time = current_time + delta * m_sender->clockPeriod();
    } else {
        // Randomization - ignore delta
        if (m_strict_fifo) {
            if (m_last_arrival_time < current_time) {
                m_last_arrival_time = current_time;
            }
            arrival_time = m_last_arrival_time +
                           random_time() * m_sender->clockPeriod();
        } else {
            arrival_time = current_time +
                           random_time() * m_sender->clockPeriod();
        }
    }

    // Check the arrival time
    assert(arrival_time > current_time);
    if (m_strict_fifo) {
        if (arrival_time < m_last_arrival_time) {
            panic("FIFO ordering violated: %s name: %s current time: %d "
                  "delta: %d arrival_time: %d last arrival_time: %d\n",
                  *this, m_name, current_time,
                  delta * m_sender->clockPeriod(),
                  arrival_time, m_last_arrival_time);
        }
    }

    // If running a cache trace, don't worry about the last arrival checks
    if (!g_system_ptr->m_warmup_enabled) {
        m_last_arrival_time = arrival_time;
    }

    // compute the delay cycles and set enqueue time
    Message* msg_ptr = message.get();
    assert(msg_ptr != NULL);

    assert(m_sender->clockEdge() >= msg_ptr->getLastEnqueueTime() &&
           "ensure we aren't dequeued early");

    msg_ptr->setDelayedTicks(m_sender->clockEdge() -
                             msg_ptr->getLastEnqueueTime() +
                             msg_ptr->getDelayedTicks());
    msg_ptr->setLastEnqueueTime(arrival_time);

    // Insert the message into the priority heap
    MessageBufferNode thisNode(arrival_time, m_msg_counter, message);
    m_prio_heap.push_back(thisNode);
    push_heap(m_prio_heap.begin(), m_prio_heap.end(),
        greater<MessageBufferNode>());

    DPRINTF(RubyQueue, "Enqueue arrival_time: %lld, Message: %s\n",
            arrival_time, *(message.get()));

    // Schedule the wakeup
    if (m_consumer != NULL) {
        m_consumer->scheduleEventAbsolute(arrival_time);
        m_consumer->storeEventInfo(m_vnet_id);
    } else {
        panic("No consumer: %s name: %s\n", *this, m_name);
    }
}

Cycles
MessageBuffer::dequeue_getDelayCycles(MsgPtr& message)
{
    dequeue(message);
    return setAndReturnDelayCycles(message);
}

void
MessageBuffer::dequeue(MsgPtr& message)
{
    DPRINTF(RubyQueue, "Dequeueing\n");
    message = m_prio_heap.front().m_msgptr;

    pop();
    DPRINTF(RubyQueue, "Enqueue message is %s\n", (*(message.get())));
}

Cycles
MessageBuffer::dequeue_getDelayCycles()
{
    // get MsgPtr of the message about to be dequeued
    MsgPtr message = m_prio_heap.front().m_msgptr;

    // get the delay cycles
    Cycles delayCycles = setAndReturnDelayCycles(message);
    dequeue();

    return delayCycles;
}

void
MessageBuffer::pop()
{
    DPRINTF(RubyQueue, "Popping\n");
    assert(isReady());

    // record previous size and time so the current buffer size isn't
    // adjusted until next cycle
    if (m_time_last_time_pop < m_receiver->curCycle()) {
        m_size_at_cycle_start = m_prio_heap.size();
        m_time_last_time_pop = m_receiver->curCycle();
    }

    pop_heap(m_prio_heap.begin(), m_prio_heap.end(),
        greater<MessageBufferNode>());
    m_prio_heap.pop_back();
}

void
MessageBuffer::clear()
{
    m_prio_heap.clear();

    m_msg_counter = 0;
    m_time_last_time_enqueue = Cycles(0);
    m_time_last_time_pop = Cycles(0);
    m_size_at_cycle_start = 0;
    m_msgs_this_cycle = 0;
}

void
MessageBuffer::recycle()
{
    DPRINTF(RubyQueue, "Recycling.\n");
    assert(isReady());
    MessageBufferNode node = m_prio_heap.front();
    pop_heap(m_prio_heap.begin(), m_prio_heap.end(),
        greater<MessageBufferNode>());

    node.m_time = m_receiver->clockEdge(m_recycle_latency);
    m_prio_heap.back() = node;
    push_heap(m_prio_heap.begin(), m_prio_heap.end(),
        greater<MessageBufferNode>());
    m_consumer->
        scheduleEventAbsolute(m_receiver->clockEdge(m_recycle_latency));
}

void
MessageBuffer::reanalyzeMessages(const Address& addr)
{
    DPRINTF(RubyQueue, "ReanalyzeMessages\n");
    assert(m_stall_msg_map.count(addr) > 0);
    Tick nextTick = m_receiver->clockEdge(Cycles(1));

    //
    // Put all stalled messages associated with this address back on the
    // prio heap
    //
    while(!m_stall_msg_map[addr].empty()) {
        m_msg_counter++;
        MessageBufferNode msgNode(nextTick, m_msg_counter,
                                  m_stall_msg_map[addr].front());

        m_prio_heap.push_back(msgNode);
        push_heap(m_prio_heap.begin(), m_prio_heap.end(),
                  greater<MessageBufferNode>());

        m_consumer->scheduleEventAbsolute(nextTick);
        m_stall_msg_map[addr].pop_front();
    }
    m_stall_msg_map.erase(addr);
}

void
MessageBuffer::reanalyzeAllMessages()
{
    DPRINTF(RubyQueue, "ReanalyzeAllMessages\n");
    Tick nextTick = m_receiver->clockEdge(Cycles(1));

    //
    // Put all stalled messages associated with this address back on the
    // prio heap
    //
    for (StallMsgMapType::iterator map_iter = m_stall_msg_map.begin();
         map_iter != m_stall_msg_map.end();
         ++map_iter) {

        while(!(map_iter->second).empty()) {
            m_msg_counter++;
            MessageBufferNode msgNode(nextTick, m_msg_counter,
                                      (map_iter->second).front());

            m_prio_heap.push_back(msgNode);
            push_heap(m_prio_heap.begin(), m_prio_heap.end(),
                      greater<MessageBufferNode>());

            m_consumer->scheduleEventAbsolute(nextTick);
            (map_iter->second).pop_front();
        }
    }
    m_stall_msg_map.clear();
}

void
MessageBuffer::stallMessage(const Address& addr)
{
    DPRINTF(RubyQueue, "Stalling due to %s\n", addr);
    assert(isReady());
    assert(addr.getOffset() == 0);
    MsgPtr message = m_prio_heap.front().m_msgptr;

    pop();

    //
    // Note: no event is scheduled to analyze the map at a later time.
    // Instead the controller is responsible to call reanalyzeMessages when
    // these addresses change state.
    //
    (m_stall_msg_map[addr]).push_back(message);
}

Cycles
MessageBuffer::setAndReturnDelayCycles(MsgPtr msg_ptr)
{
    // get the delay cycles of the message at the top of the queue

    // this function should only be called on dequeue
    // ensure the msg hasn't been enqueued
    assert(msg_ptr->getLastEnqueueTime() <= m_receiver->clockEdge());

    msg_ptr->setDelayedTicks(m_receiver->clockEdge() -
                             msg_ptr->getLastEnqueueTime() +
                             msg_ptr->getDelayedTicks());

    return m_receiver->ticksToCycles(msg_ptr->getDelayedTicks());
}

void
MessageBuffer::print(ostream& out) const
{
    ccprintf(out, "[MessageBuffer: ");
    if (m_consumer != NULL) {
        ccprintf(out, " consumer-yes ");
    }

    vector<MessageBufferNode> copy(m_prio_heap);
    sort_heap(copy.begin(), copy.end(), greater<MessageBufferNode>());
    ccprintf(out, "%s] %s", copy, m_name);
}

void
MessageBuffer::printStats(ostream& out)
{
    out << "MessageBuffer: " << m_name << " stats - msgs:" << m_msg_counter
        << " full:" << m_not_avail_count << endl;
}

bool
MessageBuffer::isReady() const
{
    return ((m_prio_heap.size() > 0) &&
            (m_prio_heap.front().m_time <= m_receiver->clockEdge()));
}

bool
MessageBuffer::functionalRead(Packet *pkt)
{
    // Check the priority heap and read any messages that may
    // correspond to the address in the packet.
    for (unsigned int i = 0; i < m_prio_heap.size(); ++i) {
        Message *msg = m_prio_heap[i].m_msgptr.get();
        if (msg->functionalRead(pkt)) return true;
    }

    // Read the messages in the stall queue that correspond
    // to the address in the packet.
    for (StallMsgMapType::iterator map_iter = m_stall_msg_map.begin();
         map_iter != m_stall_msg_map.end();
         ++map_iter) {

        for (std::list<MsgPtr>::iterator it = (map_iter->second).begin();
            it != (map_iter->second).end(); ++it) {

            Message *msg = (*it).get();
            if (msg->functionalRead(pkt)) return true;
        }
    }
    return false;
}

uint32_t
MessageBuffer::functionalWrite(Packet *pkt)
{
    uint32_t num_functional_writes = 0;

    // Check the priority heap and write any messages that may
    // correspond to the address in the packet.
    for (unsigned int i = 0; i < m_prio_heap.size(); ++i) {
        Message *msg = m_prio_heap[i].m_msgptr.get();
        if (msg->functionalWrite(pkt)) {
            num_functional_writes++;
        }
    }

    // Check the stall queue and write any messages that may
    // correspond to the address in the packet.
    for (StallMsgMapType::iterator map_iter = m_stall_msg_map.begin();
         map_iter != m_stall_msg_map.end();
         ++map_iter) {

        for (std::list<MsgPtr>::iterator it = (map_iter->second).begin();
            it != (map_iter->second).end(); ++it) {

            Message *msg = (*it).get();
            if (msg->functionalWrite(pkt)) {
                num_functional_writes++;
            }
        }
    }

    return num_functional_writes;
}
