/*
 * Copyright (c) 2002-2005 The Regents of The University of Michigan
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
 */

#ifndef __REMOTE_GDB_HH__
#define __REMOTE_GDB_HH__

#include <sys/signal.h>

#include <map>

#include "arch/types.hh"
#include "base/pollevent.hh"
#include "base/socket.hh"
#include "cpu/pc_event.hh"

class System;
class ThreadContext;

class GDBListener;

enum GDBCommands
{
    GDBSignal              = '?', // last signal
    GDBSetBaud             = 'b', // set baud (depracated)
    GDBSetBreak            = 'B', // set breakpoint (depracated)
    GDBCont                = 'c', // resume
    GDBAsyncCont           = 'C', // continue with signal
    GDBDebug               = 'd', // toggle debug flags (deprecated)
    GDBDetach              = 'D', // detach remote gdb
    GDBRegR                = 'g', // read general registers
    GDBRegW                = 'G', // write general registers
    GDBSetThread           = 'H', // set thread
    GDBCycleStep           = 'i', // step a single cycle
    GDBSigCycleStep        = 'I', // signal then cycle step
    GDBKill                = 'k', // kill program
    GDBMemR                = 'm', // read memory
    GDBMemW                = 'M', // write memory
    GDBReadReg             = 'p', // read register
    GDBSetReg              = 'P', // write register
    GDBQueryVar            = 'q', // query variable
    GDBSetVar              = 'Q', // set variable
    GDBReset               = 'r', // reset system.  (Deprecated)
    GDBStep                = 's', // step
    GDBAsyncStep           = 'S', // signal and step
    GDBThreadAlive         = 'T', // find out if the thread is alive
    GDBTargetExit          = 'W', // target exited
    GDBBinaryDload         = 'X', // write memory
    GDBClrHwBkpt           = 'z', // remove breakpoint or watchpoint
    GDBSetHwBkpt           = 'Z'  // insert breakpoint or watchpoint
};

const char GDBStart = '$';
const char GDBEnd = '#';
const char GDBGoodP = '+';
const char GDBBadP = '-';

const int GDBPacketBufLen = 1024;

class BaseRemoteGDB
{
  private:
    friend void debugger();
    friend class GDBListener;

    //Helper functions
  protected:
    int digit2i(char);
    char i2digit(int);
    Addr hex2i(const char **);
    //Address formats, break types, and gdb commands may change
    //between architectures, so they're defined as virtual
    //functions.
    virtual void mem2hex(void *, const void *, int);
    virtual const char * hex2mem(void *, const char *, int);
    virtual const char * break_type(char c);
    virtual const char * gdb_command(char cmd);

  protected:
    class Event : public PollEvent
    {
      protected:
        BaseRemoteGDB *gdb;

      public:
        Event(BaseRemoteGDB *g, int fd, int e);
        void process(int revent);
    };

    friend class Event;
    Event *event;
    GDBListener *listener;
    int number;

  protected:
    //The socket commands come in through
    int fd;

  protected:
#ifdef notyet
    label_t recover;
#endif
    bool active;
    bool attached;

    System *system;
    ThreadContext *context;

  protected:
    class GdbRegCache
    {
      public:
        GdbRegCache(size_t newSize) : regs(new uint64_t[newSize]), size(newSize)
        {}
        ~GdbRegCache()
        {
            delete [] regs;
        }

        uint64_t * regs;
        size_t size;
        size_t bytes() { return size * sizeof(uint64_t); }
    };

    GdbRegCache gdbregs;

  protected:
    uint8_t getbyte();
    void putbyte(uint8_t b);

    int recv(char *data, int len);
    void send(const char *data);

  protected:
    // Machine memory
    virtual bool read(Addr addr, size_t size, char *data);
    virtual bool write(Addr addr, size_t size, const char *data);

    template <class T> T read(Addr addr);
    template <class T> void write(Addr addr, T data);

  public:
    BaseRemoteGDB(System *system, ThreadContext *context, size_t cacheSize);
    virtual ~BaseRemoteGDB();

    void replaceThreadContext(ThreadContext *tc) { context = tc; }

    void attach(int fd);
    void detach();
    bool isattached();

    virtual bool acc(Addr addr, size_t len) = 0;
    bool trap(int type);
    virtual bool breakpoint()
    {
        return trap(SIGTRAP);
    }

  protected:
    virtual void getregs() = 0;
    virtual void setregs() = 0;

    virtual void clearSingleStep() = 0;
    virtual void setSingleStep() = 0;

    PCEventQueue *getPcEventQueue();

  protected:
    class HardBreakpoint : public PCEvent
    {
      private:
        BaseRemoteGDB *gdb;

      public:
        int refcount;

      public:
        HardBreakpoint(BaseRemoteGDB *_gdb, Addr addr);
        const std::string name() const { return gdb->name() + ".hwbkpt"; }

        virtual void process(ThreadContext *tc);
    };
    friend class HardBreakpoint;

    typedef std::map<Addr, HardBreakpoint *> break_map_t;
    typedef break_map_t::iterator break_iter_t;
    break_map_t hardBreakMap;

    bool insertSoftBreak(Addr addr, size_t len);
    bool removeSoftBreak(Addr addr, size_t len);
    virtual bool insertHardBreak(Addr addr, size_t len);
    bool removeHardBreak(Addr addr, size_t len);

  protected:
    void clearTempBreakpoint(Addr &bkpt);
    void setTempBreakpoint(Addr bkpt);

  public:
    std::string name();
};

template <class T>
inline T
BaseRemoteGDB::read(Addr addr)
{
    T temp;
    read(addr, sizeof(T), (char *)&temp);
    return temp;
}

template <class T>
inline void
BaseRemoteGDB::write(Addr addr, T data)
{ write(addr, sizeof(T), (const char *)&data); }

class GDBListener
{
  protected:
    class Event : public PollEvent
    {
      protected:
        GDBListener *listener;

      public:
        Event(GDBListener *l, int fd, int e);
        void process(int revent);
    };

    friend class Event;
    Event *event;

  protected:
    ListenSocket listener;
    BaseRemoteGDB *gdb;
    int port;

  public:
    GDBListener(BaseRemoteGDB *g, int p);
    ~GDBListener();

    void accept();
    void listen();
    std::string name();
};

#endif /* __REMOTE_GDB_H__ */
