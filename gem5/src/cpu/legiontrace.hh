/*
 * Copyright (c) 2001-2005 The Regents of The University of Michigan
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
 * Authors: Steve Reinhardt
 *          Nathan Binkert
 */

#ifndef __CPU_LEGIONTRACE_HH__
#define __CPU_LEGIONTRACE_HH__

#include "base/trace.hh"
#include "base/types.hh"
#include "cpu/static_inst.hh"
#include "params/LegionTrace.hh"
#include "sim/insttracer.hh"

class ThreadContext;

namespace Trace {

void setupSharedData();

class LegionTraceRecord : public InstRecord
{
  public:
    LegionTraceRecord(Tick _when, ThreadContext *_thread,
               const StaticInstPtr _staticInst, TheISA::PCState _pc,
               bool spec, const StaticInstPtr _macroStaticInst = NULL)
        : InstRecord(_when, _thread, _staticInst, _pc, spec,
                _macroStaticInst)
    {
    }

    void dump();
};

class LegionTrace : public InstTracer
{
  public:
    typedef LegionTraceParams Params;
    LegionTrace(const Params *p) : InstTracer(p)
    {}

    LegionTraceRecord *
    getInstRecord(Tick when, ThreadContext *tc,
            const StaticInstPtr staticInst, TheISA::PCState pc,
            const StaticInstPtr macroStaticInst = NULL)
    {
        if (tc->misspeculating())
            return NULL;

        return new LegionTraceRecord(when, tc,
                staticInst, pc, tc->misspeculating(), macroStaticInst);
    }
};

} // namespace Trace

#endif // __CPU_LEGIONTRACE_HH__
