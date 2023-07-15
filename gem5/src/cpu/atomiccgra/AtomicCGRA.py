# Copyright (c) 2012 ARM Limited
# All rights reserved.
#
# The license below extends only to copyright in the software and shall
# not be construed as granting a license to any other intellectual
# property including but not limited to intellectual property relating
# to a hardware implementation of the functionality of the software
# licensed hereunder.  You may use the software subject to the license
# terms below provided that you ensure that this notice is replicated
# unmodified and in its entirety in all distributions of the software,
# modified or unmodified, in source code or in binary form.
#
# Copyright (c) 2007 The Regents of The University of Michigan
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

# Last edited: 10 July 2019
# Author: Mahesh Balasubramanian
# Update: Parameterized Rfsize and CGRA dims and create multiple commonly
#         used CGRA sizes. 


from m5.params import *
#from BaseCGRA import BaseCGRA
from m5.objects.BaseCGRA import BaseCGRA

class AtomicCGRA(BaseCGRA):
    """Simple CGRA model executing a configurable number of
    instructions per cycle. This model uses the simplified 'atomic'
    memory mode."""

    type = 'AtomicCGRA'
    cxx_header = "cpu/atomiccgra/atomiccgra.hh"

    @classmethod
    def memory_mode(cls):
        return 'atomic'

    @classmethod
    def support_take_over(cls):
        return True

    
    #dcache_port1 = MasterPort("Data Port1")
    #dcache_port2 = MasterPort("Data Port2")
    #dcache_port3 = MasterPort("Data Port3")
    #dcache_port4 = MasterPort("Data Port4")

    #_data_ports = ['dcache_port1', 'dcache_port2', 'dcache_port3','dcache_port4']

    #def connectDataPorts(self, bus, uncached_bus = None):
    #    for p in self._data_ports:
    #        exec('self.%s = bus.slave' % p)
    CGRA_rows = Param.Int(4, "CGRA Rows")
    CGRA_cols = Param.Int(4, "CGRA Columns")
    rfsize = Param.Int(4, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")
    master = MasterPort("vector port for connecting slaves")
    width = Param.Int(1, "CPU width")
    simulate_data_stalls = Param.Bool(False, "Simulate dcache stall cycles")
    simulate_inst_stalls = Param.Bool(False, "Simulate icache stall cycles")
    fastmem = Param.Bool(False, "Access memory directly")
    simpoint_profile = Param.Bool(False, "Generate SimPoint BBVs")
    simpoint_interval = Param.UInt64(100000000, "SimPoint Interval Size (insts)")
    simpoint_profile_file = Param.String("simpoint.bb.gz", "SimPoint BBV file")

## set of cgra classes with varying sizes and rf 0 with torus architecture.
class CGRA4x4R0torus(AtomicCGRA):
    CGRA_rows = Param.Int(4, "CGRA Rows")
    CGRA_cols = Param.Int(4, "CGRA Columns")
    rfsize = Param.Int(0, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA8x8R0torus(AtomicCGRA):
    CGRA_rows = Param.Int(8, "CGRA Rows")
    CGRA_cols = Param.Int(8, "CGRA Columns")
    rfsize = Param.Int(0, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA16x16R0torus(AtomicCGRA):
    CGRA_rows = Param.Int(16, "CGRA Rows")
    CGRA_cols = Param.Int(16, "CGRA Columns")
    rfsize = Param.Int(0, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA32x32R0torus(AtomicCGRA):
    CGRA_rows = Param.Int(32, "CGRA Rows")
    CGRA_cols = Param.Int(32, "CGRA Columns")
    rfsize = Param.Int(0, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA64x64R0torus(AtomicCGRA):
    CGRA_rows = Param.Int(64, "CGRA Rows")
    CGRA_cols = Param.Int(64, "CGRA Columns")
    rfsize = Param.Int(0, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

## set of cgra classes. with size from 4x4 to 64x64 and rf 4 and torus

class CGRA4x4R4torus(AtomicCGRA):
    CGRA_rows = Param.Int(4, "CGRA Rows")
    CGRA_cols = Param.Int(4, "CGRA Columns")
    rfsize = Param.Int(4, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA8x8R4torus(AtomicCGRA):
    CGRA_rows = Param.Int(8, "CGRA Rows")
    CGRA_cols = Param.Int(8, "CGRA Columns")
    rfsize = Param.Int(4, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA16x16R4torus(AtomicCGRA):
    CGRA_rows = Param.Int(16, "CGRA Rows")
    CGRA_cols = Param.Int(16, "CGRA Columns")
    rfsize = Param.Int(4, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA32x32R4torus(AtomicCGRA):
    CGRA_rows = Param.Int(32, "CGRA Rows")
    CGRA_cols = Param.Int(32, "CGRA Columns")
    rfsize = Param.Int(4, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA64x64R4torus(AtomicCGRA):
    CGRA_rows = Param.Int(64, "CGRA Rows")
    CGRA_cols = Param.Int(64, "CGRA Columns")
    rfsize = Param.Int(4, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

## set of cgra classes. with size from 4x4 to 64x64 and rf 8 and torus
class CGRA4x4R8torus(AtomicCGRA):
    CGRA_rows = Param.Int(4, "CGRA Rows")
    CGRA_cols = Param.Int(4, "CGRA Columns")
    rfsize = Param.Int(8, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA8x8R8torus(AtomicCGRA):
    CGRA_rows = Param.Int(8, "CGRA Rows")
    CGRA_cols = Param.Int(8, "CGRA Columns")
    rfsize = Param.Int(8, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA16x16R8torus(AtomicCGRA):
    CGRA_rows = Param.Int(16, "CGRA Rows")
    CGRA_cols = Param.Int(16, "CGRA Columns")
    rfsize = Param.Int(8, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA32x32R8torus(AtomicCGRA):
    CGRA_rows = Param.Int(32, "CGRA Rows")
    CGRA_cols = Param.Int(32, "CGRA Columns")
    rfsize = Param.Int(8, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA64x64R8torus(AtomicCGRA):
    CGRA_rows = Param.Int(64, "CGRA Rows")
    CGRA_cols = Param.Int(64, "CGRA Columns")
    rfsize = Param.Int(8, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

## set of cgra classes. with size from 4x4 to 64x64 and rf 16 and torus
class CGRA4x4R16torus(AtomicCGRA):
    CGRA_rows = Param.Int(4, "CGRA Rows")
    CGRA_cols = Param.Int(4, "CGRA Columns")
    rfsize = Param.Int(16, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA8x8R16torus(AtomicCGRA):
    CGRA_rows = Param.Int(8, "CGRA Rows")
    CGRA_cols = Param.Int(8, "CGRA Columns")
    rfsize = Param.Int(16, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA16x16R16torus(AtomicCGRA):
    CGRA_rows = Param.Int(16, "CGRA Rows")
    CGRA_cols = Param.Int(16, "CGRA Columns")
    rfsize = Param.Int(16, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA32x32R16torus(AtomicCGRA):
    CGRA_rows = Param.Int(32, "CGRA Rows")
    CGRA_cols = Param.Int(32, "CGRA Columns")
    rfsize = Param.Int(16, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")

class CGRA64x64R16torus(AtomicCGRA):
    CGRA_rows = Param.Int(64, "CGRA Rows")
    CGRA_cols = Param.Int(64, "CGRA Columns")
    rfsize = Param.Int(16, "RegFile per PE size")
    connection = Param.Int(0, "Connection type of CGRA -- 0: Simple_connection, 1: Diagonal Connection, 2: Hop_Connection")
