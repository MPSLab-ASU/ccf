# Copyright (c) 2012, 2017, 2019 ARM Limited
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

from abc import ABCMeta, abstractmethod
import m5
from m5.objects import *
from m5.proxy import *
m5.util.addToPath('../configs/')
from common import FSConfig
from common.Caches import *
from base_config import *
from common.cores.arm.O3_ARM_v7a import *
from common.Benchmarks import SysConfig

from common import SysPaths

class ArmSESystemUniprocessor(BaseSESystemUniprocessor):
    """Syscall-emulation builder for ARM uniprocessor systems.

    A small tweak of the syscall-emulation builder to use more
    representative cache configurations.
    """

    def __init__(self, **kwargs):
        super(ArmSESystemUniprocessor, self).__init__(**kwargs)

    def create_caches_private(self, cpu):
        # The atomic SE configurations do not use caches
        if self.mem_mode == "timing":
            # Use the more representative cache configuration
            cpu.addTwoLevelCacheHierarchy(O3_ARM_v7a_ICache(),
                                          O3_ARM_v7a_DCache(),
                                          O3_ARM_v7aL2())

class LinuxArmSystemBuilder(object):
    """Mix-in that implements create_system.

    This mix-in is intended as a convenient way of adding an
    ARM-specific create_system method to a class deriving from one of
    the generic base systems.
    """
    def __init__(self, machine_type, aarch64_kernel, **kwargs):
        """
        Arguments:
          machine_type -- String describing the platform to simulate
          num_cpus -- integer number of CPUs in the system
          use_ruby -- True if ruby is used instead of the classic memory system
        """
        self.machine_type = machine_type
        self.num_cpus = kwargs.get('num_cpus', 1)
        self.mem_size = kwargs.get('mem_size', '256MB')
        self.use_ruby = kwargs.get('use_ruby', False)
        self.aarch64_kernel = aarch64_kernel

    def create_system(self):
        if self.aarch64_kernel:
            gem5_kernel = "vmlinux.arm64"
            disk_image = "m5_exit.squashfs.arm64"
        else:
            gem5_kernel = "vmlinux.arm"
            disk_image = "m5_exit.squashfs.arm"

        default_kernels = {
            "VExpress_GEM5_V1": gem5_kernel,
            "VExpress_GEM5_Foundation": gem5_kernel,
        }

        sc = SysConfig(None, self.mem_size, [disk_image], "/dev/sda")
        system = FSConfig.makeArmSystem(self.mem_mode,
                                        self.machine_type, self.num_cpus,
                                        sc, ruby=self.use_ruby)

        # We typically want the simulator to panic if the kernel
        # panics or oopses. This prevents the simulator from running
        # an obviously failed test case until the end of time.
        system.workload.panic_on_panic = True
        system.workload.panic_on_oops = True

        system.workload.object_file = SysPaths.binary(
                    default_kernels[self.machine_type])

        self.init_system(system)

        system.workload.dtb_filename = \
            os.path.join(m5.options.outdir, 'system.dtb')
        system.generateDtb(system.workload.dtb_filename)
        return system

class LinuxArmFSSystem(LinuxArmSystemBuilder,
                       BaseFSSystem):
    """Basic ARM full system builder."""

    def __init__(self,
                 machine_type='VExpress_GEM5_Foundation',
                 aarch64_kernel=True,
                 **kwargs):
        """Initialize an ARM system that supports full system simulation.

        Note: Keyword arguments that are not listed below will be
        passed to the BaseFSSystem.

        Keyword Arguments:
          machine_type -- String describing the platform to simulate
        """
        BaseFSSystem.__init__(self, **kwargs)
        LinuxArmSystemBuilder.__init__(
            self, machine_type, aarch64_kernel, **kwargs)

    def create_caches_private(self, cpu):
        # Use the more representative cache configuration
        cpu.addTwoLevelCacheHierarchy(O3_ARM_v7a_ICache(),
                                      O3_ARM_v7a_DCache(),
                                      O3_ARM_v7aL2())

class LinuxArmFSSystemUniprocessor(LinuxArmSystemBuilder,
                                   BaseFSSystemUniprocessor):
    """Basic ARM full system builder for uniprocessor systems.

    Note: This class is a specialization of the ArmFSSystem and is
    only really needed to provide backwards compatibility for existing
    test cases.
    """

    def __init__(self,
                 machine_type='VExpress_GEM5_Foundation',
                 aarch64_kernel=True,
                 **kwargs):
        BaseFSSystemUniprocessor.__init__(self, **kwargs)
        LinuxArmSystemBuilder.__init__(
            self, machine_type, aarch64_kernel, **kwargs)

class LinuxArmFSSwitcheroo(LinuxArmSystemBuilder, BaseFSSwitcheroo):
    """Uniprocessor ARM system prepared for CPU switching"""

    def __init__(self,
                 machine_type='VExpress_GEM5_Foundation',
                 aarch64_kernel=True,
                 **kwargs):
        BaseFSSwitcheroo.__init__(self, **kwargs)
        LinuxArmSystemBuilder.__init__(
            self, machine_type, aarch64_kernel, **kwargs)
