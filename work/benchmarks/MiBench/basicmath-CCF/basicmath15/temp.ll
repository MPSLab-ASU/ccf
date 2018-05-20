; ModuleID = 'isqrt.c'
source_filename = "isqrt.c"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

%struct.int_sqrt = type { i32, i32 }

; Function Attrs: nounwind
define void @usqrt(i32, %struct.int_sqrt* nocapture) local_unnamed_addr #0 !dbg !8 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !20, metadata !27), !dbg !28
  tail call void @llvm.dbg.value(metadata %struct.int_sqrt* %1, i64 0, metadata !21, metadata !27), !dbg !29
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !22, metadata !27), !dbg !30
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !23, metadata !27), !dbg !31
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !24, metadata !27), !dbg !32
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !25, metadata !27), !dbg !33
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !25, metadata !27), !dbg !33
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !23, metadata !27), !dbg !31
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !22, metadata !27), !dbg !30
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !20, metadata !27), !dbg !28
  br label %3, !dbg !34

; <label>:3:                                      ; preds = %3, %2
  %4 = phi i32 [ 0, %2 ], [ %21, %3 ]
  %5 = phi i32 [ 0, %2 ], [ %20, %3 ]
  %6 = phi i32 [ 0, %2 ], [ %18, %3 ]
  %7 = phi i32 [ %0, %2 ], [ %11, %3 ]
  %8 = shl i32 %5, 2, !dbg !36
  %9 = lshr i32 %7, 30, !dbg !39
  %10 = or i32 %8, %9, !dbg !40
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !23, metadata !27), !dbg !31
  %11 = shl i32 %7, 2, !dbg !41
  tail call void @llvm.dbg.value(metadata i32 %11, i64 0, metadata !20, metadata !27), !dbg !28
  %12 = shl i32 %6, 1, !dbg !42
  tail call void @llvm.dbg.value(metadata i32 %12, i64 0, metadata !22, metadata !27), !dbg !30
  %13 = shl i32 %6, 2, !dbg !43
  %14 = or i32 %13, 1, !dbg !44
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !24, metadata !27), !dbg !32
  %15 = icmp ult i32 %10, %14, !dbg !45
  %16 = xor i1 %15, true, !dbg !47
  %17 = zext i1 %16 to i32, !dbg !47
  %18 = or i32 %12, %17, !dbg !47
  %19 = select i1 %15, i32 0, i32 %14, !dbg !47
  %20 = sub i32 %10, %19, !dbg !47
  tail call void @llvm.dbg.value(metadata i32 %20, i64 0, metadata !23, metadata !27), !dbg !31
  tail call void @llvm.dbg.value(metadata i32 %18, i64 0, metadata !22, metadata !27), !dbg !30
  %21 = add nuw nsw i32 %4, 1, !dbg !48
  tail call void @llvm.dbg.value(metadata i32 %21, i64 0, metadata !25, metadata !27), !dbg !33
  tail call void @llvm.dbg.value(metadata i32 %21, i64 0, metadata !25, metadata !27), !dbg !33
  tail call void @llvm.dbg.value(metadata i32 %20, i64 0, metadata !23, metadata !27), !dbg !31
  tail call void @llvm.dbg.value(metadata i32 %18, i64 0, metadata !22, metadata !27), !dbg !30
  tail call void @llvm.dbg.value(metadata i32 %11, i64 0, metadata !20, metadata !27), !dbg !28
  %22 = icmp eq i32 %21, 32, !dbg !49
  br i1 %22, label %23, label %3, !dbg !34, !llvm.loop !50

; <label>:23:                                     ; preds = %3
  %24 = getelementptr inbounds %struct.int_sqrt, %struct.int_sqrt* %1, i32 0, i32 0, !dbg !52
  store i32 %18, i32* %24, align 4, !dbg !52
  ret void, !dbg !53
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata, metadata) #1

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "isqrt.c", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath15")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 1, !"min_enum_size", i32 4}
!7 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!8 = distinct !DISubprogram(name: "usqrt", scope: !1, file: !1, line: 45, type: !9, isLocal: false, isDefinition: true, scopeLine: 46, flags: DIFlagPrototyped, isOptimized: true, unit: !0, variables: !19)
!9 = !DISubroutineType(types: !10)
!10 = !{null, !11, !12}
!11 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 32)
!13 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "int_sqrt", file: !14, line: 67, size: 64, elements: !15)
!14 = !DIFile(filename: "./snipmath.h", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath15")
!15 = !{!16, !18}
!16 = !DIDerivedType(tag: DW_TAG_member, name: "sqrt", scope: !13, file: !14, line: 68, baseType: !17, size: 32)
!17 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!18 = !DIDerivedType(tag: DW_TAG_member, name: "frac", scope: !13, file: !14, line: 69, baseType: !17, size: 32, offset: 32)
!19 = !{!20, !21, !22, !23, !24, !25}
!20 = !DILocalVariable(name: "x", arg: 1, scope: !8, file: !1, line: 45, type: !11)
!21 = !DILocalVariable(name: "q", arg: 2, scope: !8, file: !1, line: 45, type: !12)
!22 = !DILocalVariable(name: "a", scope: !8, file: !1, line: 47, type: !11)
!23 = !DILocalVariable(name: "r", scope: !8, file: !1, line: 48, type: !11)
!24 = !DILocalVariable(name: "e", scope: !8, file: !1, line: 49, type: !11)
!25 = !DILocalVariable(name: "i", scope: !8, file: !1, line: 51, type: !26)
!26 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!27 = !DIExpression()
!28 = !DILocation(line: 45, column: 26, scope: !8)
!29 = !DILocation(line: 45, column: 46, scope: !8)
!30 = !DILocation(line: 47, column: 21, scope: !8)
!31 = !DILocation(line: 48, column: 21, scope: !8)
!32 = !DILocation(line: 49, column: 21, scope: !8)
!33 = !DILocation(line: 51, column: 11, scope: !8)
!34 = !DILocation(line: 53, column: 7, scope: !35)
!35 = distinct !DILexicalBlock(scope: !8, file: !1, line: 53, column: 7)
!36 = !DILocation(line: 55, column: 20, scope: !37)
!37 = distinct !DILexicalBlock(scope: !38, file: !1, line: 54, column: 7)
!38 = distinct !DILexicalBlock(scope: !35, file: !1, line: 53, column: 7)
!39 = !DILocation(line: 55, column: 28, scope: !37)
!40 = !DILocation(line: 55, column: 26, scope: !37)
!41 = !DILocation(line: 55, column: 43, scope: !37)
!42 = !DILocation(line: 56, column: 15, scope: !37)
!43 = !DILocation(line: 57, column: 20, scope: !37)
!44 = !DILocation(line: 57, column: 26, scope: !37)
!45 = !DILocation(line: 58, column: 19, scope: !46)
!46 = distinct !DILexicalBlock(scope: !37, file: !1, line: 58, column: 17)
!47 = !DILocation(line: 58, column: 17, scope: !37)
!48 = !DILocation(line: 53, column: 37, scope: !38)
!49 = !DILocation(line: 53, column: 21, scope: !38)
!50 = distinct !{!50, !34, !51}
!51 = !DILocation(line: 63, column: 7, scope: !35)
!52 = !DILocation(line: 64, column: 7, scope: !8)
!53 = !DILocation(line: 65, column: 1, scope: !8)
