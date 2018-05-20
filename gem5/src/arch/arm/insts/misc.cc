/*
 * Copyright (c) 2010 ARM Limited
 * Copyright (c) 2013 Advanced Micro Devices, Inc.
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
 *
 * Authors: Gabe Black
 */

#include "arch/arm/insts/misc.hh"
#include "cpu/reg_class.hh"

std::string
MrsOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ss << ", ";
    bool foundPsr = false;
    for (unsigned i = 0; i < numSrcRegs(); i++) {
        RegIndex idx = srcRegIdx(i);
        RegIndex rel_idx;
        if (regIdxToClass(idx, &rel_idx) != MiscRegClass) {
            continue;
        }
        if (rel_idx == MISCREG_CPSR) {
            ss << "cpsr";
            foundPsr = true;
            break;
        }
        if (rel_idx == MISCREG_SPSR) {
            ss << "spsr";
            foundPsr = true;
            break;
        }
    }
    if (!foundPsr) {
        ss << "????";
    }
    return ss.str();
}

void
MsrBase::printMsrBase(std::ostream &os) const
{
    printMnemonic(os);
    bool apsr = false;
    bool foundPsr = false;
    for (unsigned i = 0; i < numDestRegs(); i++) {
        int idx = destRegIdx(i);
        if (idx < Misc_Reg_Base) {
            continue;
        }
        idx -= Misc_Reg_Base;
        if (idx == MISCREG_CPSR) {
            os << "cpsr_";
            foundPsr = true;
            break;
        }
        if (idx == MISCREG_SPSR) {
            if (bits(byteMask, 1, 0)) {
                os << "spsr_";
            } else {
                os << "apsr_";
                apsr = true;
            }
            foundPsr = true;
            break;
        }
    }
    if (!foundPsr) {
        os << "????";
        return;
    }
    if (bits(byteMask, 3)) {
        if (apsr) {
            os << "nzcvq";
        } else {
            os << "f";
        }
    }
    if (bits(byteMask, 2)) {
        if (apsr) {
            os << "g";
        } else {
            os << "s";
        }
    }
    if (bits(byteMask, 1)) {
        os << "x";
    }
    if (bits(byteMask, 0)) {
        os << "c";
    }
}

std::string
MsrImmOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMsrBase(ss);
    ccprintf(ss, ", #%#x", imm);
    return ss.str();
}

std::string
MsrRegOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMsrBase(ss);
    ss << ", ";
    printReg(ss, op1);
    return ss.str();
}

std::string
ImmOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    ccprintf(ss, "#%d", imm);
    return ss.str();
}

std::string
RegImmOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ccprintf(ss, ", #%d", imm);
    return ss.str();
}

std::string
RegRegOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ss << ", ";
    printReg(ss, op1);
    return ss.str();
}

std::string
RegRegRegImmOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ss << ", ";
    printReg(ss, op1);
    ss << ", ";
    printReg(ss, op2);
    ccprintf(ss, ", #%d", imm);
    return ss.str();
}

std::string
RegRegRegRegOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ss << ", ";
    printReg(ss, op1);
    ss << ", ";
    printReg(ss, op2);
    ss << ", ";
    printReg(ss, op3);
    return ss.str();
}

std::string
RegRegRegOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ss << ", ";
    printReg(ss, op1);
    ss << ", ";
    printReg(ss, op2);
    return ss.str();
}

std::string
RegRegImmOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ss << ", ";
    printReg(ss, op1);
    ccprintf(ss, ", #%d", imm);
    return ss.str();
}

std::string
RegRegImmImmOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ss << ", ";
    printReg(ss, op1);
    ccprintf(ss, ", #%d, #%d", imm1, imm2);
    return ss.str();
}

std::string
RegImmRegOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ccprintf(ss, ", #%d, ", imm);
    printReg(ss, op1);
    return ss.str();
}

std::string
RegImmRegShiftOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    std::stringstream ss;
    printMnemonic(ss);
    printReg(ss, dest);
    ccprintf(ss, ", #%d, ", imm);
    printShiftOperand(ss, op1, true, shiftAmt, INTREG_ZERO, shiftType);
    printReg(ss, op1);
    return ss.str();
}

std::string
UnknownOp::generateDisassembly(Addr pc, const SymbolTable *symtab) const
{
    return csprintf("%-10s (inst %#08x)", "unknown", machInst);
}
