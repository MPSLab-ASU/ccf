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
ALGO=""
MSA=10
MAPII=10
MAX_MAP=1000
MAX_II=50
LAMBDA=0.02

if [ -f "CGRA_config.csv" ]; then
    INPUTFILE="CGRA_config.csv"
    X=$(grep -w X $INPUTFILE | cut -d, -f2)
    Y=$(grep -w Y $INPUTFILE | cut -d, -f2)
    R=$(grep -w R $INPUTFILE | cut -d, -f2)
    ALGO=$(grep -w ALGO $INPUTFILE | cut -d, -f2)
    MSA=$(grep -w MSA $INPUTFILE | cut -d, -f2)
    MAPII=$(grep -w MAPII $INPUTFILE | cut -d, -f2)
    MAX_MAP=$(grep -w MAX_MAP $INPUTFILE | cut -d, -f2)
    MAX_II=$(grep -w MAX_II $INPUTFILE | cut -d, -f2)
    LAMBDA=$(grep -w LAMBDA $INPUTFILE | cut -d, -f2)


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
llvmedge="$(find . -name "*.txt" | grep -i loop | grep -i edge)"
llvmnode="$(find . -name "*.txt" | grep -i loop | grep -i node)"

echo $llvmnode

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

pwd
find ${ccf_root}/mappings -maxdepth 1 -mindepth 1 -type d | while read dir; do
    if [[ $dir == *"$ALGO" ]]; then
        map="$dir/Release"
        nodefile="$dir/DFGFiles"
        $map/$ALGO -EDGE $llvmedge -NODE $llvmnode -X $X -Y $Y -R $R -MSA $MSA -MAPII $MAPII -MAX_MAP $MAX_MAP -MAX_II $MAX_II -LAMBDA $LAMBDA #> $schfile
        $nodefile/nodefile $llvmnode DUMP_node.txt > final_node.txt
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
echo Instruction Generator
if [ $ALGO == "FalconCrimson" ]; then
  echo running FalconCrimson
  $opcodegen1 $finalnode $edge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge > cgra_instructions.txt
else
  echo Running something else 
  $opcodegen $finalnode $edge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge > cgra_instructions.txt
#  $opcodegen $finalnode $edge $llvmnode $llvmedge $obj prolog.sch kernel.sch epilog.sch $X $Y $R $liveoutnode $liveoutedge &> cgra_instructions.debug
fi
