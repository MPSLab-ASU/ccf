# Copyright (c) 2006-2007 The Regents of The University of Michigan
# Copyright (c) 2010 Advanced Micro Devices, Inc.
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
# Authors: Ron Dreslinski
#          Tushar Krishna

import m5
from m5.objects import *
from m5.defines import buildEnv
from m5.util import addToPath
import os, optparse, sys
addToPath('../common')
addToPath('../ruby')
addToPath('../topologies')

import Options
import Ruby

# Get paths we might need.  It's expected this file is in m5/configs/example.
config_path = os.path.dirname(os.path.abspath(__file__))
config_root = os.path.dirname(config_path)
m5_root = os.path.dirname(config_root)

parser = optparse.OptionParser()
Options.addCommonOptions(parser)

parser.add_option("--synthetic", type="int", default=0,
                  help="Synthetic Traffic type. 0 = Uniform Random,\
                        1 = Tornado, 2 = Bit Complement")

parser.add_option("-i", "--injectionrate", type="float", default=0.1,
                  metavar="I",
                  help="Injection rate in packets per cycle per node. \
                        Takes decimal value between 0 to 1 (eg. 0.225). \
                        Number of digits after 0 depends upon --precision.")

parser.add_option("--precision", type="int", default=3,
                  help="Number of digits of precision after decimal point\
                        for injection rate")

parser.add_option("--sim-cycles", type="int", default=1000,
                   help="Number of simulation cycles")

parser.add_option("--fixed-pkts", action="store_true",
                  help="Network_test: inject --maxpackets and stop")

parser.add_option("--maxpackets", type="int", default=1,
                  help="Stop injecting after --maxpackets. \
                        Works only with --fixed-pkts")

#
# Add the ruby specific and protocol specific options
#
Ruby.define_options(parser)

execfile(os.path.join(config_root, "common", "Options.py"))

(options, args) = parser.parse_args()

if args:
     print "Error: script doesn't take any positional arguments"
     sys.exit(1)

block_size = 64

if options.num_cpus > block_size:
     print "Error: Number of cores %d limited to %d because of false sharing" \
           % (options.num_cpus, block_size)
     sys.exit(1)


cpus = [ NetworkTest(fixed_pkts=options.fixed_pkts,
                     max_packets=options.maxpackets,
                     sim_cycles=options.sim_cycles,
                     traffic_type=options.synthetic,
                     inj_rate=options.injectionrate,
                     precision=options.precision,
                     num_memories=options.num_dirs) \
         for i in xrange(options.num_cpus) ]

# create the desired simulated system
system = System(cpu = cpus, physmem = SimpleMemory(),
                mem_ranges = [AddrRange(options.mem_size)])


# Create a top-level voltage domain and clock domain
system.voltage_domain = VoltageDomain(voltage = options.sys_voltage)

system.clk_domain = SrcClockDomain(clock = options.sys_clock,
                                   voltage_domain = system.voltage_domain)

Ruby.create_system(options, system)

# Create a seperate clock domain for Ruby
system.ruby.clk_domain = SrcClockDomain(clock = options.ruby_clock,
                                        voltage_domain = system.voltage_domain)

i = 0
for ruby_port in system.ruby._cpu_ruby_ports:
     #
     # Tie the cpu test ports to the ruby cpu port
     #
     cpus[i].test = ruby_port.slave
     ruby_port.access_phys_mem = False

     i += 1

# -----------------------
# run simulation
# -----------------------

root = Root( full_system = False, system = system )
root.system.mem_mode = 'timing'

# Not much point in this being higher than the L1 latency
m5.ticks.setGlobalFrequency('1ns')

# instantiate configuration
m5.instantiate()

# simulate until program terminates
exit_event = m5.simulate(options.abs_max_tick)

print 'Exiting @ tick', m5.curTick(), 'because', exit_event.getCause()
