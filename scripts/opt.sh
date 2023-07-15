#!/bin/bash
#Author: Shail Dave, Arkan Abuyazid
file="$1"
install_path="$2"
name=${file%.ll}

llfile="$name.ll"

toolchain="$3"

opt -loop-simplify -strip-debug -O3 -force-vector-width=1 $llfile -o temp.bc
llvm-dis temp.bc -o $llfile

echo "CondDDGGen Pass" 
echo $llfile
#$llvmbuild/bin/opt -load $llvmbuild/lib/LLVMDDGGen.so -DDGGen $llfile -o temp.bc 
opt -enable-new-pm=0 -load ${install_path}/lib/LLVMCondDDGGen.so -CondDDGGen $llfile -o CondDDGGen.bc

#echo "passed LLVMCond"
if [ -f "CondDDGGen.bc" ]; then
  echo "found CondDDGGen.bc"
else
  echo "No CondDDGGen.bc found check llvm pass"
fi
 
llvm-dis CondDDGGen.bc -o CondDDGGen.ll
cp CondDDGGen.bc $name.bc

if [ -d "CGRAExec" ]; then
	echo "CGRAGen Pass" 
        opt -enable-new-pm=0 -load ${install_path}/lib/LLVMCGRAGen.so -CGRAGen CondDDGGen.bc -o CGRAGen.bc
    	echo "CGRAGen Pass Complete"

fi

# Check whether the directory CGRAExec exists
#if [ -d "CGRAExec" ]; then
#    echo "InvokeCGRA Pass" 
#	opt -enable-new-pm=0 -load ${install_path}/lib/LLVMInvokeCGRA.so -InvokeCGRA CondDDGGen.bc -o InvokeCGRA.bc
#	llvm-dis InvokeCGRA.bc -o InvokeCGRA.ll
#    echo "CGRAGen Pass" 
#	opt -enable-new-pm=0 -load ${install_path}/lib/LLVMCGRAGen.so -CGRAGen InvokeCGRA.bc -o CGRAGen.bc
#    echo "CGRAGen Pass Complete"
#fi
exit 0
