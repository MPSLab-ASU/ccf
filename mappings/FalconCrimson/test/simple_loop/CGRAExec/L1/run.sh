#!/bin/bash

############################################
############################################
## Author : Shail Dave              	  ##
##          Arizona State University	  ##
##		  : Mahesh Bala                   ##
##          Arizona State University	  ##
## Notes  : Script File To Compile A Loop ##
##	    for its Execution on CGRA         ##
##					                      ##
############################################
############################################

#Get Architecture Information from the csv file and set corresponding architecture arguments
INPUTFILE="CGRA_config.csv"
X=$(grep X $INPUTFILE | cut -d, -f2)
Y=$(grep Y $INPUTFILE | cut -d, -f2)
R=$(grep R $INPUTFILE | cut -d, -f2)
IC=$(grep IC $INPUTFILE | cut -d, -f2)
MO=$(grep MODE $INPUTFILE | cut -d, -f2)

obj="$1"

#Setting Paths
LEVEL=../..
toolchain="/home/mahesh/ccf"
script="$toolchain/scripts"
opcodegen="$toolchain/InstructionGenerator/insgen"

#Detect node and edge file
llvmedge="$(find ./ -name "*.txt" | grep -i loop | grep -i edge)"
llvmnode="$(find ./ -name "*.txt" | grep -i loop | grep -i node)"

#RAMP or Ullmann - Scheduling and Mapping
$script/map.sh $llvmnode $llvmedge -X $X -Y $Y -R $R -MODE $MO
finalnode="$(find ./ -name "*.txt" | grep -i final | grep -i node)"
node="$(find ./ -name "*.txt" | grep -i DUMP | grep -i node)"
edge="$(find ./ -name "*.txt" | grep -i DUMP | grep -i edge)"

liveoutnode="$(find ./ -name "*.txt" | grep -i liveout | grep -i node)"
liveoutedge="$(find ./ -name "*.txt" | grep -i liveout | grep -i edge)"

#Instruction Generator
$opcodegen $finalnode $edge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge > cgra_instructions.txt
