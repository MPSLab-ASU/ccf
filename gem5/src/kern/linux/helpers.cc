/*
 * Copyright (c) 2016 ARM Limited
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

#include "kern/linux/helpers.hh"

#include "config/the_isa.hh"
#include "cpu/thread_context.hh"
#include "mem/port_proxy.hh"
#include "sim/byteswap.hh"
#include "sim/system.hh"

struct DmesgEntry {
    uint64_t ts_nsec;
    uint16_t len;
    uint16_t text_len;
    uint16_t dict_len;
    uint8_t facility;
    uint8_t flags;
} M5_ATTR_PACKED;

static int
dumpDmesgEntry(const uint8_t *base, const uint8_t *end, const ByteOrder bo,
               std::ostream &os)
{
    const size_t max_length = end - base;
    DmesgEntry de;

    if (max_length < sizeof(de)) {
        warn("Malformed dmesg entry\n");
        return -1;
    }

    memcpy(&de, base, sizeof(de));
    de.ts_nsec = gtoh(de.ts_nsec, bo);
    de.len = gtoh(de.len, bo);
    de.text_len = gtoh(de.text_len, bo);

    if (de.len < sizeof(de) ||
        max_length < de.len ||
        max_length < sizeof(DmesgEntry) + de.text_len) {

        warn("Malformed dmesg entry:\n");
        warn("\tMax length: %i\n", max_length);
        warn("\tde.len: %i\n", de.len);
        warn("\tde.text_len: %i\n", de.text_len);
        return -1;
    }

    ccprintf(os, "[%.6f] ", de.ts_nsec * 10e-9);
    os.write((char *)base + sizeof(de), de.text_len);
    os << std::endl;

    return de.len;
}

void
Linux::dumpDmesg(ThreadContext *tc, std::ostream &os)
{
    System *system = tc->getSystemPtr();
    const ByteOrder bo = system->getGuestByteOrder();
    const auto &symtab = system->workload->symtab(tc);
    PortProxy &proxy = tc->getVirtProxy();

    auto lb = symtab.find("__log_buf");
    auto lb_len = symtab.find("log_buf_len");
    auto first = symtab.find("log_first_idx");
    auto next = symtab.find("log_next_idx");

    auto end_it = symtab.end();

    if (lb == end_it || lb_len == end_it ||
            first == end_it || next == end_it) {
        warn("Failed to find kernel dmesg symbols.\n");
        return;
    }

    uint32_t log_buf_len = proxy.read<uint32_t>(lb_len->address, bo);
    uint32_t log_first_idx = proxy.read<uint32_t>(first->address, bo);
    uint32_t log_next_idx = proxy.read<uint32_t>(next->address, bo);

    if (log_first_idx >= log_buf_len || log_next_idx >= log_buf_len) {
        warn("dmesg pointers/length corrupted\n");
        return;
    }

    // Normalize and read the dmesg ring buffer
    std::vector<uint8_t> log_buf(log_buf_len);
    int length;
    if (log_first_idx < log_next_idx) {
        length = log_next_idx - log_first_idx;
        if (length < 0 || length > log_buf.size()) {
            warn("Unexpected dmesg buffer length\n");
            return;
        }
        proxy.readBlob(lb->address + log_first_idx, log_buf.data(), length);
    } else {
        const int length_2 = log_buf_len - log_first_idx;
        if (length_2 < 0 || length_2 + log_next_idx > log_buf.size()) {
            warn("Unexpected dmesg buffer length\n");
            return;
        }
        length = log_buf_len;
        proxy.readBlob(lb->address + log_first_idx, log_buf.data(), length_2);
        proxy.readBlob(lb->address, log_buf.data() + length_2, log_next_idx);
    }

    // Print dmesg buffer content
    const uint8_t *cur = log_buf.data(), *end = log_buf.data() + length;
    while (cur < end) {
        int ret = dumpDmesgEntry(cur, end, bo, os);
        if (ret < 0)
            return;
        cur += ret;
    }
}
