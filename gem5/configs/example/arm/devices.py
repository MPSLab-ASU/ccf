# Copyright (c) 2016-2017, 2019 ARM Limited
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

# System components used by the bigLITTLE.py configuration script

from __future__ import print_function
from __future__ import absolute_import

import six

import m5
from m5.objects import *
m5.util.addToPath('../../')
from common.Caches import *
from common import ObjectList

if six.PY3:
    long = int

have_kvm = "ArmV8KvmCPU" in ObjectList.cpu_list.get_names()
have_fastmodel = "FastModelCortexA76" in ObjectList.cpu_list.get_names()

class L1I(L1_ICache):
    tag_latency = 1
    data_latency = 1
    response_latency = 1
    mshrs = 4
    tgts_per_mshr = 8
    size = '48kB'
    assoc = 3


class L1D(L1_DCache):
    tag_latency = 2
    data_latency = 2
    response_latency = 1
    mshrs = 16
    tgts_per_mshr = 16
    size = '32kB'
    assoc = 2
    write_buffers = 16


class WalkCache(PageTableWalkerCache):
    tag_latency = 4
    data_latency = 4
    response_latency = 4
    mshrs = 6
    tgts_per_mshr = 8
    size = '1kB'
    assoc = 8
    write_buffers = 16


class L2(L2Cache):
    tag_latency = 12
    data_latency = 12
    response_latency = 5
    mshrs = 32
    tgts_per_mshr = 8
    size = '1MB'
    assoc = 16
    write_buffers = 8
    clusivity='mostly_excl'


class L3(Cache):
    size = '16MB'
    assoc = 16
    tag_latency = 20
    data_latency = 20
    response_latency = 20
    mshrs = 20
    tgts_per_mshr = 12
    clusivity='mostly_excl'


class MemBus(SystemXBar):
    badaddr_responder = BadAddr(warn_access="warn")
    default = Self.badaddr_responder.pio


class CpuCluster(SubSystem):
    def __init__(self, system,  num_cpus, cpu_clock, cpu_voltage,
                 cpu_type, l1i_type, l1d_type, wcache_type, l2_type):
        super(CpuCluster, self).__init__()
        self._cpu_type = cpu_type
        self._l1i_type = l1i_type
        self._l1d_type = l1d_type
        self._wcache_type = wcache_type
        self._l2_type = l2_type

        assert num_cpus > 0

        self.voltage_domain = VoltageDomain(voltage=cpu_voltage)
        self.clk_domain = SrcClockDomain(clock=cpu_clock,
                                         voltage_domain=self.voltage_domain)

        self.cpus = [ self._cpu_type(cpu_id=system.numCpus() + idx,
                                     clk_domain=self.clk_domain)
                      for idx in range(num_cpus) ]

        for cpu in self.cpus:
            cpu.createThreads()
            cpu.createInterruptController()
            cpu.socket_id = system.numCpuClusters()
        system.addCpuCluster(self, num_cpus)

    def requireCaches(self):
        return self._cpu_type.require_caches()

    def memoryMode(self):
        return self._cpu_type.memory_mode()

    def addL1(self):
        for cpu in self.cpus:
            l1i = None if self._l1i_type is None else self._l1i_type()
            l1d = None if self._l1d_type is None else self._l1d_type()
            iwc = None if self._wcache_type is None else self._wcache_type()
            dwc = None if self._wcache_type is None else self._wcache_type()
            cpu.addPrivateSplitL1Caches(l1i, l1d, iwc, dwc)

    def addL2(self, clk_domain):
        if self._l2_type is None:
            return
        self.toL2Bus = L2XBar(width=64, clk_domain=clk_domain)
        self.l2 = self._l2_type()
        for cpu in self.cpus:
            cpu.connectAllPorts(self.toL2Bus)
        self.toL2Bus.master = self.l2.cpu_side

    def addPMUs(self, ints, events=[]):
        """
        Instantiates 1 ArmPMU per PE. The method is accepting a list of
        interrupt numbers (ints) used by the PMU and a list of events to
        register in it.

        :param ints: List of interrupt numbers. The code will iterate over
            the cpu list in order and will assign to every cpu in the cluster
            a PMU with the matching interrupt.
        :type ints: List[int]
        :param events: Additional events to be measured by the PMUs
        :type events: List[Union[ProbeEvent, SoftwareIncrement]]
        """
        assert len(ints) == len(self.cpus)
        for cpu, pint in zip(self.cpus, ints):
            int_cls = ArmPPI if pint < 32 else ArmSPI
            for isa in cpu.isa:
                isa.pmu = ArmPMU(interrupt=int_cls(num=pint))
                isa.pmu.addArchEvents(cpu=cpu, itb=cpu.itb, dtb=cpu.dtb,
                                      icache=getattr(cpu, 'icache', None),
                                      dcache=getattr(cpu, 'dcache', None),
                                      l2cache=getattr(self, 'l2', None))
                for ev in events:
                    isa.pmu.addEvent(ev)

    def connectMemSide(self, bus):
        bus.slave
        try:
            self.l2.mem_side = bus.slave
        except AttributeError:
            for cpu in self.cpus:
                cpu.connectAllPorts(bus)


class AtomicCluster(CpuCluster):
    def __init__(self, system, num_cpus, cpu_clock, cpu_voltage="1.0V"):
        cpu_config = [ ObjectList.cpu_list.get("AtomicSimpleCPU"), None,
                       None, None, None ]
        super(AtomicCluster, self).__init__(system, num_cpus, cpu_clock,
                                            cpu_voltage, *cpu_config)
    def addL1(self):
        pass

class KvmCluster(CpuCluster):
    def __init__(self, system, num_cpus, cpu_clock, cpu_voltage="1.0V"):
        cpu_config = [ ObjectList.cpu_list.get("ArmV8KvmCPU"), None, None,
            None, None ]
        super(KvmCluster, self).__init__(system, num_cpus, cpu_clock,
                                         cpu_voltage, *cpu_config)
    def addL1(self):
        pass

class FastmodelCluster(SubSystem):
    def __init__(self, system,  num_cpus, cpu_clock, cpu_voltage="1.0V"):
        super(FastmodelCluster, self).__init__()

        # Setup GIC
        gic = system.realview.gic
        gic.sc_gic.cpu_affinities = ','.join(
            [ '0.0.%d.0' % i for i in range(num_cpus) ])

        # Parse the base address of redistributor.
        redist_base = gic.get_redist_bases()[0]
        redist_frame_size = 0x40000 if gic.sc_gic.has_gicv4_1 else 0x20000
        gic.sc_gic.reg_base_per_redistributor = ','.join([
            '0.0.%d.0=%#x' % (i, redist_base + redist_frame_size * i)
            for i in range(num_cpus)
        ])

        gic_a2t = AmbaToTlmBridge64(amba=gic.amba_m)
        gic_t2g = TlmToGem5Bridge64(tlm=gic_a2t.tlm, gem5=system.iobus.slave)
        gic_g2t = Gem5ToTlmBridge64(gem5=system.membus.master)
        gic_g2t.addr_ranges = gic.get_addr_ranges()
        gic_t2a = AmbaFromTlmBridge64(tlm=gic_g2t.tlm)
        gic.amba_s = gic_t2a.amba

        system.gic_hub = SubSystem()
        system.gic_hub.gic_a2t = gic_a2t
        system.gic_hub.gic_t2g = gic_t2g
        system.gic_hub.gic_g2t = gic_g2t
        system.gic_hub.gic_t2a = gic_t2a

        self.voltage_domain = VoltageDomain(voltage=cpu_voltage)
        self.clk_domain = SrcClockDomain(clock=cpu_clock,
                                         voltage_domain=self.voltage_domain)

        # Setup CPU
        assert num_cpus <= 4
        CpuClasses = [FastModelCortexA76x1, FastModelCortexA76x2,
                      FastModelCortexA76x3, FastModelCortexA76x4]
        CpuClass = CpuClasses[num_cpus - 1]

        cpu = CpuClass(GICDISABLE=False)
        for core in cpu.cores:
            core.semihosting_enable = False
            core.RVBARADDR = 0x10
            core.redistributor = gic.redistributor
        self.cpus = [ cpu ]

        a2t = AmbaToTlmBridge64(amba=cpu.amba)
        t2g = TlmToGem5Bridge64(tlm=a2t.tlm, gem5=system.membus.slave)
        system.gic_hub.a2t = a2t
        system.gic_hub.t2g = t2g

        system.addCpuCluster(self, num_cpus)

    def requireCaches(self):
        return False

    def memoryMode(self):
        return 'atomic_noncaching'

    def addL1(self):
        pass

    def addL2(self, clk_domain):
        pass

    def connectMemSide(self, bus):
        pass

def simpleSystem(BaseSystem, caches, mem_size, platform=None, **kwargs):
    """
    Create a simple system example.  The base class in configurable so
    that it is possible (e.g) to link the platform (hardware configuration)
    with a baremetal ArmSystem or with a LinuxArmSystem.
    """
    class SimpleSystem(BaseSystem):
        cache_line_size = 64

        def __init__(self, caches, mem_size, platform=None, **kwargs):
            super(SimpleSystem, self).__init__(**kwargs)

            self.voltage_domain = VoltageDomain(voltage="1.0V")
            self.clk_domain = SrcClockDomain(
                clock="1GHz",
                voltage_domain=Parent.voltage_domain)

            if platform is None:
                self.realview = VExpress_GEM5_V1()
            else:
                self.realview = platform

            if hasattr(self.realview.gic, 'cpu_addr'):
                self.gic_cpu_addr = self.realview.gic.cpu_addr
            self.flags_addr = self.realview.realview_io.pio_addr + 0x30

            self.membus = MemBus()

            self.intrctrl = IntrControl()
            self.terminal = Terminal()
            self.vncserver = VncServer()

            self.iobus = IOXBar()
            # CPUs->PIO
            self.iobridge = Bridge(delay='50ns')
            # Device DMA -> MEM
            mem_range = self.realview._mem_regions[0]
            assert long(mem_range.size()) >= long(Addr(mem_size))
            self.mem_ranges = [
                AddrRange(start=mem_range.start, size=mem_size) ]

            self._caches = caches
            if self._caches:
                self.iocache = IOCache(addr_ranges=[self.mem_ranges[0]])
            else:
                self.dmabridge = Bridge(delay='50ns',
                                        ranges=[self.mem_ranges[0]])

            self._clusters = []
            self._num_cpus = 0

        def attach_pci(self, dev):
            self.realview.attachPciDevice(dev, self.iobus)

        def connect(self):
            self.iobridge.master = self.iobus.slave
            self.iobridge.slave = self.membus.master

            if self._caches:
                self.iocache.mem_side = self.membus.slave
                self.iocache.cpu_side = self.iobus.master
            else:
                self.dmabridge.master = self.membus.slave
                self.dmabridge.slave = self.iobus.master

            if hasattr(self.realview.gic, 'cpu_addr'):
                self.gic_cpu_addr = self.realview.gic.cpu_addr
            self.realview.attachOnChipIO(self.membus, self.iobridge)
            self.realview.attachIO(self.iobus)
            self.system_port = self.membus.slave

        def numCpuClusters(self):
            return len(self._clusters)

        def addCpuCluster(self, cpu_cluster, num_cpus):
            assert cpu_cluster not in self._clusters
            assert num_cpus > 0
            self._clusters.append(cpu_cluster)
            self._num_cpus += num_cpus

        def numCpus(self):
            return self._num_cpus

        def addCaches(self, need_caches, last_cache_level):
            if not need_caches:
                # connect each cluster to the memory hierarchy
                for cluster in self._clusters:
                    cluster.connectMemSide(self.membus)
                return

            cluster_mem_bus = self.membus
            assert last_cache_level >= 1 and last_cache_level <= 3
            for cluster in self._clusters:
                cluster.addL1()
            if last_cache_level > 1:
                for cluster in self._clusters:
                    cluster.addL2(cluster.clk_domain)
            if last_cache_level > 2:
                max_clock_cluster = max(self._clusters,
                                        key=lambda c: c.clk_domain.clock[0])
                self.l3 = L3(clk_domain=max_clock_cluster.clk_domain)
                self.toL3Bus = L2XBar(width=64)
                self.toL3Bus.master = self.l3.cpu_side
                self.l3.mem_side = self.membus.slave
                cluster_mem_bus = self.toL3Bus

            # connect each cluster to the memory hierarchy
            for cluster in self._clusters:
                cluster.connectMemSide(cluster_mem_bus)

    return SimpleSystem(caches, mem_size, platform, **kwargs)
