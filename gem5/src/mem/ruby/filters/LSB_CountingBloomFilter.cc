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

#include "base/intmath.hh"
#include "base/str.hh"
#include "mem/ruby/filters/LSB_CountingBloomFilter.hh"
#include "mem/ruby/system/System.hh"

using namespace std;

LSB_CountingBloomFilter::LSB_CountingBloomFilter(string str)
{
    string head, tail;
#ifndef NDEBUG
    bool success =
#endif
        split_first(str, head, tail, '_');
    assert(success);

    m_filter_size = atoi(head.c_str());
    m_filter_size_bits = floorLog2(m_filter_size);

    m_count = atoi(tail.c_str());
    m_count_bits = floorLog2(m_count);

    m_filter.resize(m_filter_size);
    clear();
}

LSB_CountingBloomFilter::~LSB_CountingBloomFilter()
{
}

void
LSB_CountingBloomFilter::clear()
{
    for (int i = 0; i < m_filter_size; i++) {
        m_filter[i] = 0;
    }
}

void
LSB_CountingBloomFilter::increment(const Address& addr)
{
    int i = get_index(addr);
    if (m_filter[i] < m_count)
        m_filter[i] += 1;
}


void
LSB_CountingBloomFilter::decrement(const Address& addr)
{
    int i = get_index(addr);
    if (m_filter[i] > 0)
        m_filter[i] -= 1;
}

void
LSB_CountingBloomFilter::merge(AbstractBloomFilter * other_filter)
{
    // TODO
}

void
LSB_CountingBloomFilter::set(const Address& addr)
{
    // TODO
}

void
LSB_CountingBloomFilter::unset(const Address& addr)
{
    // TODO
}

bool
LSB_CountingBloomFilter::isSet(const Address& addr)
{
    // TODO
    return false;
}

int
LSB_CountingBloomFilter::getCount(const Address& addr)
{
    return m_filter[get_index(addr)];
}

int
LSB_CountingBloomFilter::getTotalCount()
{
    int count = 0;

    for (int i = 0; i < m_filter_size; i++) {
        count += m_filter[i];
    }
    return count;
}

int
LSB_CountingBloomFilter::getIndex(const Address& addr)
{
    return get_index(addr);
}

void
LSB_CountingBloomFilter::print(ostream& out) const
{
}

int
LSB_CountingBloomFilter::readBit(const int index)
{
    return 0;
    // TODO
}

void
LSB_CountingBloomFilter::writeBit(const int index, const int value)
{
    // TODO
}

int
LSB_CountingBloomFilter::get_index(const Address& addr)
{
    return addr.bitSelect(RubySystem::getBlockSizeBits(),
                          RubySystem::getBlockSizeBits() +
                          m_filter_size_bits - 1);
}


