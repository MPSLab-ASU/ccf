#!/bin/bash
cfile="$1"
name=${cfile%.c}
llfile="$name.ll"
dfgfile="$name.dfg"
schfile="$name.sch"
#toolchain="/home/shail/ccf-init"
toolchain="/home/mahesh/git_repos/ccf"
#map="$toolchain/RAMP/Release"
map="/home/mahesh/git_repos/ullmann/Debug"

nodefile="$toolchain/RAMP/DFGFiles"

node="$1"
edge="$2"
X="$3"
Y="$4"
R="$5"
MODE="$6"

#$map/RAMP -EDGE $edge -NODE $node -X $X -Y $Y -R $R > $schfile

$map/Ullmann -EDGE $edge -NODE $node -X $X -Y $Y -R $R -MSA 10 -MAPII 10 -MAX_MAP 1000 -MAX_II 50 -LAMBDA 0.02 -MAP_MODE $MODE > debug


$nodefile/nodefile $node DUMP_node.txt > final_node.txt

exit
