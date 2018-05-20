# Copyright (c) 2012 The Regents of The University of Michigan
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


from m5.objects import *

# Simple ALU Instructions have a latency of 1
class O3_ARM_v7a_Simple_Int(FUDesc):
    opList = [ OpDesc(opClass='IntAlu', opLat=1) ]
    count = 2

# Complex ALU instructions have a variable latencies
class O3_ARM_v7a_Complex_Int(FUDesc):
    opList = [ OpDesc(opClass='IntMult', opLat=3, issueLat=1),
               OpDesc(opClass='IntDiv', opLat=12, issueLat=12),
               OpDesc(opClass='IprAccess', opLat=3, issueLat=1) ]
    count = 1


# Floating point and SIMD instructions 
class O3_ARM_v7a_FP(FUDesc):
    opList = [ OpDesc(opClass='SimdAdd', opLat=4),
               OpDesc(opClass='SimdAddAcc', opLat=4),
               OpDesc(opClass='SimdAlu', opLat=4),
               OpDesc(opClass='SimdCmp', opLat=4),
               OpDesc(opClass='SimdCvt', opLat=3),
               OpDesc(opClass='SimdMisc', opLat=3),
               OpDesc(opClass='SimdMult',opLat=5),
               OpDesc(opClass='SimdMultAcc',opLat=5),
               OpDesc(opClass='SimdShift',opLat=3),
               OpDesc(opClass='SimdShiftAcc', opLat=3),
               OpDesc(opClass='SimdSqrt', opLat=9),
               OpDesc(opClass='SimdFloatAdd',opLat=5),
               OpDesc(opClass='SimdFloatAlu',opLat=5),
               OpDesc(opClass='SimdFloatCmp', opLat=3),
               OpDesc(opClass='SimdFloatCvt', opLat=3),
               OpDesc(opClass='SimdFloatDiv', opLat=3),
               OpDesc(opClass='SimdFloatMisc', opLat=3),
               OpDesc(opClass='SimdFloatMult', opLat=3),
               OpDesc(opClass='SimdFloatMultAcc',opLat=1),
               OpDesc(opClass='SimdFloatSqrt', opLat=9),
               OpDesc(opClass='FloatAdd', opLat=5),
               OpDesc(opClass='FloatCmp', opLat=5),
               OpDesc(opClass='FloatCvt', opLat=5),
               OpDesc(opClass='FloatDiv', opLat=9, issueLat=9),
               OpDesc(opClass='FloatSqrt', opLat=33, issueLat=33),
               OpDesc(opClass='FloatMult', opLat=4) ]
    count = 2


# Load/Store Units
class O3_ARM_v7a_Load(FUDesc):
    opList = [ OpDesc(opClass='MemRead',opLat=2) ]
    count = 1

class O3_ARM_v7a_Store(FUDesc):
    opList = [OpDesc(opClass='MemWrite',opLat=2) ]
    count = 1

# Functional Units for this CPU
class O3_ARM_v7a_FUP(FUPool):
    FUList = [O3_ARM_v7a_Simple_Int(), O3_ARM_v7a_Complex_Int(),
              O3_ARM_v7a_Load(), O3_ARM_v7a_Store(), O3_ARM_v7a_FP()]

# Tournament Branch Predictor
class O3_ARM_v7a_BP(BranchPredictor):
    predType = "tournament"
    localPredictorSize = 2048
    localCtrBits = 2
    localHistoryTableSize = 1024
    globalPredictorSize = 8192
    globalCtrBits = 2
    choicePredictorSize = 8192
    choiceCtrBits = 2
    BTBEntries = 2048
    BTBTagSize = 18
    RASSize = 16
    instShiftAmt = 2

class O3_ARM_v7a_3(DerivO3CPU):
    LQEntries = 16
    SQEntries = 16
    LSQDepCheckShift = 0
    LFSTSize = 1024
    SSITSize = 1024
    decodeToFetchDelay = 1
    renameToFetchDelay = 1
    iewToFetchDelay = 1
    commitToFetchDelay = 1
    renameToDecodeDelay = 1
    iewToDecodeDelay = 1
    commitToDecodeDelay = 1
    iewToRenameDelay = 1
    commitToRenameDelay = 1
    commitToIEWDelay = 1
    fetchWidth = 3
    fetchBufferSize = 16
    fetchToDecodeDelay = 3
    decodeWidth = 3
    decodeToRenameDelay = 2
    renameWidth = 3
    renameToIEWDelay = 1
    issueToExecuteDelay = 1
    dispatchWidth = 6
    issueWidth = 8
    wbWidth = 8
    wbDepth = 1
    fuPool = O3_ARM_v7a_FUP()
    iewToCommitDelay = 1
    renameToROBDelay = 1
    commitWidth = 8
    squashWidth = 8
    trapLatency = 13
    backComSize = 5
    forwardComSize = 5
    numPhysIntRegs = 128
    numPhysFloatRegs = 128
    numIQEntries = 32
    numROBEntries = 40

    switched_out = False
    branchPred = O3_ARM_v7a_BP()

# Instruction Cache
class O3_ARM_v7a_ICache(BaseCache):
    hit_latency = 1
    response_latency = 1
    mshrs = 2
    tgts_per_mshr = 8
    size = '32kB'
    assoc = 2
    is_top_level = 'true'

# Data Cache
class O3_ARM_v7a_DCache(BaseCache):
    hit_latency = 2
    response_latency = 2
    mshrs = 6
    tgts_per_mshr = 8
    size = '32kB'
    assoc = 2
    write_buffers = 16
    is_top_level = 'true'

# TLB Cache 
# Use a cache as a L2 TLB
class O3_ARM_v7aWalkCache(BaseCache):
    hit_latency = 4
    response_latency = 4
    mshrs = 6
    tgts_per_mshr = 8
    size = '1kB'
    assoc = 8
    write_buffers = 16
    is_top_level = 'true'


# L2 Cache
class O3_ARM_v7aL2(BaseCache):
    hit_latency = 12
    response_latency = 12
    mshrs = 16
    tgts_per_mshr = 8
    size = '1MB'
    assoc = 16
    write_buffers = 8
    prefetch_on_access = 'true'
    # Simple stride prefetcher
    prefetcher = StridePrefetcher(degree=8, latency = 1)

