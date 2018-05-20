# Copyright (c) 2005-2007 The Regents of The University of Michigan
# Copyright (c) 2011 Regents of the University of California
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
# Authors: Nathan Binkert
#          Rick Strong

from m5.SimObject import SimObject
from m5.defines import buildEnv
from m5.params import *
from m5.proxy import *

from SimpleMemory import *

class MemoryMode(Enum): vals = ['invalid', 'atomic', 'timing',
                                'atomic_noncaching']

class System(MemObject):
    type = 'System'
    cxx_header = "sim/system.hh"
    system_port = MasterPort("System port")

    @classmethod
    def export_method_cxx_predecls(cls, code):
        code('#include "sim/system.hh"')

    @classmethod
    def export_methods(cls, code):
        code('''
      Enums::MemoryMode getMemoryMode() const;
      void setMemoryMode(Enums::MemoryMode mode);
''')

    memories = VectorParam.AbstractMemory(Self.all,
                                          "All memories in the system")
    mem_mode = Param.MemoryMode('atomic', "The mode the memory system is in")

    # The memory ranges are to be populated when creating the system
    # such that these can be passed from the I/O subsystem through an
    # I/O bridge or cache
    mem_ranges = VectorParam.AddrRange([], "Ranges that constitute main memory")

    cache_line_size = Param.Unsigned(64, "Cache line size in bytes")

    work_item_id = Param.Int(-1, "specific work item id")
    num_work_ids = Param.Int(16, "Number of distinct work item types")
    work_begin_cpu_id_exit = Param.Int(-1,
        "work started on specific id, now exit simulation")
    work_begin_ckpt_count = Param.Counter(0,
        "create checkpoint when work items begin count value is reached")
    work_begin_exit_count = Param.Counter(0,
        "exit simulation when work items begin count value is reached")
    work_end_ckpt_count = Param.Counter(0,
        "create checkpoint when work items end count value is reached")
    work_end_exit_count = Param.Counter(0,
        "exit simulation when work items end count value is reached")
    work_cpus_ckpt_count = Param.Counter(0,
        "create checkpoint when active cpu count value is reached")

    init_param = Param.UInt64(0, "numerical value to pass into simulator")
    boot_osflags = Param.String("a", "boot flags to pass to the kernel")
    kernel = Param.String("", "file that contains the kernel code")
    readfile = Param.String("", "file to read startup script from")
    symbolfile = Param.String("", "file to get the symbols from")
    load_addr_mask = Param.UInt64(0xffffffffff,
            "Address to mask loading binaries with");
