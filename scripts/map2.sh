#!/bin/bash
cfile="$1"
name=${cfile%.c}
llfile="$name.ll"
dfgfile="$name.dfg"
schfile="$name.sch"
#toolchain="/home/shail/ccf-init"
toolchain="/home/mahesh"
#map="$toolchain/RAMP/Release"
map="/home/mahesh/REGIMap_2.8/Release/"

nodefile="$toolchain/REGIMap_2.8/DFGFiles"

node="$1"
edge="$2"
X="$3"
Y="$4"
R="$5"
MODE="$6"

$map/REGIMap_2.2 -EDGE $edge -NODE $node -X $X -Y $Y -R $R -REGI 0


$nodefile/nodefile $node DUMP_node.txt > final_node.txt

exit
