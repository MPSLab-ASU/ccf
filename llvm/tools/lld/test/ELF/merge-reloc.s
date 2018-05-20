# REQUIRES: x86
# RUN: llvm-mc -filetype=obj -triple=x86_64-pc-linux %s -o %t.o
# RUN: ld.lld %t.o -r -o %t-rel
# RUN: llvm-readobj -s -section-data %t-rel | FileCheck %s

# When linker generates a relocatable object it should keep "merge"
# sections as-is: do not merge content, do not join regular and
# "merge" sections, do not joint "merge" sections with different
# entry size.

# CHECK:      Section {
# CHECK:        Index:
# CHECK:        Name: .data
# CHECK-NEXT:   Type: SHT_PROGBITS
# CHECK-NEXT:   Flags [
# CHECK-NEXT:     SHF_ALLOC
# CHECK-NEXT:     SHF_MERGE
# CHECK-NEXT:   ]
# CHECK-NEXT:   Address:
# CHECK-NEXT:   Offset:
# CHECK-NEXT:   Size: 12
# CHECK-NEXT:   Link: 0
# CHECK-NEXT:   Info: 0
# CHECK-NEXT:   AddressAlignment: 4
# CHECK-NEXT:   EntrySize: 4
# CHECK-NEXT:   SectionData (
# CHECK-NEXT:     0000: 42000000 42000000 42000000
# CHECK-NEXT:   )
# CHECK-NEXT: }
# CHECK:      Section {
# CHECK:        Index:
# CHECK:        Name: .data
# CHECK-NEXT:   Type: SHT_PROGBITS
# CHECK-NEXT:   Flags [
# CHECK-NEXT:     SHF_ALLOC
# CHECK-NEXT:     SHF_MERGE
# CHECK-NEXT:   ]
# CHECK-NEXT:   Address:
# CHECK-NEXT:   Offset:
# CHECK-NEXT:   Size: 16
# CHECK-NEXT:   Link: 0
# CHECK-NEXT:   Info: 0
# CHECK-NEXT:   AddressAlignment: 8
# CHECK-NEXT:   EntrySize: 8
# CHECK-NEXT:   SectionData (
# CHECK-NEXT:     0000: 42000000 42000000 42000000 42000000
# CHECK-NEXT:   )
# CHECK-NEXT: }
# CHECK:      Section {
# CHECK:        Index:
# CHECK:        Name: .data
# CHECK-NEXT:   Type: SHT_PROGBITS
# CHECK-NEXT:   Flags [
# CHECK-NEXT:     SHF_ALLOC
# CHECK-NEXT:     SHF_WRITE
# CHECK-NEXT:   ]
# CHECK-NEXT:   Address:
# CHECK-NEXT:   Offset:
# CHECK-NEXT:   Size: 16
# CHECK-NEXT:   Link: 0
# CHECK-NEXT:   Info: 0
# CHECK-NEXT:   AddressAlignment: 1
# CHECK-NEXT:   EntrySize: 0
# CHECK-NEXT:   SectionData (
# CHECK-NEXT:     0000: 42000000 42000000 42000000 42000000
# CHECK-NEXT:   )
# CHECK-NEXT: }

        .section        .data.1,"aM",@progbits,4
        .align  4
        .global foo
foo:
        .long   0x42
        .long   0x42
        .long   0x42

        .section        .data.2,"aM",@progbits,8
        .align  8
        .global bar
bar:
        .long   0x42
        .long   0x42
        .long   0x42
        .long   0x42

        .data
        .global gar
zed:
        .long   0x42
        .long   0x42
        .long   0x42
        .long   0x42
