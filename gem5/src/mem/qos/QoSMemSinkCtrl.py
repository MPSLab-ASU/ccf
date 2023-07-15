# Copyright (c) 2018-2020 ARM Limited
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
# Author: Matteo Andreozzi

from m5.params import *
from m5.objects.QoSMemCtrl import *
from m5.objects.QoSMemSinkInterface import *

class QoSMemSinkCtrl(QoSMemCtrl):
    type = 'QoSMemSinkCtrl'
    cxx_header = "mem/qos/mem_sink.hh"
    cxx_class = "QoS::MemSinkCtrl"
    port = ResponsePort("Response ports")


    interface = Param.QoSMemSinkInterface(QoSMemSinkInterface(),
                "Interface to memory")

    # the basic configuration of the controller architecture, note
    # that each entry corresponds to a burst for the specific DRAM
    # configuration (e.g. x32 with burst length 8 is 32 bytes) and not
    # the cacheline size or request/packet size
    write_buffer_size = Param.Unsigned(64, "Number of write queue entries")
    read_buffer_size = Param.Unsigned(32, "Number of read queue entries")

    # memory packet size
    memory_packet_size = Param.MemorySize("32B", "Memory packet size")

    # request latency - minimum timing between requests
    request_latency = Param.Latency("20ns", "Memory latency between requests")

    # response latency - time to issue a response once a request is serviced
    response_latency = Param.Latency("20ns", "Memory response latency")
