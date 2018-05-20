#!/bin/sh
#qemu-arm ./basicmath_small > output_small.txt

~/gem5/build/ARM/gem5.opt --debug-flags=CGRA,CGRA_Detailed ~/gem5/configs/example/se_hetro.py -n 2 --cpu-type atomic -c ./basicmath_small > output_small.txt
