/*
 * Copyright (c) 2015 RISC-V Foundation
 * Copyright (c) 2017 The University of Virginia
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

#include "arch/riscv/insts/amo.hh"

#include <sstream>
#include <string>

#include "arch/riscv/insts/bitfields.hh"
#include "arch/riscv/utility.hh"
#include "cpu/exec_context.hh"
#include "cpu/static_inst.hh"

using namespace std;

namespace RiscvISA
{

// memfence micro instruction
string
MemFenceMicro::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    ss << csprintf("0x%08x", machInst) << ' ' << mnemonic;
    return ss.str();
}

Fault MemFenceMicro::execute(ExecContext *xc,
    Trace::InstRecord *traceData) const
{
    return NoFault;
}

// load-reserved
string
LoadReserved::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    ss << mnemonic;
    if (AQ || RL)
        ss << '_';
    if (AQ)
        ss << "aq";
    if (RL)
        ss << "rl";
    ss << ' ' << registerName(RegId(IntRegClass, RD)) << ", ("
            << registerName(RegId(IntRegClass, RS1)) << ')';
    return ss.str();
}

string
LoadReservedMicro::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    ss << mnemonic << ' ' << registerName(_destRegIdx[0]) << ", ("
            << registerName(_srcRegIdx[0]) << ')';
    return ss.str();
}

// store-conditional
string
StoreCond::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    ss << mnemonic;
    if (AQ || RL)
        ss << '_';
    if (AQ)
        ss << "aq";
    if (RL)
        ss << "rl";
    ss << ' ' << registerName(RegId(IntRegClass, RD)) << ", "
            << registerName(RegId(IntRegClass, RS2)) << ", ("
            << registerName(RegId(IntRegClass, RS1)) << ')';
    return ss.str();
}

string
StoreCondMicro::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    ss << mnemonic << ' ' << registerName(_destRegIdx[0]) << ", "
            << registerName(_srcRegIdx[1]) << ", ("
            << registerName(_srcRegIdx[0]) << ')';
    return ss.str();
}

// AMOs
string
AtomicMemOp::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    ss << mnemonic;
    if (AQ || RL)
        ss << '_';
    if (AQ)
        ss << "aq";
    if (RL)
        ss << "rl";
    ss << ' ' << registerName(RegId(IntRegClass, RD)) << ", "
            << registerName(RegId(IntRegClass, RS2)) << ", ("
            << registerName(RegId(IntRegClass, RS1)) << ')';
    return ss.str();
}

string
AtomicMemOpMicro::generateDisassembly(
        Addr pc, const Loader::SymbolTable *symtab) const
{
    stringstream ss;
    ss << mnemonic << ' ' << registerName(_destRegIdx[0]) << ", "
            << registerName(_srcRegIdx[1]) << ", ("
            << registerName(_srcRegIdx[0]) << ')';
    return ss.str();
}

}
