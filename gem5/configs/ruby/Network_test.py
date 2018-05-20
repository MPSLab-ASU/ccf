# Copyright (c) 2006-2007 The Regents of The University of Michigan
# Copyright (c) 2009 Advanced Micro Devices, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: Brad Beckmann

import m5
from m5.objects import *
from m5.defines import buildEnv
from m5.util import addToPath
from Ruby import create_topology

#
# Note: the cache latency is only used by the sequencer on fast path hits
#
class Cache(RubyCache):
    latency = 3

def define_options(parser):
    return

def create_system(options, system, piobus, dma_ports, ruby_system):
    
    if buildEnv['PROTOCOL'] != 'Network_test':
        panic("This script requires the Network_test protocol to be built.")

    cpu_sequencers = []

    #
    # The Garnet tester protocol does not support fs nor dma
    #
    assert(piobus == None)
    assert(dma_ports == [])
    
    #
    # The ruby network creation expects the list of nodes in the system to be
    # consistent with the NetDest list.  Therefore the l1 controller nodes must be
    # listed before the directory nodes and directory nodes before dma nodes, etc.
    #
    l1_cntrl_nodes = []
    dir_cntrl_nodes = []

    #
    # Must create the individual controllers before the network to ensure the
    # controller constructors are called before the network constructor
    #

    for i in xrange(options.num_cpus):
        #
        # First create the Ruby objects associated with this cpu
        # Only one cache exists for this protocol, so by default use the L1D
        # config parameters.
        #
        cache = Cache(size = options.l1d_size,
                      assoc = options.l1d_assoc)

        #
        # Only one unified L1 cache exists.  Can cache instructions and data.
        #
        l1_cntrl = L1Cache_Controller(version = i,
                                      cacheMemory = cache,
                                      ruby_system = ruby_system)

        cpu_seq = RubySequencer(icache = cache,
                                dcache = cache,
                                using_network_tester = True,
                                ruby_system = ruby_system)

        l1_cntrl.sequencer = cpu_seq
        exec("ruby_system.l1_cntrl%d = l1_cntrl" % i)

        #
        # Add controllers and sequencers to the appropriate lists
        #
        cpu_sequencers.append(cpu_seq)
        l1_cntrl_nodes.append(l1_cntrl)

    phys_mem_size = sum(map(lambda r: r.size(), system.mem_ranges))
    assert(phys_mem_size % options.num_dirs == 0)
    mem_module_size = phys_mem_size / options.num_dirs

    # Run each of the ruby memory controllers at a ratio of the frequency of
    # the ruby system.
    # clk_divider value is a fix to pass regression.
    ruby_system.memctrl_clk_domain = DerivedClockDomain(
                                          clk_domain=ruby_system.clk_domain,
                                          clk_divider=3)

    for i in xrange(options.num_dirs):
        #
        # Create the Ruby objects associated with the directory controller
        #

        mem_cntrl = RubyMemoryControl(
                              clk_domain = ruby_system.memctrl_clk_domain,
                              version = i,
                              ruby_system = ruby_system)

        dir_size = MemorySize('0B')
        dir_size.value = mem_module_size

        dir_cntrl = Directory_Controller(version = i,
                                         directory = \
                                         RubyDirectoryMemory(version = i,
                                                             size = dir_size),
                                         memBuffer = mem_cntrl,
                                         ruby_system = ruby_system)

        exec("ruby_system.dir_cntrl%d = dir_cntrl" % i)
        dir_cntrl_nodes.append(dir_cntrl)

    all_cntrls = l1_cntrl_nodes + dir_cntrl_nodes
    topology = create_topology(all_cntrls, options)
    return (cpu_sequencers, dir_cntrl_nodes, topology)
