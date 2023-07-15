/*
 * Copyright (c) 2020 ARM Limited
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

#ifndef __DEV_ARM_GENERIC_TIMER_MISCREGS_TYPES_HH__
#define __DEV_ARM_GENERIC_TIMER_MISCREGS_TYPES_HH__

#include "base/bitunion.hh"

namespace ArmISA
{
    BitUnion64(CNTKCTL)
        // IF Armv8.6-ECV
        Bitfield<17> evntis;
        // ENDIF Armv8.6-ECV
        Bitfield<9> el0pten;
        Bitfield<8> el0vten;
        Bitfield<7,4> evnti;
        Bitfield<3> evntdir;
        Bitfield<2> evnten;
        Bitfield<1> el0vcten;
        Bitfield<0> el0pcten;
    EndBitUnion(CNTKCTL)

    BitUnion64(CNTHCTL)
        // IF Armv8.6-ECV
        Bitfield<17> evntis;
        Bitfield<16> el1nvvct;
        Bitfield<15> el1nvpct;
        Bitfield<14> el1tvct;
        Bitfield<13> el1tvt;
        Bitfield<12> ecv;
        // ENDIF Armv8.6-ECV
        Bitfield<7,4> evnti;
        Bitfield<3> evntdir;
        Bitfield<2> evnten;
        Bitfield<1> el1pcen;
        Bitfield<0> el1pcten;
    EndBitUnion(CNTHCTL)
    // IF Armv8.1-VHE && HCR_EL2.E2H == 1
    BitUnion64(CNTHCTL_E2H)
        // IF Armv8.6-ECV
        Bitfield<17> evntis;
        Bitfield<16> el1nvvct;
        Bitfield<15> el1nvpct;
        Bitfield<14> el1tvct;
        Bitfield<13> el1tvt;
        Bitfield<12> ecv;
        // ENDIF Armv8.6-ECV
        Bitfield<11> el1pten;
        Bitfield<10> el1pcten;
        Bitfield<9> el0pten;
        Bitfield<8> el0vten;
        Bitfield<7,4> evnti;
        Bitfield<3> evntdir;
        Bitfield<2> evnten;
        Bitfield<1> el0vcten;
        Bitfield<0> el0pcten;
    EndBitUnion(CNTHCTL_E2H)
    // ENDIF Armv8.1-VHE && HCR_EL2.E2H == 1
}

#endif // __DEV_ARM_GENERIC_TIMER_MISCREGS_TYPES_HH__
