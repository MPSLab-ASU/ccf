; ModuleID = 'add.c'
source_filename = "add.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-none-linux-eabi"

@.str = private unnamed_addr constant [33 x i8] c"iterations:%d\09, c:%d, result:%d\0A\00", align 1

; Function Attrs: nofree nounwind
define dso_local i32 @main(i32 %0, i8** nocapture readonly %1) local_unnamed_addr #0 !dbg !14 {
  call void @llvm.dbg.value(metadata i32 %0, metadata !18, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i8** %1, metadata !19, metadata !DIExpression()), !dbg !26
  %3 = getelementptr inbounds i8*, i8** %1, i32 1, !dbg !27
  %4 = load i8*, i8** %3, align 4, !dbg !27, !tbaa !28
  call void @llvm.dbg.value(metadata i8* %4, metadata !32, metadata !DIExpression()) #4, !dbg !40
  %5 = tail call i32 @strtol(i8* nocapture nonnull %4, i8** null, i32 10) #4, !dbg !42
  call void @llvm.dbg.value(metadata i32 %5, metadata !20, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 30, metadata !21, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 3, metadata !22, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 0, metadata !23, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 0, metadata !24, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 0, metadata !25, metadata !DIExpression()), !dbg !26
  %6 = icmp sgt i32 %5, 0, !dbg !43
  br i1 %6, label %7, label %15, !dbg !46

7:                                                ; preds = %2, %7
  %8 = phi i32 [ %13, %7 ], [ 0, %2 ]
  %9 = phi i32 [ %12, %7 ], [ 0, %2 ]
  call void @llvm.dbg.value(metadata i32 %8, metadata !25, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 %9, metadata !24, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 %8, metadata !23, metadata !DIExpression()), !dbg !26
  %10 = icmp ult i32 %8, 5, !dbg !47
  %11 = add nsw i32 %9, 33
  %12 = select i1 %10, i32 %11, i32 %9, !dbg !50
  call void @llvm.dbg.value(metadata i32 %12, metadata !24, metadata !DIExpression()), !dbg !26
  %13 = add nuw nsw i32 %8, 1, !dbg !51
  call void @llvm.dbg.value(metadata i32 %13, metadata !23, metadata !DIExpression()), !dbg !26
  call void @llvm.dbg.value(metadata i32 %13, metadata !25, metadata !DIExpression()), !dbg !26
  %14 = icmp eq i32 %13, %5, !dbg !43
  br i1 %14, label %15, label %7, !dbg !46, !llvm.loop !52

15:                                               ; preds = %7, %2
  %16 = phi i32 [ 0, %2 ], [ %5, %7 ], !dbg !26
  %17 = phi i32 [ 0, %2 ], [ %12, %7 ], !dbg !26
  %18 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @.str, i32 0, i32 0), i32 %5, i32 %17, i32 %16), !dbg !57
  ret i32 0, !dbg !58
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: nofree nounwind willreturn
declare dso_local i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #2

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #3

attributes #0 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "min-legal-vector-width"="0" "no-jump-tables"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #1 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #2 = { nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #3 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!9, !10, !11, !12}
!llvm.ident = !{!13}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 13.0.0 (https://github.com/MPSLab-ASU/CCF-20.04/ 69d7c3299a1e6f5d4e7173a82d1ebc6a79ac1477)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "add.c", directory: "/home/local/ASUAD/quoclon1/ccf2_git/CCF-20.04/benchmarks/Micro-benchmarks/simple_loop_1")
!2 = !{}
!3 = !{!4, !5, !8}
!4 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!5 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !6, size: 32)
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 32)
!7 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!8 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!9 = !{i32 7, !"Dwarf Version", i32 4}
!10 = !{i32 2, !"Debug Info Version", i32 3}
!11 = !{i32 1, !"wchar_size", i32 4}
!12 = !{i32 1, !"min_enum_size", i32 4}
!13 = !{!"clang version 13.0.0 (https://github.com/MPSLab-ASU/CCF-20.04/ 69d7c3299a1e6f5d4e7173a82d1ebc6a79ac1477)"}
!14 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 4, type: !15, scopeLine: 5, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !17)
!15 = !DISubroutineType(types: !16)
!16 = !{!4, !4, !5}
!17 = !{!18, !19, !20, !21, !22, !23, !24, !25}
!18 = !DILocalVariable(name: "argc", arg: 1, scope: !14, file: !1, line: 4, type: !4)
!19 = !DILocalVariable(name: "argv", arg: 2, scope: !14, file: !1, line: 4, type: !5)
!20 = !DILocalVariable(name: "iterations", scope: !14, file: !1, line: 7, type: !4)
!21 = !DILocalVariable(name: "a", scope: !14, file: !1, line: 9, type: !4)
!22 = !DILocalVariable(name: "b", scope: !14, file: !1, line: 9, type: !4)
!23 = !DILocalVariable(name: "result", scope: !14, file: !1, line: 9, type: !4)
!24 = !DILocalVariable(name: "c", scope: !14, file: !1, line: 10, type: !4)
!25 = !DILocalVariable(name: "i", scope: !14, file: !1, line: 13, type: !4)
!26 = !DILocation(line: 0, scope: !14)
!27 = !DILocation(line: 7, column: 25, scope: !14)
!28 = !{!29, !29, i64 0}
!29 = !{!"any pointer", !30, i64 0}
!30 = !{!"omnipotent char", !31, i64 0}
!31 = !{!"Simple C/C++ TBAA"}
!32 = !DILocalVariable(name: "__nptr", arg: 1, scope: !33, file: !34, line: 361, type: !37)
!33 = distinct !DISubprogram(name: "atoi", scope: !34, file: !34, line: 361, type: !35, scopeLine: 362, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !39)
!34 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdlib.h", directory: "")
!35 = !DISubroutineType(types: !36)
!36 = !{!4, !37}
!37 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !38, size: 32)
!38 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !7)
!39 = !{!32}
!40 = !DILocation(line: 0, scope: !33, inlinedAt: !41)
!41 = distinct !DILocation(line: 7, column: 20, scope: !14)
!42 = !DILocation(line: 363, column: 16, scope: !33, inlinedAt: !41)
!43 = !DILocation(line: 16, column: 12, scope: !44)
!44 = distinct !DILexicalBlock(scope: !45, file: !1, line: 16, column: 3)
!45 = distinct !DILexicalBlock(scope: !14, file: !1, line: 16, column: 3)
!46 = !DILocation(line: 16, column: 3, scope: !45)
!47 = !DILocation(line: 18, column: 9, scope: !48)
!48 = distinct !DILexicalBlock(scope: !49, file: !1, line: 18, column: 8)
!49 = distinct !DILexicalBlock(scope: !44, file: !1, line: 17, column: 3)
!50 = !DILocation(line: 18, column: 8, scope: !49)
!51 = !DILocation(line: 16, column: 26, scope: !44)
!52 = distinct !{!52, !46, !53, !54, !55, !56}
!53 = !DILocation(line: 21, column: 3, scope: !45)
!54 = !{!"llvm.loop.mustprogress"}
!55 = !{!"llvm.loop.CGRA.enable"}
!56 = !{!"llvm.loop.unroll.disable"}
!57 = !DILocation(line: 24, column: 3, scope: !14)
!58 = !DILocation(line: 27, column: 1, scope: !14)
