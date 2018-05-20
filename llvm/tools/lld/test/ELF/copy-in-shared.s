// REQUIRES: x86
// RUN: llvm-mc -filetype=obj -triple=x86_64-pc-linux %p/Inputs/copy-in-shared.s -o %t1.o
// RUN: ld.lld -shared %t1.o -o %t1.so
// RUN: llvm-mc -filetype=obj -triple=x86_64-pc-linux %s -o %t2.o
// RUN: not ld.lld %t2.o %t1.so -o %t2.so -shared 2>&1 | FileCheck %s


.quad foo

// CHECK: {{.*}}.o:(.text+0x0): can't create dynamic relocation R_X86_64_64 against symbol 'foo' defined in {{.*}}.so
