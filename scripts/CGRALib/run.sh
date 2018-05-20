#!/bin/bash

############################################
############################################
## Author : Shail Dave              	    ##
##          Arizona State University	    ##
##				                                ##
## Notes  : Script File To Compile A Loop ##
##	    for its Execution on CGRA         ##
##					                              ##
############################################
############################################

#Set Architecture Parameters
X=4
Y=4
R=4

obj="$1"

#Setting Paths
LEVEL=../..
toolchain="/home/shail/ccf-init"
script="$toolchain/scripts"
opcodegen="$toolchain/InstructionGenerator/insgen"

#Detect node and edge file
llvmedge="$(find ./ -name "*.txt" | grep -i loop | grep -i edge)"
llvmnode="$(find ./ -name "*.txt" | grep -i loop | grep -i node)"

#RAMP - Scheduling and Mapping
$script/map.sh $llvmnode $llvmedge -X $X -Y $Y -R $R
finalnode="$(find ./ -name "*.txt" | grep -i final | grep -i node)"
node="$(find ./ -name "*.txt" | grep -i DUMP | grep -i node)"
edge="$(find ./ -name "*.txt" | grep -i DUMP | grep -i edge)"

liveoutnode="$(find ./ -name "*.txt" | grep -i liveout | grep -i node)"
liveoutedge="$(find ./ -name "*.txt" | grep -i liveout | grep -i edge)"

#Instruction Generator
$opcodegen $finalnode $edge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge > cgra_instructions.txt
