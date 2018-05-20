#!/bin/bash
#Author: Shail Dave
file="$1"
name=${file%.ll}

llfile="$name.ll"

toolchain="$HOME/shail/ccf"
llvmbin="$toolchain/llvm/build/bin"
llvmlib="$toolchain/llvm/build/lib"
llvmbuild="$toolchain/llvm/build"

$llvmbuild/bin/opt -strip-debug -O3 $llfile -o temp.bc
llvm-dis temp.bc -o $llfile

$llvmbuild/bin/opt -load $llvmbuild/lib/LLVMDDGGen.so -DDGGen $llfile -o temp.bc 
llvm-dis temp.bc -o temporary.ll
cp temp.bc $name.bc

# Check whether the directory CGRAExec exists
if [ -d "CGRAExec" ]; then
	$llvmbuild/bin/opt -load $llvmbuild/lib/LLVMInvokeCGRA.so -InvokeCGRA temp.bc -o temp1.bc
	llvm-dis temp1.bc -o temporaryIR.ll
	$llvmbuild/bin/opt -load $llvmbuild/lib/LLVMCGRAGen.so -CGRAGen temp1.bc -o $name.bc
fi

exit
