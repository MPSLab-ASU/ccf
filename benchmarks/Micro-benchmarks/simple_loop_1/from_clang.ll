; ModuleID = '/home/local/ASUAD/mbalasu2/CCF-20.04/scripts/CGRALib/cgra.c'
source_filename = "/home/local/ASUAD/mbalasu2/CCF-20.04/scripts/CGRALib/cgra.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-none-linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque

@totalLoops = dso_local global i32 0, align 4, !dbg !0
@dynamicTCVal = dso_local local_unnamed_addr global i32 0, align 4, !dbg !16
@initCGRA = dso_local local_unnamed_addr global i32* null, align 4, !dbg !36
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [16 x i8] c"/epilog_ins.bin\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"/prolog_ins.bin\00", align 1
@.str.4 = private unnamed_addr constant [16 x i8] c"/kernel_ins.bin\00", align 1
@.str.5 = private unnamed_addr constant [17 x i8] c"/CGRA_config.txt\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.7 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.8 = private unnamed_addr constant [32 x i8] c"\0A**********EPISIZE %d*********\0A\00", align 1
@.str.9 = private unnamed_addr constant [32 x i8] c"\0A**********PROSIZE %d*********\0A\00", align 1
@.str.10 = private unnamed_addr constant [33 x i8] c"\0A**********KERNSIZE %d*********\0A\00", align 1
@.str.11 = private unnamed_addr constant [37 x i8] c"\0A******SIZE OF UNSIGNED LONG%d*****\0A\00", align 1
@.str.12 = private unnamed_addr constant [42 x i8] c"\0A******SIZE OF UNSIGNED LONG LONG%d*****\0A\00", align 1
@epilog = dso_local local_unnamed_addr global i32* null, align 4, !dbg !22
@prolog = dso_local local_unnamed_addr global i32* null, align 4, !dbg !19
@kernel = dso_local local_unnamed_addr global i32* null, align 4, !dbg !24
@.str.13 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@.str.15 = private unnamed_addr constant [39 x i8] c"\0A************XDIM and YDim are %d, %d\0A\00", align 1
@.str.16 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.17 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.18 = private unnamed_addr constant [55 x i8] c"From FILE: PROLOGPC= %lx, EPILOGPC=%lx,  KernelPC=%lx\0A\00", align 1
@prologPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !30
@epilogPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !34
@kernelPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !32
@__const.configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
@.str.20 = private unnamed_addr constant [14 x i8] c"\0Aloopno = %s\0A\00", align 1
@__const.configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.21 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.22 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.23 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.24 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@__const.checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.26 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@.str.30 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@ArrPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !26
@pth = dso_local local_unnamed_addr global i32 0, align 4, !dbg !38
@str = private unnamed_addr constant [34 x i8] c"from cgra.c Initialize Parameters\00", align 1
@str.34 = private unnamed_addr constant [52 x i8] c"\0A*******************before while loop**************\00", align 1
@str.35 = private unnamed_addr constant [14 x i8] c"configureCGRA\00", align 1
@str.36 = private unnamed_addr constant [16 x i8] c"checkTotalLoops\00", align 1
@str.37 = private unnamed_addr constant [12 x i8] c"\0A\0ArunOnCGRA\00", align 1
@str.38 = private unnamed_addr constant [19 x i8] c"\0A\0AaccelerateOnCGRA\00", align 1
@str.39 = private unnamed_addr constant [15 x i8] c"\0Adeleting cgra\00", align 1
@str.40 = private unnamed_addr constant [11 x i8] c"createCGRA\00", align 1
@str.41 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00", align 1

; Function Attrs: nounwind
define dso_local i32 @initializeParameters(i32 %0) local_unnamed_addr #0 !dbg !47 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  %5 = alloca [40 x i8], align 1
  %6 = alloca [40 x i8], align 1
  %7 = alloca [40 x i8], align 1
  %8 = alloca [40 x i8], align 1
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca [256 x i8], align 1
  %14 = alloca [40 x i8], align 1
  %15 = alloca i32, align 4
  call void @llvm.dbg.value(metadata i32 %0, metadata !52, metadata !DIExpression()), !dbg !148
  %16 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([34 x i8], [34 x i8]* @str, i32 0, i32 0)), !dbg !149
  call void @llvm.dbg.value(metadata i32 0, metadata !53, metadata !DIExpression()), !dbg !148
  %17 = load i32*, i32** @initCGRA, align 4, !dbg !151, !tbaa !152
  %18 = add i32 %0, -1, !dbg !156
  %19 = mul i32 %18, 7, !dbg !157
  %20 = getelementptr inbounds i32, i32* %17, i32 %19, !dbg !158
  store i32 2011168768, i32* %20, align 4, !dbg !159, !tbaa !160
  %21 = getelementptr inbounds i32, i32* %20, i32 1, !dbg !162
  store i32 2011168768, i32* %21, align 4, !dbg !163, !tbaa !160
  %22 = getelementptr inbounds i32, i32* %20, i32 2, !dbg !164
  %23 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0, !dbg !165
  %24 = bitcast i32* %22 to i8*, !dbg !165
  call void @llvm.memset.p0i8.i64(i8* nonnull align 4 dereferenceable(20) %24, i8 0, i64 20, i1 false), !dbg !166
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %23) #12, !dbg !165
  call void @llvm.dbg.declare(metadata [25 x i8]* %2, metadata !119, metadata !DIExpression()), !dbg !167
  %25 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0, !dbg !168
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %25) #12, !dbg !168
  call void @llvm.dbg.declare(metadata [20 x i8]* %3, metadata !123, metadata !DIExpression()), !dbg !169
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %25, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false), !dbg !169
  %26 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %23, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #12, !dbg !170
  %27 = call i8* @strcat(i8* nonnull %25, i8* nonnull %23) #12, !dbg !171
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0, !dbg !172
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %28) #12, !dbg !172
  call void @llvm.dbg.declare(metadata [40 x i8]* %4, metadata !127, metadata !DIExpression()), !dbg !173
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %28, i8 0, i32 40, i1 false), !dbg !173
  %29 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0, !dbg !174
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %29) #12, !dbg !174
  call void @llvm.dbg.declare(metadata [40 x i8]* %5, metadata !128, metadata !DIExpression()), !dbg !175
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %29, i8 0, i32 40, i1 false), !dbg !175
  %30 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0, !dbg !176
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %30) #12, !dbg !176
  call void @llvm.dbg.declare(metadata [40 x i8]* %6, metadata !129, metadata !DIExpression()), !dbg !177
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %30, i8 0, i32 40, i1 false), !dbg !177
  %31 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0, !dbg !178
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %31) #12, !dbg !178
  call void @llvm.dbg.declare(metadata [40 x i8]* %7, metadata !130, metadata !DIExpression()), !dbg !179
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %31, i8 0, i32 40, i1 false), !dbg !179
  %32 = getelementptr inbounds [40 x i8], [40 x i8]* %8, i32 0, i32 0, !dbg !180
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %32) #12, !dbg !180
  call void @llvm.dbg.declare(metadata [40 x i8]* %8, metadata !131, metadata !DIExpression()), !dbg !181
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %32, i8 0, i32 40, i1 false), !dbg !181
  %33 = call i8* @strcat(i8* nonnull %28, i8* nonnull %25) #12, !dbg !182
  %34 = call i32 @strlen(i8* nonnull %28), !dbg !183
  %35 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %34, !dbg !183
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %35, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 16, i1 false), !dbg !183
  %36 = call i8* @strcat(i8* nonnull %29, i8* nonnull %25) #12, !dbg !184
  %37 = call i32 @strlen(i8* nonnull %29), !dbg !185
  %38 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %37, !dbg !185
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %38, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 16, i1 false), !dbg !185
  %39 = call i8* @strcat(i8* nonnull %30, i8* nonnull %25) #12, !dbg !186
  %40 = call i32 @strlen(i8* nonnull %30), !dbg !187
  %41 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %40, !dbg !187
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %41, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.4, i32 0, i32 0), i32 16, i1 false), !dbg !187
  %42 = call i8* @strcat(i8* nonnull %32, i8* nonnull %25) #12, !dbg !188
  %43 = call i32 @strlen(i8* nonnull %32), !dbg !189
  %44 = getelementptr [40 x i8], [40 x i8]* %8, i32 0, i32 %43, !dbg !189
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(17) %44, i8* nonnull align 1 dereferenceable(17) getelementptr inbounds ([17 x i8], [17 x i8]* @.str.5, i32 0, i32 0), i32 17, i1 false), !dbg !189
  %45 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0)), !dbg !190
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %45, metadata !54, metadata !DIExpression()), !dbg !148
  %46 = call %struct._IO_FILE* @fopen(i8* nonnull %29, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0)), !dbg !191
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %46, metadata !115, metadata !DIExpression()), !dbg !148
  %47 = call %struct._IO_FILE* @fopen(i8* nonnull %30, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0)), !dbg !192
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %47, metadata !116, metadata !DIExpression()), !dbg !148
  %48 = call %struct._IO_FILE* @fopen(i8* nonnull %32, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !193
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %48, metadata !118, metadata !DIExpression()), !dbg !148
  %49 = bitcast i32* %9 to i8*, !dbg !194
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %49) #12, !dbg !194
  %50 = bitcast i32* %10 to i8*, !dbg !194
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %50) #12, !dbg !194
  %51 = bitcast i32* %11 to i8*, !dbg !194
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %51) #12, !dbg !194
  %52 = bitcast i32* %12 to i8*, !dbg !194
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %52) #12, !dbg !194
  call void @llvm.dbg.value(metadata i32* %9, metadata !132, metadata !DIExpression(DW_OP_deref)), !dbg !148
  %53 = call i32 @fread(i8* nonnull %49, i32 4, i32 1, %struct._IO_FILE* %45), !dbg !195
  call void @llvm.dbg.value(metadata i32* %10, metadata !133, metadata !DIExpression(DW_OP_deref)), !dbg !148
  %54 = call i32 @fread(i8* nonnull %50, i32 4, i32 1, %struct._IO_FILE* %46), !dbg !196
  call void @llvm.dbg.value(metadata i32* %11, metadata !134, metadata !DIExpression(DW_OP_deref)), !dbg !148
  %55 = call i32 @fread(i8* nonnull %51, i32 4, i32 1, %struct._IO_FILE* %47), !dbg !197
  %56 = load i32, i32* %9, align 4, !dbg !198, !tbaa !160
  call void @llvm.dbg.value(metadata i32 %56, metadata !132, metadata !DIExpression()), !dbg !148
  %57 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.8, i32 0, i32 0), i32 %56), !dbg !199
  %58 = load i32, i32* %10, align 4, !dbg !200, !tbaa !160
  call void @llvm.dbg.value(metadata i32 %58, metadata !133, metadata !DIExpression()), !dbg !148
  %59 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.9, i32 0, i32 0), i32 %58), !dbg !201
  %60 = load i32, i32* %11, align 4, !dbg !202, !tbaa !160
  call void @llvm.dbg.value(metadata i32 %60, metadata !134, metadata !DIExpression()), !dbg !148
  %61 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @.str.10, i32 0, i32 0), i32 %60), !dbg !203
  %62 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([37 x i8], [37 x i8]* @.str.11, i32 0, i32 0), i32 4), !dbg !204
  %63 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([42 x i8], [42 x i8]* @.str.12, i32 0, i32 0), i32 8), !dbg !205
  call void @llvm.dbg.value(metadata i32 %56, metadata !132, metadata !DIExpression()), !dbg !148
  %64 = shl i32 %56, 3, !dbg !206
  %65 = call noalias i8* @malloc(i32 %64) #12, !dbg !207
  store i8* %65, i8** bitcast (i32** @epilog to i8**), align 4, !dbg !208, !tbaa !152
  call void @llvm.dbg.value(metadata i32 %58, metadata !133, metadata !DIExpression()), !dbg !148
  %66 = shl i32 %58, 3, !dbg !209
  %67 = call noalias i8* @malloc(i32 %66) #12, !dbg !210
  store i8* %67, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !211, !tbaa !152
  call void @llvm.dbg.value(metadata i32 %60, metadata !134, metadata !DIExpression()), !dbg !148
  %68 = shl i32 %60, 3, !dbg !212
  %69 = call noalias i8* @malloc(i32 %68) #12, !dbg !213
  store i8* %69, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !214, !tbaa !152
  call void @llvm.dbg.value(metadata i32 %56, metadata !132, metadata !DIExpression()), !dbg !148
  %70 = call i32 @fread(i8* %65, i32 8, i32 %56, %struct._IO_FILE* %45), !dbg !215
  %71 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !216, !tbaa !152
  call void @llvm.dbg.value(metadata i32 %58, metadata !133, metadata !DIExpression()), !dbg !148
  %72 = call i32 @fread(i8* %71, i32 8, i32 %58, %struct._IO_FILE* %46), !dbg !217
  %73 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !218, !tbaa !152
  call void @llvm.dbg.value(metadata i32 %60, metadata !134, metadata !DIExpression()), !dbg !148
  %74 = call i32 @fread(i8* %73, i32 8, i32 %60, %struct._IO_FILE* %47), !dbg !219
  %75 = call i8* @strcat(i8* nonnull %31, i8* nonnull %25) #12, !dbg !220
  %76 = call i32 @strlen(i8* nonnull %31), !dbg !221
  %77 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %76, !dbg !221
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(26) %77, i8* nonnull align 1 dereferenceable(26) getelementptr inbounds ([26 x i8], [26 x i8]* @.str.13, i32 0, i32 0), i32 26, i1 false), !dbg !221
  %78 = call %struct._IO_FILE* @fopen(i8* nonnull %31, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !222
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %78, metadata !117, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32* %12, metadata !135, metadata !DIExpression(DW_OP_deref)), !dbg !148
  %79 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %78, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %12) #12, !dbg !223
  %80 = getelementptr inbounds [256 x i8], [256 x i8]* %13, i32 0, i32 0, !dbg !224
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %80) #12, !dbg !224
  call void @llvm.dbg.declare(metadata [256 x i8]* %13, metadata !136, metadata !DIExpression()), !dbg !225
  call void @llvm.dbg.value(metadata i32 0, metadata !140, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 0, metadata !141, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 0, metadata !142, metadata !DIExpression()), !dbg !148
  %81 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([52 x i8], [52 x i8]* @str.34, i32 0, i32 0)), !dbg !226
  call void @llvm.dbg.value(metadata i32 0, metadata !140, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 0, metadata !141, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 0, metadata !142, metadata !DIExpression()), !dbg !148
  %82 = call i8* @fgets(i8* nonnull %80, i32 256, %struct._IO_FILE* %48), !dbg !227
  %83 = icmp eq i8* %82, null, !dbg !228
  br i1 %83, label %101, label %84, !dbg !228

84:                                               ; preds = %1, %96
  %85 = phi i32 [ %98, %96 ], [ 0, %1 ]
  %86 = phi i32 [ %97, %96 ], [ 0, %1 ]
  %87 = phi i32 [ %88, %96 ], [ 0, %1 ]
  call void @llvm.dbg.value(metadata i32 %85, metadata !140, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 %86, metadata !141, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 %87, metadata !142, metadata !DIExpression()), !dbg !148
  %88 = add nuw nsw i32 %87, 1, !dbg !229
  call void @llvm.dbg.value(metadata i32 %88, metadata !142, metadata !DIExpression()), !dbg !148
  %89 = icmp eq i32 %87, 0, !dbg !231
  br i1 %89, label %90, label %92, !dbg !233

90:                                               ; preds = %84
  call void @llvm.dbg.value(metadata i8* %80, metadata !234, metadata !DIExpression()) #12, !dbg !242
  %91 = call i32 @strtol(i8* nocapture nonnull %80, i8** null, i32 10) #12, !dbg !244
  call void @llvm.dbg.value(metadata i32 %91, metadata !140, metadata !DIExpression()), !dbg !148
  br label %96, !dbg !245

92:                                               ; preds = %84
  %93 = icmp eq i32 %88, 2, !dbg !246
  br i1 %93, label %94, label %101, !dbg !248

94:                                               ; preds = %92
  call void @llvm.dbg.value(metadata i8* %80, metadata !234, metadata !DIExpression()) #12, !dbg !249
  %95 = call i32 @strtol(i8* nocapture nonnull %80, i8** null, i32 10) #12, !dbg !251
  call void @llvm.dbg.value(metadata i32 %95, metadata !141, metadata !DIExpression()), !dbg !148
  br label %96

96:                                               ; preds = %94, %90
  %97 = phi i32 [ %86, %90 ], [ %95, %94 ], !dbg !148
  %98 = phi i32 [ %91, %90 ], [ %85, %94 ], !dbg !148
  call void @llvm.dbg.value(metadata i32 %98, metadata !140, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 %97, metadata !141, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 %88, metadata !142, metadata !DIExpression()), !dbg !148
  %99 = call i8* @fgets(i8* nonnull %80, i32 256, %struct._IO_FILE* %48), !dbg !227
  %100 = icmp eq i8* %99, null, !dbg !228
  br i1 %100, label %101, label %84, !dbg !228, !llvm.loop !252

101:                                              ; preds = %96, %92, %1
  %102 = phi i32 [ 0, %1 ], [ %86, %92 ], [ %97, %96 ], !dbg !256
  %103 = phi i32 [ 0, %1 ], [ %85, %92 ], [ %98, %96 ], !dbg !148
  %104 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([39 x i8], [39 x i8]* @.str.15, i32 0, i32 0), i32 %103, i32 %102), !dbg !257
  call void @llvm.dbg.value(metadata i32 %60, metadata !134, metadata !DIExpression()), !dbg !148
  %105 = mul nsw i32 %103, %102, !dbg !258
  %106 = sdiv i32 %60, %105, !dbg !259
  call void @llvm.dbg.value(metadata i32 %106, metadata !143, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 %56, metadata !132, metadata !DIExpression()), !dbg !148
  %107 = sdiv i32 %56, %105, !dbg !260
  call void @llvm.dbg.value(metadata i32 %107, metadata !144, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32 %58, metadata !133, metadata !DIExpression()), !dbg !148
  %108 = sdiv i32 %58, %105, !dbg !261
  call void @llvm.dbg.value(metadata i32 %108, metadata !145, metadata !DIExpression()), !dbg !148
  %109 = load i32*, i32** @initCGRA, align 4, !dbg !262, !tbaa !152
  %110 = getelementptr inbounds i32, i32* %109, i32 %19, !dbg !263
  %111 = getelementptr inbounds i32, i32* %110, i32 2, !dbg !264
  store i32 %106, i32* %111, align 4, !dbg !265, !tbaa !160
  %112 = getelementptr inbounds i32, i32* %110, i32 3, !dbg !266
  store i32 %107, i32* %112, align 4, !dbg !267, !tbaa !160
  %113 = getelementptr inbounds i32, i32* %110, i32 4, !dbg !268
  store i32 %108, i32* %113, align 4, !dbg !269, !tbaa !160
  %114 = load i32, i32* %12, align 4, !dbg !270, !tbaa !160
  call void @llvm.dbg.value(metadata i32 %114, metadata !135, metadata !DIExpression()), !dbg !148
  %115 = sdiv i32 %114, %105, !dbg !271
  %116 = getelementptr inbounds i32, i32* %110, i32 6, !dbg !272
  store i32 %115, i32* %116, align 4, !dbg !273, !tbaa !160
  %117 = call i32 @fclose(%struct._IO_FILE* %45), !dbg !274
  %118 = call i32 @fclose(%struct._IO_FILE* %46), !dbg !275
  %119 = call i32 @fclose(%struct._IO_FILE* %47), !dbg !276
  %120 = call i32 @fclose(%struct._IO_FILE* %78), !dbg !277
  %121 = call i32 @fclose(%struct._IO_FILE* %48), !dbg !278
  %122 = getelementptr inbounds [40 x i8], [40 x i8]* %14, i32 0, i32 0, !dbg !279
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %122) #12, !dbg !279
  call void @llvm.dbg.declare(metadata [40 x i8]* %14, metadata !146, metadata !DIExpression()), !dbg !280
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %122, i8 0, i32 40, i1 false), !dbg !280
  %123 = call i8* @strcat(i8* nonnull %122, i8* nonnull %25) #12, !dbg !281
  %124 = call i32 @strlen(i8* nonnull %122), !dbg !282
  %125 = getelementptr [40 x i8], [40 x i8]* %14, i32 0, i32 %124, !dbg !282
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(18) %125, i8* nonnull align 1 dereferenceable(18) getelementptr inbounds ([18 x i8], [18 x i8]* @.str.16, i32 0, i32 0), i32 18, i1 false), !dbg !282
  %126 = bitcast i32* %15 to i8*, !dbg !283
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %126) #12, !dbg !283
  call void @llvm.dbg.value(metadata i32 0, metadata !147, metadata !DIExpression()), !dbg !148
  store i32 0, i32* %15, align 4, !dbg !284, !tbaa !160
  %127 = call %struct._IO_FILE* @fopen(i8* nonnull %122, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !285
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %127, metadata !117, metadata !DIExpression()), !dbg !148
  call void @llvm.dbg.value(metadata i32* %15, metadata !147, metadata !DIExpression(DW_OP_deref)), !dbg !148
  %128 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %127, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %15) #12, !dbg !286
  %129 = call i32 @fclose(%struct._IO_FILE* %127), !dbg !287
  %130 = load i32, i32* %15, align 4, !dbg !288, !tbaa !160
  call void @llvm.dbg.value(metadata i32 %130, metadata !147, metadata !DIExpression()), !dbg !148
  %131 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.17, i32 0, i32 0), i32 %130), !dbg !289
  %132 = load i32, i32* %15, align 4, !dbg !290, !tbaa !160
  call void @llvm.dbg.value(metadata i32 %132, metadata !147, metadata !DIExpression()), !dbg !148
  %133 = load i32*, i32** @initCGRA, align 4, !dbg !291, !tbaa !152
  %134 = getelementptr inbounds i32, i32* %133, i32 %19, !dbg !292
  %135 = getelementptr inbounds i32, i32* %134, i32 5, !dbg !293
  store i32 %132, i32* %135, align 4, !dbg !294, !tbaa !160
  %136 = load i32*, i32** @prolog, align 4, !dbg !295, !tbaa !152
  %137 = ptrtoint i32* %136 to i32, !dbg !296
  %138 = load i32*, i32** @epilog, align 4, !dbg !297, !tbaa !152
  %139 = ptrtoint i32* %138 to i32, !dbg !298
  %140 = load i32*, i32** @kernel, align 4, !dbg !299, !tbaa !152
  %141 = ptrtoint i32* %140 to i32, !dbg !300
  %142 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([55 x i8], [55 x i8]* @.str.18, i32 0, i32 0), i32 %137, i32 %139, i32 %141), !dbg !301
  %143 = load i32*, i32** @prolog, align 4, !dbg !302, !tbaa !152
  %144 = ptrtoint i32* %143 to i32, !dbg !303
  %145 = load i32*, i32** @prologPtr, align 4, !dbg !304, !tbaa !152
  %146 = getelementptr inbounds i32, i32* %145, i32 %18, !dbg !304
  store i32 %144, i32* %146, align 4, !dbg !305, !tbaa !306
  %147 = load i32*, i32** @epilog, align 4, !dbg !308, !tbaa !152
  %148 = ptrtoint i32* %147 to i32, !dbg !309
  %149 = load i32*, i32** @epilogPtr, align 4, !dbg !310, !tbaa !152
  %150 = getelementptr inbounds i32, i32* %149, i32 %18, !dbg !310
  store i32 %148, i32* %150, align 4, !dbg !311, !tbaa !306
  %151 = load i32*, i32** @kernel, align 4, !dbg !312, !tbaa !152
  %152 = ptrtoint i32* %151 to i32, !dbg !313
  %153 = load i32*, i32** @kernelPtr, align 4, !dbg !314, !tbaa !152
  %154 = getelementptr inbounds i32, i32* %153, i32 %18, !dbg !314
  store i32 %152, i32* %154, align 4, !dbg !315, !tbaa !306
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %126) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %122) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %80) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %52) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %51) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %50) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %49) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %32) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %31) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %30) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %29) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %28) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %25) #12, !dbg !316
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %23) #12, !dbg !316
  ret i32 0, !dbg !317
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #3

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i32, i1 immarg) #3

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @sprintf(i8* noalias nocapture noundef writeonly, i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nounwind willreturn
declare dso_local i8* @strcat(i8* noalias returned writeonly, i8* noalias nocapture readonly) local_unnamed_addr #4

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i1 immarg) #5

; Function Attrs: nofree nounwind
declare dso_local noalias noundef %struct._IO_FILE* @fopen(i8* nocapture noundef readonly, i8* nocapture noundef readonly) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fread(i8* nocapture noundef, i32 noundef, i32 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: inaccessiblememonly nofree nounwind willreturn
declare dso_local noalias noundef i8* @malloc(i32) local_unnamed_addr #6

declare dso_local i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #7

; Function Attrs: nofree nounwind
declare !dbg !318 dso_local noundef i8* @fgets(i8* noundef, i32 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare !dbg !322 dso_local noundef i32 @fclose(%struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #3

; Function Attrs: nofree nounwind
define dso_local i32 @configureCGRA(i32 %0) local_unnamed_addr #8 !dbg !325 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  call void @llvm.dbg.value(metadata i32 %0, metadata !327, metadata !DIExpression()), !dbg !337
  %5 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([14 x i8], [14 x i8]* @str.35, i32 0, i32 0)), !dbg !338
  call void @llvm.dbg.value(metadata i32 0, metadata !328, metadata !DIExpression()), !dbg !337
  %6 = load i32*, i32** @initCGRA, align 4, !dbg !340, !tbaa !152
  %7 = add i32 %0, -1, !dbg !341
  %8 = mul i32 %7, 7, !dbg !342
  %9 = getelementptr inbounds i32, i32* %6, i32 %8, !dbg !343
  %10 = getelementptr inbounds i32, i32* %9, i32 5, !dbg !344
  %11 = load i32, i32* %10, align 4, !dbg !345, !tbaa !160
  call void @llvm.dbg.value(metadata i32 %11, metadata !329, metadata !DIExpression()), !dbg !337
  %12 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0, !dbg !346
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %12) #12, !dbg !346
  call void @llvm.dbg.declare(metadata [25 x i8]* %2, metadata !331, metadata !DIExpression()), !dbg !347
  %13 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0, !dbg !348
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %13) #12, !dbg !348
  call void @llvm.dbg.declare(metadata [20 x i8]* %3, metadata !332, metadata !DIExpression()), !dbg !349
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %13, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false), !dbg !349
  %14 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %12, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #12, !dbg !350
  %15 = call i8* @strcat(i8* nonnull %13, i8* nonnull %12) #12, !dbg !351
  %16 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([14 x i8], [14 x i8]* @.str.20, i32 0, i32 0), i8* nonnull %12), !dbg !352
  %17 = icmp slt i32 %11, 1, !dbg !354
  br i1 %17, label %18, label %24, !dbg !355

18:                                               ; preds = %1
  %19 = load i32, i32* @dynamicTCVal, align 4, !dbg !356, !tbaa !160
  %20 = add nsw i32 %19, %11, !dbg !357
  call void @llvm.dbg.value(metadata i32 %20, metadata !333, metadata !DIExpression()), !dbg !358
  %21 = load i32*, i32** @initCGRA, align 4, !dbg !359, !tbaa !152
  %22 = getelementptr inbounds i32, i32* %21, i32 %8, !dbg !360
  %23 = getelementptr inbounds i32, i32* %22, i32 5, !dbg !361
  store i32 %20, i32* %23, align 4, !dbg !362, !tbaa !160
  br label %24, !dbg !363

24:                                               ; preds = %18, %1
  %25 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0, !dbg !364
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %25) #12, !dbg !364
  call void @llvm.dbg.declare(metadata [40 x i8]* %4, metadata !336, metadata !DIExpression()), !dbg !365
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %25, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i1 false), !dbg !365
  %26 = call i32 @strlen(i8* nonnull %25), !dbg !366
  %27 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %26, !dbg !366
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(14) %27, i8* nonnull align 1 dereferenceable(14) getelementptr inbounds ([14 x i8], [14 x i8]* @.str.21, i32 0, i32 0), i32 14, i1 false), !dbg !366
  %28 = call %struct._IO_FILE* @fopen(i8* nonnull %25, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.22, i32 0, i32 0)), !dbg !367
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %28, metadata !330, metadata !DIExpression()), !dbg !337
  call void @llvm.dbg.value(metadata i32 0, metadata !328, metadata !DIExpression()), !dbg !337
  br label %29, !dbg !368

29:                                               ; preds = %24, %29
  %30 = phi i32 [ 0, %24 ], [ %36, %29 ]
  call void @llvm.dbg.value(metadata i32 %30, metadata !328, metadata !DIExpression()), !dbg !337
  %31 = load i32*, i32** @initCGRA, align 4, !dbg !370, !tbaa !152
  %32 = getelementptr inbounds i32, i32* %31, i32 %8, !dbg !372
  %33 = getelementptr inbounds i32, i32* %32, i32 %30, !dbg !373
  %34 = load i32, i32* %33, align 4, !dbg !374, !tbaa !160
  %35 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %28, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.23, i32 0, i32 0), i32 %34), !dbg !375
  %36 = add nuw nsw i32 %30, 1, !dbg !376
  call void @llvm.dbg.value(metadata i32 %36, metadata !328, metadata !DIExpression()), !dbg !337
  %37 = icmp eq i32 %36, 7, !dbg !377
  br i1 %37, label %38, label %29, !dbg !368, !llvm.loop !378

38:                                               ; preds = %29
  %39 = load i32*, i32** @epilogPtr, align 4, !dbg !380, !tbaa !152
  %40 = getelementptr inbounds i32, i32* %39, i32 %7, !dbg !380
  %41 = load i32, i32* %40, align 4, !dbg !380, !tbaa !306
  %42 = zext i32 %41 to i64, !dbg !381
  %43 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %28, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.24, i32 0, i32 0), i64 %42), !dbg !382
  %44 = load i32*, i32** @prologPtr, align 4, !dbg !383, !tbaa !152
  %45 = getelementptr inbounds i32, i32* %44, i32 %7, !dbg !383
  %46 = load i32, i32* %45, align 4, !dbg !383, !tbaa !306
  %47 = zext i32 %46 to i64, !dbg !384
  %48 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %28, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.24, i32 0, i32 0), i64 %47), !dbg !385
  %49 = load i32*, i32** @kernelPtr, align 4, !dbg !386, !tbaa !152
  %50 = getelementptr inbounds i32, i32* %49, i32 %7, !dbg !386
  %51 = load i32, i32* %50, align 4, !dbg !386, !tbaa !306
  %52 = zext i32 %51 to i64, !dbg !387
  %53 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %28, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.24, i32 0, i32 0), i64 %52), !dbg !388
  %54 = call i32 @fclose(%struct._IO_FILE* %28), !dbg !389
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %25) #12, !dbg !390
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %13) #12, !dbg !390
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %12) #12, !dbg !390
  ret i32 0, !dbg !391
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fprintf(%struct._IO_FILE* nocapture noundef, i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @checkTotalLoops() local_unnamed_addr #0 !dbg !392 {
  %1 = alloca [40 x i8], align 1
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([16 x i8], [16 x i8]* @str.36, i32 0, i32 0)), !dbg !398
  %3 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0, !dbg !400
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %3) #12, !dbg !400
  call void @llvm.dbg.declare(metadata [40 x i8]* %1, metadata !396, metadata !DIExpression()), !dbg !401
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %3, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false), !dbg !401
  %4 = call %struct._IO_FILE* @fopen(i8* nonnull %3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !402
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %4, metadata !397, metadata !DIExpression()), !dbg !403
  %5 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.26, i32 0, i32 0), i32* nonnull @totalLoops) #12, !dbg !404
  %6 = tail call i32 @fclose(%struct._IO_FILE* %4), !dbg !405
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %3) #12, !dbg !406
  ret void, !dbg !406
}

; Function Attrs: nounwind
define dso_local i8* @runOnCGRA() local_unnamed_addr #0 !dbg !407 {
  %1 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([12 x i8], [12 x i8]* @str.37, i32 0, i32 0)), !dbg !410
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #12, !dbg !411, !srcloc !412
  ret i8* null, !dbg !413
}

; Function Attrs: nounwind
define dso_local void @accelerateOnCGRA(i32 %0) local_unnamed_addr #0 !dbg !414 {
  call void @llvm.dbg.value(metadata i32 %0, metadata !418, metadata !DIExpression()), !dbg !421
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([19 x i8], [19 x i8]* @str.38, i32 0, i32 0)), !dbg !422
  call void @llvm.dbg.value(metadata i32 0, metadata !419, metadata !DIExpression()), !dbg !421
  %3 = tail call i32 @configureCGRA(i32 %0), !dbg !423
  call void @llvm.dbg.value(metadata i32 0, metadata !420, metadata !DIExpression()), !dbg !421
  %4 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @.str.30, i32 0, i32 0), i32 %0), !dbg !424
  %5 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([12 x i8], [12 x i8]* @str.37, i32 0, i32 0)) #12, !dbg !425
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #12, !dbg !427, !srcloc !412
  ret void, !dbg !428
}

; Function Attrs: nofree nounwind
define dso_local void @deleteCGRA() local_unnamed_addr #8 !dbg !429 {
  %1 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([15 x i8], [15 x i8]* @str.39, i32 0, i32 0)), !dbg !430
  ret void, !dbg !431
}

; Function Attrs: nounwind
define dso_local void @createCGRA() local_unnamed_addr #0 !dbg !432 {
  %1 = alloca [40 x i8], align 1
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([11 x i8], [11 x i8]* @str.40, i32 0, i32 0)), !dbg !436
  call void @llvm.dbg.value(metadata i32 0, metadata !434, metadata !DIExpression()), !dbg !438
  call void @llvm.dbg.value(metadata i32 1, metadata !435, metadata !DIExpression()), !dbg !438
  %3 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([16 x i8], [16 x i8]* @str.36, i32 0, i32 0)) #12, !dbg !439
  %4 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0, !dbg !441
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %4) #12, !dbg !441
  call void @llvm.dbg.declare(metadata [40 x i8]* %1, metadata !396, metadata !DIExpression()) #12, !dbg !442
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %4, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false) #12, !dbg !442
  %5 = call %struct._IO_FILE* @fopen(i8* nonnull %4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)) #12, !dbg !443
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %5, metadata !397, metadata !DIExpression()) #12, !dbg !444
  %6 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %5, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.26, i32 0, i32 0), i32* nonnull @totalLoops) #12, !dbg !445
  %7 = tail call i32 @fclose(%struct._IO_FILE* %5) #12, !dbg !446
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %4) #12, !dbg !447
  %8 = load i32, i32* @totalLoops, align 4, !dbg !448, !tbaa !160
  %9 = mul i32 %8, 28, !dbg !449
  %10 = tail call noalias i8* @malloc(i32 %9) #12, !dbg !450
  store i8* %10, i8** bitcast (i32** @initCGRA to i8**), align 4, !dbg !451, !tbaa !152
  %11 = shl i32 %8, 2, !dbg !452
  %12 = tail call noalias i8* @malloc(i32 %11) #12, !dbg !453
  store i8* %12, i8** bitcast (i32** @prologPtr to i8**), align 4, !dbg !454, !tbaa !152
  %13 = tail call noalias i8* @malloc(i32 %11) #12, !dbg !455
  store i8* %13, i8** bitcast (i32** @kernelPtr to i8**), align 4, !dbg !456, !tbaa !152
  %14 = tail call noalias i8* @malloc(i32 %11) #12, !dbg !457
  store i8* %14, i8** bitcast (i32** @epilogPtr to i8**), align 4, !dbg !458, !tbaa !152
  call void @llvm.dbg.value(metadata i32 1, metadata !435, metadata !DIExpression()), !dbg !438
  %15 = icmp eq i32 %8, 0, !dbg !459
  br i1 %15, label %22, label %16, !dbg !462

16:                                               ; preds = %0, %16
  %17 = phi i32 [ %19, %16 ], [ 1, %0 ]
  call void @llvm.dbg.value(metadata i32 %17, metadata !435, metadata !DIExpression()), !dbg !438
  %18 = tail call i32 @initializeParameters(i32 %17), !dbg !463
  %19 = add i32 %17, 1, !dbg !465
  call void @llvm.dbg.value(metadata i32 %19, metadata !435, metadata !DIExpression()), !dbg !438
  %20 = load i32, i32* @totalLoops, align 4, !dbg !466, !tbaa !160
  %21 = icmp ugt i32 %19, %20, !dbg !459
  br i1 %21, label %22, label %16, !dbg !462, !llvm.loop !467

22:                                               ; preds = %16, %0
  %23 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @str.41, i32 0, i32 0)), !dbg !469
  ret void, !dbg !470
}

; Function Attrs: nofree nounwind willreturn
declare dso_local i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #9

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #1

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #10

; Function Attrs: argmemonly nofree nounwind readonly willreturn
declare i32 @strlen(i8* nocapture) local_unnamed_addr #11

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #5

attributes #0 = { nounwind "disable-tail-calls"="false" "frame-pointer"="none" "min-legal-vector-width"="0" "no-jump-tables"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #3 = { argmemonly nofree nosync nounwind willreturn }
attributes #4 = { argmemonly nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #5 = { argmemonly nofree nosync nounwind willreturn writeonly }
attributes #6 = { inaccessiblememonly nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #7 = { "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #8 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "min-legal-vector-width"="0" "no-jump-tables"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #9 = { nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #10 = { nofree nounwind }
attributes #11 = { argmemonly nofree nounwind readonly willreturn }
attributes #12 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!42, !43, !44, !45}
!llvm.ident = !{!46}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "totalLoops", scope: !2, file: !18, line: 30, type: !29, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 13.0.0 (https://github.com/MPSLab-ASU/CCF-20.04/ 69d7c3299a1e6f5d4e7173a82d1ebc6a79ac1477)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !15, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/local/ASUAD/mbalasu2/CCF-20.04/scripts/CGRALib/cgra.c", directory: "/home/local/ASUAD/quoclon1/ccf2_git/CCF-20.04/benchmarks/Micro-benchmarks/simple_loop_1")
!4 = !{}
!5 = !{!6, !8, !7, !9, !10, !11, !12}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 32)
!7 = !DIBasicType(name: "long long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!8 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!10 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !11, size: 32)
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 32)
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 32)
!14 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!15 = !{!0, !16, !19, !22, !24, !26, !30, !32, !34, !36, !38}
!16 = !DIGlobalVariableExpression(var: !17, expr: !DIExpression())
!17 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !2, file: !18, line: 37, type: !11, isLocal: false, isDefinition: true)
!18 = !DIFile(filename: "mbalasu2/CCF-20.04/scripts/CGRALib/cgra.h", directory: "/home/local/ASUAD")
!19 = !DIGlobalVariableExpression(var: !20, expr: !DIExpression())
!20 = distinct !DIGlobalVariable(name: "prolog", scope: !2, file: !18, line: 24, type: !21, isLocal: false, isDefinition: true)
!21 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !8, size: 32)
!22 = !DIGlobalVariableExpression(var: !23, expr: !DIExpression())
!23 = distinct !DIGlobalVariable(name: "epilog", scope: !2, file: !18, line: 24, type: !21, isLocal: false, isDefinition: true)
!24 = !DIGlobalVariableExpression(var: !25, expr: !DIExpression())
!25 = distinct !DIGlobalVariable(name: "kernel", scope: !2, file: !18, line: 24, type: !21, isLocal: false, isDefinition: true)
!26 = !DIGlobalVariableExpression(var: !27, expr: !DIExpression())
!27 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !2, file: !18, line: 25, type: !28, isLocal: false, isDefinition: true)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !29, size: 32)
!29 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!30 = !DIGlobalVariableExpression(var: !31, expr: !DIExpression())
!31 = distinct !DIGlobalVariable(name: "prologPtr", scope: !2, file: !18, line: 27, type: !21, isLocal: false, isDefinition: true)
!32 = !DIGlobalVariableExpression(var: !33, expr: !DIExpression())
!33 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !2, file: !18, line: 27, type: !21, isLocal: false, isDefinition: true)
!34 = !DIGlobalVariableExpression(var: !35, expr: !DIExpression())
!35 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !2, file: !18, line: 27, type: !21, isLocal: false, isDefinition: true)
!36 = !DIGlobalVariableExpression(var: !37, expr: !DIExpression())
!37 = distinct !DIGlobalVariable(name: "initCGRA", scope: !2, file: !18, line: 29, type: !10, isLocal: false, isDefinition: true)
!38 = !DIGlobalVariableExpression(var: !39, expr: !DIExpression())
!39 = distinct !DIGlobalVariable(name: "pth", scope: !2, file: !18, line: 32, type: !40, isLocal: false, isDefinition: true)
!40 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !41, line: 27, baseType: !8)
!41 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "")
!42 = !{i32 7, !"Dwarf Version", i32 4}
!43 = !{i32 2, !"Debug Info Version", i32 3}
!44 = !{i32 1, !"wchar_size", i32 4}
!45 = !{i32 1, !"min_enum_size", i32 4}
!46 = !{!"clang version 13.0.0 (https://github.com/MPSLab-ASU/CCF-20.04/ 69d7c3299a1e6f5d4e7173a82d1ebc6a79ac1477)"}
!47 = distinct !DISubprogram(name: "initializeParameters", scope: !48, file: !48, line: 12, type: !49, scopeLine: 13, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !51)
!48 = !DIFile(filename: "mbalasu2/CCF-20.04/scripts/CGRALib/cgra.c", directory: "/home/local/ASUAD")
!49 = !DISubroutineType(types: !50)
!50 = !{!11, !29}
!51 = !{!52, !53, !54, !115, !116, !117, !118, !119, !123, !127, !128, !129, !130, !131, !132, !133, !134, !135, !136, !140, !141, !142, !143, !144, !145, !146, !147}
!52 = !DILocalVariable(name: "loopID", arg: 1, scope: !47, file: !48, line: 12, type: !29)
!53 = !DILocalVariable(name: "i", scope: !47, file: !48, line: 16, type: !29)
!54 = !DILocalVariable(name: "epi", scope: !47, file: !48, line: 26, type: !55)
!55 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !56, size: 32)
!56 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !57, line: 7, baseType: !58)
!57 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types/FILE.h", directory: "")
!58 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !59, line: 49, size: 1216, elements: !60)
!59 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types/struct_FILE.h", directory: "")
!60 = !{!61, !62, !63, !64, !65, !66, !67, !68, !69, !70, !71, !72, !73, !76, !78, !79, !80, !84, !86, !88, !92, !95, !99, !102, !105, !106, !107, !110, !111}
!61 = !DIDerivedType(tag: DW_TAG_member, name: "_flags", scope: !58, file: !59, line: 51, baseType: !11, size: 32)
!62 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_ptr", scope: !58, file: !59, line: 54, baseType: !13, size: 32, offset: 32)
!63 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_end", scope: !58, file: !59, line: 55, baseType: !13, size: 32, offset: 64)
!64 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_base", scope: !58, file: !59, line: 56, baseType: !13, size: 32, offset: 96)
!65 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_base", scope: !58, file: !59, line: 57, baseType: !13, size: 32, offset: 128)
!66 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_ptr", scope: !58, file: !59, line: 58, baseType: !13, size: 32, offset: 160)
!67 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_end", scope: !58, file: !59, line: 59, baseType: !13, size: 32, offset: 192)
!68 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_base", scope: !58, file: !59, line: 60, baseType: !13, size: 32, offset: 224)
!69 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_end", scope: !58, file: !59, line: 61, baseType: !13, size: 32, offset: 256)
!70 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_base", scope: !58, file: !59, line: 64, baseType: !13, size: 32, offset: 288)
!71 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_backup_base", scope: !58, file: !59, line: 65, baseType: !13, size: 32, offset: 320)
!72 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_end", scope: !58, file: !59, line: 66, baseType: !13, size: 32, offset: 352)
!73 = !DIDerivedType(tag: DW_TAG_member, name: "_markers", scope: !58, file: !59, line: 68, baseType: !74, size: 32, offset: 384)
!74 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !75, size: 32)
!75 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_marker", file: !59, line: 36, flags: DIFlagFwdDecl)
!76 = !DIDerivedType(tag: DW_TAG_member, name: "_chain", scope: !58, file: !59, line: 70, baseType: !77, size: 32, offset: 416)
!77 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !58, size: 32)
!78 = !DIDerivedType(tag: DW_TAG_member, name: "_fileno", scope: !58, file: !59, line: 72, baseType: !11, size: 32, offset: 448)
!79 = !DIDerivedType(tag: DW_TAG_member, name: "_flags2", scope: !58, file: !59, line: 73, baseType: !11, size: 32, offset: 480)
!80 = !DIDerivedType(tag: DW_TAG_member, name: "_old_offset", scope: !58, file: !59, line: 74, baseType: !81, size: 32, offset: 512)
!81 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off_t", file: !82, line: 152, baseType: !83)
!82 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "")
!83 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!84 = !DIDerivedType(tag: DW_TAG_member, name: "_cur_column", scope: !58, file: !59, line: 77, baseType: !85, size: 16, offset: 544)
!85 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!86 = !DIDerivedType(tag: DW_TAG_member, name: "_vtable_offset", scope: !58, file: !59, line: 78, baseType: !87, size: 8, offset: 560)
!87 = !DIBasicType(name: "signed char", size: 8, encoding: DW_ATE_signed_char)
!88 = !DIDerivedType(tag: DW_TAG_member, name: "_shortbuf", scope: !58, file: !59, line: 79, baseType: !89, size: 8, offset: 568)
!89 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 8, elements: !90)
!90 = !{!91}
!91 = !DISubrange(count: 1)
!92 = !DIDerivedType(tag: DW_TAG_member, name: "_lock", scope: !58, file: !59, line: 81, baseType: !93, size: 32, offset: 576)
!93 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !94, size: 32)
!94 = !DIDerivedType(tag: DW_TAG_typedef, name: "_IO_lock_t", file: !59, line: 43, baseType: null)
!95 = !DIDerivedType(tag: DW_TAG_member, name: "_offset", scope: !58, file: !59, line: 89, baseType: !96, size: 64, offset: 640)
!96 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off64_t", file: !82, line: 153, baseType: !97)
!97 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int64_t", file: !82, line: 47, baseType: !98)
!98 = !DIBasicType(name: "long long int", size: 64, encoding: DW_ATE_signed)
!99 = !DIDerivedType(tag: DW_TAG_member, name: "_codecvt", scope: !58, file: !59, line: 91, baseType: !100, size: 32, offset: 704)
!100 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !101, size: 32)
!101 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_codecvt", file: !59, line: 37, flags: DIFlagFwdDecl)
!102 = !DIDerivedType(tag: DW_TAG_member, name: "_wide_data", scope: !58, file: !59, line: 92, baseType: !103, size: 32, offset: 736)
!103 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !104, size: 32)
!104 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_wide_data", file: !59, line: 38, flags: DIFlagFwdDecl)
!105 = !DIDerivedType(tag: DW_TAG_member, name: "_freeres_list", scope: !58, file: !59, line: 93, baseType: !77, size: 32, offset: 768)
!106 = !DIDerivedType(tag: DW_TAG_member, name: "_freeres_buf", scope: !58, file: !59, line: 94, baseType: !9, size: 32, offset: 800)
!107 = !DIDerivedType(tag: DW_TAG_member, name: "__pad5", scope: !58, file: !59, line: 95, baseType: !108, size: 32, offset: 832)
!108 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !109, line: 46, baseType: !29)
!109 = !DIFile(filename: "sw/lib/clang/13.0.0/include/stddef.h", directory: "/home/local/ASUAD/quoclon1")
!110 = !DIDerivedType(tag: DW_TAG_member, name: "_mode", scope: !58, file: !59, line: 96, baseType: !11, size: 32, offset: 864)
!111 = !DIDerivedType(tag: DW_TAG_member, name: "_unused2", scope: !58, file: !59, line: 98, baseType: !112, size: 320, offset: 896)
!112 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 320, elements: !113)
!113 = !{!114}
!114 = !DISubrange(count: 40)
!115 = !DILocalVariable(name: "pro", scope: !47, file: !48, line: 26, type: !55)
!116 = !DILocalVariable(name: "kern", scope: !47, file: !48, line: 26, type: !55)
!117 = !DILocalVariable(name: "count", scope: !47, file: !48, line: 26, type: !55)
!118 = !DILocalVariable(name: "config", scope: !47, file: !48, line: 26, type: !55)
!119 = !DILocalVariable(name: "loopno", scope: !47, file: !48, line: 27, type: !120)
!120 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 200, elements: !121)
!121 = !{!122}
!122 = !DISubrange(count: 25)
!123 = !DILocalVariable(name: "directoryPath", scope: !47, file: !48, line: 28, type: !124)
!124 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 160, elements: !125)
!125 = !{!126}
!126 = !DISubrange(count: 20)
!127 = !DILocalVariable(name: "epifile", scope: !47, file: !48, line: 33, type: !112)
!128 = !DILocalVariable(name: "profile", scope: !47, file: !48, line: 34, type: !112)
!129 = !DILocalVariable(name: "kernfile", scope: !47, file: !48, line: 35, type: !112)
!130 = !DILocalVariable(name: "liveOutfile", scope: !47, file: !48, line: 36, type: !112)
!131 = !DILocalVariable(name: "configfile", scope: !47, file: !48, line: 37, type: !112)
!132 = !DILocalVariable(name: "episize", scope: !47, file: !48, line: 56, type: !11)
!133 = !DILocalVariable(name: "prosize", scope: !47, file: !48, line: 56, type: !11)
!134 = !DILocalVariable(name: "kernelsize", scope: !47, file: !48, line: 56, type: !11)
!135 = !DILocalVariable(name: "livevar_st_size", scope: !47, file: !48, line: 56, type: !11)
!136 = !DILocalVariable(name: "line", scope: !47, file: !48, line: 81, type: !137)
!137 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 2048, elements: !138)
!138 = !{!139}
!139 = !DISubrange(count: 256)
!140 = !DILocalVariable(name: "XDim", scope: !47, file: !48, line: 82, type: !11)
!141 = !DILocalVariable(name: "YDim", scope: !47, file: !48, line: 82, type: !11)
!142 = !DILocalVariable(name: "iter", scope: !47, file: !48, line: 83, type: !11)
!143 = !DILocalVariable(name: "II", scope: !47, file: !48, line: 97, type: !11)
!144 = !DILocalVariable(name: "epiLength", scope: !47, file: !48, line: 98, type: !11)
!145 = !DILocalVariable(name: "prolength", scope: !47, file: !48, line: 99, type: !11)
!146 = !DILocalVariable(name: "loopitfile", scope: !47, file: !48, line: 111, type: !112)
!147 = !DILocalVariable(name: "kernelCount", scope: !47, file: !48, line: 115, type: !11)
!148 = !DILocation(line: 0, scope: !47)
!149 = !DILocation(line: 15, column: 13, scope: !150)
!150 = distinct !DILexicalBlock(scope: !47, file: !48, line: 15, column: 6)
!151 = !DILocation(line: 18, column: 5, scope: !47)
!152 = !{!153, !153, i64 0}
!153 = !{!"any pointer", !154, i64 0}
!154 = !{!"omnipotent char", !155, i64 0}
!155 = !{!"Simple C/C++ TBAA"}
!156 = !DILocation(line: 18, column: 25, scope: !47)
!157 = !DILocation(line: 18, column: 17, scope: !47)
!158 = !DILocation(line: 18, column: 14, scope: !47)
!159 = !DILocation(line: 18, column: 30, scope: !47)
!160 = !{!161, !161, i64 0}
!161 = !{!"int", !154, i64 0}
!162 = !DILocation(line: 19, column: 29, scope: !47)
!163 = !DILocation(line: 19, column: 34, scope: !47)
!164 = !DILocation(line: 20, column: 29, scope: !47)
!165 = !DILocation(line: 27, column: 3, scope: !47)
!166 = !DILocation(line: 21, column: 34, scope: !47)
!167 = !DILocation(line: 27, column: 8, scope: !47)
!168 = !DILocation(line: 28, column: 3, scope: !47)
!169 = !DILocation(line: 28, column: 8, scope: !47)
!170 = !DILocation(line: 30, column: 3, scope: !47)
!171 = !DILocation(line: 31, column: 3, scope: !47)
!172 = !DILocation(line: 33, column: 3, scope: !47)
!173 = !DILocation(line: 33, column: 8, scope: !47)
!174 = !DILocation(line: 34, column: 3, scope: !47)
!175 = !DILocation(line: 34, column: 8, scope: !47)
!176 = !DILocation(line: 35, column: 3, scope: !47)
!177 = !DILocation(line: 35, column: 8, scope: !47)
!178 = !DILocation(line: 36, column: 3, scope: !47)
!179 = !DILocation(line: 36, column: 8, scope: !47)
!180 = !DILocation(line: 37, column: 3, scope: !47)
!181 = !DILocation(line: 37, column: 8, scope: !47)
!182 = !DILocation(line: 39, column: 3, scope: !47)
!183 = !DILocation(line: 40, column: 3, scope: !47)
!184 = !DILocation(line: 42, column: 3, scope: !47)
!185 = !DILocation(line: 43, column: 3, scope: !47)
!186 = !DILocation(line: 45, column: 3, scope: !47)
!187 = !DILocation(line: 46, column: 3, scope: !47)
!188 = !DILocation(line: 48, column: 3, scope: !47)
!189 = !DILocation(line: 49, column: 3, scope: !47)
!190 = !DILocation(line: 51, column: 7, scope: !47)
!191 = !DILocation(line: 52, column: 7, scope: !47)
!192 = !DILocation(line: 53, column: 8, scope: !47)
!193 = !DILocation(line: 54, column: 10, scope: !47)
!194 = !DILocation(line: 56, column: 3, scope: !47)
!195 = !DILocation(line: 58, column: 3, scope: !47)
!196 = !DILocation(line: 59, column: 3, scope: !47)
!197 = !DILocation(line: 60, column: 3, scope: !47)
!198 = !DILocation(line: 62, column: 46, scope: !47)
!199 = !DILocation(line: 62, column: 3, scope: !47)
!200 = !DILocation(line: 63, column: 46, scope: !47)
!201 = !DILocation(line: 63, column: 3, scope: !47)
!202 = !DILocation(line: 64, column: 47, scope: !47)
!203 = !DILocation(line: 64, column: 3, scope: !47)
!204 = !DILocation(line: 65, column: 3, scope: !47)
!205 = !DILocation(line: 66, column: 3, scope: !47)
!206 = !DILocation(line: 68, column: 45, scope: !47)
!207 = !DILocation(line: 68, column: 31, scope: !47)
!208 = !DILocation(line: 68, column: 9, scope: !47)
!209 = !DILocation(line: 69, column: 45, scope: !47)
!210 = !DILocation(line: 69, column: 31, scope: !47)
!211 = !DILocation(line: 69, column: 9, scope: !47)
!212 = !DILocation(line: 70, column: 48, scope: !47)
!213 = !DILocation(line: 70, column: 31, scope: !47)
!214 = !DILocation(line: 70, column: 9, scope: !47)
!215 = !DILocation(line: 72, column: 3, scope: !47)
!216 = !DILocation(line: 73, column: 9, scope: !47)
!217 = !DILocation(line: 73, column: 3, scope: !47)
!218 = !DILocation(line: 74, column: 9, scope: !47)
!219 = !DILocation(line: 74, column: 3, scope: !47)
!220 = !DILocation(line: 76, column: 3, scope: !47)
!221 = !DILocation(line: 77, column: 3, scope: !47)
!222 = !DILocation(line: 78, column: 9, scope: !47)
!223 = !DILocation(line: 79, column: 3, scope: !47)
!224 = !DILocation(line: 81, column: 3, scope: !47)
!225 = !DILocation(line: 81, column: 8, scope: !47)
!226 = !DILocation(line: 84, column: 3, scope: !47)
!227 = !DILocation(line: 85, column: 9, scope: !47)
!228 = !DILocation(line: 85, column: 3, scope: !47)
!229 = !DILocation(line: 87, column: 9, scope: !230)
!230 = distinct !DILexicalBlock(scope: !47, file: !48, line: 86, column: 3)
!231 = !DILocation(line: 88, column: 12, scope: !232)
!232 = distinct !DILexicalBlock(scope: !230, file: !48, line: 88, column: 8)
!233 = !DILocation(line: 88, column: 8, scope: !230)
!234 = !DILocalVariable(name: "__nptr", arg: 1, scope: !235, file: !236, line: 361, type: !239)
!235 = distinct !DISubprogram(name: "atoi", scope: !236, file: !236, line: 361, type: !237, scopeLine: 362, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !241)
!236 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdlib.h", directory: "")
!237 = !DISubroutineType(types: !238)
!238 = !{!11, !239}
!239 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !240, size: 32)
!240 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !14)
!241 = !{!234}
!242 = !DILocation(line: 0, scope: !235, inlinedAt: !243)
!243 = distinct !DILocation(line: 89, column: 14, scope: !232)
!244 = !DILocation(line: 363, column: 16, scope: !235, inlinedAt: !243)
!245 = !DILocation(line: 89, column: 7, scope: !232)
!246 = !DILocation(line: 90, column: 17, scope: !247)
!247 = distinct !DILexicalBlock(scope: !232, file: !48, line: 90, column: 13)
!248 = !DILocation(line: 90, column: 13, scope: !232)
!249 = !DILocation(line: 0, scope: !235, inlinedAt: !250)
!250 = distinct !DILocation(line: 91, column: 14, scope: !247)
!251 = !DILocation(line: 363, column: 16, scope: !235, inlinedAt: !250)
!252 = distinct !{!252, !228, !253, !254, !255}
!253 = !DILocation(line: 94, column: 3, scope: !47)
!254 = !{!"llvm.loop.mustprogress"}
!255 = !{!"llvm.loop.unroll.disable"}
!256 = !DILocation(line: 82, column: 15, scope: !47)
!257 = !DILocation(line: 96, column: 3, scope: !47)
!258 = !DILocation(line: 97, column: 28, scope: !47)
!259 = !DILocation(line: 97, column: 22, scope: !47)
!260 = !DILocation(line: 98, column: 26, scope: !47)
!261 = !DILocation(line: 99, column: 26, scope: !47)
!262 = !DILocation(line: 100, column: 5, scope: !47)
!263 = !DILocation(line: 100, column: 14, scope: !47)
!264 = !DILocation(line: 100, column: 29, scope: !47)
!265 = !DILocation(line: 100, column: 34, scope: !47)
!266 = !DILocation(line: 101, column: 29, scope: !47)
!267 = !DILocation(line: 101, column: 34, scope: !47)
!268 = !DILocation(line: 102, column: 29, scope: !47)
!269 = !DILocation(line: 102, column: 34, scope: !47)
!270 = !DILocation(line: 103, column: 36, scope: !47)
!271 = !DILocation(line: 103, column: 51, scope: !47)
!272 = !DILocation(line: 103, column: 29, scope: !47)
!273 = !DILocation(line: 103, column: 34, scope: !47)
!274 = !DILocation(line: 105, column: 3, scope: !47)
!275 = !DILocation(line: 106, column: 3, scope: !47)
!276 = !DILocation(line: 107, column: 3, scope: !47)
!277 = !DILocation(line: 108, column: 3, scope: !47)
!278 = !DILocation(line: 109, column: 3, scope: !47)
!279 = !DILocation(line: 111, column: 3, scope: !47)
!280 = !DILocation(line: 111, column: 8, scope: !47)
!281 = !DILocation(line: 112, column: 3, scope: !47)
!282 = !DILocation(line: 113, column: 3, scope: !47)
!283 = !DILocation(line: 115, column: 3, scope: !47)
!284 = !DILocation(line: 115, column: 7, scope: !47)
!285 = !DILocation(line: 116, column: 9, scope: !47)
!286 = !DILocation(line: 117, column: 3, scope: !47)
!287 = !DILocation(line: 118, column: 3, scope: !47)
!288 = !DILocation(line: 119, column: 29, scope: !47)
!289 = !DILocation(line: 119, column: 3, scope: !47)
!290 = !DILocation(line: 121, column: 36, scope: !47)
!291 = !DILocation(line: 121, column: 5, scope: !47)
!292 = !DILocation(line: 121, column: 14, scope: !47)
!293 = !DILocation(line: 121, column: 29, scope: !47)
!294 = !DILocation(line: 121, column: 34, scope: !47)
!295 = !DILocation(line: 123, column: 83, scope: !47)
!296 = !DILocation(line: 123, column: 68, scope: !47)
!297 = !DILocation(line: 123, column: 105, scope: !47)
!298 = !DILocation(line: 123, column: 90, scope: !47)
!299 = !DILocation(line: 123, column: 127, scope: !47)
!300 = !DILocation(line: 123, column: 112, scope: !47)
!301 = !DILocation(line: 123, column: 3, scope: !47)
!302 = !DILocation(line: 125, column: 46, scope: !47)
!303 = !DILocation(line: 125, column: 25, scope: !47)
!304 = !DILocation(line: 125, column: 3, scope: !47)
!305 = !DILocation(line: 125, column: 23, scope: !47)
!306 = !{!307, !307, i64 0}
!307 = !{!"long", !154, i64 0}
!308 = !DILocation(line: 126, column: 46, scope: !47)
!309 = !DILocation(line: 126, column: 25, scope: !47)
!310 = !DILocation(line: 126, column: 3, scope: !47)
!311 = !DILocation(line: 126, column: 23, scope: !47)
!312 = !DILocation(line: 127, column: 46, scope: !47)
!313 = !DILocation(line: 127, column: 25, scope: !47)
!314 = !DILocation(line: 127, column: 3, scope: !47)
!315 = !DILocation(line: 127, column: 23, scope: !47)
!316 = !DILocation(line: 131, column: 1, scope: !47)
!317 = !DILocation(line: 130, column: 3, scope: !47)
!318 = !DISubprogram(name: "fgets", scope: !319, file: !319, line: 564, type: !320, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!319 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdio.h", directory: "")
!320 = !DISubroutineType(types: !321)
!321 = !{!13, !13, !11, !77}
!322 = !DISubprogram(name: "fclose", scope: !319, file: !319, line: 213, type: !323, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!323 = !DISubroutineType(types: !324)
!324 = !{!11, !77}
!325 = distinct !DISubprogram(name: "configureCGRA", scope: !48, file: !48, line: 133, type: !49, scopeLine: 134, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !326)
!326 = !{!327, !328, !329, !330, !331, !332, !333, !336}
!327 = !DILocalVariable(name: "loopID", arg: 1, scope: !325, file: !48, line: 133, type: !29)
!328 = !DILocalVariable(name: "i", scope: !325, file: !48, line: 136, type: !29)
!329 = !DILocalVariable(name: "kernelCount", scope: !325, file: !48, line: 137, type: !11)
!330 = !DILocalVariable(name: "count", scope: !325, file: !48, line: 138, type: !55)
!331 = !DILocalVariable(name: "loopno", scope: !325, file: !48, line: 139, type: !120)
!332 = !DILocalVariable(name: "directoryPath", scope: !325, file: !48, line: 140, type: !124)
!333 = !DILocalVariable(name: "newTC", scope: !334, file: !48, line: 148, type: !11)
!334 = distinct !DILexicalBlock(scope: !335, file: !48, line: 147, column: 3)
!335 = distinct !DILexicalBlock(scope: !325, file: !48, line: 146, column: 6)
!336 = !DILocalVariable(name: "initCGRAfile", scope: !325, file: !48, line: 152, type: !112)
!337 = !DILocation(line: 0, scope: !325)
!338 = !DILocation(line: 135, column: 13, scope: !339)
!339 = distinct !DILexicalBlock(scope: !325, file: !48, line: 135, column: 6)
!340 = !DILocation(line: 137, column: 23, scope: !325)
!341 = !DILocation(line: 137, column: 43, scope: !325)
!342 = !DILocation(line: 137, column: 35, scope: !325)
!343 = !DILocation(line: 137, column: 32, scope: !325)
!344 = !DILocation(line: 137, column: 47, scope: !325)
!345 = !DILocation(line: 137, column: 21, scope: !325)
!346 = !DILocation(line: 139, column: 3, scope: !325)
!347 = !DILocation(line: 139, column: 8, scope: !325)
!348 = !DILocation(line: 140, column: 3, scope: !325)
!349 = !DILocation(line: 140, column: 8, scope: !325)
!350 = !DILocation(line: 142, column: 3, scope: !325)
!351 = !DILocation(line: 143, column: 3, scope: !325)
!352 = !DILocation(line: 144, column: 13, scope: !353)
!353 = distinct !DILexicalBlock(scope: !325, file: !48, line: 144, column: 6)
!354 = !DILocation(line: 146, column: 18, scope: !335)
!355 = !DILocation(line: 146, column: 6, scope: !325)
!356 = !DILocation(line: 148, column: 31, scope: !334)
!357 = !DILocation(line: 148, column: 29, scope: !334)
!358 = !DILocation(line: 0, scope: !334)
!359 = !DILocation(line: 149, column: 7, scope: !334)
!360 = !DILocation(line: 149, column: 16, scope: !334)
!361 = !DILocation(line: 149, column: 31, scope: !334)
!362 = !DILocation(line: 149, column: 36, scope: !334)
!363 = !DILocation(line: 150, column: 3, scope: !334)
!364 = !DILocation(line: 152, column: 3, scope: !325)
!365 = !DILocation(line: 152, column: 8, scope: !325)
!366 = !DILocation(line: 153, column: 3, scope: !325)
!367 = !DILocation(line: 154, column: 11, scope: !325)
!368 = !DILocation(line: 155, column: 3, scope: !369)
!369 = distinct !DILexicalBlock(scope: !325, file: !48, line: 155, column: 3)
!370 = !DILocation(line: 156, column: 30, scope: !371)
!371 = distinct !DILexicalBlock(scope: !369, file: !48, line: 155, column: 3)
!372 = !DILocation(line: 156, column: 39, scope: !371)
!373 = !DILocation(line: 156, column: 54, scope: !371)
!374 = !DILocation(line: 156, column: 28, scope: !371)
!375 = !DILocation(line: 156, column: 5, scope: !371)
!376 = !DILocation(line: 155, column: 18, scope: !371)
!377 = !DILocation(line: 155, column: 13, scope: !371)
!378 = distinct !{!378, !368, !379, !254, !255}
!379 = !DILocation(line: 156, column: 58, scope: !369)
!380 = !DILocation(line: 157, column: 48, scope: !325)
!381 = !DILocation(line: 157, column: 27, scope: !325)
!382 = !DILocation(line: 157, column: 3, scope: !325)
!383 = !DILocation(line: 158, column: 48, scope: !325)
!384 = !DILocation(line: 158, column: 27, scope: !325)
!385 = !DILocation(line: 158, column: 3, scope: !325)
!386 = !DILocation(line: 159, column: 48, scope: !325)
!387 = !DILocation(line: 159, column: 27, scope: !325)
!388 = !DILocation(line: 159, column: 3, scope: !325)
!389 = !DILocation(line: 160, column: 3, scope: !325)
!390 = !DILocation(line: 164, column: 1, scope: !325)
!391 = !DILocation(line: 163, column: 3, scope: !325)
!392 = distinct !DISubprogram(name: "checkTotalLoops", scope: !48, file: !48, line: 166, type: !393, scopeLine: 167, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !395)
!393 = !DISubroutineType(types: !394)
!394 = !{null}
!395 = !{!396, !397}
!396 = !DILocalVariable(name: "myfile", scope: !392, file: !48, line: 169, type: !112)
!397 = !DILocalVariable(name: "count", scope: !392, file: !48, line: 170, type: !55)
!398 = !DILocation(line: 168, column: 15, scope: !399)
!399 = distinct !DILexicalBlock(scope: !392, file: !48, line: 168, column: 8)
!400 = !DILocation(line: 169, column: 5, scope: !392)
!401 = !DILocation(line: 169, column: 10, scope: !392)
!402 = !DILocation(line: 171, column: 13, scope: !392)
!403 = !DILocation(line: 0, scope: !392)
!404 = !DILocation(line: 172, column: 5, scope: !392)
!405 = !DILocation(line: 173, column: 5, scope: !392)
!406 = !DILocation(line: 176, column: 1, scope: !392)
!407 = distinct !DISubprogram(name: "runOnCGRA", scope: !48, file: !48, line: 179, type: !408, scopeLine: 180, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!408 = !DISubroutineType(types: !409)
!409 = !{!9}
!410 = !DILocation(line: 181, column: 5, scope: !407)
!411 = !DILocation(line: 182, column: 5, scope: !407)
!412 = !{i32 5275}
!413 = !DILocation(line: 183, column: 5, scope: !407)
!414 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !48, file: !48, line: 186, type: !415, scopeLine: 187, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !417)
!415 = !DISubroutineType(types: !416)
!416 = !{null, !29}
!417 = !{!418, !419, !420}
!418 = !DILocalVariable(name: "loopNo", arg: 1, scope: !414, file: !48, line: 186, type: !29)
!419 = !DILocalVariable(name: "result", scope: !414, file: !48, line: 189, type: !11)
!420 = !DILocalVariable(name: "configure", scope: !414, file: !48, line: 191, type: !11)
!421 = !DILocation(line: 0, scope: !414)
!422 = !DILocation(line: 188, column: 5, scope: !414)
!423 = !DILocation(line: 191, column: 21, scope: !414)
!424 = !DILocation(line: 197, column: 5, scope: !414)
!425 = !DILocation(line: 181, column: 5, scope: !407, inlinedAt: !426)
!426 = distinct !DILocation(line: 202, column: 5, scope: !414)
!427 = !DILocation(line: 182, column: 5, scope: !407, inlinedAt: !426)
!428 = !DILocation(line: 208, column: 1, scope: !414)
!429 = distinct !DISubprogram(name: "deleteCGRA", scope: !48, file: !48, line: 210, type: !393, scopeLine: 211, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!430 = !DILocation(line: 212, column: 3, scope: !429)
!431 = !DILocation(line: 220, column: 1, scope: !429)
!432 = distinct !DISubprogram(name: "createCGRA", scope: !48, file: !48, line: 223, type: !393, scopeLine: 224, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !433)
!433 = !{!434, !435}
!434 = !DILocalVariable(name: "result", scope: !432, file: !48, line: 226, type: !11)
!435 = !DILocalVariable(name: "i", scope: !432, file: !48, line: 227, type: !29)
!436 = !DILocation(line: 225, column: 13, scope: !437)
!437 = distinct !DILexicalBlock(scope: !432, file: !48, line: 225, column: 6)
!438 = !DILocation(line: 0, scope: !432)
!439 = !DILocation(line: 168, column: 15, scope: !399, inlinedAt: !440)
!440 = distinct !DILocation(line: 229, column: 3, scope: !432)
!441 = !DILocation(line: 169, column: 5, scope: !392, inlinedAt: !440)
!442 = !DILocation(line: 169, column: 10, scope: !392, inlinedAt: !440)
!443 = !DILocation(line: 171, column: 13, scope: !392, inlinedAt: !440)
!444 = !DILocation(line: 0, scope: !392, inlinedAt: !440)
!445 = !DILocation(line: 172, column: 5, scope: !392, inlinedAt: !440)
!446 = !DILocation(line: 173, column: 5, scope: !392, inlinedAt: !440)
!447 = !DILocation(line: 176, column: 1, scope: !392, inlinedAt: !440)
!448 = !DILocation(line: 230, column: 43, scope: !432)
!449 = !DILocation(line: 230, column: 42, scope: !432)
!450 = !DILocation(line: 230, column: 22, scope: !432)
!451 = !DILocation(line: 230, column: 13, scope: !432)
!452 = !DILocation(line: 231, column: 40, scope: !432)
!453 = !DILocation(line: 231, column: 22, scope: !432)
!454 = !DILocation(line: 231, column: 13, scope: !432)
!455 = !DILocation(line: 232, column: 22, scope: !432)
!456 = !DILocation(line: 232, column: 13, scope: !432)
!457 = !DILocation(line: 233, column: 22, scope: !432)
!458 = !DILocation(line: 233, column: 13, scope: !432)
!459 = !DILocation(line: 239, column: 16, scope: !460)
!460 = distinct !DILexicalBlock(scope: !461, file: !48, line: 239, column: 3)
!461 = distinct !DILexicalBlock(scope: !432, file: !48, line: 239, column: 3)
!462 = !DILocation(line: 239, column: 3, scope: !461)
!463 = !DILocation(line: 241, column: 7, scope: !464)
!464 = distinct !DILexicalBlock(scope: !460, file: !48, line: 240, column: 3)
!465 = !DILocation(line: 239, column: 32, scope: !460)
!466 = !DILocation(line: 239, column: 19, scope: !460)
!467 = distinct !{!467, !462, !468, !254, !255}
!468 = !DILocation(line: 242, column: 3, scope: !461)
!469 = !DILocation(line: 245, column: 3, scope: !432)
!470 = !DILocation(line: 252, column: 1, scope: !432)
