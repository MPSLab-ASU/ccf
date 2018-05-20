# Copyright (c) 2012-2013 ARM Limited
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
#          Andreas Hansson

from abc import ABCMeta, abstractmethod
import m5
from m5.objects import *
from m5.proxy import *
m5.util.addToPath('../configs/common')
import FSConfig
from Caches import *

_have_kvm_support = 'BaseKvmCPU' in globals()

class BaseSystem(object):
    """Base system builder.

    This class provides some basic functionality for creating an ARM
    system with the usual peripherals (caches, GIC, etc.). It allows
    customization by defining separate methods for different parts of
    the initialization process.
    """

    __metaclass__ = ABCMeta

    def __init__(self, mem_mode='timing', mem_class=SimpleMemory,
                 cpu_class=TimingSimpleCPU, num_cpus=1, checker=False):
        """Initialize a simple base system.

        Keyword Arguments:
          mem_mode -- String describing the memory mode (timing or atomic)
          mem_class -- Memory controller class to use
          cpu_class -- CPU class to use
          num_cpus -- Number of CPUs to instantiate
          checker -- Set to True to add checker CPUs
        """
        self.mem_mode = mem_mode
        self.mem_class = mem_class
        self.cpu_class = cpu_class
        self.num_cpus = num_cpus
        self.checker = checker

    def create_cpus(self, cpu_clk_domain):
        """Return a list of CPU objects to add to a system."""
        cpus = [ self.cpu_class(clk_domain = cpu_clk_domain,
                                cpu_id=i)
                 for i in range(self.num_cpus) ]
        if self.checker:
            for c in cpus:
                c.addCheckerCpu()
        return cpus

    def create_caches_private(self, cpu):
        """Add private caches to a CPU.

        Arguments:
          cpu -- CPU instance to work on.
        """
        cpu.addPrivateSplitL1Caches(L1Cache(size='32kB', assoc=1),
                                    L1Cache(size='32kB', assoc=4))

    def create_caches_shared(self, system):
        """Add shared caches to a system.

        Arguments:
          system -- System to work on.

        Returns:
          A bus that CPUs should use to connect to the shared cache.
        """
        system.toL2Bus = CoherentBus(clk_domain=system.cpu_clk_domain)
        system.l2c = L2Cache(clk_domain=system.cpu_clk_domain,
                             size='4MB', assoc=8)
        system.l2c.cpu_side = system.toL2Bus.master
        system.l2c.mem_side = system.membus.slave
        return system.toL2Bus

    def init_cpu(self, system, cpu, sha_bus):
        """Initialize a CPU.

        Arguments:
          system -- System to work on.
          cpu -- CPU to initialize.
        """
        if not cpu.switched_out:
            self.create_caches_private(cpu)
            cpu.createInterruptController()
            cpu.connectAllPorts(sha_bus if sha_bus != None else system.membus,
                                system.membus)

    def init_kvm(self, system):
        """Do KVM-specific system initialization.

        Arguments:
          system -- System to work on.
        """
        system.vm = KvmVM()

    def init_system(self, system):
        """Initialize a system.

        Arguments:
          system -- System to initialize.
        """
        self.create_clk_src(system)
        system.cpu = self.create_cpus(system.cpu_clk_domain)

        if _have_kvm_support and \
                any([isinstance(c, BaseKvmCPU) for c in system.cpu]):
            self.init_kvm(system)

        sha_bus = self.create_caches_shared(system)
        for cpu in system.cpu:
            self.init_cpu(system, cpu, sha_bus)

    def create_clk_src(self,system):
        # Create system clock domain. This provides clock value to every
        # clocked object that lies beneath it unless explicitly overwritten
        # by a different clock domain.
        system.voltage_domain = VoltageDomain()
        system.clk_domain = SrcClockDomain(clock = '1GHz',
                                           voltage_domain =
                                           system.voltage_domain)

        # Create a seperate clock domain for components that should
        # run at CPUs frequency
        system.cpu_clk_domain = SrcClockDomain(clock = '2GHz',
                                               voltage_domain =
                                               system.voltage_domain)

    @abstractmethod
    def create_system(self):
        """Create an return an initialized system."""
        pass

    @abstractmethod
    def create_root(self):
        """Create and return a simulation root using the system
        defined by this class."""
        pass

class BaseSESystem(BaseSystem):
    """Basic syscall-emulation builder."""

    def __init__(self, **kwargs):
        BaseSystem.__init__(self, **kwargs)

    def init_system(self, system):
        BaseSystem.init_system(self, system)

    def create_system(self):
        system = System(physmem = self.mem_class(),
                        membus = CoherentBus(),
                        mem_mode = self.mem_mode)
        system.system_port = system.membus.slave
        system.physmem.port = system.membus.master
        self.init_system(system)
        return system

    def create_root(self):
        system = self.create_system()
        m5.ticks.setGlobalFrequency('1THz')
        return Root(full_system=False, system=system)

class BaseSESystemUniprocessor(BaseSESystem):
    """Basic syscall-emulation builder for uniprocessor systems.

    Note: This class is only really needed to provide backwards
    compatibility in existing test cases.
    """

    def __init__(self, **kwargs):
        BaseSESystem.__init__(self, **kwargs)

    def create_caches_private(self, cpu):
        # The atomic SE configurations do not use caches
        if self.mem_mode == "timing":
            # @todo We might want to revisit these rather enthusiastic L1 sizes
            cpu.addTwoLevelCacheHierarchy(L1Cache(size='128kB'),
                                          L1Cache(size='256kB'),
                                          L2Cache(size='2MB'))

    def create_caches_shared(self, system):
        return None

class BaseFSSystem(BaseSystem):
    """Basic full system builder."""

    def __init__(self, **kwargs):
        BaseSystem.__init__(self, **kwargs)

    def init_system(self, system):
        BaseSystem.init_system(self, system)

        # create the memory controllers and connect them, stick with
        # the physmem name to avoid bumping all the reference stats
        system.physmem = [self.mem_class(range = r)
                          for r in system.mem_ranges]
        for i in xrange(len(system.physmem)):
            system.physmem[i].port = system.membus.master

        # create the iocache, which by default runs at the system clock
        system.iocache = IOCache(addr_ranges=system.mem_ranges)
        system.iocache.cpu_side = system.iobus.master
        system.iocache.mem_side = system.membus.slave

    def create_root(self):
        system = self.create_system()
        m5.ticks.setGlobalFrequency('1THz')
        return Root(full_system=True, system=system)

class BaseFSSystemUniprocessor(BaseFSSystem):
    """Basic full system builder for uniprocessor systems.

    Note: This class is only really needed to provide backwards
    compatibility in existing test cases.
    """

    def __init__(self, **kwargs):
        BaseFSSystem.__init__(self, **kwargs)

    def create_caches_private(self, cpu):
        cpu.addTwoLevelCacheHierarchy(L1Cache(size='32kB', assoc=1),
                                      L1Cache(size='32kB', assoc=4),
                                      L2Cache(size='4MB', assoc=8))

    def create_caches_shared(self, system):
        return None

class BaseFSSwitcheroo(BaseFSSystem):
    """Uniprocessor system prepared for CPU switching"""

    def __init__(self, cpu_classes, **kwargs):
        BaseFSSystem.__init__(self, **kwargs)
        self.cpu_classes = tuple(cpu_classes)

    def create_cpus(self, cpu_clk_domain):
        cpus = [ cclass(clk_domain = cpu_clk_domain,
                        cpu_id=0,
                        switched_out=True)
                 for cclass in self.cpu_classes ]
        cpus[0].switched_out = False
        return cpus
