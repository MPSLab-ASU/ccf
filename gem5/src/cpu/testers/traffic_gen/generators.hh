/*
 * Copyright (c) 2012-2013 ARM Limited
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
 * Authors: Thomas Grass
 *          Andreas Hansson
 *          Sascha Bischoff
 */

/**
 * @file
 * Declaration of a set of generator behaviours that are used by the
 * stand-alone traffic generator, but can also be instantiated
 * elsewhere.
 */

#ifndef __CPU_TRAFFIC_GEN_GENERATORS_HH__
#define __CPU_TRAFFIC_GEN_GENERATORS_HH__

#include "mem/packet.hh"
#include "proto/protoio.hh"

/**
 * Base class for all generators, with the shared functionality and
 * virtual functions for entering, executing and leaving the
 * generator.
 */
class BaseGen
{

  protected:

    /** Name to use for status and debug printing */
    const std::string _name;

    /** The MasterID used for generating requests */
    const MasterID masterID;

    /**
     * Generate a new request and associated packet
     *
     * @param addr Physical address to use
     * @param size Size of the request
     * @param cmd Memory command to send
     * @param flags Optional request flags
     */
    PacketPtr getPacket(Addr addr, unsigned size, const MemCmd& cmd,
                        Request::FlagsType flags = 0);

  public:

    /** Time to spend in this state */
    const Tick duration;

    /**
     * Create a base generator.
     *
     * @param _name Name to use for status and debug
     * @param master_id MasterID set on each request
     * @param _duration duration of this state before transitioning
     */
    BaseGen(const std::string& _name, MasterID master_id, Tick _duration);

    virtual ~BaseGen() { }

    /**
     * Get the name, useful for DPRINTFs.
     *
     * @return the given name
     */
    std::string name() const { return _name; }

    /**
     * Enter this generator state.
     */
    virtual void enter() = 0;

    /**
     * Get the next generated packet.
     *
     * @return A packet to be sent at the current tick
     */
    virtual PacketPtr getNextPacket() = 0;

    /**
     * Exit this generator state. By default do nothing.
     */
    virtual void exit() { };

    /**
     * Determine the tick when the next packet is available. MaxTick
     * means that there will not be any further packets in the current
     * activation cycle of the generator.
     *
     * @param elastic should the injection respond to flow control or not
     * @param delay time the previous packet spent waiting
     * @return next tick when a packet is available
     */
    virtual Tick nextPacketTick(bool elastic, Tick delay) const = 0;

};

/**
 * The idle generator does nothing.
 */
class IdleGen : public BaseGen
{

  public:

    IdleGen(const std::string& _name, MasterID master_id, Tick _duration)
        : BaseGen(_name, master_id, _duration)
    { }

    void enter() { }

    PacketPtr getNextPacket() { return NULL; }

    Tick nextPacketTick(bool elastic, Tick delay) const { return MaxTick; }
};

/**
 * The linear generator generates sequential requests from a
 * start to an end address, with a fixed block size. A
 * fraction of the requests are reads, as determined by the
 * read percent. There is an optional data limit for when to
 * stop generating new requests.
 */
class LinearGen : public BaseGen
{

  public:

    /**
     * Create a linear address sequence generator. Set
     * min_period == max_period for a fixed inter-transaction
     * time.
     *
     * @param _name Name to use for status and debug
     * @param master_id MasterID set on each request
     * @param _duration duration of this state before transitioning
     * @param start_addr Start address
     * @param end_addr End address
     * @param _blocksize Size used for transactions injected
     * @param min_period Lower limit of random inter-transaction time
     * @param max_period Upper limit of random inter-transaction time
     * @param read_percent Percent of transactions that are reads
     * @param data_limit Upper limit on how much data to read/write
     */
    LinearGen(const std::string& _name, MasterID master_id, Tick _duration,
              Addr start_addr, Addr end_addr, Addr _blocksize,
              Tick min_period, Tick max_period,
              uint8_t read_percent, Addr data_limit)
        : BaseGen(_name, master_id, _duration),
          startAddr(start_addr), endAddr(end_addr),
          blocksize(_blocksize), minPeriod(min_period),
          maxPeriod(max_period), readPercent(read_percent),
          dataLimit(data_limit)
    { }

    void enter();

    PacketPtr getNextPacket();

    Tick nextPacketTick(bool elastic, Tick delay) const;

  private:

    /** Start of address range */
    const Addr startAddr;

    /** End of address range */
    const Addr endAddr;

    /** Blocksize and address increment */
    const Addr blocksize;

    /** Request generation period */
    const Tick minPeriod;
    const Tick maxPeriod;

    /**
     * Percent of generated transactions that should be reads
     */
    const uint8_t readPercent;

    /** Maximum amount of data to manipulate */
    const Addr dataLimit;

    /** Address of next request */
    Addr nextAddr;

    /**
     * Counter to determine the amount of data
     * manipulated. Used to determine if we should continue
     * generating requests.
     */
    Addr dataManipulated;
};

/**
 * The random generator is similar to the linear one, but does
 * not generate sequential addresses. Instead it randomly
 * picks an address in the range, aligned to the block size.
 */
class RandomGen : public BaseGen
{

  public:

    /**
     * Create a random address sequence generator. Set
     * min_period == max_period for a fixed inter-transaction
     * time.
     *
     * @param _name Name to use for status and debug
     * @param master_id MasterID set on each request
     * @param _duration duration of this state before transitioning
     * @param start_addr Start address
     * @param end_addr End address
     * @param _blocksize Size used for transactions injected
     * @param min_period Lower limit of random inter-transaction time
     * @param max_period Upper limit of random inter-transaction time
     * @param read_percent Percent of transactions that are reads
     * @param data_limit Upper limit on how much data to read/write
     */
    RandomGen(const std::string& _name, MasterID master_id, Tick _duration,
              Addr start_addr, Addr end_addr, Addr _blocksize,
              Tick min_period, Tick max_period,
              uint8_t read_percent, Addr data_limit)
        : BaseGen(_name, master_id, _duration),
          startAddr(start_addr), endAddr(end_addr),
          blocksize(_blocksize), minPeriod(min_period),
          maxPeriod(max_period), readPercent(read_percent),
          dataLimit(data_limit)
    { }

    void enter();

    PacketPtr getNextPacket();

    Tick nextPacketTick(bool elastic, Tick delay) const;

  private:

    /** Start of address range */
    const Addr startAddr;

    /** End of address range */
    const Addr endAddr;

    /** Block size */
    const Addr blocksize;

    /** Request generation period */
    const Tick minPeriod;
    const Tick maxPeriod;

    /**
     * Percent of generated transactions that should be reads
     */
    const uint8_t readPercent;

    /** Maximum amount of data to manipulate */
    const Addr dataLimit;

    /**
     * Counter to determine the amount of data
     * manipulated. Used to determine if we should continue
     * generating requests.
     */
    Addr dataManipulated;
};

/**
 * The trace replay generator reads a trace file and plays
 * back the transactions. The trace is offset with respect to
 * the time when the state was entered.
 */
class TraceGen : public BaseGen
{

  private:

    /**
     * This struct stores a line in the trace file.
     */
    struct TraceElement {

        /** Specifies if the request is to be a read or a write */
        MemCmd cmd;

        /** The address for the request */
        Addr addr;

        /** The size of the access for the request */
        Addr blocksize;

        /** The time at which the request should be sent */
        Tick tick;

        /** Potential request flags to use */
        Request::FlagsType flags;

        /**
         * Check validity of this element.
         *
         * @return if this element is valid
         */
        bool isValid() const {
            return cmd != MemCmd::InvalidCmd;
        }

        /**
         * Make this element invalid.
         */
        void clear() {
            cmd = MemCmd::InvalidCmd;
        }
    };

    /**
     * The InputStream encapsulates a trace file and the
     * internal buffers and populates TraceElements based on
     * the input.
     */
    class InputStream
    {

      private:

        /// Input file stream for the protobuf trace
        ProtoInputStream trace;

      public:

        /**
         * Create a trace input stream for a given file name.
         *
         * @param filename Path to the file to read from
         */
        InputStream(const std::string& filename);

        /**
         * Reset the stream such that it can be played once
         * again.
         */
        void reset();

        /**
         * Check the trace header to make sure that it is of the right
         * format.
         */
        void init();

        /**
         * Attempt to read a trace element from the stream,
         * and also notify the caller if the end of the file
         * was reached.
         *
         * @param element Trace element to populate
         * @return True if an element could be read successfully
         */
        bool read(TraceElement& element);
    };

  public:

    /**
     * Create a trace generator.
     *
     * @param _name Name to use for status and debug
     * @param master_id MasterID set on each request
     * @param _duration duration of this state before transitioning
     * @param trace_file File to read the transactions from
     * @param addr_offset Positive offset to add to trace address
     */
    TraceGen(const std::string& _name, MasterID master_id, Tick _duration,
             const std::string& trace_file, Addr addr_offset)
        : BaseGen(_name, master_id, _duration),
          trace(trace_file),
          tickOffset(0),
          addrOffset(addr_offset),
          traceComplete(false)
    {
    }

    void enter();

    PacketPtr getNextPacket();

    void exit();

    /**
     * Returns the tick when the next request should be generated. If
     * the end of the file has been reached, it returns MaxTick to
     * indicate that there will be no more requests.
     */
    Tick nextPacketTick(bool elastic, Tick delay) const;

  private:

    /** Input stream used for reading the input trace file */
    InputStream trace;

    /** Store the current and next element in the trace */
    TraceElement currElement;
    TraceElement nextElement;

    /**
     * Stores the time when the state was entered. This is to add an
     * offset to the times stored in the trace file. This is mutable
     * to allow us to change it as part of nextPacketTick.
     */
    mutable Tick tickOffset;

    /**
     * Offset for memory requests. Used to shift the trace
     * away from the CPU address space.
     */
    Addr addrOffset;

    /**
     * Set to true when the trace replay for one instance of
     * state is complete.
     */
    bool traceComplete;
};

#endif
