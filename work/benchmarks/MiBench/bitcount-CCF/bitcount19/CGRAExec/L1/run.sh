#!/bin/bash

############################################
############################################
## Author : Shail Dave              	  ##
##          Arizona State University	  ##
##				          ##
## Notes  : Script File To Compile A Loop ##
##	    for its Execution on CGRA     ##
##					  ##
############################################
############################################

#Set Architecture Parameters
X=4
Y=4
R=4

obj="$1"

#Setting Paths
LEVEL=../..
script=/home/shail/CGRA/scripts
opcodegen=/home/shail/CGRA/InstructionGenerator/insgen

#Detect node and edge file
llvmedge="$(find ./ -name "*.txt" | grep -i loop | grep -i edge)"
llvmnode="$(find ./ -name "*.txt" | grep -i loop | grep -i node)"

#REGIMap - Scheduling and Mapping
$script/regi.sh $llvmnode $llvmedge -X $X -Y $Y -R $R
finalnode="$(find ./ -name "*.txt" | grep -i final | grep -i node)"
Reginode="$(find ./ -name "*.txt" | grep -i REGIDUMP | grep -i node)"
Regiedge="$(find ./ -name "*.txt" | grep -i REGIDUMP | grep -i edge)"

liveoutnode="$(find ./ -name "*.txt" | grep -i liveout | grep -i node)"
liveoutedge="$(find ./ -name "*.txt" | grep -i liveout | grep -i edge)"

#liveinnode="$(find ./ -name "*.txt" | grep -i livein | grep -i node)"
#liveinedge="$(find ./ -name "*.txt" | grep -i livein | grep -i edge)"

#echo -n "" > livenode.txt	#generate blank LiveStore node file
#echo -n "" > liveedge.txt	#generate blank LiveStore edge file  

#echo -n "0" > livevar_st_ins_count.txt

#Instruction Generator
$opcodegen $finalnode $Regiedge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge > cgra_instructions.txt
