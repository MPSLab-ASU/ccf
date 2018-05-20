#!/bin/bash
# Author: Shail Dave

#loopfolder="$1"
loopfolder="./"

#Manage Bin Files
mkdir $loopfolder/bin
mv *.bin $loopfolder/bin/

#Delete Files Generated For Loop Detection Scripting
rm loop_iterations.txt

#Delete Files Generated For Supporting REGIMap Implementation
rm II.txt mappingCompleted.txt states.txt
rm *latency*
#rm NonRotatingRegStats.txt

#Delete Files Generated For Supporting Gem5 Implementation
#rm prolog_cnt.txt epilog_cnt.txt

#Delete Verification Files
rm c_output.txt cgra_output.txt

#Manage LLVM Pass Files
mkdir $loopfolder/CompilerFiles
mv *.ll *.s $loopfolder/CompilerFiles

#Manage DFG Files
mkdir $loopfolder/DFGFiles
mv *node.txt *edge.txt *.dfg loop*.dot $loopfolder/DFGFiles

#Manage Dot Files
mkdir $loopfolder/dot
mv *.dot $loopfolder/dot/
#cp dot/loop*.dot ./

#Manage Schedule Files
mkdir $loopfolder/ScheduleFiles
mv *.sch $loopfolder/ScheduleFiles

#Manage Instruction Generation Files
mkdir $loopfolder/InstructionGeneratorFiles
mv cgra_instructions.txt *elf* $loopfolder/InstructionGeneratorFiles
#rm livevar_st_ins_count.txt config_ins_count.txt

#Manage Obj Files
#mkdir $loopfolder/ObjFiles
#mv *.out *obj* $loopfolder/ObjFiles

#Manage Files for Constants ESWEEK Paper
#rm constants.txt liven.txt
#mkdir $loopfolder/Stats
#mv *.txt $loopfolder/Stats

