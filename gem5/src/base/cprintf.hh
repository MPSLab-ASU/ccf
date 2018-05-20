/*
 * Copyright (c) 2002-2006 The Regents of The University of Michigan
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
 * Authors: Nathan Binkert
 *          Steve Reinhardt
 */

#ifndef __BASE_CPRINTF_HH__
#define __BASE_CPRINTF_HH__

#include <ios>
#include <iostream>
#include <list>
#include <string>

#include "base/cprintf_formats.hh"
#include "base/varargs.hh"

namespace cp {

#define CPRINTF_DECLARATION VARARGS_DECLARATION(cp::Print)
#define CPRINTF_DEFINITION VARARGS_DEFINITION(cp::Print)

struct Print
{
  protected:
    std::ostream &stream;
    const char *format;
    const char *ptr;
    bool cont;

    std::ios::fmtflags saved_flags;
    char saved_fill;
    int saved_precision;

    Format fmt;
    void process();
    void process_flag();

  public:
    Print(std::ostream &stream, const std::string &format);
    Print(std::ostream &stream, const char *format);
    ~Print();

    int
    get_number(int data)
    {
        return data;
    }
    
    template <typename T>
    int
    get_number(const T& data)
    {
        return 0;
    }

    template <typename T>
    void
    add_arg(const T &data)
    {
        if (!cont)
            process();

        if (fmt.get_width) {
            fmt.get_width = false;
            cont = true;
            fmt.width = get_number(data);
            return;
        }
            
        if (fmt.get_precision) {
            fmt.get_precision = false;
            cont = true;
            fmt.precision = get_number(data);
            return;
        }

        switch (fmt.format) {
          case Format::character:
            format_char(stream, data, fmt);
            break;

          case Format::integer:
            format_integer(stream, data, fmt);
            break;

          case Format::floating:
            format_float(stream, data, fmt);
            break;

          case Format::string:
            format_string(stream, data, fmt);
            break;

          default:
            stream << "<bad format>";
            break;
        }
    }

    void end_args();
};

} // namespace cp

typedef VarArgs::List<cp::Print> CPrintfArgsList;

inline void
ccprintf(std::ostream &stream, const char *format, const CPrintfArgsList &args)
{
    cp::Print print(stream, format);
    args.add_args(print);
}

inline void
ccprintf(std::ostream &stream, const char *format, CPRINTF_DECLARATION)
{
    cp::Print print(stream, format);
    VARARGS_ADDARGS(print);
}

inline void
cprintf(const char *format, CPRINTF_DECLARATION)
{
    ccprintf(std::cout, format, VARARGS_ALLARGS);
}

inline std::string
csprintf(const char *format, CPRINTF_DECLARATION)
{
    std::stringstream stream;
    ccprintf(stream, format, VARARGS_ALLARGS);
    return stream.str();
}

/*
 * functions again with std::string.  We have both so we don't waste
 * time converting const char * to std::string since we don't take
 * advantage of it.
 */
inline void
ccprintf(std::ostream &stream, const std::string &format,
         const CPrintfArgsList &args)
{
    ccprintf(stream, format.c_str(), args);
}

inline void
ccprintf(std::ostream &stream, const std::string &format, CPRINTF_DECLARATION)
{
    ccprintf(stream, format.c_str(), VARARGS_ALLARGS);
}

inline void
cprintf(const std::string &format, CPRINTF_DECLARATION)
{
    ccprintf(std::cout, format.c_str(), VARARGS_ALLARGS);
}

inline std::string
csprintf(const std::string &format, CPRINTF_DECLARATION)
{
    std::stringstream stream;
    ccprintf(stream, format.c_str(), VARARGS_ALLARGS);
    return stream.str();
}

#endif // __CPRINTF_HH__
