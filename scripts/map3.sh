#!/bin/bash
cfile="$1"
name=${cfile%.c}
llfile="$name.ll"
dfgfile="$name.dfg"
schfile="$name.sch"
#toolchain="/home/shail/ccf-init"
toolchain="/home/mahesh/git_repos/GraphMinor/"
#map="$toolchain/RAMP/Release"
map="/home/mahesh/git_repos/GraphMinor/Debug"

nodefile="$toolchain/DFGFiles"

node="$1"
edge="$2"
X="$3"
Y="$4"
R="$5"
MODE="$6"

$map/GraphMinor -EDGE $edge -NODE $node -X $X -Y $Y -R $R -MSA 50 -MAPII 10 -MAX_MAP 100 -MAX_II 50 -MAP_MODE $MODE > debug


$nodefile/nodefile $node DUMP_node.txt > final_node.txt

exit
