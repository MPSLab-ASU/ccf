#!/bin/bash
#Author: Shail Dave

CGRA_TIME=$1
MP_STANDALONE_TIME=$2
SPEEDUP=$(echo "scale = 4; $MP_STANDALONE_TIME/$CGRA_TIME" | bc)
speedup_int=$(echo "$MP_STANDALONE_TIME/$CGRA_TIME" | bc)

#Display Timings
echo " "
#echo "$(tput setaf 1)$(tput bold)Error! Output From CGRA Execution Is Different Than Expected!$(tput sgr0)"
echo "$(tput setaf 3)$(tput bold)************* Script Execution Statistics *************$(tput sgr0)"
echo "$(tput setaf 4)$(tput bold)CGRA+MP Execution Took ${CGRA_TIME} seconds.$(tput sgr0)"
echo "$(tput setaf 4)$(tput bold)Standalone Main Processor (MP) Execution Took ${MP_STANDALONE_TIME} seconds.$(tput sgr0)"
if [ $speedup_int -gt 1 ]
then
	echo "$(tput setaf 2)"
else
	echo "$(tput setaf 1)"
fi
echo "$(tput bold)Speedup is ${SPEEDUP}$(tput sgr0)"
echo " "


#Read Necessary Statistics From Gem5 Execution Stats File
cgra_stat="$(find ./m5out/ -name "stats.txt")"
mp_stat="$(find ./mp_standalone_out/ -name "stats.txt")"

cgra_sim_ticks=$(grep -n "sim_ticks" $cgra_stat | cut -d : -f 2 | tr -dc '0-9')
mp_sim_ticks=$(grep -n "sim_ticks" $mp_stat | cut -d : -f 2 | tr -dc '0-9')

#Display Important Statistics From Gem5 Execution
echo " "
echo "$(tput setaf 3)$(tput bold)************** Gem5 Execution Statistics **************$(tput sgr0)"
echo "$(tput setaf 6)$(tput bold)CGRA+MP Execution Took ${cgra_sim_ticks} sim ticks.$(tput sgr0)"
echo "$(tput setaf 6)$(tput bold)Standalone Main Processor (MP) Execution Took ${mp_sim_ticks} sim ticks.$(tput sgr0)"
SPEEDUP=$(echo "scale = 4; $mp_sim_ticks/$cgra_sim_ticks" | bc)
speedup_int=$(echo "$mp_sim_ticks/$cgra_sim_ticks" | bc)
if [ $speedup_int -gt 1 ]
then
	echo "$(tput setaf 2)"
else
	echo "$(tput setaf 1)"
fi
echo "$(tput bold)Speedup is ${SPEEDUP}$(tput sgr0)"
echo " "

