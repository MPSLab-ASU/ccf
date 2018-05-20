# REQUIRES: x86
# RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-linux %s -o %t

# RUN: echo "SECTIONS { . = SIZEOF_HEADERS; \
# RUN:  .text : { *(.text) }                \
# RUN:  .nonalloc : { *(.nonalloc) }        \
# RUN:  Sym = .;                            \
# RUN:  }" > %t.script
# RUN: ld.lld -o %t2 --script %t.script %t
# RUN: llvm-objdump -section-headers -t %t2 | FileCheck %s

# CHECK: SYMBOL TABLE:
# CHECK:  00000000000000f0 .nonalloc 00000000 Sym

.section .nonalloc,""
 .quad 0
