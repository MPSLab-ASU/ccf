/*
 * Copyright 2018 Google, Inc.
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

#include <vector>

#include "base/trace.hh"

#include "systemc/ext/systemc"

class Printer : public sc_core::sc_module
{
  public:
    sc_core::sc_in<const char *> input;

    SC_CTOR(Printer)
    {
        SC_THREAD(print);
    }

    using sc_core::sc_object::print;
    void
    print()
    {
        int i = 0;
        while (true) {
            wait(input.value_changed_event());
            DPRINTFN("Word %d: %s\n", i++, input.read());
        }
    }
};

class Feeder : public sc_core::sc_module
{
  public:
    sc_core::sc_in<bool> clk;
    sc_core::sc_out<const char *> output;

    std::vector<const char *> strings;

    SC_HAS_PROCESS(Feeder);
    Feeder(sc_core::sc_module_name, std::vector<const char *> _strings) :
        strings(_strings)
    {
        SC_THREAD(feed);
        sensitive << clk.pos();
    }

    void
    feed()
    {
        int i = 0;
        while (true) {
            wait();
            if (i >= strings.size())
                sc_core::sc_stop();
            else
                output = strings[i];
            i++;
        }
    }
};

int
sc_main(int argc, char *argv[])
{
    std::vector<const char *> strings;
    for (int i = 0; i < argc; i++)
        strings.push_back(argv[i]);

    sc_core::sc_clock clk;
    sc_core::sc_buffer<const char *> buf;

    Feeder feeder("feeder", strings);
    feeder.clk(clk);
    feeder.output(buf);

    Printer printer("printer");
    printer.input(buf);

    sc_core::sc_start();

    return 0;
}
