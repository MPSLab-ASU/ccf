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
#X=4
#Y=4
#R=4

X=0
Y=0
R=0
IC=0
CGRAclock=0
CPUClock=0
MODE=0
ALGO=""
SCHED=1
if [ -f "CGRA_config.csv" ]; then
    INPUTFILE="CGRA_config.csv"
    X=$(grep X $INPUTFILE | cut -d, -f2)
    Y=$(grep Y $INPUTFILE | cut -d, -f2)
    R=$(grep R $INPUTFILE | cut -d, -f2)
    MODE=$(grep MODE $INPUTFILE | cut -d, -f2)
    ALGO=$(grep ALGO $INPUTFILE | cut -d, -f2)
    SCHED=$(grep SCHED $INPUTFILE | cut -d, -f2)
else
        echo "Please include CGRA architecture file"
        exit 1
fi


obj="$1"

#Setting Paths
LEVEL=../..
#toolchain="/home/shail/ccf-init"
ccf_root="$2"
script="$ccf_root/scripts"
opcodegen="$ccf_root/InstructionGenerator/insgen"
opcodegen1="$ccf_root/InstructionGenerator/falcon_insgen"

#Detect node and edge file
llvmedge="$(find ./ -name "*.txt" | grep -i loop | grep -i edge)"
llvmnode="$(find ./ -name "*.txt" | grep -i loop | grep -i node)"

#RAMP - Scheduling and Mapping
#if [ $ALGO -eq 0 ]; then
#  $script/map.sh $llvmnode $llvmedge -X $X -Y $Y -R $R
#elif [ $ALGO -eq 1 ]; then
#  $script/map1.sh $llvmnode $llvmedge $X $Y $R $MODE 
#elif [ $ALGO -eq 2 ]; then
#  $script/map2.sh $llvmnode $llvmedge $X $Y $R $MODE
#elif [ $ALGO -eq 3 ]; then
#  $script/map3.sh $llvmnode $llvmedge $X $Y $R $MODE
#else 
#  $script/map4.sh $llvmnode $llvmedge $X $Y $R $MODE $SCHED
#fi

find ${ccf_root}/mappings -maxdepth 1 -mindepth 1 -type d | while read dir; do
    if [[ $dir == *"$ALGO" ]]; then
        map="$dir/Release"
        nodefile="$dir/DFGFiles"
        $map/$ALGO -EDGE $edge -NODE $node -X $X -Y $Y -R $R #> $schfile
        $nodefile/nodefile $node DUMP_node.txt > final_node.txt
    fi
done

finalnode="$(find ./ -name "*.txt" | grep -i final | grep -i node)"
node="$(find ./ -name "*.txt" | grep -i DUMP | grep -i node)"
edge="$(find ./ -name "*.txt" | grep -i DUMP | grep -i edge)"

liveoutnode="$(find ./ -name "*.txt" | grep -i liveout | grep -i node)"
liveoutedge="$(find ./ -name "*.txt" | grep -i liveout | grep -i edge)"

if [ $R -eq 0 ]; then
  R=4
fi


#echo "Num regs is $R"

#Instruction Generator
if [ $ALGO == "Falcon" ]; then
  $opcodegen1 $finalnode $edge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge > cgra_instructions.txt
else
  $opcodegen $finalnode $edge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge > cgra_instructions.txt
fi
