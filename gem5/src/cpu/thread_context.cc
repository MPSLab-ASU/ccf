/*
 * Copyright (c) 2012, 2016-2017 ARM Limited
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
 * Copyright (c) 2006 The Regents of The University of Michigan
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

#include "cpu/thread_context.hh"

#include "arch/generic/vec_pred_reg.hh"
#include "base/logging.hh"
#include "base/trace.hh"
#include "config/the_isa.hh"
#include "cpu/base.hh"
#include "debug/Context.hh"
#include "debug/Quiesce.hh"
#include "params/BaseCPU.hh"
#include "sim/full_system.hh"

void
ThreadContext::compare(ThreadContext *one, ThreadContext *two)
{
    DPRINTF(Context, "Comparing thread contexts\n");

    // First loop through the integer registers.
    for (int i = 0; i < TheISA::NumIntRegs; ++i) {
        RegVal t1 = one->readIntReg(i);
        RegVal t2 = two->readIntReg(i);
        if (t1 != t2)
            panic("Int reg idx %d doesn't match, one: %#x, two: %#x",
                  i, t1, t2);
    }

    // Then loop through the floating point registers.
    for (int i = 0; i < TheISA::NumFloatRegs; ++i) {
        RegVal t1 = one->readFloatReg(i);
        RegVal t2 = two->readFloatReg(i);
        if (t1 != t2)
            panic("Float reg idx %d doesn't match, one: %#x, two: %#x",
                  i, t1, t2);
    }

    // Then loop through the vector registers.
    for (int i = 0; i < TheISA::NumVecRegs; ++i) {
        RegId rid(VecRegClass, i);
        const TheISA::VecRegContainer& t1 = one->readVecReg(rid);
        const TheISA::VecRegContainer& t2 = two->readVecReg(rid);
        if (t1 != t2)
            panic("Vec reg idx %d doesn't match, one: %#x, two: %#x",
                  i, t1, t2);
    }

    // Then loop through the predicate registers.
    for (int i = 0; i < TheISA::NumVecPredRegs; ++i) {
        RegId rid(VecPredRegClass, i);
        const TheISA::VecPredRegContainer& t1 = one->readVecPredReg(rid);
        const TheISA::VecPredRegContainer& t2 = two->readVecPredReg(rid);
        if (t1 != t2)
            panic("Pred reg idx %d doesn't match, one: %#x, two: %#x",
                  i, t1, t2);
    }

    for (int i = 0; i < TheISA::NumMiscRegs; ++i) {
        RegVal t1 = one->readMiscRegNoEffect(i);
        RegVal t2 = two->readMiscRegNoEffect(i);
        if (t1 != t2)
            panic("Misc reg idx %d doesn't match, one: %#x, two: %#x",
                  i, t1, t2);
    }

    // loop through the Condition Code registers.
    for (int i = 0; i < TheISA::NumCCRegs; ++i) {
        RegVal t1 = one->readCCReg(i);
        RegVal t2 = two->readCCReg(i);
        if (t1 != t2)
            panic("CC reg idx %d doesn't match, one: %#x, two: %#x",
                  i, t1, t2);
    }
    if (!(one->pcState() == two->pcState()))
        panic("PC state doesn't match.");
    int id1 = one->cpuId();
    int id2 = two->cpuId();
    if (id1 != id2)
        panic("CPU ids don't match, one: %d, two: %d", id1, id2);

    const ContextID cid1 = one->contextId();
    const ContextID cid2 = two->contextId();
    if (cid1 != cid2)
        panic("Context ids don't match, one: %d, two: %d", id1, id2);


}

void
ThreadContext::quiesce()
{
    getSystemPtr()->threads.quiesce(contextId());
}


void
ThreadContext::quiesceTick(Tick resume)
{
    getSystemPtr()->threads.quiesceTick(contextId(), resume);
}

void
serialize(const ThreadContext &tc, CheckpointOut &cp)
{
    using namespace TheISA;

    RegVal floatRegs[NumFloatRegs];
    for (int i = 0; i < NumFloatRegs; ++i)
        floatRegs[i] = tc.readFloatRegFlat(i);
    // This is a bit ugly, but needed to maintain backwards
    // compatibility.
    arrayParamOut(cp, "floatRegs.i", floatRegs, NumFloatRegs);

    std::vector<TheISA::VecRegContainer> vecRegs(NumVecRegs);
    for (int i = 0; i < NumVecRegs; ++i) {
        vecRegs[i] = tc.readVecRegFlat(i);
    }
    SERIALIZE_CONTAINER(vecRegs);

    std::vector<TheISA::VecPredRegContainer> vecPredRegs(NumVecPredRegs);
    for (int i = 0; i < NumVecPredRegs; ++i) {
        vecPredRegs[i] = tc.readVecPredRegFlat(i);
    }
    SERIALIZE_CONTAINER(vecPredRegs);

    RegVal intRegs[NumIntRegs];
    for (int i = 0; i < NumIntRegs; ++i)
        intRegs[i] = tc.readIntRegFlat(i);
    SERIALIZE_ARRAY(intRegs, NumIntRegs);

    if (NumCCRegs) {
        RegVal ccRegs[NumCCRegs];
        for (int i = 0; i < NumCCRegs; ++i)
            ccRegs[i] = tc.readCCRegFlat(i);
        SERIALIZE_ARRAY(ccRegs, NumCCRegs);
    }

    tc.pcState().serialize(cp);

    // thread_num and cpu_id are deterministic from the config
}

void
unserialize(ThreadContext &tc, CheckpointIn &cp)
{
    using namespace TheISA;

    RegVal floatRegs[NumFloatRegs];
    // This is a bit ugly, but needed to maintain backwards
    // compatibility.
    arrayParamIn(cp, "floatRegs.i", floatRegs, NumFloatRegs);
    for (int i = 0; i < NumFloatRegs; ++i)
        tc.setFloatRegFlat(i, floatRegs[i]);

    std::vector<TheISA::VecRegContainer> vecRegs(NumVecRegs);
    UNSERIALIZE_CONTAINER(vecRegs);
    for (int i = 0; i < NumVecRegs; ++i) {
        tc.setVecRegFlat(i, vecRegs[i]);
    }

    std::vector<TheISA::VecPredRegContainer> vecPredRegs(NumVecPredRegs);
    UNSERIALIZE_CONTAINER(vecPredRegs);
    for (int i = 0; i < NumVecPredRegs; ++i) {
        tc.setVecPredRegFlat(i, vecPredRegs[i]);
    }

    RegVal intRegs[NumIntRegs];
    UNSERIALIZE_ARRAY(intRegs, NumIntRegs);
    for (int i = 0; i < NumIntRegs; ++i)
        tc.setIntRegFlat(i, intRegs[i]);

    if (NumCCRegs) {
        RegVal ccRegs[NumCCRegs];
        UNSERIALIZE_ARRAY(ccRegs, NumCCRegs);
        for (int i = 0; i < NumCCRegs; ++i)
            tc.setCCRegFlat(i, ccRegs[i]);
    }

    PCState pcState;
    pcState.unserialize(cp);
    tc.pcState(pcState);

    // thread_num and cpu_id are deterministic from the config
}

void
takeOverFrom(ThreadContext &ntc, ThreadContext &otc)
{
    assert(ntc.getProcessPtr() == otc.getProcessPtr());

    ntc.setStatus(otc.status());
    ntc.copyArchRegs(&otc);
    ntc.setContextId(otc.contextId());
    ntc.setThreadId(otc.threadId());

    if (FullSystem)
        assert(ntc.getSystemPtr() == otc.getSystemPtr());

    otc.setStatus(ThreadContext::Halted);
}
