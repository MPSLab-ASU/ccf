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

from abc import ABCMeta, abstractmethod
import m5
from m5.objects import *
from m5.proxy import *
m5.util.addToPath('../configs/common')
import FSConfig
from Caches import *
from base_config import *

class LinuxArmSystemBuilder(object):
    """Mix-in that implements create_system.

    This mix-in is intended as a convenient way of adding an
    ARM-specific create_system method to a class deriving from one of
    the generic base systems.
    """
    def __init__(self, machine_type):
        """
        Arguments:
          machine_type -- String describing the platform to simulate
        """
        self.machine_type = machine_type

    def create_system(self):
        system = FSConfig.makeArmSystem(self.mem_mode,
                                        self.machine_type, None, False)

        # We typically want the simulator to panic if the kernel
        # panics or oopses. This prevents the simulator from running
        # an obviously failed test case until the end of time.
        system.panic_on_panic = True
        system.panic_on_oops = True

        self.init_system(system)
        return system

class LinuxArmFSSystem(LinuxArmSystemBuilder,
                       BaseFSSystem):
    """Basic ARM full system builder."""

    def __init__(self, machine_type='RealView_PBX', **kwargs):
        """Initialize an ARM system that supports full system simulation.

        Note: Keyword arguments that are not listed below will be
        passed to the BaseFSSystem.

        Keyword Arguments:
          machine_type -- String describing the platform to simulate
        """
        BaseSystem.__init__(self, **kwargs)
        LinuxArmSystemBuilder.__init__(self, machine_type)

class LinuxArmFSSystemUniprocessor(LinuxArmSystemBuilder,
                                   BaseFSSystemUniprocessor):
    """Basic ARM full system builder for uniprocessor systems.

    Note: This class is a specialization of the ArmFSSystem and is
    only really needed to provide backwards compatibility for existing
    test cases.
    """

    def __init__(self, machine_type='RealView_PBX', **kwargs):
        BaseFSSystemUniprocessor.__init__(self, **kwargs)
        LinuxArmSystemBuilder.__init__(self, machine_type)


class LinuxArmFSSwitcheroo(LinuxArmSystemBuilder, BaseFSSwitcheroo):
    """Uniprocessor ARM system prepared for CPU switching"""

    def __init__(self, machine_type='RealView_PBX', **kwargs):
        BaseFSSwitcheroo.__init__(self, **kwargs)
        LinuxArmSystemBuilder.__init__(self, machine_type)
