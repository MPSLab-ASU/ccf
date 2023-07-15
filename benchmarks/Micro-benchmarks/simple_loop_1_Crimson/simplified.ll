; ModuleID = 'simplified.bc'
source_filename = "/home/local/ASUAD/aabuyaz1/documents/code/ccf_1804/scripts/CGRALib/cgra.c"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-unknown-linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%union.pthread_attr_t = type { i32, [32 x i8] }

@totalLoops = dso_local global i32 0, align 4, !dbg !0
@thread_cond_cpu = dso_local local_unnamed_addr global i32 1, align 4, !dbg !16
@thread_cond_cgra = dso_local local_unnamed_addr global i32 0, align 4, !dbg !20
@thread_exit = dso_local local_unnamed_addr global i32 0, align 4, !dbg !22
@dynamicTCVal = dso_local local_unnamed_addr global i32 0, align 4, !dbg !24
@initCGRA = dso_local local_unnamed_addr global i32* null, align 4, !dbg !42
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
@epilog = dso_local local_unnamed_addr global i32* null, align 4, !dbg !29
@prolog = dso_local local_unnamed_addr global i32* null, align 4, !dbg !26
@kernel = dso_local local_unnamed_addr global i32* null, align 4, !dbg !31
@.str.13 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@.str.15 = private unnamed_addr constant [39 x i8] c"\0A************XDIM and YDim are %d, %d\0A\00", align 1
@.str.16 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.17 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.18 = private unnamed_addr constant [55 x i8] c"From FILE: PROLOGPC= %lx, EPILOGPC=%lx,  KernelPC=%lx\0A\00", align 1
@prologPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !36
@epilogPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !40
@kernelPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !38
@__const.configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
@__const.configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.20 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.21 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.22 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.23 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@__const.checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.25 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@.str.29 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@pth = dso_local global i32 0, align 4, !dbg !44
@ArrPtr = dso_local local_unnamed_addr global i32* null, align 4, !dbg !33
@str = private unnamed_addr constant [34 x i8] c"from cgra.c Initialize Parameters\00", align 1
@str.35 = private unnamed_addr constant [52 x i8] c"\0A*******************before while loop**************\00", align 1
@str.36 = private unnamed_addr constant [30 x i8] c"Entering cgra.c configureCGRA\00", align 1
@str.37 = private unnamed_addr constant [32 x i8] c"Entering cgra.c checkTotalLoops\00", align 1
@str.38 = private unnamed_addr constant [26 x i8] c"Entering cgra.c runOnCGRA\00", align 1
@str.39 = private unnamed_addr constant [33 x i8] c"Entering cgra.c accelerateOnCGRA\00", align 1
@str.40 = private unnamed_addr constant [14 x i8] c"deleting cgra\00", align 1
@str.41 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00", align 1
@str.42 = private unnamed_addr constant [26 x i8] c"Exiting cgra.c deleteCGRA\00", align 1
@str.43 = private unnamed_addr constant [27 x i8] c"Entering cgra.c createCGRA\00", align 1
@str.44 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00", align 1

; Function Attrs: nounwind
define dso_local i32 @initializeParameters(i32 %0) local_unnamed_addr #0 !dbg !53 {
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
  call void @llvm.dbg.value(metadata i32 %0, metadata !58, metadata !DIExpression()), !dbg !154
  %16 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([34 x i8], [34 x i8]* @str, i32 0, i32 0)), !dbg !155
  call void @llvm.dbg.value(metadata i32 0, metadata !59, metadata !DIExpression()), !dbg !154
  %17 = load i32*, i32** @initCGRA, align 4, !dbg !157, !tbaa !158
  %18 = add i32 %0, -1, !dbg !162
  %19 = mul i32 %18, 7, !dbg !163
  %20 = getelementptr inbounds i32, i32* %17, i32 %19, !dbg !164
  store i32 2011168768, i32* %20, align 4, !dbg !165, !tbaa !166
  %21 = getelementptr inbounds i32, i32* %20, i32 1, !dbg !168
  store i32 2011168768, i32* %21, align 4, !dbg !169, !tbaa !166
  %22 = getelementptr inbounds i32, i32* %20, i32 2, !dbg !170
  %23 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0, !dbg !171
  %24 = bitcast i32* %22 to i8*, !dbg !171
  call void @llvm.memset.p0i8.i64(i8* nonnull align 4 dereferenceable(20) %24, i8 0, i64 20, i1 false), !dbg !172
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %23) #13, !dbg !171
  call void @llvm.dbg.declare(metadata [25 x i8]* %2, metadata !125, metadata !DIExpression()), !dbg !173
  %25 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0, !dbg !174
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %25) #13, !dbg !174
  call void @llvm.dbg.declare(metadata [20 x i8]* %3, metadata !129, metadata !DIExpression()), !dbg !175
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %25, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false), !dbg !175
  %26 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %23, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #13, !dbg !176
  %27 = call i8* @strcat(i8* nonnull %25, i8* nonnull %23) #13, !dbg !177
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0, !dbg !178
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %28) #13, !dbg !178
  call void @llvm.dbg.declare(metadata [40 x i8]* %4, metadata !133, metadata !DIExpression()), !dbg !179
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %28, i8 0, i32 40, i1 false), !dbg !179
  %29 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0, !dbg !180
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %29) #13, !dbg !180
  call void @llvm.dbg.declare(metadata [40 x i8]* %5, metadata !134, metadata !DIExpression()), !dbg !181
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %29, i8 0, i32 40, i1 false), !dbg !181
  %30 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0, !dbg !182
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %30) #13, !dbg !182
  call void @llvm.dbg.declare(metadata [40 x i8]* %6, metadata !135, metadata !DIExpression()), !dbg !183
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %30, i8 0, i32 40, i1 false), !dbg !183
  %31 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0, !dbg !184
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %31) #13, !dbg !184
  call void @llvm.dbg.declare(metadata [40 x i8]* %7, metadata !136, metadata !DIExpression()), !dbg !185
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %31, i8 0, i32 40, i1 false), !dbg !185
  %32 = getelementptr inbounds [40 x i8], [40 x i8]* %8, i32 0, i32 0, !dbg !186
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %32) #13, !dbg !186
  call void @llvm.dbg.declare(metadata [40 x i8]* %8, metadata !137, metadata !DIExpression()), !dbg !187
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %32, i8 0, i32 40, i1 false), !dbg !187
  %33 = call i8* @strcat(i8* nonnull %28, i8* nonnull %25) #13, !dbg !188
  %34 = call i32 @strlen(i8* nonnull %28), !dbg !189
  %35 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %34, !dbg !189
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %35, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 16, i1 false), !dbg !189
  %36 = call i8* @strcat(i8* nonnull %29, i8* nonnull %25) #13, !dbg !190
  %37 = call i32 @strlen(i8* nonnull %29), !dbg !191
  %38 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %37, !dbg !191
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %38, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 16, i1 false), !dbg !191
  %39 = call i8* @strcat(i8* nonnull %30, i8* nonnull %25) #13, !dbg !192
  %40 = call i32 @strlen(i8* nonnull %30), !dbg !193
  %41 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %40, !dbg !193
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %41, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.4, i32 0, i32 0), i32 16, i1 false), !dbg !193
  %42 = call i8* @strcat(i8* nonnull %32, i8* nonnull %25) #13, !dbg !194
  %43 = call i32 @strlen(i8* nonnull %32), !dbg !195
  %44 = getelementptr [40 x i8], [40 x i8]* %8, i32 0, i32 %43, !dbg !195
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(17) %44, i8* nonnull align 1 dereferenceable(17) getelementptr inbounds ([17 x i8], [17 x i8]* @.str.5, i32 0, i32 0), i32 17, i1 false), !dbg !195
  %45 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0)), !dbg !196
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %45, metadata !60, metadata !DIExpression()), !dbg !154
  %46 = call %struct._IO_FILE* @fopen(i8* nonnull %29, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0)), !dbg !197
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %46, metadata !121, metadata !DIExpression()), !dbg !154
  %47 = call %struct._IO_FILE* @fopen(i8* nonnull %30, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0)), !dbg !198
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %47, metadata !122, metadata !DIExpression()), !dbg !154
  %48 = call %struct._IO_FILE* @fopen(i8* nonnull %32, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !199
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %48, metadata !124, metadata !DIExpression()), !dbg !154
  %49 = bitcast i32* %9 to i8*, !dbg !200
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %49) #13, !dbg !200
  %50 = bitcast i32* %10 to i8*, !dbg !200
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %50) #13, !dbg !200
  %51 = bitcast i32* %11 to i8*, !dbg !200
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %51) #13, !dbg !200
  %52 = bitcast i32* %12 to i8*, !dbg !200
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %52) #13, !dbg !200
  call void @llvm.dbg.value(metadata i32* %9, metadata !138, metadata !DIExpression(DW_OP_deref)), !dbg !154
  %53 = call i32 @fread(i8* nonnull %49, i32 4, i32 1, %struct._IO_FILE* %45), !dbg !201
  call void @llvm.dbg.value(metadata i32* %10, metadata !139, metadata !DIExpression(DW_OP_deref)), !dbg !154
  %54 = call i32 @fread(i8* nonnull %50, i32 4, i32 1, %struct._IO_FILE* %46), !dbg !202
  call void @llvm.dbg.value(metadata i32* %11, metadata !140, metadata !DIExpression(DW_OP_deref)), !dbg !154
  %55 = call i32 @fread(i8* nonnull %51, i32 4, i32 1, %struct._IO_FILE* %47), !dbg !203
  %56 = load i32, i32* %9, align 4, !dbg !204, !tbaa !166
  call void @llvm.dbg.value(metadata i32 %56, metadata !138, metadata !DIExpression()), !dbg !154
  %57 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.8, i32 0, i32 0), i32 %56), !dbg !205
  %58 = load i32, i32* %10, align 4, !dbg !206, !tbaa !166
  call void @llvm.dbg.value(metadata i32 %58, metadata !139, metadata !DIExpression()), !dbg !154
  %59 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.9, i32 0, i32 0), i32 %58), !dbg !207
  %60 = load i32, i32* %11, align 4, !dbg !208, !tbaa !166
  call void @llvm.dbg.value(metadata i32 %60, metadata !140, metadata !DIExpression()), !dbg !154
  %61 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @.str.10, i32 0, i32 0), i32 %60), !dbg !209
  %62 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([37 x i8], [37 x i8]* @.str.11, i32 0, i32 0), i32 4), !dbg !210
  %63 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([42 x i8], [42 x i8]* @.str.12, i32 0, i32 0), i32 8), !dbg !211
  %64 = shl i32 %56, 3, !dbg !212
  %65 = call noalias i8* @malloc(i32 %64) #13, !dbg !213
  store i8* %65, i8** bitcast (i32** @epilog to i8**), align 4, !dbg !214, !tbaa !158
  %66 = shl i32 %58, 3, !dbg !215
  %67 = call noalias i8* @malloc(i32 %66) #13, !dbg !216
  store i8* %67, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !217, !tbaa !158
  %68 = shl i32 %60, 3, !dbg !218
  %69 = call noalias i8* @malloc(i32 %68) #13, !dbg !219
  store i8* %69, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !220, !tbaa !158
  %70 = call i32 @fread(i8* %65, i32 8, i32 %56, %struct._IO_FILE* %45), !dbg !221
  %71 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !222, !tbaa !158
  %72 = call i32 @fread(i8* %71, i32 8, i32 %58, %struct._IO_FILE* %46), !dbg !223
  %73 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !224, !tbaa !158
  %74 = call i32 @fread(i8* %73, i32 8, i32 %60, %struct._IO_FILE* %47), !dbg !225
  %75 = call i8* @strcat(i8* nonnull %31, i8* nonnull %25) #13, !dbg !226
  %76 = call i32 @strlen(i8* nonnull %31), !dbg !227
  %77 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %76, !dbg !227
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(26) %77, i8* nonnull align 1 dereferenceable(26) getelementptr inbounds ([26 x i8], [26 x i8]* @.str.13, i32 0, i32 0), i32 26, i1 false), !dbg !227
  %78 = call %struct._IO_FILE* @fopen(i8* nonnull %31, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !228
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %78, metadata !123, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32* %12, metadata !141, metadata !DIExpression(DW_OP_deref)), !dbg !154
  %79 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %78, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %12) #13, !dbg !229
  %80 = getelementptr inbounds [256 x i8], [256 x i8]* %13, i32 0, i32 0, !dbg !230
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %80) #13, !dbg !230
  call void @llvm.dbg.declare(metadata [256 x i8]* %13, metadata !142, metadata !DIExpression()), !dbg !231
  call void @llvm.dbg.value(metadata i32 0, metadata !146, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 0, metadata !147, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 0, metadata !148, metadata !DIExpression()), !dbg !154
  %81 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([52 x i8], [52 x i8]* @str.35, i32 0, i32 0)), !dbg !232
  %82 = call i8* @fgets(i8* nonnull %80, i32 256, %struct._IO_FILE* %48), !dbg !233
  %83 = icmp eq i8* %82, null, !dbg !234
  br i1 %83, label %101, label %.preheader, !dbg !234

.preheader:                                       ; preds = %1
  br label %84, !dbg !234

84:                                               ; preds = %.preheader, %96
  %85 = phi i32 [ %98, %96 ], [ 0, %.preheader ]
  %86 = phi i32 [ %97, %96 ], [ 0, %.preheader ]
  %87 = phi i32 [ %88, %96 ], [ 0, %.preheader ]
  call void @llvm.dbg.value(metadata i32 %85, metadata !146, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 %86, metadata !147, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 %87, metadata !148, metadata !DIExpression()), !dbg !154
  %88 = add nuw nsw i32 %87, 1, !dbg !235
  call void @llvm.dbg.value(metadata i32 %88, metadata !148, metadata !DIExpression()), !dbg !154
  %89 = icmp eq i32 %87, 0, !dbg !237
  br i1 %89, label %90, label %92, !dbg !239

90:                                               ; preds = %84
  call void @llvm.dbg.value(metadata i8* %80, metadata !240, metadata !DIExpression()) #13, !dbg !248
  %91 = call i32 @strtol(i8* nocapture nonnull %80, i8** null, i32 10) #13, !dbg !250
  call void @llvm.dbg.value(metadata i32 %91, metadata !146, metadata !DIExpression()), !dbg !154
  br label %96, !dbg !251

92:                                               ; preds = %84
  %93 = icmp eq i32 %88, 2, !dbg !252
  br i1 %93, label %94, label %.loopexit, !dbg !254

94:                                               ; preds = %92
  call void @llvm.dbg.value(metadata i8* %80, metadata !240, metadata !DIExpression()) #13, !dbg !255
  %95 = call i32 @strtol(i8* nocapture nonnull %80, i8** null, i32 10) #13, !dbg !257
  call void @llvm.dbg.value(metadata i32 %95, metadata !147, metadata !DIExpression()), !dbg !154
  br label %96

96:                                               ; preds = %94, %90
  %97 = phi i32 [ %86, %90 ], [ %95, %94 ], !dbg !154
  %98 = phi i32 [ %91, %90 ], [ %85, %94 ], !dbg !154
  call void @llvm.dbg.value(metadata i32 %98, metadata !146, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 %97, metadata !147, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 %88, metadata !148, metadata !DIExpression()), !dbg !154
  %99 = call i8* @fgets(i8* nonnull %80, i32 256, %struct._IO_FILE* %48), !dbg !233
  %100 = icmp eq i8* %99, null, !dbg !234
  br i1 %100, label %.loopexit, label %84, !dbg !234, !llvm.loop !258

.loopexit:                                        ; preds = %92, %96
  %.ph = phi i32 [ %97, %96 ], [ %86, %92 ]
  %.ph1 = phi i32 [ %98, %96 ], [ %85, %92 ]
  br label %101, !dbg !262

101:                                              ; preds = %.loopexit, %1
  %102 = phi i32 [ 0, %1 ], [ %.ph, %.loopexit ], !dbg !263
  %103 = phi i32 [ 0, %1 ], [ %.ph1, %.loopexit ], !dbg !154
  %104 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([39 x i8], [39 x i8]* @.str.15, i32 0, i32 0), i32 %103, i32 %102), !dbg !262
  call void @llvm.dbg.value(metadata i32 %60, metadata !140, metadata !DIExpression()), !dbg !154
  %105 = mul nsw i32 %103, %102, !dbg !264
  %106 = sdiv i32 %60, %105, !dbg !265
  call void @llvm.dbg.value(metadata i32 %106, metadata !149, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 %56, metadata !138, metadata !DIExpression()), !dbg !154
  %107 = sdiv i32 %56, %105, !dbg !266
  call void @llvm.dbg.value(metadata i32 %107, metadata !150, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32 %58, metadata !139, metadata !DIExpression()), !dbg !154
  %108 = sdiv i32 %58, %105, !dbg !267
  call void @llvm.dbg.value(metadata i32 %108, metadata !151, metadata !DIExpression()), !dbg !154
  %109 = load i32*, i32** @initCGRA, align 4, !dbg !268, !tbaa !158
  %110 = getelementptr inbounds i32, i32* %109, i32 %19, !dbg !269
  %111 = getelementptr inbounds i32, i32* %110, i32 2, !dbg !270
  store i32 %106, i32* %111, align 4, !dbg !271, !tbaa !166
  %112 = getelementptr inbounds i32, i32* %110, i32 3, !dbg !272
  store i32 %107, i32* %112, align 4, !dbg !273, !tbaa !166
  %113 = getelementptr inbounds i32, i32* %110, i32 4, !dbg !274
  store i32 %108, i32* %113, align 4, !dbg !275, !tbaa !166
  %114 = load i32, i32* %12, align 4, !dbg !276, !tbaa !166
  call void @llvm.dbg.value(metadata i32 %114, metadata !141, metadata !DIExpression()), !dbg !154
  %115 = sdiv i32 %114, %105, !dbg !277
  %116 = getelementptr inbounds i32, i32* %110, i32 6, !dbg !278
  store i32 %115, i32* %116, align 4, !dbg !279, !tbaa !166
  %117 = call i32 @fclose(%struct._IO_FILE* %45), !dbg !280
  %118 = call i32 @fclose(%struct._IO_FILE* %46), !dbg !281
  %119 = call i32 @fclose(%struct._IO_FILE* %47), !dbg !282
  %120 = call i32 @fclose(%struct._IO_FILE* %78), !dbg !283
  %121 = call i32 @fclose(%struct._IO_FILE* %48), !dbg !284
  %122 = getelementptr inbounds [40 x i8], [40 x i8]* %14, i32 0, i32 0, !dbg !285
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %122) #13, !dbg !285
  call void @llvm.dbg.declare(metadata [40 x i8]* %14, metadata !152, metadata !DIExpression()), !dbg !286
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %122, i8 0, i32 40, i1 false), !dbg !286
  %123 = call i8* @strcat(i8* nonnull %122, i8* nonnull %25) #13, !dbg !287
  %124 = call i32 @strlen(i8* nonnull %122), !dbg !288
  %125 = getelementptr [40 x i8], [40 x i8]* %14, i32 0, i32 %124, !dbg !288
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(18) %125, i8* nonnull align 1 dereferenceable(18) getelementptr inbounds ([18 x i8], [18 x i8]* @.str.16, i32 0, i32 0), i32 18, i1 false), !dbg !288
  %126 = bitcast i32* %15 to i8*, !dbg !289
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %126) #13, !dbg !289
  call void @llvm.dbg.value(metadata i32 0, metadata !153, metadata !DIExpression()), !dbg !154
  store i32 0, i32* %15, align 4, !dbg !290, !tbaa !166
  %127 = call %struct._IO_FILE* @fopen(i8* nonnull %122, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !291
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %127, metadata !123, metadata !DIExpression()), !dbg !154
  call void @llvm.dbg.value(metadata i32* %15, metadata !153, metadata !DIExpression(DW_OP_deref)), !dbg !154
  %128 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %127, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %15) #13, !dbg !292
  %129 = call i32 @fclose(%struct._IO_FILE* %127), !dbg !293
  %130 = load i32, i32* %15, align 4, !dbg !294, !tbaa !166
  call void @llvm.dbg.value(metadata i32 %130, metadata !153, metadata !DIExpression()), !dbg !154
  %131 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.17, i32 0, i32 0), i32 %130), !dbg !295
  %132 = load i32, i32* %15, align 4, !dbg !296, !tbaa !166
  call void @llvm.dbg.value(metadata i32 %132, metadata !153, metadata !DIExpression()), !dbg !154
  %133 = load i32*, i32** @initCGRA, align 4, !dbg !297, !tbaa !158
  %134 = getelementptr inbounds i32, i32* %133, i32 %19, !dbg !298
  %135 = getelementptr inbounds i32, i32* %134, i32 5, !dbg !299
  store i32 %132, i32* %135, align 4, !dbg !300, !tbaa !166
  %136 = load i32*, i32** @prolog, align 4, !dbg !301, !tbaa !158
  %137 = ptrtoint i32* %136 to i32, !dbg !302
  %138 = load i32*, i32** @epilog, align 4, !dbg !303, !tbaa !158
  %139 = ptrtoint i32* %138 to i32, !dbg !304
  %140 = load i32*, i32** @kernel, align 4, !dbg !305, !tbaa !158
  %141 = ptrtoint i32* %140 to i32, !dbg !306
  %142 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([55 x i8], [55 x i8]* @.str.18, i32 0, i32 0), i32 %137, i32 %139, i32 %141), !dbg !307
  %143 = load i32*, i32** @prolog, align 4, !dbg !308, !tbaa !158
  %144 = ptrtoint i32* %143 to i32, !dbg !309
  %145 = load i32*, i32** @prologPtr, align 4, !dbg !310, !tbaa !158
  %146 = getelementptr inbounds i32, i32* %145, i32 %18, !dbg !310
  store i32 %144, i32* %146, align 4, !dbg !311, !tbaa !312
  %147 = load i32*, i32** @epilog, align 4, !dbg !314, !tbaa !158
  %148 = ptrtoint i32* %147 to i32, !dbg !315
  %149 = load i32*, i32** @epilogPtr, align 4, !dbg !316, !tbaa !158
  %150 = getelementptr inbounds i32, i32* %149, i32 %18, !dbg !316
  store i32 %148, i32* %150, align 4, !dbg !317, !tbaa !312
  %151 = load i32*, i32** @kernel, align 4, !dbg !318, !tbaa !158
  %152 = ptrtoint i32* %151 to i32, !dbg !319
  %153 = load i32*, i32** @kernelPtr, align 4, !dbg !320, !tbaa !158
  %154 = getelementptr inbounds i32, i32* %153, i32 %18, !dbg !320
  store i32 %152, i32* %154, align 4, !dbg !321, !tbaa !312
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %126) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %122) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %80) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %52) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %51) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %50) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %49) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %32) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %31) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %30) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %29) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %28) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %25) #13, !dbg !322
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %23) #13, !dbg !322
  ret i32 0, !dbg !323
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
declare !dbg !324 dso_local noundef i8* @fgets(i8* noundef, i32 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare !dbg !328 dso_local noundef i32 @fclose(%struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #3

; Function Attrs: nofree nounwind
define dso_local i32 @configureCGRA(i32 %0) local_unnamed_addr #8 !dbg !331 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  call void @llvm.dbg.value(metadata i32 %0, metadata !333, metadata !DIExpression()), !dbg !343
  %5 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([30 x i8], [30 x i8]* @str.36, i32 0, i32 0)), !dbg !344
  call void @llvm.dbg.value(metadata i32 0, metadata !334, metadata !DIExpression()), !dbg !343
  %6 = load i32*, i32** @initCGRA, align 4, !dbg !346, !tbaa !158
  %7 = add i32 %0, -1, !dbg !347
  %8 = mul i32 %7, 7, !dbg !348
  %9 = getelementptr inbounds i32, i32* %6, i32 %8, !dbg !349
  %10 = getelementptr inbounds i32, i32* %9, i32 5, !dbg !350
  %11 = load i32, i32* %10, align 4, !dbg !351, !tbaa !166
  call void @llvm.dbg.value(metadata i32 %11, metadata !335, metadata !DIExpression()), !dbg !343
  %12 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0, !dbg !352
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %12) #13, !dbg !352
  call void @llvm.dbg.declare(metadata [25 x i8]* %2, metadata !337, metadata !DIExpression()), !dbg !353
  %13 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0, !dbg !354
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %13) #13, !dbg !354
  call void @llvm.dbg.declare(metadata [20 x i8]* %3, metadata !338, metadata !DIExpression()), !dbg !355
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %13, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false), !dbg !355
  %14 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %12, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #13, !dbg !356
  %15 = call i8* @strcat(i8* nonnull %13, i8* nonnull %12) #13, !dbg !357
  %16 = icmp slt i32 %11, 1, !dbg !358
  br i1 %16, label %17, label %23, !dbg !359

17:                                               ; preds = %1
  %18 = load i32, i32* @dynamicTCVal, align 4, !dbg !360, !tbaa !166
  %19 = add nsw i32 %18, %11, !dbg !361
  call void @llvm.dbg.value(metadata i32 %19, metadata !339, metadata !DIExpression()), !dbg !362
  %20 = load i32*, i32** @initCGRA, align 4, !dbg !363, !tbaa !158
  %21 = getelementptr inbounds i32, i32* %20, i32 %8, !dbg !364
  %22 = getelementptr inbounds i32, i32* %21, i32 5, !dbg !365
  store i32 %19, i32* %22, align 4, !dbg !366, !tbaa !166
  br label %23, !dbg !367

23:                                               ; preds = %17, %1
  %24 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0, !dbg !368
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %24) #13, !dbg !368
  call void @llvm.dbg.declare(metadata [40 x i8]* %4, metadata !342, metadata !DIExpression()), !dbg !369
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %24, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i1 false), !dbg !369
  %25 = call i32 @strlen(i8* nonnull %24), !dbg !370
  %26 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %25, !dbg !370
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(14) %26, i8* nonnull align 1 dereferenceable(14) getelementptr inbounds ([14 x i8], [14 x i8]* @.str.20, i32 0, i32 0), i32 14, i1 false), !dbg !370
  %27 = call %struct._IO_FILE* @fopen(i8* nonnull %24, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.21, i32 0, i32 0)), !dbg !371
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %27, metadata !336, metadata !DIExpression()), !dbg !343
  call void @llvm.dbg.value(metadata i32 0, metadata !334, metadata !DIExpression()), !dbg !343
  br label %28, !dbg !372

28:                                               ; preds = %28, %23
  %29 = phi i32 [ 0, %23 ], [ %35, %28 ]
  call void @llvm.dbg.value(metadata i32 %29, metadata !334, metadata !DIExpression()), !dbg !343
  %30 = load i32*, i32** @initCGRA, align 4, !dbg !374, !tbaa !158
  %31 = getelementptr inbounds i32, i32* %30, i32 %8, !dbg !376
  %32 = getelementptr inbounds i32, i32* %31, i32 %29, !dbg !377
  %33 = load i32, i32* %32, align 4, !dbg !378, !tbaa !166
  %34 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.22, i32 0, i32 0), i32 %33), !dbg !379
  %35 = add nuw nsw i32 %29, 1, !dbg !380
  call void @llvm.dbg.value(metadata i32 %35, metadata !334, metadata !DIExpression()), !dbg !343
  %36 = icmp eq i32 %35, 7, !dbg !381
  br i1 %36, label %37, label %28, !dbg !372, !llvm.loop !382

37:                                               ; preds = %28
  %38 = load i32*, i32** @epilogPtr, align 4, !dbg !384, !tbaa !158
  %39 = getelementptr inbounds i32, i32* %38, i32 %7, !dbg !384
  %40 = load i32, i32* %39, align 4, !dbg !384, !tbaa !312
  %41 = zext i32 %40 to i64, !dbg !385
  %42 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i64 %41), !dbg !386
  %43 = load i32*, i32** @prologPtr, align 4, !dbg !387, !tbaa !158
  %44 = getelementptr inbounds i32, i32* %43, i32 %7, !dbg !387
  %45 = load i32, i32* %44, align 4, !dbg !387, !tbaa !312
  %46 = zext i32 %45 to i64, !dbg !388
  %47 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i64 %46), !dbg !389
  %48 = load i32*, i32** @kernelPtr, align 4, !dbg !390, !tbaa !158
  %49 = getelementptr inbounds i32, i32* %48, i32 %7, !dbg !390
  %50 = load i32, i32* %49, align 4, !dbg !390, !tbaa !312
  %51 = zext i32 %50 to i64, !dbg !391
  %52 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i64 %51), !dbg !392
  %53 = call i32 @fclose(%struct._IO_FILE* %27), !dbg !393
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %24) #13, !dbg !394
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %13) #13, !dbg !394
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %12) #13, !dbg !394
  ret i32 0, !dbg !395
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fprintf(%struct._IO_FILE* nocapture noundef, i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @checkTotalLoops() local_unnamed_addr #0 !dbg !396 {
  %1 = alloca [40 x i8], align 1
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @str.37, i32 0, i32 0)), !dbg !402
  %3 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0, !dbg !404
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %3) #13, !dbg !404
  call void @llvm.dbg.declare(metadata [40 x i8]* %1, metadata !400, metadata !DIExpression()), !dbg !405
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %3, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false), !dbg !405
  %4 = call %struct._IO_FILE* @fopen(i8* nonnull %3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)), !dbg !406
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %4, metadata !401, metadata !DIExpression()), !dbg !407
  %5 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.25, i32 0, i32 0), i32* nonnull @totalLoops) #13, !dbg !408
  %6 = tail call i32 @fclose(%struct._IO_FILE* %4), !dbg !409
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %3) #13, !dbg !410
  ret void, !dbg !410
}

; Function Attrs: nounwind
define dso_local i8* @runOnCGRA(i8* nocapture readnone %0) #0 !dbg !411 {
  call void @llvm.dbg.value(metadata i8* undef, metadata !415, metadata !DIExpression()), !dbg !416
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([26 x i8], [26 x i8]* @str.38, i32 0, i32 0)), !dbg !417
  br label %3, !dbg !419

3:                                                ; preds = %14, %1
  %4 = load i32, i32* @thread_cond_cgra, align 4, !dbg !420, !tbaa !166
  %5 = icmp eq i32 %4, 1, !dbg !422
  br i1 %5, label %10, label %.preheader, !dbg !423

.preheader:                                       ; preds = %3
  br label %6, !dbg !423

6:                                                ; preds = %.preheader, %6
  %7 = tail call i32 @usleep(i32 1) #13, !dbg !424
  %8 = load i32, i32* @thread_cond_cgra, align 4, !dbg !420, !tbaa !166
  %9 = icmp eq i32 %8, 1, !dbg !422
  br i1 %9, label %.loopexit, label %6, !dbg !423, !llvm.loop !425

.loopexit:                                        ; preds = %6
  br label %10, !dbg !427

10:                                               ; preds = %.loopexit, %3
  %11 = load i32, i32* @thread_exit, align 4, !dbg !427, !tbaa !166
  %12 = icmp eq i32 %11, 1, !dbg !429
  br i1 %12, label %13, label %14, !dbg !430

13:                                               ; preds = %10
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #13, !dbg !431, !srcloc !433
  ret i8* null, !dbg !434

14:                                               ; preds = %10
  store i32 0, i32* @thread_cond_cgra, align 4, !dbg !435, !tbaa !166
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #13, !dbg !436, !srcloc !437
  %15 = tail call i32 @usleep(i32 1) #13, !dbg !438
  store i32 1, i32* @thread_cond_cpu, align 4, !dbg !439, !tbaa !166
  br label %3, !dbg !419, !llvm.loop !440
}

declare !dbg !442 dso_local i32 @usleep(i32) local_unnamed_addr #7

; Function Attrs: nounwind
define dso_local void @accelerateOnCGRA(i32 %0) local_unnamed_addr #0 !dbg !444 {
  call void @llvm.dbg.value(metadata i32 %0, metadata !448, metadata !DIExpression()), !dbg !451
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @str.39, i32 0, i32 0)), !dbg !452
  call void @llvm.dbg.value(metadata i32 0, metadata !449, metadata !DIExpression()), !dbg !451
  %3 = tail call i32 @configureCGRA(i32 %0), !dbg !454
  call void @llvm.dbg.value(metadata i32 0, metadata !450, metadata !DIExpression()), !dbg !451
  %4 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @.str.29, i32 0, i32 0), i32 %0), !dbg !455
  store i32 0, i32* @thread_cond_cpu, align 4, !dbg !456, !tbaa !166
  store i32 1, i32* @thread_cond_cgra, align 4, !dbg !457, !tbaa !166
  br label %5, !dbg !458

5:                                                ; preds = %5, %1
  %6 = tail call i32 @usleep(i32 1) #13, !dbg !459
  %7 = load i32, i32* @thread_cond_cpu, align 4, !dbg !460, !tbaa !166
  %8 = icmp eq i32 %7, 1, !dbg !461
  br i1 %8, label %9, label %5, !dbg !458, !llvm.loop !462

9:                                                ; preds = %5
  ret void, !dbg !464
}

; Function Attrs: nounwind
define dso_local void @deleteCGRA() local_unnamed_addr #0 !dbg !465 {
  %1 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([14 x i8], [14 x i8]* @str.40, i32 0, i32 0)), !dbg !466
  store i32 1, i32* @thread_exit, align 4, !dbg !468, !tbaa !166
  store i32 1, i32* @thread_cond_cgra, align 4, !dbg !469, !tbaa !166
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([40 x i8], [40 x i8]* @str.41, i32 0, i32 0)), !dbg !470
  %3 = load i32, i32* @pth, align 4, !dbg !471, !tbaa !312
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #13, !dbg !472
  %5 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([26 x i8], [26 x i8]* @str.42, i32 0, i32 0)), !dbg !473
  ret void, !dbg !475
}

declare !dbg !476 dso_local i32 @pthread_join(i32, i8**) local_unnamed_addr #7

; Function Attrs: nounwind
define dso_local void @createCGRA() local_unnamed_addr #0 !dbg !481 {
  %1 = alloca [40 x i8], align 1
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([27 x i8], [27 x i8]* @str.43, i32 0, i32 0)), !dbg !485
  call void @llvm.dbg.value(metadata i32 0, metadata !483, metadata !DIExpression()), !dbg !487
  call void @llvm.dbg.value(metadata i32 1, metadata !484, metadata !DIExpression()), !dbg !487
  %3 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @str.37, i32 0, i32 0)) #13, !dbg !488
  %4 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0, !dbg !490
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %4) #13, !dbg !490
  call void @llvm.dbg.declare(metadata [40 x i8]* %1, metadata !400, metadata !DIExpression()) #13, !dbg !491
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %4, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false) #13, !dbg !491
  %5 = call %struct._IO_FILE* @fopen(i8* nonnull %4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)) #13, !dbg !492
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %5, metadata !401, metadata !DIExpression()) #13, !dbg !493
  %6 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %5, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.25, i32 0, i32 0), i32* nonnull @totalLoops) #13, !dbg !494
  %7 = tail call i32 @fclose(%struct._IO_FILE* %5) #13, !dbg !495
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %4) #13, !dbg !496
  %8 = load i32, i32* @totalLoops, align 4, !dbg !497, !tbaa !166
  %9 = mul i32 %8, 28, !dbg !498
  %10 = tail call noalias i8* @malloc(i32 %9) #13, !dbg !499
  store i8* %10, i8** bitcast (i32** @initCGRA to i8**), align 4, !dbg !500, !tbaa !158
  %11 = shl i32 %8, 2, !dbg !501
  %12 = tail call noalias i8* @malloc(i32 %11) #13, !dbg !502
  store i8* %12, i8** bitcast (i32** @prologPtr to i8**), align 4, !dbg !503, !tbaa !158
  %13 = tail call noalias i8* @malloc(i32 %11) #13, !dbg !504
  store i8* %13, i8** bitcast (i32** @kernelPtr to i8**), align 4, !dbg !505, !tbaa !158
  %14 = tail call noalias i8* @malloc(i32 %11) #13, !dbg !506
  store i8* %14, i8** bitcast (i32** @epilogPtr to i8**), align 4, !dbg !507, !tbaa !158
  %15 = icmp eq i32 %8, 0, !dbg !508
  br i1 %15, label %22, label %.preheader, !dbg !511

.preheader:                                       ; preds = %0
  br label %16, !dbg !511

16:                                               ; preds = %.preheader, %16
  %17 = phi i32 [ %19, %16 ], [ 1, %.preheader ]
  call void @llvm.dbg.value(metadata i32 %17, metadata !484, metadata !DIExpression()), !dbg !487
  %18 = tail call i32 @initializeParameters(i32 %17), !dbg !512
  %19 = add i32 %17, 1, !dbg !513
  call void @llvm.dbg.value(metadata i32 %19, metadata !484, metadata !DIExpression()), !dbg !487
  %20 = load i32, i32* @totalLoops, align 4, !dbg !514, !tbaa !166
  %21 = icmp ugt i32 %19, %20, !dbg !508
  br i1 %21, label %.loopexit, label %16, !dbg !511, !llvm.loop !515

.loopexit:                                        ; preds = %16
  br label %22, !dbg !517

22:                                               ; preds = %.loopexit, %0
  %23 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @str.44, i32 0, i32 0)), !dbg !517
  tail call void asm sideeffect "mov r8,$0", "r"(i32 71) #13, !dbg !518, !srcloc !519
  %24 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #13, !dbg !520
  call void @llvm.dbg.value(metadata i32 %24, metadata !483, metadata !DIExpression()), !dbg !487
  ret void, !dbg !521
}

; Function Attrs: nounwind
declare !dbg !522 dso_local i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #9

; Function Attrs: nofree nounwind willreturn
declare dso_local i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #10

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #1

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #11

; Function Attrs: argmemonly nofree nounwind readonly willreturn
declare i32 @strlen(i8* nocapture) local_unnamed_addr #12

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
attributes #9 = { nounwind "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #10 = { nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #11 = { nofree nounwind }
attributes #12 = { argmemonly nofree nounwind readonly willreturn }
attributes #13 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!48, !49, !50, !51}
!llvm.ident = !{!52}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "totalLoops", scope: !2, file: !18, line: 30, type: !19, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 13.0.0 (https://github.com/llvm/llvm-project.git 0d4534237de38fcf1b2a71bcb0f74cb5675fe7e1)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !15, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/local/ASUAD/aabuyaz1/documents/code/ccf_1804/scripts/CGRALib/cgra.c", directory: "/home/local/ASUAD/aabuyaz1/documents/code/ccf_1804/benchmarks/Micro-benchmarks/simple_loop_1_Crimson")
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
!15 = !{!0, !16, !20, !22, !24, !26, !29, !31, !33, !36, !38, !40, !42, !44}
!16 = !DIGlobalVariableExpression(var: !17, expr: !DIExpression())
!17 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !2, file: !18, line: 33, type: !19, isLocal: false, isDefinition: true)
!18 = !DIFile(filename: "scripts/CGRALib/cgra.h", directory: "/home/local/ASUAD/aabuyaz1/documents/code/ccf_1804")
!19 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!20 = !DIGlobalVariableExpression(var: !21, expr: !DIExpression())
!21 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !2, file: !18, line: 34, type: !19, isLocal: false, isDefinition: true)
!22 = !DIGlobalVariableExpression(var: !23, expr: !DIExpression())
!23 = distinct !DIGlobalVariable(name: "thread_exit", scope: !2, file: !18, line: 35, type: !19, isLocal: false, isDefinition: true)
!24 = !DIGlobalVariableExpression(var: !25, expr: !DIExpression())
!25 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !2, file: !18, line: 37, type: !11, isLocal: false, isDefinition: true)
!26 = !DIGlobalVariableExpression(var: !27, expr: !DIExpression())
!27 = distinct !DIGlobalVariable(name: "prolog", scope: !2, file: !18, line: 24, type: !28, isLocal: false, isDefinition: true)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !8, size: 32)
!29 = !DIGlobalVariableExpression(var: !30, expr: !DIExpression())
!30 = distinct !DIGlobalVariable(name: "epilog", scope: !2, file: !18, line: 24, type: !28, isLocal: false, isDefinition: true)
!31 = !DIGlobalVariableExpression(var: !32, expr: !DIExpression())
!32 = distinct !DIGlobalVariable(name: "kernel", scope: !2, file: !18, line: 24, type: !28, isLocal: false, isDefinition: true)
!33 = !DIGlobalVariableExpression(var: !34, expr: !DIExpression())
!34 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !2, file: !18, line: 25, type: !35, isLocal: false, isDefinition: true)
!35 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 32)
!36 = !DIGlobalVariableExpression(var: !37, expr: !DIExpression())
!37 = distinct !DIGlobalVariable(name: "prologPtr", scope: !2, file: !18, line: 27, type: !28, isLocal: false, isDefinition: true)
!38 = !DIGlobalVariableExpression(var: !39, expr: !DIExpression())
!39 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !2, file: !18, line: 27, type: !28, isLocal: false, isDefinition: true)
!40 = !DIGlobalVariableExpression(var: !41, expr: !DIExpression())
!41 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !2, file: !18, line: 27, type: !28, isLocal: false, isDefinition: true)
!42 = !DIGlobalVariableExpression(var: !43, expr: !DIExpression())
!43 = distinct !DIGlobalVariable(name: "initCGRA", scope: !2, file: !18, line: 29, type: !10, isLocal: false, isDefinition: true)
!44 = !DIGlobalVariableExpression(var: !45, expr: !DIExpression())
!45 = distinct !DIGlobalVariable(name: "pth", scope: !2, file: !18, line: 32, type: !46, isLocal: false, isDefinition: true)
!46 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !47, line: 27, baseType: !8)
!47 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "")
!48 = !{i32 7, !"Dwarf Version", i32 4}
!49 = !{i32 2, !"Debug Info Version", i32 3}
!50 = !{i32 1, !"wchar_size", i32 4}
!51 = !{i32 1, !"min_enum_size", i32 4}
!52 = !{!"clang version 13.0.0 (https://github.com/llvm/llvm-project.git 0d4534237de38fcf1b2a71bcb0f74cb5675fe7e1)"}
!53 = distinct !DISubprogram(name: "initializeParameters", scope: !54, file: !54, line: 9, type: !55, scopeLine: 10, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !57)
!54 = !DIFile(filename: "scripts/CGRALib/cgra.c", directory: "/home/local/ASUAD/aabuyaz1/documents/code/ccf_1804")
!55 = !DISubroutineType(types: !56)
!56 = !{!11, !19}
!57 = !{!58, !59, !60, !121, !122, !123, !124, !125, !129, !133, !134, !135, !136, !137, !138, !139, !140, !141, !142, !146, !147, !148, !149, !150, !151, !152, !153}
!58 = !DILocalVariable(name: "loopID", arg: 1, scope: !53, file: !54, line: 9, type: !19)
!59 = !DILocalVariable(name: "i", scope: !53, file: !54, line: 13, type: !19)
!60 = !DILocalVariable(name: "epi", scope: !53, file: !54, line: 23, type: !61)
!61 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !62, size: 32)
!62 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !63, line: 7, baseType: !64)
!63 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types/FILE.h", directory: "")
!64 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !65, line: 49, size: 1216, elements: !66)
!65 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types/struct_FILE.h", directory: "")
!66 = !{!67, !68, !69, !70, !71, !72, !73, !74, !75, !76, !77, !78, !79, !82, !84, !85, !86, !90, !92, !94, !98, !101, !105, !108, !111, !112, !113, !116, !117}
!67 = !DIDerivedType(tag: DW_TAG_member, name: "_flags", scope: !64, file: !65, line: 51, baseType: !11, size: 32)
!68 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_ptr", scope: !64, file: !65, line: 54, baseType: !13, size: 32, offset: 32)
!69 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_end", scope: !64, file: !65, line: 55, baseType: !13, size: 32, offset: 64)
!70 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_base", scope: !64, file: !65, line: 56, baseType: !13, size: 32, offset: 96)
!71 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_base", scope: !64, file: !65, line: 57, baseType: !13, size: 32, offset: 128)
!72 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_ptr", scope: !64, file: !65, line: 58, baseType: !13, size: 32, offset: 160)
!73 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_end", scope: !64, file: !65, line: 59, baseType: !13, size: 32, offset: 192)
!74 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_base", scope: !64, file: !65, line: 60, baseType: !13, size: 32, offset: 224)
!75 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_end", scope: !64, file: !65, line: 61, baseType: !13, size: 32, offset: 256)
!76 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_base", scope: !64, file: !65, line: 64, baseType: !13, size: 32, offset: 288)
!77 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_backup_base", scope: !64, file: !65, line: 65, baseType: !13, size: 32, offset: 320)
!78 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_end", scope: !64, file: !65, line: 66, baseType: !13, size: 32, offset: 352)
!79 = !DIDerivedType(tag: DW_TAG_member, name: "_markers", scope: !64, file: !65, line: 68, baseType: !80, size: 32, offset: 384)
!80 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !81, size: 32)
!81 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_marker", file: !65, line: 36, flags: DIFlagFwdDecl)
!82 = !DIDerivedType(tag: DW_TAG_member, name: "_chain", scope: !64, file: !65, line: 70, baseType: !83, size: 32, offset: 416)
!83 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !64, size: 32)
!84 = !DIDerivedType(tag: DW_TAG_member, name: "_fileno", scope: !64, file: !65, line: 72, baseType: !11, size: 32, offset: 448)
!85 = !DIDerivedType(tag: DW_TAG_member, name: "_flags2", scope: !64, file: !65, line: 73, baseType: !11, size: 32, offset: 480)
!86 = !DIDerivedType(tag: DW_TAG_member, name: "_old_offset", scope: !64, file: !65, line: 74, baseType: !87, size: 32, offset: 512)
!87 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off_t", file: !88, line: 152, baseType: !89)
!88 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "")
!89 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!90 = !DIDerivedType(tag: DW_TAG_member, name: "_cur_column", scope: !64, file: !65, line: 77, baseType: !91, size: 16, offset: 544)
!91 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!92 = !DIDerivedType(tag: DW_TAG_member, name: "_vtable_offset", scope: !64, file: !65, line: 78, baseType: !93, size: 8, offset: 560)
!93 = !DIBasicType(name: "signed char", size: 8, encoding: DW_ATE_signed_char)
!94 = !DIDerivedType(tag: DW_TAG_member, name: "_shortbuf", scope: !64, file: !65, line: 79, baseType: !95, size: 8, offset: 568)
!95 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 8, elements: !96)
!96 = !{!97}
!97 = !DISubrange(count: 1)
!98 = !DIDerivedType(tag: DW_TAG_member, name: "_lock", scope: !64, file: !65, line: 81, baseType: !99, size: 32, offset: 576)
!99 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !100, size: 32)
!100 = !DIDerivedType(tag: DW_TAG_typedef, name: "_IO_lock_t", file: !65, line: 43, baseType: null)
!101 = !DIDerivedType(tag: DW_TAG_member, name: "_offset", scope: !64, file: !65, line: 89, baseType: !102, size: 64, offset: 640)
!102 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off64_t", file: !88, line: 153, baseType: !103)
!103 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int64_t", file: !88, line: 47, baseType: !104)
!104 = !DIBasicType(name: "long long int", size: 64, encoding: DW_ATE_signed)
!105 = !DIDerivedType(tag: DW_TAG_member, name: "_codecvt", scope: !64, file: !65, line: 91, baseType: !106, size: 32, offset: 704)
!106 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !107, size: 32)
!107 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_codecvt", file: !65, line: 37, flags: DIFlagFwdDecl)
!108 = !DIDerivedType(tag: DW_TAG_member, name: "_wide_data", scope: !64, file: !65, line: 92, baseType: !109, size: 32, offset: 736)
!109 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !110, size: 32)
!110 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_wide_data", file: !65, line: 38, flags: DIFlagFwdDecl)
!111 = !DIDerivedType(tag: DW_TAG_member, name: "_freeres_list", scope: !64, file: !65, line: 93, baseType: !83, size: 32, offset: 768)
!112 = !DIDerivedType(tag: DW_TAG_member, name: "_freeres_buf", scope: !64, file: !65, line: 94, baseType: !9, size: 32, offset: 800)
!113 = !DIDerivedType(tag: DW_TAG_member, name: "__pad5", scope: !64, file: !65, line: 95, baseType: !114, size: 32, offset: 832)
!114 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !115, line: 46, baseType: !19)
!115 = !DIFile(filename: "sw/lib/clang/13.0.0/include/stddef.h", directory: "/home/local/ASUAD/aabuyaz1")
!116 = !DIDerivedType(tag: DW_TAG_member, name: "_mode", scope: !64, file: !65, line: 96, baseType: !11, size: 32, offset: 864)
!117 = !DIDerivedType(tag: DW_TAG_member, name: "_unused2", scope: !64, file: !65, line: 98, baseType: !118, size: 320, offset: 896)
!118 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 320, elements: !119)
!119 = !{!120}
!120 = !DISubrange(count: 40)
!121 = !DILocalVariable(name: "pro", scope: !53, file: !54, line: 23, type: !61)
!122 = !DILocalVariable(name: "kern", scope: !53, file: !54, line: 23, type: !61)
!123 = !DILocalVariable(name: "count", scope: !53, file: !54, line: 23, type: !61)
!124 = !DILocalVariable(name: "config", scope: !53, file: !54, line: 23, type: !61)
!125 = !DILocalVariable(name: "loopno", scope: !53, file: !54, line: 24, type: !126)
!126 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 200, elements: !127)
!127 = !{!128}
!128 = !DISubrange(count: 25)
!129 = !DILocalVariable(name: "directoryPath", scope: !53, file: !54, line: 25, type: !130)
!130 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 160, elements: !131)
!131 = !{!132}
!132 = !DISubrange(count: 20)
!133 = !DILocalVariable(name: "epifile", scope: !53, file: !54, line: 30, type: !118)
!134 = !DILocalVariable(name: "profile", scope: !53, file: !54, line: 31, type: !118)
!135 = !DILocalVariable(name: "kernfile", scope: !53, file: !54, line: 32, type: !118)
!136 = !DILocalVariable(name: "liveOutfile", scope: !53, file: !54, line: 33, type: !118)
!137 = !DILocalVariable(name: "configfile", scope: !53, file: !54, line: 34, type: !118)
!138 = !DILocalVariable(name: "episize", scope: !53, file: !54, line: 53, type: !11)
!139 = !DILocalVariable(name: "prosize", scope: !53, file: !54, line: 53, type: !11)
!140 = !DILocalVariable(name: "kernelsize", scope: !53, file: !54, line: 53, type: !11)
!141 = !DILocalVariable(name: "livevar_st_size", scope: !53, file: !54, line: 53, type: !11)
!142 = !DILocalVariable(name: "line", scope: !53, file: !54, line: 78, type: !143)
!143 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 2048, elements: !144)
!144 = !{!145}
!145 = !DISubrange(count: 256)
!146 = !DILocalVariable(name: "XDim", scope: !53, file: !54, line: 79, type: !11)
!147 = !DILocalVariable(name: "YDim", scope: !53, file: !54, line: 79, type: !11)
!148 = !DILocalVariable(name: "iter", scope: !53, file: !54, line: 80, type: !11)
!149 = !DILocalVariable(name: "II", scope: !53, file: !54, line: 94, type: !11)
!150 = !DILocalVariable(name: "epiLength", scope: !53, file: !54, line: 95, type: !11)
!151 = !DILocalVariable(name: "prolength", scope: !53, file: !54, line: 96, type: !11)
!152 = !DILocalVariable(name: "loopitfile", scope: !53, file: !54, line: 108, type: !118)
!153 = !DILocalVariable(name: "kernelCount", scope: !53, file: !54, line: 112, type: !11)
!154 = !DILocation(line: 0, scope: !53)
!155 = !DILocation(line: 12, column: 13, scope: !156)
!156 = distinct !DILexicalBlock(scope: !53, file: !54, line: 12, column: 6)
!157 = !DILocation(line: 15, column: 5, scope: !53)
!158 = !{!159, !159, i64 0}
!159 = !{!"any pointer", !160, i64 0}
!160 = !{!"omnipotent char", !161, i64 0}
!161 = !{!"Simple C/C++ TBAA"}
!162 = !DILocation(line: 15, column: 25, scope: !53)
!163 = !DILocation(line: 15, column: 17, scope: !53)
!164 = !DILocation(line: 15, column: 14, scope: !53)
!165 = !DILocation(line: 15, column: 30, scope: !53)
!166 = !{!167, !167, i64 0}
!167 = !{!"int", !160, i64 0}
!168 = !DILocation(line: 16, column: 29, scope: !53)
!169 = !DILocation(line: 16, column: 34, scope: !53)
!170 = !DILocation(line: 17, column: 29, scope: !53)
!171 = !DILocation(line: 24, column: 3, scope: !53)
!172 = !DILocation(line: 18, column: 34, scope: !53)
!173 = !DILocation(line: 24, column: 8, scope: !53)
!174 = !DILocation(line: 25, column: 3, scope: !53)
!175 = !DILocation(line: 25, column: 8, scope: !53)
!176 = !DILocation(line: 27, column: 3, scope: !53)
!177 = !DILocation(line: 28, column: 3, scope: !53)
!178 = !DILocation(line: 30, column: 3, scope: !53)
!179 = !DILocation(line: 30, column: 8, scope: !53)
!180 = !DILocation(line: 31, column: 3, scope: !53)
!181 = !DILocation(line: 31, column: 8, scope: !53)
!182 = !DILocation(line: 32, column: 3, scope: !53)
!183 = !DILocation(line: 32, column: 8, scope: !53)
!184 = !DILocation(line: 33, column: 3, scope: !53)
!185 = !DILocation(line: 33, column: 8, scope: !53)
!186 = !DILocation(line: 34, column: 3, scope: !53)
!187 = !DILocation(line: 34, column: 8, scope: !53)
!188 = !DILocation(line: 36, column: 3, scope: !53)
!189 = !DILocation(line: 37, column: 3, scope: !53)
!190 = !DILocation(line: 39, column: 3, scope: !53)
!191 = !DILocation(line: 40, column: 3, scope: !53)
!192 = !DILocation(line: 42, column: 3, scope: !53)
!193 = !DILocation(line: 43, column: 3, scope: !53)
!194 = !DILocation(line: 45, column: 3, scope: !53)
!195 = !DILocation(line: 46, column: 3, scope: !53)
!196 = !DILocation(line: 48, column: 7, scope: !53)
!197 = !DILocation(line: 49, column: 7, scope: !53)
!198 = !DILocation(line: 50, column: 8, scope: !53)
!199 = !DILocation(line: 51, column: 10, scope: !53)
!200 = !DILocation(line: 53, column: 3, scope: !53)
!201 = !DILocation(line: 55, column: 3, scope: !53)
!202 = !DILocation(line: 56, column: 3, scope: !53)
!203 = !DILocation(line: 57, column: 3, scope: !53)
!204 = !DILocation(line: 59, column: 46, scope: !53)
!205 = !DILocation(line: 59, column: 3, scope: !53)
!206 = !DILocation(line: 60, column: 46, scope: !53)
!207 = !DILocation(line: 60, column: 3, scope: !53)
!208 = !DILocation(line: 61, column: 47, scope: !53)
!209 = !DILocation(line: 61, column: 3, scope: !53)
!210 = !DILocation(line: 62, column: 3, scope: !53)
!211 = !DILocation(line: 63, column: 3, scope: !53)
!212 = !DILocation(line: 65, column: 45, scope: !53)
!213 = !DILocation(line: 65, column: 31, scope: !53)
!214 = !DILocation(line: 65, column: 9, scope: !53)
!215 = !DILocation(line: 66, column: 45, scope: !53)
!216 = !DILocation(line: 66, column: 31, scope: !53)
!217 = !DILocation(line: 66, column: 9, scope: !53)
!218 = !DILocation(line: 67, column: 48, scope: !53)
!219 = !DILocation(line: 67, column: 31, scope: !53)
!220 = !DILocation(line: 67, column: 9, scope: !53)
!221 = !DILocation(line: 69, column: 3, scope: !53)
!222 = !DILocation(line: 70, column: 9, scope: !53)
!223 = !DILocation(line: 70, column: 3, scope: !53)
!224 = !DILocation(line: 71, column: 9, scope: !53)
!225 = !DILocation(line: 71, column: 3, scope: !53)
!226 = !DILocation(line: 73, column: 3, scope: !53)
!227 = !DILocation(line: 74, column: 3, scope: !53)
!228 = !DILocation(line: 75, column: 9, scope: !53)
!229 = !DILocation(line: 76, column: 3, scope: !53)
!230 = !DILocation(line: 78, column: 3, scope: !53)
!231 = !DILocation(line: 78, column: 8, scope: !53)
!232 = !DILocation(line: 81, column: 3, scope: !53)
!233 = !DILocation(line: 82, column: 9, scope: !53)
!234 = !DILocation(line: 82, column: 3, scope: !53)
!235 = !DILocation(line: 84, column: 9, scope: !236)
!236 = distinct !DILexicalBlock(scope: !53, file: !54, line: 83, column: 3)
!237 = !DILocation(line: 85, column: 12, scope: !238)
!238 = distinct !DILexicalBlock(scope: !236, file: !54, line: 85, column: 8)
!239 = !DILocation(line: 85, column: 8, scope: !236)
!240 = !DILocalVariable(name: "__nptr", arg: 1, scope: !241, file: !242, line: 361, type: !245)
!241 = distinct !DISubprogram(name: "atoi", scope: !242, file: !242, line: 361, type: !243, scopeLine: 362, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !247)
!242 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdlib.h", directory: "")
!243 = !DISubroutineType(types: !244)
!244 = !{!11, !245}
!245 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !246, size: 32)
!246 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !14)
!247 = !{!240}
!248 = !DILocation(line: 0, scope: !241, inlinedAt: !249)
!249 = distinct !DILocation(line: 86, column: 14, scope: !238)
!250 = !DILocation(line: 363, column: 16, scope: !241, inlinedAt: !249)
!251 = !DILocation(line: 86, column: 7, scope: !238)
!252 = !DILocation(line: 87, column: 17, scope: !253)
!253 = distinct !DILexicalBlock(scope: !238, file: !54, line: 87, column: 13)
!254 = !DILocation(line: 87, column: 13, scope: !238)
!255 = !DILocation(line: 0, scope: !241, inlinedAt: !256)
!256 = distinct !DILocation(line: 88, column: 14, scope: !253)
!257 = !DILocation(line: 363, column: 16, scope: !241, inlinedAt: !256)
!258 = distinct !{!258, !234, !259, !260, !261}
!259 = !DILocation(line: 91, column: 3, scope: !53)
!260 = !{!"llvm.loop.mustprogress"}
!261 = !{!"llvm.loop.unroll.disable"}
!262 = !DILocation(line: 93, column: 3, scope: !53)
!263 = !DILocation(line: 79, column: 15, scope: !53)
!264 = !DILocation(line: 94, column: 28, scope: !53)
!265 = !DILocation(line: 94, column: 22, scope: !53)
!266 = !DILocation(line: 95, column: 26, scope: !53)
!267 = !DILocation(line: 96, column: 26, scope: !53)
!268 = !DILocation(line: 97, column: 5, scope: !53)
!269 = !DILocation(line: 97, column: 14, scope: !53)
!270 = !DILocation(line: 97, column: 29, scope: !53)
!271 = !DILocation(line: 97, column: 34, scope: !53)
!272 = !DILocation(line: 98, column: 29, scope: !53)
!273 = !DILocation(line: 98, column: 34, scope: !53)
!274 = !DILocation(line: 99, column: 29, scope: !53)
!275 = !DILocation(line: 99, column: 34, scope: !53)
!276 = !DILocation(line: 100, column: 36, scope: !53)
!277 = !DILocation(line: 100, column: 51, scope: !53)
!278 = !DILocation(line: 100, column: 29, scope: !53)
!279 = !DILocation(line: 100, column: 34, scope: !53)
!280 = !DILocation(line: 102, column: 3, scope: !53)
!281 = !DILocation(line: 103, column: 3, scope: !53)
!282 = !DILocation(line: 104, column: 3, scope: !53)
!283 = !DILocation(line: 105, column: 3, scope: !53)
!284 = !DILocation(line: 106, column: 3, scope: !53)
!285 = !DILocation(line: 108, column: 3, scope: !53)
!286 = !DILocation(line: 108, column: 8, scope: !53)
!287 = !DILocation(line: 109, column: 3, scope: !53)
!288 = !DILocation(line: 110, column: 3, scope: !53)
!289 = !DILocation(line: 112, column: 3, scope: !53)
!290 = !DILocation(line: 112, column: 7, scope: !53)
!291 = !DILocation(line: 113, column: 9, scope: !53)
!292 = !DILocation(line: 114, column: 3, scope: !53)
!293 = !DILocation(line: 115, column: 3, scope: !53)
!294 = !DILocation(line: 116, column: 29, scope: !53)
!295 = !DILocation(line: 116, column: 3, scope: !53)
!296 = !DILocation(line: 118, column: 36, scope: !53)
!297 = !DILocation(line: 118, column: 5, scope: !53)
!298 = !DILocation(line: 118, column: 14, scope: !53)
!299 = !DILocation(line: 118, column: 29, scope: !53)
!300 = !DILocation(line: 118, column: 34, scope: !53)
!301 = !DILocation(line: 120, column: 83, scope: !53)
!302 = !DILocation(line: 120, column: 68, scope: !53)
!303 = !DILocation(line: 120, column: 105, scope: !53)
!304 = !DILocation(line: 120, column: 90, scope: !53)
!305 = !DILocation(line: 120, column: 127, scope: !53)
!306 = !DILocation(line: 120, column: 112, scope: !53)
!307 = !DILocation(line: 120, column: 3, scope: !53)
!308 = !DILocation(line: 122, column: 46, scope: !53)
!309 = !DILocation(line: 122, column: 25, scope: !53)
!310 = !DILocation(line: 122, column: 3, scope: !53)
!311 = !DILocation(line: 122, column: 23, scope: !53)
!312 = !{!313, !313, i64 0}
!313 = !{!"long", !160, i64 0}
!314 = !DILocation(line: 123, column: 46, scope: !53)
!315 = !DILocation(line: 123, column: 25, scope: !53)
!316 = !DILocation(line: 123, column: 3, scope: !53)
!317 = !DILocation(line: 123, column: 23, scope: !53)
!318 = !DILocation(line: 124, column: 46, scope: !53)
!319 = !DILocation(line: 124, column: 25, scope: !53)
!320 = !DILocation(line: 124, column: 3, scope: !53)
!321 = !DILocation(line: 124, column: 23, scope: !53)
!322 = !DILocation(line: 128, column: 1, scope: !53)
!323 = !DILocation(line: 127, column: 3, scope: !53)
!324 = !DISubprogram(name: "fgets", scope: !325, file: !325, line: 564, type: !326, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!325 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdio.h", directory: "")
!326 = !DISubroutineType(types: !327)
!327 = !{!13, !13, !11, !83}
!328 = !DISubprogram(name: "fclose", scope: !325, file: !325, line: 213, type: !329, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!329 = !DISubroutineType(types: !330)
!330 = !{!11, !83}
!331 = distinct !DISubprogram(name: "configureCGRA", scope: !54, file: !54, line: 130, type: !55, scopeLine: 131, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !332)
!332 = !{!333, !334, !335, !336, !337, !338, !339, !342}
!333 = !DILocalVariable(name: "loopID", arg: 1, scope: !331, file: !54, line: 130, type: !19)
!334 = !DILocalVariable(name: "i", scope: !331, file: !54, line: 133, type: !19)
!335 = !DILocalVariable(name: "kernelCount", scope: !331, file: !54, line: 134, type: !11)
!336 = !DILocalVariable(name: "count", scope: !331, file: !54, line: 135, type: !61)
!337 = !DILocalVariable(name: "loopno", scope: !331, file: !54, line: 136, type: !126)
!338 = !DILocalVariable(name: "directoryPath", scope: !331, file: !54, line: 137, type: !130)
!339 = !DILocalVariable(name: "newTC", scope: !340, file: !54, line: 144, type: !11)
!340 = distinct !DILexicalBlock(scope: !341, file: !54, line: 143, column: 3)
!341 = distinct !DILexicalBlock(scope: !331, file: !54, line: 142, column: 6)
!342 = !DILocalVariable(name: "initCGRAfile", scope: !331, file: !54, line: 148, type: !118)
!343 = !DILocation(line: 0, scope: !331)
!344 = !DILocation(line: 132, column: 13, scope: !345)
!345 = distinct !DILexicalBlock(scope: !331, file: !54, line: 132, column: 6)
!346 = !DILocation(line: 134, column: 23, scope: !331)
!347 = !DILocation(line: 134, column: 43, scope: !331)
!348 = !DILocation(line: 134, column: 35, scope: !331)
!349 = !DILocation(line: 134, column: 32, scope: !331)
!350 = !DILocation(line: 134, column: 47, scope: !331)
!351 = !DILocation(line: 134, column: 21, scope: !331)
!352 = !DILocation(line: 136, column: 3, scope: !331)
!353 = !DILocation(line: 136, column: 8, scope: !331)
!354 = !DILocation(line: 137, column: 3, scope: !331)
!355 = !DILocation(line: 137, column: 8, scope: !331)
!356 = !DILocation(line: 139, column: 3, scope: !331)
!357 = !DILocation(line: 140, column: 3, scope: !331)
!358 = !DILocation(line: 142, column: 18, scope: !341)
!359 = !DILocation(line: 142, column: 6, scope: !331)
!360 = !DILocation(line: 144, column: 31, scope: !340)
!361 = !DILocation(line: 144, column: 29, scope: !340)
!362 = !DILocation(line: 0, scope: !340)
!363 = !DILocation(line: 145, column: 7, scope: !340)
!364 = !DILocation(line: 145, column: 16, scope: !340)
!365 = !DILocation(line: 145, column: 31, scope: !340)
!366 = !DILocation(line: 145, column: 36, scope: !340)
!367 = !DILocation(line: 146, column: 3, scope: !340)
!368 = !DILocation(line: 148, column: 3, scope: !331)
!369 = !DILocation(line: 148, column: 8, scope: !331)
!370 = !DILocation(line: 149, column: 3, scope: !331)
!371 = !DILocation(line: 150, column: 11, scope: !331)
!372 = !DILocation(line: 151, column: 3, scope: !373)
!373 = distinct !DILexicalBlock(scope: !331, file: !54, line: 151, column: 3)
!374 = !DILocation(line: 152, column: 30, scope: !375)
!375 = distinct !DILexicalBlock(scope: !373, file: !54, line: 151, column: 3)
!376 = !DILocation(line: 152, column: 39, scope: !375)
!377 = !DILocation(line: 152, column: 54, scope: !375)
!378 = !DILocation(line: 152, column: 28, scope: !375)
!379 = !DILocation(line: 152, column: 5, scope: !375)
!380 = !DILocation(line: 151, column: 18, scope: !375)
!381 = !DILocation(line: 151, column: 13, scope: !375)
!382 = distinct !{!382, !372, !383, !260, !261}
!383 = !DILocation(line: 152, column: 58, scope: !373)
!384 = !DILocation(line: 153, column: 48, scope: !331)
!385 = !DILocation(line: 153, column: 27, scope: !331)
!386 = !DILocation(line: 153, column: 3, scope: !331)
!387 = !DILocation(line: 154, column: 48, scope: !331)
!388 = !DILocation(line: 154, column: 27, scope: !331)
!389 = !DILocation(line: 154, column: 3, scope: !331)
!390 = !DILocation(line: 155, column: 48, scope: !331)
!391 = !DILocation(line: 155, column: 27, scope: !331)
!392 = !DILocation(line: 155, column: 3, scope: !331)
!393 = !DILocation(line: 156, column: 3, scope: !331)
!394 = !DILocation(line: 160, column: 1, scope: !331)
!395 = !DILocation(line: 159, column: 3, scope: !331)
!396 = distinct !DISubprogram(name: "checkTotalLoops", scope: !54, file: !54, line: 162, type: !397, scopeLine: 163, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !399)
!397 = !DISubroutineType(types: !398)
!398 = !{null}
!399 = !{!400, !401}
!400 = !DILocalVariable(name: "myfile", scope: !396, file: !54, line: 165, type: !118)
!401 = !DILocalVariable(name: "count", scope: !396, file: !54, line: 166, type: !61)
!402 = !DILocation(line: 164, column: 15, scope: !403)
!403 = distinct !DILexicalBlock(scope: !396, file: !54, line: 164, column: 8)
!404 = !DILocation(line: 165, column: 5, scope: !396)
!405 = !DILocation(line: 165, column: 10, scope: !396)
!406 = !DILocation(line: 167, column: 13, scope: !396)
!407 = !DILocation(line: 0, scope: !396)
!408 = !DILocation(line: 168, column: 5, scope: !396)
!409 = !DILocation(line: 169, column: 5, scope: !396)
!410 = !DILocation(line: 172, column: 1, scope: !396)
!411 = distinct !DISubprogram(name: "runOnCGRA", scope: !54, file: !54, line: 174, type: !412, scopeLine: 175, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !414)
!412 = !DISubroutineType(types: !413)
!413 = !{!9, !9}
!414 = !{!415}
!415 = !DILocalVariable(name: "arg", arg: 1, scope: !411, file: !54, line: 174, type: !9)
!416 = !DILocation(line: 0, scope: !411)
!417 = !DILocation(line: 176, column: 15, scope: !418)
!418 = distinct !DILexicalBlock(scope: !411, file: !54, line: 176, column: 8)
!419 = !DILocation(line: 177, column: 5, scope: !411)
!420 = !DILocation(line: 178, column: 15, scope: !421)
!421 = distinct !DILexicalBlock(scope: !411, file: !54, line: 177, column: 14)
!422 = !DILocation(line: 178, column: 32, scope: !421)
!423 = !DILocation(line: 178, column: 9, scope: !421)
!424 = !DILocation(line: 178, column: 38, scope: !421)
!425 = distinct !{!425, !423, !426, !260, !261}
!426 = !DILocation(line: 178, column: 46, scope: !421)
!427 = !DILocation(line: 180, column: 12, scope: !428)
!428 = distinct !DILexicalBlock(scope: !421, file: !54, line: 180, column: 12)
!429 = !DILocation(line: 180, column: 24, scope: !428)
!430 = !DILocation(line: 180, column: 12, scope: !421)
!431 = !DILocation(line: 181, column: 7, scope: !432)
!432 = distinct !DILexicalBlock(scope: !428, file: !54, line: 180, column: 30)
!433 = !{i32 5320}
!434 = !DILocation(line: 191, column: 3, scope: !411)
!435 = !DILocation(line: 185, column: 19, scope: !421)
!436 = !DILocation(line: 186, column: 2, scope: !421)
!437 = !{i32 5428}
!438 = !DILocation(line: 187, column: 2, scope: !421)
!439 = !DILocation(line: 188, column: 18, scope: !421)
!440 = distinct !{!440, !419, !441, !261}
!441 = !DILocation(line: 189, column: 5, scope: !411)
!442 = !DISubprogram(name: "usleep", scope: !443, file: !443, line: 460, type: !55, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!443 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/unistd.h", directory: "")
!444 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !54, file: !54, line: 194, type: !445, scopeLine: 195, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !447)
!445 = !DISubroutineType(types: !446)
!446 = !{null, !19}
!447 = !{!448, !449, !450}
!448 = !DILocalVariable(name: "loopNo", arg: 1, scope: !444, file: !54, line: 194, type: !19)
!449 = !DILocalVariable(name: "result", scope: !444, file: !54, line: 197, type: !11)
!450 = !DILocalVariable(name: "configure", scope: !444, file: !54, line: 199, type: !11)
!451 = !DILocation(line: 0, scope: !444)
!452 = !DILocation(line: 196, column: 15, scope: !453)
!453 = distinct !DILexicalBlock(scope: !444, file: !54, line: 196, column: 8)
!454 = !DILocation(line: 199, column: 21, scope: !444)
!455 = !DILocation(line: 204, column: 5, scope: !444)
!456 = !DILocation(line: 206, column: 21, scope: !444)
!457 = !DILocation(line: 207, column: 22, scope: !444)
!458 = !DILocation(line: 209, column: 5, scope: !444)
!459 = !DILocation(line: 209, column: 33, scope: !444)
!460 = !DILocation(line: 209, column: 11, scope: !444)
!461 = !DILocation(line: 209, column: 27, scope: !444)
!462 = distinct !{!462, !458, !463, !260, !261}
!463 = !DILocation(line: 209, column: 41, scope: !444)
!464 = !DILocation(line: 212, column: 1, scope: !444)
!465 = distinct !DISubprogram(name: "deleteCGRA", scope: !54, file: !54, line: 214, type: !397, scopeLine: 215, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!466 = !DILocation(line: 216, column: 13, scope: !467)
!467 = distinct !DILexicalBlock(scope: !465, file: !54, line: 216, column: 6)
!468 = !DILocation(line: 218, column: 15, scope: !465)
!469 = !DILocation(line: 219, column: 20, scope: !465)
!470 = !DILocation(line: 221, column: 3, scope: !465)
!471 = !DILocation(line: 222, column: 16, scope: !465)
!472 = !DILocation(line: 222, column: 3, scope: !465)
!473 = !DILocation(line: 223, column: 13, scope: !474)
!474 = distinct !DILexicalBlock(scope: !465, file: !54, line: 223, column: 6)
!475 = !DILocation(line: 224, column: 1, scope: !465)
!476 = !DISubprogram(name: "pthread_join", scope: !477, file: !477, line: 215, type: !478, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!477 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/pthread.h", directory: "")
!478 = !DISubroutineType(types: !479)
!479 = !{!11, !8, !480}
!480 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !9, size: 32)
!481 = distinct !DISubprogram(name: "createCGRA", scope: !54, file: !54, line: 227, type: !397, scopeLine: 228, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !482)
!482 = !{!483, !484}
!483 = !DILocalVariable(name: "result", scope: !481, file: !54, line: 230, type: !11)
!484 = !DILocalVariable(name: "i", scope: !481, file: !54, line: 231, type: !19)
!485 = !DILocation(line: 229, column: 13, scope: !486)
!486 = distinct !DILexicalBlock(scope: !481, file: !54, line: 229, column: 6)
!487 = !DILocation(line: 0, scope: !481)
!488 = !DILocation(line: 164, column: 15, scope: !403, inlinedAt: !489)
!489 = distinct !DILocation(line: 233, column: 3, scope: !481)
!490 = !DILocation(line: 165, column: 5, scope: !396, inlinedAt: !489)
!491 = !DILocation(line: 165, column: 10, scope: !396, inlinedAt: !489)
!492 = !DILocation(line: 167, column: 13, scope: !396, inlinedAt: !489)
!493 = !DILocation(line: 0, scope: !396, inlinedAt: !489)
!494 = !DILocation(line: 168, column: 5, scope: !396, inlinedAt: !489)
!495 = !DILocation(line: 169, column: 5, scope: !396, inlinedAt: !489)
!496 = !DILocation(line: 172, column: 1, scope: !396, inlinedAt: !489)
!497 = !DILocation(line: 234, column: 43, scope: !481)
!498 = !DILocation(line: 234, column: 42, scope: !481)
!499 = !DILocation(line: 234, column: 22, scope: !481)
!500 = !DILocation(line: 234, column: 13, scope: !481)
!501 = !DILocation(line: 235, column: 40, scope: !481)
!502 = !DILocation(line: 235, column: 22, scope: !481)
!503 = !DILocation(line: 235, column: 13, scope: !481)
!504 = !DILocation(line: 236, column: 22, scope: !481)
!505 = !DILocation(line: 236, column: 13, scope: !481)
!506 = !DILocation(line: 237, column: 22, scope: !481)
!507 = !DILocation(line: 237, column: 13, scope: !481)
!508 = !DILocation(line: 240, column: 16, scope: !509)
!509 = distinct !DILexicalBlock(scope: !510, file: !54, line: 240, column: 3)
!510 = distinct !DILexicalBlock(scope: !481, file: !54, line: 240, column: 3)
!511 = !DILocation(line: 240, column: 3, scope: !510)
!512 = !DILocation(line: 241, column: 7, scope: !509)
!513 = !DILocation(line: 240, column: 32, scope: !509)
!514 = !DILocation(line: 240, column: 19, scope: !509)
!515 = distinct !{!515, !511, !516, !260, !261}
!516 = !DILocation(line: 241, column: 29, scope: !510)
!517 = !DILocation(line: 243, column: 3, scope: !481)
!518 = !DILocation(line: 244, column: 3, scope: !481)
!519 = !{i32 6755}
!520 = !DILocation(line: 245, column: 12, scope: !481)
!521 = !DILocation(line: 247, column: 1, scope: !481)
!522 = !DISubprogram(name: "pthread_create", scope: !477, file: !477, line: 198, type: !523, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!523 = !DISubroutineType(types: !524)
!524 = !{!11, !28, !525, !534, !9}
!525 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !526, size: 32)
!526 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !527)
!527 = distinct !DICompositeType(tag: DW_TAG_union_type, name: "pthread_attr_t", file: !47, line: 56, size: 288, elements: !528)
!528 = !{!529, !533}
!529 = !DIDerivedType(tag: DW_TAG_member, name: "__size", scope: !527, file: !47, line: 58, baseType: !530, size: 288)
!530 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 288, elements: !531)
!531 = !{!532}
!532 = !DISubrange(count: 36)
!533 = !DIDerivedType(tag: DW_TAG_member, name: "__align", scope: !527, file: !47, line: 59, baseType: !89, size: 32)
!534 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !412, size: 32)
