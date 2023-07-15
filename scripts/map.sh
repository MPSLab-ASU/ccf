#!/bin/bash
cfile="$1"
name=${cfile%.c}
llfile="$name.ll"
dfgfile="$name.dfg"
schfile="$name.sch"
#toolchain="/home/shail/ccf-init"
toolchain="/home/mahesh/git_repos/ccf"
map="$toolchain/RAMP/Release"
nodefile="$toolchain/RAMP/DFGFiles"

node="$1"
edge="$2"
X="$4"
Y="$6"
R="$8"

$map/RAMP -EDGE $edge -NODE $node -X $X -Y $Y -R $R #> $schfile

$nodefile/nodefile $node DUMP_node.txt > final_node.txt

exit
