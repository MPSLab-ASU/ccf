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
#include "mem/ruby/filters/BlockBloomFilter.hh"
#include "mem/ruby/system/System.hh"

using namespace std;

BlockBloomFilter::BlockBloomFilter(string str)
{
    string head, tail;

#ifndef NDEBUG
    bool success =
#endif
        split_first(str, head, tail, '_');

    assert(success);

    m_filter_size = atoi(head.c_str());
    m_filter_size_bits = floorLog2(m_filter_size);

    m_filter.resize(m_filter_size);

    clear();
}

BlockBloomFilter::~BlockBloomFilter()
{
}

void
BlockBloomFilter::clear()
{
    for (int i = 0; i < m_filter_size; i++) {
        m_filter[i] = 0;
    }
}

void
BlockBloomFilter::increment(const Address& addr)
{
    // Not used
}

void
BlockBloomFilter::decrement(const Address& addr)
{
    // Not used
}

void
BlockBloomFilter::merge(AbstractBloomFilter * other_filter)
{
    // TODO
}

void
BlockBloomFilter::set(const Address& addr)
{
    int i = get_index(addr);
    m_filter[i] = 1;
}

void
BlockBloomFilter::unset(const Address& addr)
{
    int i = get_index(addr);
    m_filter[i] = 0;
}

bool
BlockBloomFilter::isSet(const Address& addr)
{
    int i = get_index(addr);
    return (m_filter[i]);
}

int
BlockBloomFilter::getCount(const Address& addr)
{
    return m_filter[get_index(addr)];
}

int
BlockBloomFilter::getTotalCount()
{
    int count = 0;

    for (int i = 0; i < m_filter_size; i++) {
        if (m_filter[i]) {
            count++;
        }
    }
    return count;
}

int
BlockBloomFilter::getIndex(const Address& addr)
{
    return get_index(addr);
}

void
BlockBloomFilter::print(ostream& out) const
{
}

int
BlockBloomFilter::readBit(const int index)
{
    return m_filter[index];
}

void
BlockBloomFilter::writeBit(const int index, const int value)
{
    m_filter[index] = value;
}

int
BlockBloomFilter::get_index(const Address& addr)
{
    // Pull out some bit field ==> B1
    // Pull out additional bits, not the same as B1 ==> B2
    //  XOR B1 and B2 to get hash index
    physical_address_t block_bits =
        addr.bitSelect(RubySystem::getBlockSizeBits(),
                       2 * RubySystem::getBlockSizeBits() - 1);
    int offset = 5;
    physical_address_t other_bits =
        addr.bitSelect(2 * RubySystem::getBlockSizeBits() + offset,
                       2 * RubySystem::getBlockSizeBits() + offset +
                       m_filter_size_bits - 1);
    int index = block_bits ^ other_bits;
    assert(index < m_filter_size);
    return index;
}


