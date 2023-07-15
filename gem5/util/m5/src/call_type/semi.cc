/*
 * Copyright 2020 Google Inc.
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

#include <cstring>

#include "args.hh"
#include "call_type.hh"
#include "dispatch_table.hh"

extern "C"
{
#define M5OP(name, func) __typeof__(name) M5OP_MERGE_TOKENS(name, _semi);
M5OP_FOREACH
#undef M5OP
}

namespace
{

DispatchTable semi_dispatch = {
#define M5OP(name, func) .name = &::M5OP_MERGE_TOKENS(name, _semi),
M5OP_FOREACH
#undef M5OP
};

class SemiCallType : public CallType
{
  public:
    SemiCallType() : CallType("semi") {}

    bool isDefault() const override { return CALL_TYPE_IS_DEFAULT; }
    const DispatchTable &getDispatch() const override { return semi_dispatch; }

    void
    printDesc(std::ostream &os) const override
    {
        os << "Use the semi-hosting based invocation method.";
    }
} semi_call_type;

} // anonymous namespace
