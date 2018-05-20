; ModuleID = 'bstr_i.c'
source_filename = "bstr_i.c"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

@.str = private unnamed_addr constant [3 x i8] c"01\00", align 1

; Function Attrs: nounwind readonly
define i32 @bstr_i(i8* readonly) local_unnamed_addr #0 !dbg !8 {
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !15, metadata !18), !dbg !19
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !17, metadata !18), !dbg !20
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !17, metadata !18), !dbg !20
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !15, metadata !18), !dbg !19
  %2 = icmp eq i8* %0, null, !dbg !21
  br i1 %2, label %24, label %3, !dbg !22

; <label>:3:                                      ; preds = %1
  %4 = load i8, i8* %0, align 1, !dbg !23, !tbaa !24
  %5 = icmp eq i8 %4, 0, !dbg !23
  br i1 %5, label %24, label %6, !dbg !27

; <label>:6:                                      ; preds = %3
  br label %7, !dbg !23

; <label>:7:                                      ; preds = %6, %14
  %8 = phi i8 [ %20, %14 ], [ %4, %6 ]
  %9 = phi i8* [ %15, %14 ], [ %0, %6 ]
  %10 = phi i32 [ %19, %14 ], [ 0, %6 ]
  %11 = zext i8 %8 to i32, !dbg !23
  %12 = tail call i8* @memchr(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %11, i32 3), !dbg !28
  %13 = icmp eq i8* %12, null, !dbg !27
  br i1 %13, label %22, label %14, !dbg !29

; <label>:14:                                     ; preds = %7
  %15 = getelementptr inbounds i8, i8* %9, i32 1, !dbg !30
  tail call void @llvm.dbg.value(metadata i8* %15, i64 0, metadata !15, metadata !18), !dbg !19
  %16 = shl i32 %10, 1, !dbg !32
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !17, metadata !18), !dbg !20
  %17 = and i8 %8, 1, !dbg !33
  %18 = zext i8 %17 to i32, !dbg !33
  %19 = or i32 %16, %18, !dbg !34
  tail call void @llvm.dbg.value(metadata i32 %19, i64 0, metadata !17, metadata !18), !dbg !20
  tail call void @llvm.dbg.value(metadata i32 %19, i64 0, metadata !17, metadata !18), !dbg !20
  tail call void @llvm.dbg.value(metadata i8* %15, i64 0, metadata !15, metadata !18), !dbg !19
  %20 = load i8, i8* %15, align 1, !dbg !23, !tbaa !24
  %21 = icmp eq i8 %20, 0, !dbg !23
  br i1 %21, label %22, label %7, !dbg !27

; <label>:22:                                     ; preds = %14, %7
  %23 = phi i32 [ %19, %14 ], [ %10, %7 ]
  br label %24, !dbg !35

; <label>:24:                                     ; preds = %22, %3, %1
  %25 = phi i32 [ 0, %1 ], [ 0, %3 ], [ %23, %22 ]
  ret i32 %25, !dbg !35
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata, metadata) #1

; Function Attrs: nounwind readonly
declare i8* @memchr(i8*, i32, i32) #2

attributes #0 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { nounwind readonly }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "bstr_i.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount24")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 1, !"min_enum_size", i32 4}
!7 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!8 = distinct !DISubprogram(name: "bstr_i", scope: !1, file: !1, line: 12, type: !9, isLocal: false, isDefinition: true, scopeLine: 13, flags: DIFlagPrototyped, isOptimized: true, unit: !0, variables: !14)
!9 = !DISubroutineType(types: !10)
!10 = !{!11, !12}
!11 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 32)
!13 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!14 = !{!15, !16, !17}
!15 = !DILocalVariable(name: "cptr", arg: 1, scope: !8, file: !1, line: 12, type: !12)
!16 = !DILocalVariable(name: "i", scope: !8, file: !1, line: 14, type: !11)
!17 = !DILocalVariable(name: "j", scope: !8, file: !1, line: 14, type: !11)
!18 = !DIExpression()
!19 = !DILocation(line: 12, column: 27, scope: !8)
!20 = !DILocation(line: 14, column: 23, scope: !8)
!21 = !DILocation(line: 16, column: 14, scope: !8)
!22 = !DILocation(line: 16, column: 19, scope: !8)
!23 = !DILocation(line: 16, column: 22, scope: !8)
!24 = !{!25, !25, i64 0}
!25 = !{!"omnipotent char", !26, i64 0}
!26 = !{!"Simple C/C++ TBAA"}
!27 = !DILocation(line: 16, column: 28, scope: !8)
!28 = !DILocation(line: 16, column: 31, scope: !8)
!29 = !DILocation(line: 16, column: 7, scope: !8)
!30 = !DILocation(line: 18, column: 22, scope: !31)
!31 = distinct !DILexicalBlock(scope: !8, file: !1, line: 17, column: 7)
!32 = !DILocation(line: 19, column: 15, scope: !31)
!33 = !DILocation(line: 20, column: 21, scope: !31)
!34 = !DILocation(line: 20, column: 15, scope: !31)
!35 = !DILocation(line: 22, column: 7, scope: !8)
