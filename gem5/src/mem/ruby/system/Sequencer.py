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
# Authors: Steve Reinhardt
#          Brad Beckmann

from m5.params import *
from m5.proxy import *
from MemObject import MemObject

class RubyPort(MemObject):
    type = 'RubyPort'
    abstract = True
    cxx_header = "mem/ruby/system/RubyPort.hh"
    slave = VectorSlavePort("CPU slave port")
    master = VectorMasterPort("CPU master port")
    version = Param.Int(0, "")
    pio_port = MasterPort("Ruby_pio_port")
    using_ruby_tester = Param.Bool(False, "")
    using_network_tester = Param.Bool(False, "")
    access_phys_mem = Param.Bool(False,
        "should the rubyport atomically update phys_mem")
    ruby_system = Param.RubySystem("")
    system = Param.System(Parent.any, "system object")
    support_data_reqs = Param.Bool(True, "data cache requests supported")
    support_inst_reqs = Param.Bool(True, "inst cache requests supported")


class RubyPortProxy(RubyPort):
    type = 'RubyPortProxy'
    cxx_header = "mem/ruby/system/RubyPortProxy.hh"
    access_phys_mem = True
    
class RubySequencer(RubyPort):
    type = 'RubySequencer'
    cxx_class = 'Sequencer'
    cxx_header = "mem/ruby/system/Sequencer.hh"
    icache = Param.RubyCache("")
    dcache = Param.RubyCache("")
    max_outstanding_requests = Param.Int(16,
        "max requests (incl. prefetches) outstanding")
    deadlock_threshold = Param.Cycles(500000,
        "max outstanding cycles for a request before deadlock/livelock declared")

class DMASequencer(RubyPort):
    type = 'DMASequencer'
    cxx_header = "mem/ruby/system/DMASequencer.hh"
    access_phys_mem = True
