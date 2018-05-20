#!/bin/sh
#qemu-arm ./bitcnts 75000 > output_small.txt

~/gem5/build/ARM/gem5.opt  ~/gem5/configs/example/se_hetro.py -n 2 --cpu-type atomic -c ./bitcnts --options="75000" > output_small.txt 
