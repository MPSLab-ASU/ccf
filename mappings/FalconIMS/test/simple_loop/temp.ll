; ModuleID = 'add.c'
source_filename = "add.c"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

@.str = private unnamed_addr constant [20 x i8] c"a[%d]=%d\09 b[%d]=%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"c[%d]=%d\0A\00", align 1

; Function Attrs: nounwind
define i32 @main() local_unnamed_addr #0 !dbg !8 {
  %1 = alloca [10000 x i32], align 4
  %2 = alloca [10000 x i32], align 4
  %3 = alloca [10000 x i32], align 4
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !13, metadata !20), !dbg !21
  %4 = bitcast [10000 x i32]* %1 to i8*, !dbg !22
  call void @llvm.lifetime.start(i64 40000, i8* nonnull %4) #4, !dbg !22
  tail call void @llvm.dbg.declare(metadata [10000 x i32]* %1, metadata !14, metadata !20), !dbg !23
  %5 = bitcast [10000 x i32]* %2 to i8*, !dbg !22
  call void @llvm.lifetime.start(i64 40000, i8* nonnull %5) #4, !dbg !22
  tail call void @llvm.dbg.declare(metadata [10000 x i32]* %2, metadata !18, metadata !20), !dbg !24
  %6 = bitcast [10000 x i32]* %3 to i8*, !dbg !22
  call void @llvm.lifetime.start(i64 40000, i8* nonnull %6) #4, !dbg !22
  tail call void @llvm.dbg.declare(metadata [10000 x i32]* %3, metadata !19, metadata !20), !dbg !25
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !13, metadata !20), !dbg !21
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !13, metadata !20), !dbg !21
  br label %7, !dbg !26

; <label>:7:                                      ; preds = %7, %0
  %8 = phi i32 [ 0, %0 ], [ %10, %7 ]
  %9 = getelementptr inbounds [10000 x i32], [10000 x i32]* %1, i32 0, i32 %8, !dbg !28
  store i32 %8, i32* %9, align 4, !dbg !31, !tbaa !32
  %10 = add nuw nsw i32 %8, 1, !dbg !36
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !13, metadata !20), !dbg !21
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !13, metadata !20), !dbg !21
  %11 = icmp eq i32 %10, 10000, !dbg !37
  br i1 %11, label %12, label %7, !dbg !26, !llvm.loop !38

; <label>:12:                                     ; preds = %7
  br label %13, !dbg !40

; <label>:13:                                     ; preds = %12, %13
  %14 = phi i32 [ %17, %13 ], [ 0, %12 ]
  %15 = add nuw nsw i32 %14, 2, !dbg !40
  %16 = getelementptr inbounds [10000 x i32], [10000 x i32]* %2, i32 0, i32 %14, !dbg !44
  store i32 %15, i32* %16, align 4, !dbg !45, !tbaa !32
  %17 = add nuw nsw i32 %14, 1, !dbg !46
  tail call void @llvm.dbg.value(metadata i32 %17, i64 0, metadata !13, metadata !20), !dbg !21
  tail call void @llvm.dbg.value(metadata i32 %17, i64 0, metadata !13, metadata !20), !dbg !21
  %18 = icmp eq i32 %17, 10000, !dbg !47
  br i1 %18, label %19, label %13, !dbg !48, !llvm.loop !49

; <label>:19:                                     ; preds = %13
  br label %20, !dbg !51

; <label>:20:                                     ; preds = %19, %20
  %21 = phi i32 [ %27, %20 ], [ 0, %19 ]
  %22 = getelementptr inbounds [10000 x i32], [10000 x i32]* %1, i32 0, i32 %21, !dbg !51
  %23 = load i32, i32* %22, align 4, !dbg !51, !tbaa !32
  %24 = getelementptr inbounds [10000 x i32], [10000 x i32]* %2, i32 0, i32 %21, !dbg !55
  %25 = load i32, i32* %24, align 4, !dbg !55, !tbaa !32
  %26 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i32 %21, i32 %23, i32 %21, i32 %25), !dbg !56
  %27 = add nuw nsw i32 %21, 1, !dbg !57
  tail call void @llvm.dbg.value(metadata i32 %27, i64 0, metadata !13, metadata !20), !dbg !21
  tail call void @llvm.dbg.value(metadata i32 %27, i64 0, metadata !13, metadata !20), !dbg !21
  %28 = icmp eq i32 %27, 10, !dbg !58
  br i1 %28, label %29, label %20, !dbg !59, !llvm.loop !60

; <label>:29:                                     ; preds = %20
  br label %30, !dbg !62

; <label>:30:                                     ; preds = %29, %30
  %31 = phi i32 [ %38, %30 ], [ 0, %29 ]
  %32 = getelementptr inbounds [10000 x i32], [10000 x i32]* %1, i32 0, i32 %31, !dbg !62
  %33 = load i32, i32* %32, align 4, !dbg !62, !tbaa !32
  %34 = getelementptr inbounds [10000 x i32], [10000 x i32]* %2, i32 0, i32 %31, !dbg !66
  %35 = load i32, i32* %34, align 4, !dbg !66, !tbaa !32
  %36 = add nsw i32 %35, %33, !dbg !67
  %37 = getelementptr inbounds [10000 x i32], [10000 x i32]* %3, i32 0, i32 %31, !dbg !68
  store i32 %36, i32* %37, align 4, !dbg !69, !tbaa !32
  %38 = add nuw nsw i32 %31, 1, !dbg !70
  tail call void @llvm.dbg.value(metadata i32 %38, i64 0, metadata !13, metadata !20), !dbg !21
  tail call void @llvm.dbg.value(metadata i32 %38, i64 0, metadata !13, metadata !20), !dbg !21
  %39 = icmp eq i32 %38, 10000, !dbg !71
  br i1 %39, label %40, label %30, !dbg !72, !llvm.loop !73

; <label>:40:                                     ; preds = %30
  br label %41, !dbg !76

; <label>:41:                                     ; preds = %40, %41
  %42 = phi i32 [ %46, %41 ], [ 0, %40 ]
  %43 = getelementptr inbounds [10000 x i32], [10000 x i32]* %3, i32 0, i32 %42, !dbg !76
  %44 = load i32, i32* %43, align 4, !dbg !76, !tbaa !32
  %45 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i32 %42, i32 %44), !dbg !80
  %46 = add nuw nsw i32 %42, 1, !dbg !81
  tail call void @llvm.dbg.value(metadata i32 %46, i64 0, metadata !13, metadata !20), !dbg !21
  tail call void @llvm.dbg.value(metadata i32 %46, i64 0, metadata !13, metadata !20), !dbg !21
  %47 = icmp eq i32 %46, 10, !dbg !82
  br i1 %47, label %48, label %41, !dbg !83, !llvm.loop !84

; <label>:48:                                     ; preds = %41
  call void @llvm.lifetime.end(i64 40000, i8* nonnull %6) #4, !dbg !86
  call void @llvm.lifetime.end(i64 40000, i8* nonnull %5) #4, !dbg !86
  call void @llvm.lifetime.end(i64 40000, i8* nonnull %4) #4, !dbg !86
  ret i32 0, !dbg !86
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #2

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata, metadata) #2

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind readnone }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "add.c", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 1, !"min_enum_size", i32 4}
!7 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!8 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 4, type: !9, isLocal: false, isDefinition: true, scopeLine: 5, isOptimized: true, unit: !0, variables: !12)
!9 = !DISubroutineType(types: !10)
!10 = !{!11}
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !{!13, !14, !18, !19}
!13 = !DILocalVariable(name: "i", scope: !8, file: !1, line: 7, type: !11)
!14 = !DILocalVariable(name: "a", scope: !8, file: !1, line: 9, type: !15)
!15 = !DICompositeType(tag: DW_TAG_array_type, baseType: !11, size: 320000, elements: !16)
!16 = !{!17}
!17 = !DISubrange(count: 10000)
!18 = !DILocalVariable(name: "b", scope: !8, file: !1, line: 9, type: !15)
!19 = !DILocalVariable(name: "c", scope: !8, file: !1, line: 9, type: !15)
!20 = !DIExpression()
!21 = !DILocation(line: 7, column: 5, scope: !8)
!22 = !DILocation(line: 9, column: 1, scope: !8)
!23 = !DILocation(line: 9, column: 5, scope: !8)
!24 = !DILocation(line: 9, column: 11, scope: !8)
!25 = !DILocation(line: 9, column: 17, scope: !8)
!26 = !DILocation(line: 12, column: 1, scope: !27)
!27 = distinct !DILexicalBlock(scope: !8, file: !1, line: 12, column: 1)
!28 = !DILocation(line: 14, column: 3, scope: !29)
!29 = distinct !DILexicalBlock(scope: !30, file: !1, line: 13, column: 1)
!30 = distinct !DILexicalBlock(scope: !27, file: !1, line: 12, column: 1)
!31 = !DILocation(line: 14, column: 8, scope: !29)
!32 = !{!33, !33, i64 0}
!33 = !{!"int", !34, i64 0}
!34 = !{!"omnipotent char", !35, i64 0}
!35 = !{!"Simple C/C++ TBAA"}
!36 = !DILocation(line: 12, column: 16, scope: !30)
!37 = !DILocation(line: 12, column: 11, scope: !30)
!38 = distinct !{!38, !26, !39}
!39 = !DILocation(line: 15, column: 1, scope: !27)
!40 = !DILocation(line: 19, column: 11, scope: !41)
!41 = distinct !DILexicalBlock(scope: !42, file: !1, line: 18, column: 1)
!42 = distinct !DILexicalBlock(scope: !43, file: !1, line: 17, column: 1)
!43 = distinct !DILexicalBlock(scope: !8, file: !1, line: 17, column: 1)
!44 = !DILocation(line: 19, column: 3, scope: !41)
!45 = !DILocation(line: 19, column: 8, scope: !41)
!46 = !DILocation(line: 17, column: 16, scope: !42)
!47 = !DILocation(line: 17, column: 11, scope: !42)
!48 = !DILocation(line: 17, column: 1, scope: !43)
!49 = distinct !{!49, !48, !50}
!50 = !DILocation(line: 20, column: 1, scope: !43)
!51 = !DILocation(line: 25, column: 36, scope: !52)
!52 = distinct !DILexicalBlock(scope: !53, file: !1, line: 24, column: 1)
!53 = distinct !DILexicalBlock(scope: !54, file: !1, line: 23, column: 1)
!54 = distinct !DILexicalBlock(scope: !8, file: !1, line: 23, column: 1)
!55 = !DILocation(line: 25, column: 43, scope: !52)
!56 = !DILocation(line: 25, column: 3, scope: !52)
!57 = !DILocation(line: 23, column: 17, scope: !53)
!58 = !DILocation(line: 23, column: 11, scope: !53)
!59 = !DILocation(line: 23, column: 1, scope: !54)
!60 = distinct !{!60, !59, !61}
!61 = !DILocation(line: 27, column: 1, scope: !54)
!62 = !DILocation(line: 32, column: 11, scope: !63)
!63 = distinct !DILexicalBlock(scope: !64, file: !1, line: 31, column: 1)
!64 = distinct !DILexicalBlock(scope: !65, file: !1, line: 30, column: 1)
!65 = distinct !DILexicalBlock(scope: !8, file: !1, line: 30, column: 1)
!66 = !DILocation(line: 32, column: 18, scope: !63)
!67 = !DILocation(line: 32, column: 16, scope: !63)
!68 = !DILocation(line: 32, column: 4, scope: !63)
!69 = !DILocation(line: 32, column: 9, scope: !63)
!70 = !DILocation(line: 30, column: 17, scope: !64)
!71 = !DILocation(line: 30, column: 12, scope: !64)
!72 = !DILocation(line: 30, column: 1, scope: !65)
!73 = distinct !{!73, !72, !74, !75}
!74 = !DILocation(line: 33, column: 1, scope: !65)
!75 = !{!"llvm.loop.CGRA.enable"}
!76 = !DILocation(line: 39, column: 25, scope: !77)
!77 = distinct !DILexicalBlock(scope: !78, file: !1, line: 38, column: 1)
!78 = distinct !DILexicalBlock(scope: !79, file: !1, line: 37, column: 1)
!79 = distinct !DILexicalBlock(scope: !8, file: !1, line: 37, column: 1)
!80 = !DILocation(line: 39, column: 3, scope: !77)
!81 = !DILocation(line: 37, column: 17, scope: !78)
!82 = !DILocation(line: 37, column: 11, scope: !78)
!83 = !DILocation(line: 37, column: 1, scope: !79)
!84 = distinct !{!84, !83, !85}
!85 = !DILocation(line: 40, column: 1, scope: !79)
!86 = !DILocation(line: 42, column: 1, scope: !8)
