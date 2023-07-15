/*
 * Copyright (c) 2020 ARM Limited
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
 * Copyright (c) 2003-2005 The Regents of The University of Michigan
 * Copyright (c) 2010 The Hewlett-Packard Development Company
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

#ifndef __BASE_DEBUG_HH__
#define __BASE_DEBUG_HH__

#include <initializer_list>
#include <map>
#include <string>
#include <vector>

namespace Debug {

void breakpoint();

class Flag
{
  protected:
    static bool _globalEnable; // whether debug tracings are enabled

    const char *_name;
    const char *_desc;

    virtual void sync() { }

  public:
    Flag(const char *name, const char *desc);
    virtual ~Flag();

    std::string name() const { return _name; }
    std::string desc() const { return _desc; }

    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual bool status() const = 0;

    operator bool() const { return status(); }
    bool operator!() const { return !status(); }

    static void globalEnable();
    static void globalDisable();
};

class SimpleFlag : public Flag
{
  protected:
    bool _tracing; // tracing is enabled and flag is on
    bool _status;  // flag status

    void sync() override { _tracing = _globalEnable && _status; }

  public:
    SimpleFlag(const char *name, const char *desc)
        : Flag(name, desc), _status(false)
    { }

    bool status() const override { return _tracing; }

    void enable() override  { _status = true;  sync(); }
    void disable() override { _status = false; sync(); }
};

class CompoundFlag : public Flag
{
  protected:
    std::vector<Flag *> _kids;

  public:
    template<typename... Args>
    CompoundFlag(const char *name, const char *desc,
                 std::initializer_list<Flag *> flags)
        : Flag(name, desc),
          _kids(flags)
    {
    }

    const std::vector<Flag *> &kids() const { return _kids; }

    void enable() override;
    void disable() override;
    bool status() const override;
};

typedef std::map<std::string, Flag *> FlagsMap;
FlagsMap &allFlags();

Flag *findFlag(const std::string &name);

bool changeFlag(const char *s, bool value);

} // namespace Debug

void setDebugFlag(const char *string);

void clearDebugFlag(const char *string);

void dumpDebugFlags();

/**
 * \def DTRACE(x)
 *
 * @ingroup api_trace
 * @{
 */
#if TRACING_ON
#   define DTRACE(x) (Debug::x)
#else // !TRACING_ON
#   define DTRACE(x) (false)
#endif  // TRACING_ON
/** @} */ // end of api_trace

#endif // __BASE_DEBUG_HH__
