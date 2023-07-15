#!/bin/bash

#Get Architecture Information from the csv file and set corresponding architecture arguments
X=0
Y=0
R=0
IC=0
CGRAclock=0
CPUClock=0
ccf_root="/home/local/ASUAD/quoclon1/CCF_INS/ins3"
if [ -f "CGRA_config.csv" ]; then
    INPUTFILE="CGRA_config.csv"
    X=$(grep -w X $INPUTFILE | cut -d, -f2)
    Y=$(grep -w Y $INPUTFILE | cut -d, -f2)
    R=$(grep -w R $INPUTFILE | cut -d, -f2)
    IC=$(grep -w IC $INPUTFILE | cut -d, -f2)
    CGRAclock=$(grep -w Cclock $INPUTFILE | cut -d, -f2)
    CPUclock=$(grep -w CPUclock $INPUTFILE | cut -d, -f2)
else
    echo "Please include CGRA architecture file"
    exit 1
fi

echo $X
echo $Y
echo $R
echo "cgra"$X"x"$Y"r"$R""

#if [ "$R" -le  "2" ] || [ "$R" -eq "3" ] || [ "$R" -eq "5" ] || [ "$R" -eq "6" ] || [ "$R" -eq "7" ] || [ "$R" -gt "8" ] && [ "$R" -lt "16" ] || [ "$R" -gt "16" ]; then
#    echo -e "\e[31mRegister size of 4, 8 and 16 are only supported! Please change the CGRA_config.csv file to reflect this.\e[0m"
#    exit 1
#fi

echo "Compilation and simulation for CGRA for $X x $Y with $R registers per PE"

Ghz="GHz"
cgraclock="$CGRAclock$Ghz"
cpuclock="$CPUclock$Ghz"

if [ -d CGRAExec ]; then
     rm -rf CGRAExec;
fi

echo "CPU frequency @ $cpuclock"
echo "CGRA frequency @ $cgraclock" 

echo -e "\e[36m---------- CCF Compilation Stats Begin ----------\e[0m"
res1=$(date +%s.%N)

# Compile the c file my invoking Makefile make here.
make clean
make

res2=$(date +%s.%N)
dt=$(echo "$res2 - $res1" | bc)
dd=$(echo "$dt/86400" | bc)
dt2=$(echo "$dt-86400*$dd" | bc)
dh=$(echo "$dt2/3600" | bc)
dt3=$(echo "$dt2-3600*$dh" | bc)
dm=$(echo "$dt3/60" | bc)
ds=$(echo "$dt3-60*$dm" | bc)

printf "Total compilation time: %d:%02d:%02d:%02.4f\n" $dd $dh $dm $ds
echo -e "\e[36m---------- CCF Compilation Stats End ----------\e[0m"
############################ Running on gem5 ###############################################

#--debug-flags=CGRA,CGRA_Detailed,Setup_DEBUG,PE_DEBUG,Regfile_DEBUG

echo CGRA${X}x${Y}R${R}torus
# CGRA Execution
${ccf_root}/gem5/build/ARM/gem5.opt --debug-flags=CGRA_Detailed ${ccf_root}/gem5/configs/example/se_cgra.py -n 2 --cpu-type "AtomicSimpleCPU" --cgra --cgra-type "CGRA${X}x${Y}R${R}torus" -c ./add_small -o 10 > output_small.txt


# ARM execution
#~/ccf/gem5/build/ARM/gem5.opt -d ARM/ ~/git_repos/ccf/gem5/configs/example/se.py --cpu-type atomic -c ./add_arm -o "10" > output.txt



#echo "Output of the program saved in output.txt"

#if [ -d m5out ]; then
#  cd m5out
#  statfile="stats.txt"
#  if [ -s $statfile ]; then
#    echo -e "\e[36m---------- CCF Simulation Stats Begin ----------\e[0m"
#    cpucycles=$(grep "system.cpu0.numCycles" $statfile | awk '{print $2}')
#    cgracycles=$(grep "system.cpu1.numCycles" $statfile | awk '{print $2}')
# 
#    cputick=$(echo "1/$CPUclock" | bc -l)
#    cputime=$(echo "$cpucycles * $cputick * 1000 * 10^-12" | bc -l)
#    echo -e "\e[33mCPU Execution time @ $cpuclock:\t\t$cputime (seconds)\e[0m"
#    cgratick=$(echo "1/$CGRAclock" | bc -l)
#    cgratime=$(echo "$cgracycles * $cgratick * 1000 * 10^-12" | bc -l)
#    echo -e "\e[33mCGRA Execution time @ $cgraclock:\t\t$cgratime (seconds)\e[0m"
#    totaltime=$(echo "$cputime + $cgratime" | bc -l)
#    echo -e "\e[92mTotal Execution time of the program:\t$totaltime (seconds)\e[0m" 
#    echo -e "\e[36m---------- CCF Simulation Stats End ----------\e[0m"
#    cd ../
#  else
#    echo -e "\e[31mEmpty Stats file! Something wrong with gem5 execution!\e[0m"
#    exit 1
#  fi
#else 
#  echo -e "\e[31mStats file was not generated. Something wrong with gem5 execution!\e[0m"
#  exit 1
#fi 
