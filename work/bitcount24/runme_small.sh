#!/bin/sh
#qemu-arm ./bitcnts 75000 > output_small.txt
gem5="$HOME/shail/gem5"

$gem5/build/ARM/gem5.opt --debug-flags=CGRA,CGRA_Detailed $gem5/configs/example/se_hetro.py -n 2 --cpu-type AtomicSimpleCPU -c ./bitcnts --options="75000" > output_small.txt 
