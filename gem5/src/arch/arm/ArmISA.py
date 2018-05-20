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
# Authors: Andreas Sandberg

from m5.params import *
from m5.SimObject import SimObject

class ArmISA(SimObject):
    type = 'ArmISA'
    cxx_class = 'ArmISA::ISA'
    cxx_header = "arch/arm/isa.hh"

    # 0x35 Implementor is '5' from "M5"
    # 0x0 Variant
    # 0xf Architecture from CPUID scheme
    # 0xc00 Primary part number ("c" or higher implies ARM v7)
    # 0x0 Revision
    midr = Param.UInt32(0x350fc000, "Main ID Register")

    # See section B4.1.93 - B4.1.94 of the ARM ARM
    #
    # !ThumbEE | !Jazelle | Thumb | ARM
    # Note: ThumbEE is disabled for now since we don't support CP14
    # config registers and jumping to ThumbEE vectors
    id_pfr0 = Param.UInt32(0x00000031, "Processor Feature Register 0")
    # !Timer | !Virti | !M Profile | !TrustZone | ARMv4
    id_pfr1 = Param.UInt32(0x00000001, "Processor Feature Register 1")

    # See section B4.1.89 - B4.1.92 of the ARM ARM
    #  VMSAv7 support
    id_mmfr0 = Param.UInt32(0x00000003, "Memory Model Feature Register 0")
    id_mmfr1 = Param.UInt32(0x00000000, "Memory Model Feature Register 1")
    # no HW access | WFI stalling | ISB and DSB |
    # all TLB maintenance | no Harvard
    id_mmfr2 = Param.UInt32(0x01230000, "Memory Model Feature Register 2")
    # SuperSec | Coherent TLB | Bcast Maint |
    # BP Maint | Cache Maint Set/way | Cache Maint MVA
    id_mmfr3 = Param.UInt32(0xF0102211, "Memory Model Feature Register 3")

    # See section B4.1.84 of ARM ARM
    # All values are latest for ARMv7-A profile
    id_isar0 = Param.UInt32(0x02101111, "Instruction Set Attribute Register 0")
    id_isar1 = Param.UInt32(0x02112111, "Instruction Set Attribute Register 1")
    id_isar2 = Param.UInt32(0x21232141, "Instruction Set Attribute Register 2")
    id_isar3 = Param.UInt32(0x01112131, "Instruction Set Attribute Register 3")
    id_isar4 = Param.UInt32(0x10010142, "Instruction Set Attribute Register 4")
    id_isar5 = Param.UInt32(0x00000000, "Instruction Set Attribute Register 5")


    fpsid = Param.UInt32(0x410430A0, "Floating-point System ID Register")
