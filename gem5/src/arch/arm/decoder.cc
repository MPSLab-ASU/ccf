/*
 * Copyright (c) 2012 Google
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
 * Authors: Gabe Black
 */

#include "arch/arm/decoder.hh"
#include "arch/arm/isa_traits.hh"
#include "arch/arm/utility.hh"
#include "base/trace.hh"
#include "debug/Decoder.hh"

namespace ArmISA
{

GenericISA::BasicDecodeCache Decoder::defaultCache;

void
Decoder::process()
{
    // emi is typically ready, with some caveats below...
    instDone = true;

    if (!emi.thumb) {
        emi.instBits = data;
        emi.sevenAndFour = bits(data, 7) && bits(data, 4);
        emi.isMisc = (bits(data, 24, 23) == 0x2 &&
                      bits(data, 20) == 0);
        consumeBytes(4);
        DPRINTF(Decoder, "Arm inst: %#x.\n", (uint64_t)emi);
    } else {
        uint16_t word = (data >> (offset * 8));
        if (bigThumb) {
            // A 32 bit thumb inst is half collected.
            emi.instBits = emi.instBits | word;
            bigThumb = false;
            consumeBytes(2);
            DPRINTF(Decoder, "Second half of 32 bit Thumb: %#x.\n",
                    emi.instBits);
        } else {
            uint16_t highBits = word & 0xF800;
            if (highBits == 0xE800 || highBits == 0xF000 ||
                    highBits == 0xF800) {
                // The start of a 32 bit thumb inst.
                emi.bigThumb = 1;
                if (offset == 0) {
                    // We've got the whole thing.
                    emi.instBits = (data >> 16) | (data << 16);
                    DPRINTF(Decoder, "All of 32 bit Thumb: %#x.\n",
                            emi.instBits);
                    consumeBytes(4);
                } else {
                    // We only have the first half word.
                    DPRINTF(Decoder,
                            "First half of 32 bit Thumb.\n");
                    emi.instBits = (uint32_t)word << 16;
                    bigThumb = true;
                    consumeBytes(2);
                    // emi not ready yet.
                    instDone = false;
                }
            } else {
                // A 16 bit thumb inst.
                consumeBytes(2);
                emi.instBits = word;
                // Set the condition code field artificially.
                emi.condCode = COND_UC;
                DPRINTF(Decoder, "16 bit Thumb: %#x.\n",
                        emi.instBits);
                if (bits(word, 15, 8) == 0xbf &&
                        bits(word, 3, 0) != 0x0) {
                    foundIt = true;
                    itBits = bits(word, 7, 0);
                    DPRINTF(Decoder,
                            "IT detected, cond = %#x, mask = %#x\n",
                            itBits.cond, itBits.mask);
                }
            }
        }
    }
}

//Use this to give data to the decoder. This should be used
//when there is control flow.
void
Decoder::moreBytes(const PCState &pc, Addr fetchPC, MachInst inst)
{
    data = inst;
    offset = (fetchPC >= pc.instAddr()) ? 0 : pc.instAddr() - fetchPC;
    emi.thumb = pc.thumb();
    emi.fpscrLen = fpscrLen;
    emi.fpscrStride = fpscrStride;

    outOfBytes = false;
    process();
}

}
