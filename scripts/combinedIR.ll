; ModuleID = '/home/mahesh/git_repos/ccf/scripts/CGRALib/cgra.c'
source_filename = "/home/mahesh/git_repos/ccf/scripts/CGRALib/cgra.c"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%union.pthread_attr_t = type { i32, [32 x i8] }

@totalLoops = global i32 0, align 4, !dbg !0
@thread_cond_cpu = local_unnamed_addr global i32 1, align 4, !dbg !13
@thread_cond_cgra = local_unnamed_addr global i32 0, align 4, !dbg !16
@thread_exit = local_unnamed_addr global i32 0, align 4, !dbg !18
@dynamicTCVal = local_unnamed_addr global i32 0, align 4, !dbg !20
@initCGRA = common local_unnamed_addr global i32* null, align 4, !dbg !37
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [16 x i8] c"/epilog_ins.bin\00", align 1
@.str.2 = private unnamed_addr constant [16 x i8] c"/prolog_ins.bin\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"/kernel_ins.bin\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.5 = private unnamed_addr constant [32 x i8] c"\0A**********EPISIZE %d*********\0A\00", align 1
@.str.6 = private unnamed_addr constant [32 x i8] c"\0A**********PROSIZE %d*********\0A\00", align 1
@.str.7 = private unnamed_addr constant [33 x i8] c"\0A**********KERNSIZE %d*********\0A\00", align 1
@.str.8 = private unnamed_addr constant [37 x i8] c"\0A******SIZE OF UNISGNED INT %d*****\0A\00", align 1
@epilog = common local_unnamed_addr global i32* null, align 4, !dbg !24
@prolog = common local_unnamed_addr global i32* null, align 4, !dbg !22
@kernel = common local_unnamed_addr global i32* null, align 4, !dbg !26
@.str.9 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@.str.10 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.11 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.12 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.13 = private unnamed_addr constant [52 x i8] c"From FILE: PROLOGPC= %x, EPILOGPC=%x,  KernelPC=%x\0A\00", align 1
@prologPtr = common local_unnamed_addr global i32* null, align 4, !dbg !30
@epilogPtr = common local_unnamed_addr global i32* null, align 4, !dbg !35
@kernelPtr = common local_unnamed_addr global i32* null, align 4, !dbg !33
@configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
@configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.14 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.15 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.17 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.18 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@.str.20 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@pth = common global i32 0, align 4, !dbg !39
@ArrPtr = common local_unnamed_addr global i32* null, align 4, !dbg !28
@str = private unnamed_addr constant [14 x i8] c"deleting cgra\00"
@str.24 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00"
@str.25 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00"

; Function Attrs: nounwind
define i32 @initializeParameters(i32) local_unnamed_addr #0 !dbg !49 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  %5 = alloca [40 x i8], align 1
  %6 = alloca [40 x i8], align 1
  %7 = alloca [40 x i8], align 1
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca [40 x i8], align 1
  %13 = alloca i32, align 4
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !53, metadata !141), !dbg !142
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !54, metadata !141), !dbg !143
  %14 = load i32*, i32** @initCGRA, align 4, !dbg !144, !tbaa !145
  %15 = add i32 %0, -1, !dbg !149
  %16 = mul i32 %15, 7, !dbg !150
  %17 = getelementptr inbounds i32, i32* %14, i32 %16, !dbg !151
  store i32 2011168768, i32* %17, align 4, !dbg !152, !tbaa !153
  %18 = getelementptr inbounds i32, i32* %17, i32 1, !dbg !155
  store i32 2011168768, i32* %18, align 4, !dbg !156, !tbaa !153
  %19 = getelementptr inbounds i32, i32* %17, i32 2, !dbg !157
  %20 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0, !dbg !158
  %21 = bitcast i32* %19 to i8*, !dbg !158
  call void @llvm.memset.p0i8.i64(i8* %21, i8 0, i64 20, i32 4, i1 false), !dbg !159
  call void @llvm.lifetime.start(i64 25, i8* nonnull %20) #6, !dbg !158
  tail call void @llvm.dbg.declare(metadata [25 x i8]* %2, metadata !120, metadata !141), !dbg !160
  %22 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0, !dbg !161
  call void @llvm.lifetime.start(i64 20, i8* nonnull %22) #6, !dbg !161
  tail call void @llvm.dbg.declare(metadata [20 x i8]* %3, metadata !124, metadata !141), !dbg !162
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %22, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false), !dbg !162
  %23 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %20, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %0) #6, !dbg !163
  %24 = call i8* @strcat(i8* nonnull %22, i8* nonnull %20) #6, !dbg !164
  %25 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0, !dbg !165
  call void @llvm.lifetime.start(i64 40, i8* nonnull %25) #6, !dbg !165
  call void @llvm.dbg.declare(metadata [40 x i8]* %4, metadata !128, metadata !141), !dbg !166
  call void @llvm.memset.p0i8.i32(i8* nonnull %25, i8 0, i32 40, i32 1, i1 false), !dbg !166
  %26 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0, !dbg !167
  call void @llvm.lifetime.start(i64 40, i8* nonnull %26) #6, !dbg !167
  call void @llvm.dbg.declare(metadata [40 x i8]* %5, metadata !129, metadata !141), !dbg !168
  call void @llvm.memset.p0i8.i32(i8* nonnull %26, i8 0, i32 40, i32 1, i1 false), !dbg !168
  %27 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0, !dbg !169
  call void @llvm.lifetime.start(i64 40, i8* nonnull %27) #6, !dbg !169
  call void @llvm.dbg.declare(metadata [40 x i8]* %6, metadata !130, metadata !141), !dbg !170
  call void @llvm.memset.p0i8.i32(i8* nonnull %27, i8 0, i32 40, i32 1, i1 false), !dbg !170
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0, !dbg !171
  call void @llvm.lifetime.start(i64 40, i8* nonnull %28) #6, !dbg !171
  call void @llvm.dbg.declare(metadata [40 x i8]* %7, metadata !131, metadata !141), !dbg !172
  call void @llvm.memset.p0i8.i32(i8* nonnull %28, i8 0, i32 40, i32 1, i1 false), !dbg !172
  %29 = call i8* @strcat(i8* nonnull %25, i8* nonnull %22) #6, !dbg !173
  %30 = call i32 @strlen(i8* nonnull %25), !dbg !174
  %31 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %30, !dbg !174
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %31, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1, i32 0, i32 0), i32 16, i32 1, i1 false), !dbg !174
  %32 = call i8* @strcat(i8* nonnull %26, i8* nonnull %22) #6, !dbg !175
  %33 = call i32 @strlen(i8* nonnull %26), !dbg !176
  %34 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %33, !dbg !176
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %34, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 16, i32 1, i1 false), !dbg !176
  %35 = call i8* @strcat(i8* nonnull %27, i8* nonnull %22) #6, !dbg !177
  %36 = call i32 @strlen(i8* nonnull %27), !dbg !178
  %37 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %36, !dbg !178
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %37, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 16, i32 1, i1 false), !dbg !178
  %38 = call %struct._IO_FILE* @fopen(i8* nonnull %25, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i32 0, i32 0)), !dbg !179
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %38, i64 0, metadata !55, metadata !141), !dbg !180
  %39 = call %struct._IO_FILE* @fopen(i8* nonnull %26, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i32 0, i32 0)), !dbg !181
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %39, i64 0, metadata !117, metadata !141), !dbg !182
  %40 = call %struct._IO_FILE* @fopen(i8* nonnull %27, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i32 0, i32 0)), !dbg !183
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %40, i64 0, metadata !118, metadata !141), !dbg !184
  %41 = bitcast i32* %8 to i8*, !dbg !185
  call void @llvm.lifetime.start(i64 4, i8* nonnull %41) #6, !dbg !185
  %42 = bitcast i32* %9 to i8*, !dbg !185
  call void @llvm.lifetime.start(i64 4, i8* nonnull %42) #6, !dbg !185
  %43 = bitcast i32* %10 to i8*, !dbg !185
  call void @llvm.lifetime.start(i64 4, i8* nonnull %43) #6, !dbg !185
  %44 = bitcast i32* %11 to i8*, !dbg !185
  call void @llvm.lifetime.start(i64 4, i8* nonnull %44) #6, !dbg !185
  %45 = call i32 @fread(i8* nonnull %41, i32 4, i32 1, %struct._IO_FILE* %38), !dbg !186
  %46 = call i32 @fread(i8* nonnull %42, i32 4, i32 1, %struct._IO_FILE* %39), !dbg !187
  %47 = call i32 @fread(i8* nonnull %43, i32 4, i32 1, %struct._IO_FILE* %40), !dbg !188
  %48 = load i32, i32* %8, align 4, !dbg !189, !tbaa !153
  call void @llvm.dbg.value(metadata i32 %48, i64 0, metadata !132, metadata !141), !dbg !190
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5, i32 0, i32 0), i32 %48), !dbg !191
  %50 = load i32, i32* %9, align 4, !dbg !192, !tbaa !153
  call void @llvm.dbg.value(metadata i32 %50, i64 0, metadata !133, metadata !141), !dbg !193
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6, i32 0, i32 0), i32 %50), !dbg !194
  %52 = load i32, i32* %10, align 4, !dbg !195, !tbaa !153
  call void @llvm.dbg.value(metadata i32 %52, i64 0, metadata !134, metadata !141), !dbg !196
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7, i32 0, i32 0), i32 %52), !dbg !197
  %54 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8, i32 0, i32 0), i32 4), !dbg !198
  call void @llvm.dbg.value(metadata i32 %48, i64 0, metadata !132, metadata !141), !dbg !190
  %55 = shl i32 %48, 2, !dbg !199
  %56 = call noalias i8* @malloc(i32 %55) #6, !dbg !200
  store i8* %56, i8** bitcast (i32** @epilog to i8**), align 4, !dbg !201, !tbaa !145
  call void @llvm.dbg.value(metadata i32 %50, i64 0, metadata !133, metadata !141), !dbg !193
  %57 = shl i32 %50, 2, !dbg !202
  %58 = call noalias i8* @malloc(i32 %57) #6, !dbg !203
  store i8* %58, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !204, !tbaa !145
  call void @llvm.dbg.value(metadata i32 %52, i64 0, metadata !134, metadata !141), !dbg !196
  %59 = shl i32 %52, 2, !dbg !205
  %60 = call noalias i8* @malloc(i32 %59) #6, !dbg !206
  store i8* %60, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !207, !tbaa !145
  call void @llvm.dbg.value(metadata i32 %48, i64 0, metadata !132, metadata !141), !dbg !190
  %61 = call i32 @fread(i8* %56, i32 4, i32 %48, %struct._IO_FILE* %38), !dbg !208
  %62 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !209, !tbaa !145
  call void @llvm.dbg.value(metadata i32 %50, i64 0, metadata !133, metadata !141), !dbg !193
  %63 = call i32 @fread(i8* %62, i32 4, i32 %50, %struct._IO_FILE* %39), !dbg !210
  %64 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !211, !tbaa !145
  call void @llvm.dbg.value(metadata i32 %52, i64 0, metadata !134, metadata !141), !dbg !196
  %65 = call i32 @fread(i8* %64, i32 4, i32 %52, %struct._IO_FILE* %40), !dbg !212
  %66 = call i8* @strcat(i8* nonnull %28, i8* nonnull %22) #6, !dbg !213
  %67 = call i32 @strlen(i8* nonnull %28), !dbg !214
  %68 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %67, !dbg !214
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %68, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.9, i32 0, i32 0), i32 26, i32 1, i1 false), !dbg !214
  %69 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10, i32 0, i32 0)), !dbg !215
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %69, i64 0, metadata !119, metadata !141), !dbg !216
  call void @llvm.dbg.value(metadata i32* %11, i64 0, metadata !135, metadata !217), !dbg !218
  %70 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %69, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* nonnull %11) #6, !dbg !219
  call void @llvm.dbg.value(metadata i32 %52, i64 0, metadata !134, metadata !141), !dbg !196
  %71 = sdiv i32 %52, 16, !dbg !220
  call void @llvm.dbg.value(metadata i32 %71, i64 0, metadata !136, metadata !141), !dbg !221
  call void @llvm.dbg.value(metadata i32 %48, i64 0, metadata !132, metadata !141), !dbg !190
  %72 = sdiv i32 %48, 16, !dbg !222
  call void @llvm.dbg.value(metadata i32 %72, i64 0, metadata !137, metadata !141), !dbg !223
  call void @llvm.dbg.value(metadata i32 %50, i64 0, metadata !133, metadata !141), !dbg !193
  %73 = sdiv i32 %50, 16, !dbg !224
  call void @llvm.dbg.value(metadata i32 %73, i64 0, metadata !138, metadata !141), !dbg !225
  %74 = load i32*, i32** @initCGRA, align 4, !dbg !226, !tbaa !145
  %75 = getelementptr inbounds i32, i32* %74, i32 %16, !dbg !227
  %76 = getelementptr inbounds i32, i32* %75, i32 2, !dbg !228
  store i32 %71, i32* %76, align 4, !dbg !229, !tbaa !153
  %77 = getelementptr inbounds i32, i32* %75, i32 3, !dbg !230
  store i32 %72, i32* %77, align 4, !dbg !231, !tbaa !153
  %78 = getelementptr inbounds i32, i32* %75, i32 4, !dbg !232
  store i32 %73, i32* %78, align 4, !dbg !233, !tbaa !153
  %79 = load i32, i32* %11, align 4, !dbg !234, !tbaa !153
  call void @llvm.dbg.value(metadata i32 %79, i64 0, metadata !135, metadata !141), !dbg !218
  %80 = sdiv i32 %79, 16, !dbg !235
  %81 = getelementptr inbounds i32, i32* %75, i32 6, !dbg !236
  store i32 %80, i32* %81, align 4, !dbg !237, !tbaa !153
  %82 = call i32 @fclose(%struct._IO_FILE* %38), !dbg !238
  %83 = call i32 @fclose(%struct._IO_FILE* %39), !dbg !239
  %84 = call i32 @fclose(%struct._IO_FILE* %40), !dbg !240
  %85 = call i32 @fclose(%struct._IO_FILE* %69), !dbg !241
  %86 = getelementptr inbounds [40 x i8], [40 x i8]* %12, i32 0, i32 0, !dbg !242
  call void @llvm.lifetime.start(i64 40, i8* nonnull %86) #6, !dbg !242
  call void @llvm.dbg.declare(metadata [40 x i8]* %12, metadata !139, metadata !141), !dbg !243
  call void @llvm.memset.p0i8.i32(i8* nonnull %86, i8 0, i32 40, i32 1, i1 false), !dbg !243
  %87 = call i8* @strcat(i8* nonnull %86, i8* nonnull %22) #6, !dbg !244
  %88 = call i32 @strlen(i8* nonnull %86), !dbg !245
  %89 = getelementptr [40 x i8], [40 x i8]* %12, i32 0, i32 %88, !dbg !245
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %89, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.11, i32 0, i32 0), i32 18, i32 1, i1 false), !dbg !245
  %90 = bitcast i32* %13 to i8*, !dbg !246
  call void @llvm.lifetime.start(i64 4, i8* nonnull %90) #6, !dbg !246
  call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !140, metadata !141), !dbg !247
  store i32 0, i32* %13, align 4, !dbg !247, !tbaa !153
  %91 = call %struct._IO_FILE* @fopen(i8* nonnull %86, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10, i32 0, i32 0)), !dbg !248
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %91, i64 0, metadata !119, metadata !141), !dbg !216
  call void @llvm.dbg.value(metadata i32* %13, i64 0, metadata !140, metadata !217), !dbg !247
  %92 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %91, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* nonnull %13) #6, !dbg !249
  %93 = call i32 @fclose(%struct._IO_FILE* %91), !dbg !250
  %94 = load i32, i32* %13, align 4, !dbg !251, !tbaa !153
  call void @llvm.dbg.value(metadata i32 %94, i64 0, metadata !140, metadata !141), !dbg !247
  %95 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %94), !dbg !252
  %96 = load i32, i32* %13, align 4, !dbg !253, !tbaa !153
  call void @llvm.dbg.value(metadata i32 %96, i64 0, metadata !140, metadata !141), !dbg !247
  %97 = load i32*, i32** @initCGRA, align 4, !dbg !254, !tbaa !145
  %98 = getelementptr inbounds i32, i32* %97, i32 %16, !dbg !255
  %99 = getelementptr inbounds i32, i32* %98, i32 5, !dbg !256
  store i32 %96, i32* %99, align 4, !dbg !257, !tbaa !153
  %100 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !dbg !258, !tbaa !145
  %101 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !dbg !259, !tbaa !145
  %102 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !dbg !260, !tbaa !145
  %103 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.13, i32 0, i32 0), i32 %100, i32 %101, i32 %102), !dbg !261
  %104 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !dbg !262, !tbaa !145
  %105 = load i32*, i32** @prologPtr, align 4, !dbg !263, !tbaa !145
  %106 = getelementptr inbounds i32, i32* %105, i32 %15, !dbg !263
  store i32 %104, i32* %106, align 4, !dbg !264, !tbaa !265
  %107 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !dbg !267, !tbaa !145
  %108 = load i32*, i32** @epilogPtr, align 4, !dbg !268, !tbaa !145
  %109 = getelementptr inbounds i32, i32* %108, i32 %15, !dbg !268
  store i32 %107, i32* %109, align 4, !dbg !269, !tbaa !265
  %110 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !dbg !270, !tbaa !145
  %111 = load i32*, i32** @kernelPtr, align 4, !dbg !271, !tbaa !145
  %112 = getelementptr inbounds i32, i32* %111, i32 %15, !dbg !271
  store i32 %110, i32* %112, align 4, !dbg !272, !tbaa !265
  call void @llvm.lifetime.end(i64 4, i8* nonnull %90) #6, !dbg !273
  call void @llvm.lifetime.end(i64 40, i8* nonnull %86) #6, !dbg !273
  call void @llvm.lifetime.end(i64 4, i8* nonnull %44) #6, !dbg !273
  call void @llvm.lifetime.end(i64 4, i8* nonnull %43) #6, !dbg !273
  call void @llvm.lifetime.end(i64 4, i8* nonnull %42) #6, !dbg !273
  call void @llvm.lifetime.end(i64 4, i8* nonnull %41) #6, !dbg !273
  call void @llvm.lifetime.end(i64 40, i8* nonnull %28) #6, !dbg !273
  call void @llvm.lifetime.end(i64 40, i8* nonnull %27) #6, !dbg !273
  call void @llvm.lifetime.end(i64 40, i8* nonnull %26) #6, !dbg !273
  call void @llvm.lifetime.end(i64 40, i8* nonnull %25) #6, !dbg !273
  call void @llvm.lifetime.end(i64 20, i8* nonnull %22) #6, !dbg !273
  call void @llvm.lifetime.end(i64 25, i8* nonnull %20) #6, !dbg !273
  ret i32 0, !dbg !274
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture writeonly, i8* nocapture readonly, i32, i32, i1) #2

; Function Attrs: nounwind
declare i32 @sprintf(i8* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: nounwind
declare i8* @strcat(i8*, i8* nocapture readonly) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i32, i1) #2

; Function Attrs: nounwind
declare noalias %struct._IO_FILE* @fopen(i8* nocapture readonly, i8* nocapture readonly) local_unnamed_addr #3

; Function Attrs: nounwind
declare i32 @fread(i8* nocapture, i32, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #3

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: nounwind
declare noalias i8* @malloc(i32) local_unnamed_addr #3

declare i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #4

; Function Attrs: nounwind
declare i32 @fclose(%struct._IO_FILE* nocapture) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #2

; Function Attrs: nounwind
define i32 @configureCGRA(i32) local_unnamed_addr #0 !dbg !275 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !277, metadata !141), !dbg !287
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !278, metadata !141), !dbg !288
  %5 = load i32*, i32** @initCGRA, align 4, !dbg !289, !tbaa !145
  %6 = add i32 %0, -1, !dbg !290
  %7 = mul i32 %6, 7, !dbg !291
  %8 = getelementptr inbounds i32, i32* %5, i32 %7, !dbg !292
  %9 = getelementptr inbounds i32, i32* %8, i32 5, !dbg !293
  %10 = load i32, i32* %9, align 4, !dbg !294, !tbaa !153
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !279, metadata !141), !dbg !295
  %11 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0, !dbg !296
  call void @llvm.lifetime.start(i64 25, i8* nonnull %11) #6, !dbg !296
  tail call void @llvm.dbg.declare(metadata [25 x i8]* %2, metadata !281, metadata !141), !dbg !297
  %12 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0, !dbg !298
  call void @llvm.lifetime.start(i64 20, i8* nonnull %12) #6, !dbg !298
  tail call void @llvm.dbg.declare(metadata [20 x i8]* %3, metadata !282, metadata !141), !dbg !299
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %12, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false), !dbg !299
  %13 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %11, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %0) #6, !dbg !300
  %14 = call i8* @strcat(i8* nonnull %12, i8* nonnull %11) #6, !dbg !301
  %15 = icmp slt i32 %10, 1, !dbg !302
  br i1 %15, label %16, label %22, !dbg !303

; <label>:16:                                     ; preds = %1
  %17 = load i32, i32* @dynamicTCVal, align 4, !dbg !304, !tbaa !153
  %18 = add nsw i32 %17, %10, !dbg !305
  call void @llvm.dbg.value(metadata i32 %18, i64 0, metadata !283, metadata !141), !dbg !306
  %19 = load i32*, i32** @initCGRA, align 4, !dbg !307, !tbaa !145
  %20 = getelementptr inbounds i32, i32* %19, i32 %7, !dbg !308
  %21 = getelementptr inbounds i32, i32* %20, i32 5, !dbg !309
  store i32 %18, i32* %21, align 4, !dbg !310, !tbaa !153
  br label %22, !dbg !311

; <label>:22:                                     ; preds = %16, %1
  %23 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0, !dbg !312
  call void @llvm.lifetime.start(i64 40, i8* nonnull %23) #6, !dbg !312
  call void @llvm.dbg.declare(metadata [40 x i8]* %4, metadata !286, metadata !141), !dbg !313
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %23, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i32 1, i1 false), !dbg !313
  %24 = call i32 @strlen(i8* nonnull %23), !dbg !314
  %25 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %24, !dbg !314
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %25, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.14, i32 0, i32 0), i32 14, i32 1, i1 false), !dbg !314
  %26 = call %struct._IO_FILE* @fopen(i8* nonnull %23, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.15, i32 0, i32 0)), !dbg !315
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %26, i64 0, metadata !280, metadata !141), !dbg !316
  call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !278, metadata !141), !dbg !288
  call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !278, metadata !141), !dbg !288
  br label %27, !dbg !317

; <label>:27:                                     ; preds = %27, %22
  %28 = phi i32 [ 0, %22 ], [ %34, %27 ]
  %29 = load i32*, i32** @initCGRA, align 4, !dbg !319, !tbaa !145
  %30 = getelementptr inbounds i32, i32* %29, i32 %7, !dbg !321
  %31 = getelementptr inbounds i32, i32* %30, i32 %28, !dbg !322
  %32 = load i32, i32* %31, align 4, !dbg !323, !tbaa !153
  %33 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %32), !dbg !324
  %34 = add nuw nsw i32 %28, 1, !dbg !325
  call void @llvm.dbg.value(metadata i32 %34, i64 0, metadata !278, metadata !141), !dbg !288
  call void @llvm.dbg.value(metadata i32 %34, i64 0, metadata !278, metadata !141), !dbg !288
  %35 = icmp eq i32 %34, 7, !dbg !326
  br i1 %35, label %36, label %27, !dbg !317, !llvm.loop !327

; <label>:36:                                     ; preds = %27
  %37 = load i32*, i32** @epilogPtr, align 4, !dbg !329, !tbaa !145
  %38 = getelementptr inbounds i32, i32* %37, i32 %6, !dbg !329
  %39 = load i32, i32* %38, align 4, !dbg !329, !tbaa !265
  %40 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %39), !dbg !330
  %41 = load i32*, i32** @prologPtr, align 4, !dbg !331, !tbaa !145
  %42 = getelementptr inbounds i32, i32* %41, i32 %6, !dbg !331
  %43 = load i32, i32* %42, align 4, !dbg !331, !tbaa !265
  %44 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %43), !dbg !332
  %45 = load i32*, i32** @kernelPtr, align 4, !dbg !333, !tbaa !145
  %46 = getelementptr inbounds i32, i32* %45, i32 %6, !dbg !333
  %47 = load i32, i32* %46, align 4, !dbg !333, !tbaa !265
  %48 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %47), !dbg !334
  %49 = call i32 @fclose(%struct._IO_FILE* %26), !dbg !335
  call void @llvm.lifetime.end(i64 40, i8* nonnull %23) #6, !dbg !336
  call void @llvm.lifetime.end(i64 20, i8* nonnull %12) #6, !dbg !336
  call void @llvm.lifetime.end(i64 25, i8* nonnull %11) #6, !dbg !336
  ret i32 0, !dbg !337
}

; Function Attrs: nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: nounwind
define void @checkTotalLoops() local_unnamed_addr #0 !dbg !338 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0, !dbg !344
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #6, !dbg !344
  tail call void @llvm.dbg.declare(metadata [40 x i8]* %1, metadata !342, metadata !141), !dbg !345
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false), !dbg !345
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10, i32 0, i32 0)), !dbg !346
  tail call void @llvm.dbg.value(metadata %struct._IO_FILE* %3, i64 0, metadata !343, metadata !141), !dbg !347
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.18, i32 0, i32 0), i32* nonnull @totalLoops) #6, !dbg !348
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3), !dbg !349
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #6, !dbg !350
  ret void, !dbg !350
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #0 !dbg !351 {
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !355, metadata !141), !dbg !356
  br label %2, !dbg !357

; <label>:2:                                      ; preds = %1, %15
  %3 = load i32, i32* @thread_cond_cgra, align 4, !dbg !358, !tbaa !153
  %4 = icmp eq i32 %3, 1, !dbg !360
  br i1 %4, label %11, label %5, !dbg !361

; <label>:5:                                      ; preds = %2
  br label %6, !dbg !362

; <label>:6:                                      ; preds = %5, %6
  %7 = tail call i32 @usleep(i32 1) #6, !dbg !362
  %8 = load i32, i32* @thread_cond_cgra, align 4, !dbg !358, !tbaa !153
  %9 = icmp eq i32 %8, 1, !dbg !360
  br i1 %9, label %10, label %6, !dbg !361, !llvm.loop !363

; <label>:10:                                     ; preds = %6
  br label %11, !dbg !365

; <label>:11:                                     ; preds = %10, %2
  %12 = load i32, i32* @thread_exit, align 4, !dbg !365, !tbaa !153
  %13 = icmp eq i32 %12, 1, !dbg !367
  br i1 %13, label %14, label %15, !dbg !368

; <label>:14:                                     ; preds = %11
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #6, !dbg !369, !srcloc !371
  ret i8* null, !dbg !372

; <label>:15:                                     ; preds = %11
  store i32 0, i32* @thread_cond_cgra, align 4, !dbg !373, !tbaa !153
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #6, !dbg !374, !srcloc !375
  %16 = tail call i32 @usleep(i32 1) #6, !dbg !376
  store i32 1, i32* @thread_cond_cpu, align 4, !dbg !377, !tbaa !153
  br label %2, !dbg !357, !llvm.loop !378
}

declare i32 @usleep(i32) local_unnamed_addr #4

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #0 !dbg !380 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !384, metadata !141), !dbg !387
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !385, metadata !141), !dbg !388
  %2 = tail call i32 @configureCGRA(i32 %0), !dbg !389
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !386, metadata !141), !dbg !390
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.20, i32 0, i32 0), i32 %0), !dbg !391
  store i32 0, i32* @thread_cond_cpu, align 4, !dbg !392, !tbaa !153
  store i32 1, i32* @thread_cond_cgra, align 4, !dbg !393, !tbaa !153
  br label %4, !dbg !394

; <label>:4:                                      ; preds = %1, %4
  %5 = tail call i32 @usleep(i32 1) #6, !dbg !395
  %6 = load i32, i32* @thread_cond_cpu, align 4, !dbg !396, !tbaa !153
  %7 = icmp eq i32 %6, 1, !dbg !397
  br i1 %7, label %8, label %4, !dbg !394, !llvm.loop !398

; <label>:8:                                      ; preds = %4
  ret void, !dbg !400
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #0 !dbg !401 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0)), !dbg !402
  store i32 1, i32* @thread_exit, align 4, !dbg !403, !tbaa !153
  store i32 1, i32* @thread_cond_cgra, align 4, !dbg !404, !tbaa !153
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.24, i32 0, i32 0)), !dbg !405
  %3 = load i32, i32* @pth, align 4, !dbg !406, !tbaa !265
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #6, !dbg !407
  ret void, !dbg !408
}

declare i32 @pthread_join(i32, i8**) local_unnamed_addr #4

; Function Attrs: nounwind
define void @createCGRA() local_unnamed_addr #0 !dbg !409 {
  %1 = alloca [40 x i8], align 1
  tail call void @llvm.dbg.declare(metadata [40 x i8]* %1, metadata !342, metadata !141), !dbg !413
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !411, metadata !141), !dbg !415
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !412, metadata !141), !dbg !416
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0, !dbg !417
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #6, !dbg !417
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false) #6, !dbg !413
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10, i32 0, i32 0)) #6, !dbg !418
  tail call void @llvm.dbg.value(metadata %struct._IO_FILE* %3, i64 0, metadata !343, metadata !141) #6, !dbg !419
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.18, i32 0, i32 0), i32* nonnull @totalLoops) #6, !dbg !420
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3) #6, !dbg !421
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #6, !dbg !422
  %6 = load i32, i32* @totalLoops, align 4, !dbg !423, !tbaa !153
  %7 = mul i32 %6, 28, !dbg !424
  %8 = tail call noalias i8* @malloc(i32 %7) #6, !dbg !425
  store i8* %8, i8** bitcast (i32** @initCGRA to i8**), align 4, !dbg !426, !tbaa !145
  %9 = shl i32 %6, 2, !dbg !427
  %10 = tail call noalias i8* @malloc(i32 %9) #6, !dbg !428
  store i8* %10, i8** bitcast (i32** @prologPtr to i8**), align 4, !dbg !429, !tbaa !145
  %11 = tail call noalias i8* @malloc(i32 %9) #6, !dbg !430
  store i8* %11, i8** bitcast (i32** @kernelPtr to i8**), align 4, !dbg !431, !tbaa !145
  %12 = tail call noalias i8* @malloc(i32 %9) #6, !dbg !432
  store i8* %12, i8** bitcast (i32** @epilogPtr to i8**), align 4, !dbg !433, !tbaa !145
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !412, metadata !141), !dbg !416
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !412, metadata !141), !dbg !416
  %13 = icmp eq i32 %6, 0, !dbg !434
  br i1 %13, label %22, label %14, !dbg !437

; <label>:14:                                     ; preds = %0
  br label %15, !dbg !438

; <label>:15:                                     ; preds = %14, %15
  %16 = phi i32 [ %18, %15 ], [ 1, %14 ]
  %17 = tail call i32 @initializeParameters(i32 %16), !dbg !438
  %18 = add i32 %16, 1, !dbg !439
  tail call void @llvm.dbg.value(metadata i32 %18, i64 0, metadata !412, metadata !141), !dbg !416
  tail call void @llvm.dbg.value(metadata i32 %18, i64 0, metadata !412, metadata !141), !dbg !416
  %19 = load i32, i32* @totalLoops, align 4, !dbg !440, !tbaa !153
  %20 = icmp ugt i32 %18, %19, !dbg !434
  br i1 %20, label %21, label %15, !dbg !437, !llvm.loop !441

; <label>:21:                                     ; preds = %15
  br label %22, !dbg !443

; <label>:22:                                     ; preds = %21, %0
  %23 = tail call i32 @puts(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str.25, i32 0, i32 0)), !dbg !443
  %24 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #6, !dbg !444
  tail call void @llvm.dbg.value(metadata i32 %24, i64 0, metadata !411, metadata !141), !dbg !415
  ret void, !dbg !445
}

; Function Attrs: nounwind
declare i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #3

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata, metadata) #1

; Function Attrs: nounwind readonly
declare i32 @strlen(i8* nocapture) #5

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) #6

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #2

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind readonly }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!44, !45, !46, !47}
!llvm.ident = !{!48}

!0 = !DIGlobalVariableExpression(var: !1)
!1 = distinct !DIGlobalVariable(name: "totalLoops", scope: !2, file: !15, line: 29, type: !7, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !12)
!3 = !DIFile(filename: "/home/mahesh/git_repos/ccf/scripts/CGRALib/cgra.c", directory: "/home/mahesh/git_repos/ccf/scripts")
!4 = !{}
!5 = !{!6, !7, !8, !9, !10}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 32)
!7 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!8 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!10 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !11, size: 32)
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !{!0, !13, !16, !18, !20, !22, !24, !26, !28, !30, !33, !35, !37, !39}
!13 = !DIGlobalVariableExpression(var: !14)
!14 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !2, file: !15, line: 32, type: !7, isLocal: false, isDefinition: true)
!15 = !DIFile(filename: "/home/mahesh/git_repos/ccf/scripts/CGRALib/cgra.h", directory: "/home/mahesh/git_repos/ccf/scripts")
!16 = !DIGlobalVariableExpression(var: !17)
!17 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !2, file: !15, line: 33, type: !7, isLocal: false, isDefinition: true)
!18 = !DIGlobalVariableExpression(var: !19)
!19 = distinct !DIGlobalVariable(name: "thread_exit", scope: !2, file: !15, line: 34, type: !7, isLocal: false, isDefinition: true)
!20 = !DIGlobalVariableExpression(var: !21)
!21 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !2, file: !15, line: 36, type: !11, isLocal: false, isDefinition: true)
!22 = !DIGlobalVariableExpression(var: !23)
!23 = distinct !DIGlobalVariable(name: "prolog", scope: !2, file: !15, line: 23, type: !6, isLocal: false, isDefinition: true)
!24 = !DIGlobalVariableExpression(var: !25)
!25 = distinct !DIGlobalVariable(name: "epilog", scope: !2, file: !15, line: 23, type: !6, isLocal: false, isDefinition: true)
!26 = !DIGlobalVariableExpression(var: !27)
!27 = distinct !DIGlobalVariable(name: "kernel", scope: !2, file: !15, line: 23, type: !6, isLocal: false, isDefinition: true)
!28 = !DIGlobalVariableExpression(var: !29)
!29 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !2, file: !15, line: 24, type: !6, isLocal: false, isDefinition: true)
!30 = !DIGlobalVariableExpression(var: !31)
!31 = distinct !DIGlobalVariable(name: "prologPtr", scope: !2, file: !15, line: 26, type: !32, isLocal: false, isDefinition: true)
!32 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !8, size: 32)
!33 = !DIGlobalVariableExpression(var: !34)
!34 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !2, file: !15, line: 26, type: !32, isLocal: false, isDefinition: true)
!35 = !DIGlobalVariableExpression(var: !36)
!36 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !2, file: !15, line: 26, type: !32, isLocal: false, isDefinition: true)
!37 = !DIGlobalVariableExpression(var: !38)
!38 = distinct !DIGlobalVariable(name: "initCGRA", scope: !2, file: !15, line: 28, type: !10, isLocal: false, isDefinition: true)
!39 = !DIGlobalVariableExpression(var: !40)
!40 = distinct !DIGlobalVariable(name: "pth", scope: !2, file: !15, line: 31, type: !41, isLocal: false, isDefinition: true)
!41 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !42, line: 37, baseType: !43)
!42 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/mahesh/git_repos/ccf/scripts")
!43 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!44 = !{i32 2, !"Dwarf Version", i32 4}
!45 = !{i32 2, !"Debug Info Version", i32 3}
!46 = !{i32 1, !"wchar_size", i32 4}
!47 = !{i32 1, !"min_enum_size", i32 4}
!48 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!49 = distinct !DISubprogram(name: "initializeParameters", scope: !3, file: !3, line: 7, type: !50, isLocal: false, isDefinition: true, scopeLine: 8, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !52)
!50 = !DISubroutineType(types: !51)
!51 = !{!11, !7}
!52 = !{!53, !54, !55, !117, !118, !119, !120, !124, !128, !129, !130, !131, !132, !133, !134, !135, !136, !137, !138, !139, !140}
!53 = !DILocalVariable(name: "loopID", arg: 1, scope: !49, file: !3, line: 7, type: !7)
!54 = !DILocalVariable(name: "i", scope: !49, file: !3, line: 9, type: !7)
!55 = !DILocalVariable(name: "epi", scope: !49, file: !3, line: 19, type: !56)
!56 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !57, size: 32)
!57 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !58, line: 48, baseType: !59)
!58 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdio.h", directory: "/home/mahesh/git_repos/ccf/scripts")
!59 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !60, line: 245, size: 1216, elements: !61)
!60 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/libio.h", directory: "/home/mahesh/git_repos/ccf/scripts")
!61 = !{!62, !63, !66, !67, !68, !69, !70, !71, !72, !73, !74, !75, !76, !84, !85, !86, !87, !90, !92, !94, !98, !101, !105, !106, !107, !108, !109, !112, !113}
!62 = !DIDerivedType(tag: DW_TAG_member, name: "_flags", scope: !59, file: !60, line: 246, baseType: !11, size: 32)
!63 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_ptr", scope: !59, file: !60, line: 251, baseType: !64, size: 32, offset: 32)
!64 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !65, size: 32)
!65 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!66 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_end", scope: !59, file: !60, line: 252, baseType: !64, size: 32, offset: 64)
!67 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_base", scope: !59, file: !60, line: 253, baseType: !64, size: 32, offset: 96)
!68 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_base", scope: !59, file: !60, line: 254, baseType: !64, size: 32, offset: 128)
!69 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_ptr", scope: !59, file: !60, line: 255, baseType: !64, size: 32, offset: 160)
!70 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_end", scope: !59, file: !60, line: 256, baseType: !64, size: 32, offset: 192)
!71 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_base", scope: !59, file: !60, line: 257, baseType: !64, size: 32, offset: 224)
!72 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_end", scope: !59, file: !60, line: 258, baseType: !64, size: 32, offset: 256)
!73 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_base", scope: !59, file: !60, line: 260, baseType: !64, size: 32, offset: 288)
!74 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_backup_base", scope: !59, file: !60, line: 261, baseType: !64, size: 32, offset: 320)
!75 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_end", scope: !59, file: !60, line: 262, baseType: !64, size: 32, offset: 352)
!76 = !DIDerivedType(tag: DW_TAG_member, name: "_markers", scope: !59, file: !60, line: 264, baseType: !77, size: 32, offset: 384)
!77 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !78, size: 32)
!78 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_marker", file: !60, line: 160, size: 96, elements: !79)
!79 = !{!80, !81, !83}
!80 = !DIDerivedType(tag: DW_TAG_member, name: "_next", scope: !78, file: !60, line: 161, baseType: !77, size: 32)
!81 = !DIDerivedType(tag: DW_TAG_member, name: "_sbuf", scope: !78, file: !60, line: 162, baseType: !82, size: 32, offset: 32)
!82 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !59, size: 32)
!83 = !DIDerivedType(tag: DW_TAG_member, name: "_pos", scope: !78, file: !60, line: 166, baseType: !11, size: 32, offset: 64)
!84 = !DIDerivedType(tag: DW_TAG_member, name: "_chain", scope: !59, file: !60, line: 266, baseType: !82, size: 32, offset: 416)
!85 = !DIDerivedType(tag: DW_TAG_member, name: "_fileno", scope: !59, file: !60, line: 268, baseType: !11, size: 32, offset: 448)
!86 = !DIDerivedType(tag: DW_TAG_member, name: "_flags2", scope: !59, file: !60, line: 272, baseType: !11, size: 32, offset: 480)
!87 = !DIDerivedType(tag: DW_TAG_member, name: "_old_offset", scope: !59, file: !60, line: 274, baseType: !88, size: 32, offset: 512)
!88 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off_t", file: !89, line: 131, baseType: !8)
!89 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "/home/mahesh/git_repos/ccf/scripts")
!90 = !DIDerivedType(tag: DW_TAG_member, name: "_cur_column", scope: !59, file: !60, line: 278, baseType: !91, size: 16, offset: 544)
!91 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!92 = !DIDerivedType(tag: DW_TAG_member, name: "_vtable_offset", scope: !59, file: !60, line: 279, baseType: !93, size: 8, offset: 560)
!93 = !DIBasicType(name: "signed char", size: 8, encoding: DW_ATE_signed_char)
!94 = !DIDerivedType(tag: DW_TAG_member, name: "_shortbuf", scope: !59, file: !60, line: 280, baseType: !95, size: 8, offset: 568)
!95 = !DICompositeType(tag: DW_TAG_array_type, baseType: !65, size: 8, elements: !96)
!96 = !{!97}
!97 = !DISubrange(count: 1)
!98 = !DIDerivedType(tag: DW_TAG_member, name: "_lock", scope: !59, file: !60, line: 284, baseType: !99, size: 32, offset: 576)
!99 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !100, size: 32)
!100 = !DIDerivedType(tag: DW_TAG_typedef, name: "_IO_lock_t", file: !60, line: 154, baseType: null)
!101 = !DIDerivedType(tag: DW_TAG_member, name: "_offset", scope: !59, file: !60, line: 293, baseType: !102, size: 64, offset: 640)
!102 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off64_t", file: !89, line: 132, baseType: !103)
!103 = !DIDerivedType(tag: DW_TAG_typedef, name: "__quad_t", file: !89, line: 55, baseType: !104)
!104 = !DIBasicType(name: "long long int", size: 64, encoding: DW_ATE_signed)
!105 = !DIDerivedType(tag: DW_TAG_member, name: "__pad1", scope: !59, file: !60, line: 302, baseType: !9, size: 32, offset: 704)
!106 = !DIDerivedType(tag: DW_TAG_member, name: "__pad2", scope: !59, file: !60, line: 303, baseType: !9, size: 32, offset: 736)
!107 = !DIDerivedType(tag: DW_TAG_member, name: "__pad3", scope: !59, file: !60, line: 304, baseType: !9, size: 32, offset: 768)
!108 = !DIDerivedType(tag: DW_TAG_member, name: "__pad4", scope: !59, file: !60, line: 305, baseType: !9, size: 32, offset: 800)
!109 = !DIDerivedType(tag: DW_TAG_member, name: "__pad5", scope: !59, file: !60, line: 306, baseType: !110, size: 32, offset: 832)
!110 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !111, line: 62, baseType: !7)
!111 = !DIFile(filename: "/home/mahesh/git_repos/ccf/llvm/build/lib/clang/5.0.0/include/stddef.h", directory: "/home/mahesh/git_repos/ccf/scripts")
!112 = !DIDerivedType(tag: DW_TAG_member, name: "_mode", scope: !59, file: !60, line: 308, baseType: !11, size: 32, offset: 864)
!113 = !DIDerivedType(tag: DW_TAG_member, name: "_unused2", scope: !59, file: !60, line: 310, baseType: !114, size: 320, offset: 896)
!114 = !DICompositeType(tag: DW_TAG_array_type, baseType: !65, size: 320, elements: !115)
!115 = !{!116}
!116 = !DISubrange(count: 40)
!117 = !DILocalVariable(name: "pro", scope: !49, file: !3, line: 19, type: !56)
!118 = !DILocalVariable(name: "kern", scope: !49, file: !3, line: 19, type: !56)
!119 = !DILocalVariable(name: "count", scope: !49, file: !3, line: 19, type: !56)
!120 = !DILocalVariable(name: "loopno", scope: !49, file: !3, line: 20, type: !121)
!121 = !DICompositeType(tag: DW_TAG_array_type, baseType: !65, size: 200, elements: !122)
!122 = !{!123}
!123 = !DISubrange(count: 25)
!124 = !DILocalVariable(name: "directoryPath", scope: !49, file: !3, line: 21, type: !125)
!125 = !DICompositeType(tag: DW_TAG_array_type, baseType: !65, size: 160, elements: !126)
!126 = !{!127}
!127 = !DISubrange(count: 20)
!128 = !DILocalVariable(name: "epifile", scope: !49, file: !3, line: 26, type: !114)
!129 = !DILocalVariable(name: "profile", scope: !49, file: !3, line: 27, type: !114)
!130 = !DILocalVariable(name: "kernfile", scope: !49, file: !3, line: 28, type: !114)
!131 = !DILocalVariable(name: "liveOutfile", scope: !49, file: !3, line: 29, type: !114)
!132 = !DILocalVariable(name: "episize", scope: !49, file: !3, line: 45, type: !11)
!133 = !DILocalVariable(name: "prosize", scope: !49, file: !3, line: 45, type: !11)
!134 = !DILocalVariable(name: "kernelsize", scope: !49, file: !3, line: 45, type: !11)
!135 = !DILocalVariable(name: "livevar_st_size", scope: !49, file: !3, line: 45, type: !11)
!136 = !DILocalVariable(name: "II", scope: !49, file: !3, line: 69, type: !11)
!137 = !DILocalVariable(name: "epiLength", scope: !49, file: !3, line: 70, type: !11)
!138 = !DILocalVariable(name: "prolength", scope: !49, file: !3, line: 71, type: !11)
!139 = !DILocalVariable(name: "loopitfile", scope: !49, file: !3, line: 82, type: !114)
!140 = !DILocalVariable(name: "kernelCount", scope: !49, file: !3, line: 86, type: !11)
!141 = !DIExpression()
!142 = !DILocation(line: 7, column: 39, scope: !49)
!143 = !DILocation(line: 9, column: 16, scope: !49)
!144 = !DILocation(line: 11, column: 5, scope: !49)
!145 = !{!146, !146, i64 0}
!146 = !{!"any pointer", !147, i64 0}
!147 = !{!"omnipotent char", !148, i64 0}
!148 = !{!"Simple C/C++ TBAA"}
!149 = !DILocation(line: 11, column: 25, scope: !49)
!150 = !DILocation(line: 11, column: 17, scope: !49)
!151 = !DILocation(line: 11, column: 14, scope: !49)
!152 = !DILocation(line: 11, column: 30, scope: !49)
!153 = !{!154, !154, i64 0}
!154 = !{!"int", !147, i64 0}
!155 = !DILocation(line: 12, column: 29, scope: !49)
!156 = !DILocation(line: 12, column: 34, scope: !49)
!157 = !DILocation(line: 13, column: 29, scope: !49)
!158 = !DILocation(line: 20, column: 3, scope: !49)
!159 = !DILocation(line: 14, column: 34, scope: !49)
!160 = !DILocation(line: 20, column: 8, scope: !49)
!161 = !DILocation(line: 21, column: 3, scope: !49)
!162 = !DILocation(line: 21, column: 8, scope: !49)
!163 = !DILocation(line: 23, column: 3, scope: !49)
!164 = !DILocation(line: 24, column: 3, scope: !49)
!165 = !DILocation(line: 26, column: 3, scope: !49)
!166 = !DILocation(line: 26, column: 8, scope: !49)
!167 = !DILocation(line: 27, column: 3, scope: !49)
!168 = !DILocation(line: 27, column: 8, scope: !49)
!169 = !DILocation(line: 28, column: 3, scope: !49)
!170 = !DILocation(line: 28, column: 8, scope: !49)
!171 = !DILocation(line: 29, column: 3, scope: !49)
!172 = !DILocation(line: 29, column: 8, scope: !49)
!173 = !DILocation(line: 31, column: 3, scope: !49)
!174 = !DILocation(line: 32, column: 3, scope: !49)
!175 = !DILocation(line: 34, column: 3, scope: !49)
!176 = !DILocation(line: 35, column: 3, scope: !49)
!177 = !DILocation(line: 37, column: 3, scope: !49)
!178 = !DILocation(line: 38, column: 3, scope: !49)
!179 = !DILocation(line: 41, column: 7, scope: !49)
!180 = !DILocation(line: 19, column: 9, scope: !49)
!181 = !DILocation(line: 42, column: 7, scope: !49)
!182 = !DILocation(line: 19, column: 14, scope: !49)
!183 = !DILocation(line: 43, column: 8, scope: !49)
!184 = !DILocation(line: 19, column: 19, scope: !49)
!185 = !DILocation(line: 45, column: 3, scope: !49)
!186 = !DILocation(line: 47, column: 3, scope: !49)
!187 = !DILocation(line: 48, column: 3, scope: !49)
!188 = !DILocation(line: 49, column: 3, scope: !49)
!189 = !DILocation(line: 51, column: 46, scope: !49)
!190 = !DILocation(line: 45, column: 7, scope: !49)
!191 = !DILocation(line: 51, column: 3, scope: !49)
!192 = !DILocation(line: 52, column: 46, scope: !49)
!193 = !DILocation(line: 45, column: 15, scope: !49)
!194 = !DILocation(line: 52, column: 3, scope: !49)
!195 = !DILocation(line: 53, column: 47, scope: !49)
!196 = !DILocation(line: 45, column: 23, scope: !49)
!197 = !DILocation(line: 53, column: 3, scope: !49)
!198 = !DILocation(line: 54, column: 3, scope: !49)
!199 = !DILocation(line: 56, column: 39, scope: !49)
!200 = !DILocation(line: 56, column: 25, scope: !49)
!201 = !DILocation(line: 56, column: 9, scope: !49)
!202 = !DILocation(line: 57, column: 39, scope: !49)
!203 = !DILocation(line: 57, column: 25, scope: !49)
!204 = !DILocation(line: 57, column: 9, scope: !49)
!205 = !DILocation(line: 58, column: 42, scope: !49)
!206 = !DILocation(line: 58, column: 25, scope: !49)
!207 = !DILocation(line: 58, column: 9, scope: !49)
!208 = !DILocation(line: 60, column: 3, scope: !49)
!209 = !DILocation(line: 61, column: 9, scope: !49)
!210 = !DILocation(line: 61, column: 3, scope: !49)
!211 = !DILocation(line: 62, column: 9, scope: !49)
!212 = !DILocation(line: 62, column: 3, scope: !49)
!213 = !DILocation(line: 64, column: 3, scope: !49)
!214 = !DILocation(line: 65, column: 3, scope: !49)
!215 = !DILocation(line: 66, column: 9, scope: !49)
!216 = !DILocation(line: 19, column: 26, scope: !49)
!217 = !DIExpression(DW_OP_deref)
!218 = !DILocation(line: 45, column: 35, scope: !49)
!219 = !DILocation(line: 67, column: 3, scope: !49)
!220 = !DILocation(line: 69, column: 22, scope: !49)
!221 = !DILocation(line: 69, column: 7, scope: !49)
!222 = !DILocation(line: 70, column: 26, scope: !49)
!223 = !DILocation(line: 70, column: 7, scope: !49)
!224 = !DILocation(line: 71, column: 26, scope: !49)
!225 = !DILocation(line: 71, column: 7, scope: !49)
!226 = !DILocation(line: 72, column: 5, scope: !49)
!227 = !DILocation(line: 72, column: 14, scope: !49)
!228 = !DILocation(line: 72, column: 29, scope: !49)
!229 = !DILocation(line: 72, column: 34, scope: !49)
!230 = !DILocation(line: 73, column: 29, scope: !49)
!231 = !DILocation(line: 73, column: 34, scope: !49)
!232 = !DILocation(line: 74, column: 29, scope: !49)
!233 = !DILocation(line: 74, column: 34, scope: !49)
!234 = !DILocation(line: 75, column: 36, scope: !49)
!235 = !DILocation(line: 75, column: 51, scope: !49)
!236 = !DILocation(line: 75, column: 29, scope: !49)
!237 = !DILocation(line: 75, column: 34, scope: !49)
!238 = !DILocation(line: 77, column: 3, scope: !49)
!239 = !DILocation(line: 78, column: 3, scope: !49)
!240 = !DILocation(line: 79, column: 3, scope: !49)
!241 = !DILocation(line: 80, column: 3, scope: !49)
!242 = !DILocation(line: 82, column: 3, scope: !49)
!243 = !DILocation(line: 82, column: 8, scope: !49)
!244 = !DILocation(line: 83, column: 3, scope: !49)
!245 = !DILocation(line: 84, column: 3, scope: !49)
!246 = !DILocation(line: 86, column: 3, scope: !49)
!247 = !DILocation(line: 86, column: 7, scope: !49)
!248 = !DILocation(line: 87, column: 9, scope: !49)
!249 = !DILocation(line: 88, column: 3, scope: !49)
!250 = !DILocation(line: 89, column: 3, scope: !49)
!251 = !DILocation(line: 90, column: 29, scope: !49)
!252 = !DILocation(line: 90, column: 3, scope: !49)
!253 = !DILocation(line: 92, column: 36, scope: !49)
!254 = !DILocation(line: 92, column: 5, scope: !49)
!255 = !DILocation(line: 92, column: 14, scope: !49)
!256 = !DILocation(line: 92, column: 29, scope: !49)
!257 = !DILocation(line: 92, column: 34, scope: !49)
!258 = !DILocation(line: 94, column: 79, scope: !49)
!259 = !DILocation(line: 94, column: 100, scope: !49)
!260 = !DILocation(line: 94, column: 121, scope: !49)
!261 = !DILocation(line: 94, column: 3, scope: !49)
!262 = !DILocation(line: 96, column: 32, scope: !49)
!263 = !DILocation(line: 96, column: 3, scope: !49)
!264 = !DILocation(line: 96, column: 23, scope: !49)
!265 = !{!266, !266, i64 0}
!266 = !{!"long", !147, i64 0}
!267 = !DILocation(line: 97, column: 32, scope: !49)
!268 = !DILocation(line: 97, column: 3, scope: !49)
!269 = !DILocation(line: 97, column: 23, scope: !49)
!270 = !DILocation(line: 98, column: 32, scope: !49)
!271 = !DILocation(line: 98, column: 3, scope: !49)
!272 = !DILocation(line: 98, column: 23, scope: !49)
!273 = !DILocation(line: 101, column: 1, scope: !49)
!274 = !DILocation(line: 100, column: 3, scope: !49)
!275 = distinct !DISubprogram(name: "configureCGRA", scope: !3, file: !3, line: 103, type: !50, isLocal: false, isDefinition: true, scopeLine: 104, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !276)
!276 = !{!277, !278, !279, !280, !281, !282, !283, !286}
!277 = !DILocalVariable(name: "loopID", arg: 1, scope: !275, file: !3, line: 103, type: !7)
!278 = !DILocalVariable(name: "i", scope: !275, file: !3, line: 105, type: !7)
!279 = !DILocalVariable(name: "kernelCount", scope: !275, file: !3, line: 106, type: !11)
!280 = !DILocalVariable(name: "count", scope: !275, file: !3, line: 107, type: !56)
!281 = !DILocalVariable(name: "loopno", scope: !275, file: !3, line: 108, type: !121)
!282 = !DILocalVariable(name: "directoryPath", scope: !275, file: !3, line: 109, type: !125)
!283 = !DILocalVariable(name: "newTC", scope: !284, file: !3, line: 116, type: !11)
!284 = distinct !DILexicalBlock(scope: !285, file: !3, line: 115, column: 3)
!285 = distinct !DILexicalBlock(scope: !275, file: !3, line: 114, column: 6)
!286 = !DILocalVariable(name: "initCGRAfile", scope: !275, file: !3, line: 120, type: !114)
!287 = !DILocation(line: 103, column: 32, scope: !275)
!288 = !DILocation(line: 105, column: 16, scope: !275)
!289 = !DILocation(line: 106, column: 23, scope: !275)
!290 = !DILocation(line: 106, column: 43, scope: !275)
!291 = !DILocation(line: 106, column: 35, scope: !275)
!292 = !DILocation(line: 106, column: 32, scope: !275)
!293 = !DILocation(line: 106, column: 47, scope: !275)
!294 = !DILocation(line: 106, column: 21, scope: !275)
!295 = !DILocation(line: 106, column: 7, scope: !275)
!296 = !DILocation(line: 108, column: 3, scope: !275)
!297 = !DILocation(line: 108, column: 8, scope: !275)
!298 = !DILocation(line: 109, column: 3, scope: !275)
!299 = !DILocation(line: 109, column: 8, scope: !275)
!300 = !DILocation(line: 111, column: 3, scope: !275)
!301 = !DILocation(line: 112, column: 3, scope: !275)
!302 = !DILocation(line: 114, column: 18, scope: !285)
!303 = !DILocation(line: 114, column: 6, scope: !275)
!304 = !DILocation(line: 116, column: 31, scope: !284)
!305 = !DILocation(line: 116, column: 29, scope: !284)
!306 = !DILocation(line: 116, column: 9, scope: !284)
!307 = !DILocation(line: 117, column: 7, scope: !284)
!308 = !DILocation(line: 117, column: 16, scope: !284)
!309 = !DILocation(line: 117, column: 31, scope: !284)
!310 = !DILocation(line: 117, column: 36, scope: !284)
!311 = !DILocation(line: 118, column: 3, scope: !284)
!312 = !DILocation(line: 120, column: 3, scope: !275)
!313 = !DILocation(line: 120, column: 8, scope: !275)
!314 = !DILocation(line: 121, column: 3, scope: !275)
!315 = !DILocation(line: 122, column: 11, scope: !275)
!316 = !DILocation(line: 107, column: 9, scope: !275)
!317 = !DILocation(line: 123, column: 3, scope: !318)
!318 = distinct !DILexicalBlock(scope: !275, file: !3, line: 123, column: 3)
!319 = !DILocation(line: 124, column: 30, scope: !320)
!320 = distinct !DILexicalBlock(scope: !318, file: !3, line: 123, column: 3)
!321 = !DILocation(line: 124, column: 39, scope: !320)
!322 = !DILocation(line: 124, column: 54, scope: !320)
!323 = !DILocation(line: 124, column: 28, scope: !320)
!324 = !DILocation(line: 124, column: 5, scope: !320)
!325 = !DILocation(line: 123, column: 18, scope: !320)
!326 = !DILocation(line: 123, column: 13, scope: !320)
!327 = distinct !{!327, !317, !328}
!328 = !DILocation(line: 124, column: 58, scope: !318)
!329 = !DILocation(line: 125, column: 34, scope: !275)
!330 = !DILocation(line: 125, column: 3, scope: !275)
!331 = !DILocation(line: 126, column: 34, scope: !275)
!332 = !DILocation(line: 126, column: 3, scope: !275)
!333 = !DILocation(line: 127, column: 34, scope: !275)
!334 = !DILocation(line: 127, column: 3, scope: !275)
!335 = !DILocation(line: 128, column: 3, scope: !275)
!336 = !DILocation(line: 131, column: 1, scope: !275)
!337 = !DILocation(line: 130, column: 3, scope: !275)
!338 = distinct !DISubprogram(name: "checkTotalLoops", scope: !3, file: !3, line: 133, type: !339, isLocal: false, isDefinition: true, scopeLine: 134, isOptimized: true, unit: !2, variables: !341)
!339 = !DISubroutineType(types: !340)
!340 = !{null}
!341 = !{!342, !343}
!342 = !DILocalVariable(name: "myfile", scope: !338, file: !3, line: 135, type: !114)
!343 = !DILocalVariable(name: "count", scope: !338, file: !3, line: 136, type: !56)
!344 = !DILocation(line: 135, column: 5, scope: !338)
!345 = !DILocation(line: 135, column: 10, scope: !338)
!346 = !DILocation(line: 137, column: 13, scope: !338)
!347 = !DILocation(line: 136, column: 11, scope: !338)
!348 = !DILocation(line: 138, column: 5, scope: !338)
!349 = !DILocation(line: 139, column: 5, scope: !338)
!350 = !DILocation(line: 141, column: 1, scope: !338)
!351 = distinct !DISubprogram(name: "runOnCGRA", scope: !3, file: !3, line: 143, type: !352, isLocal: false, isDefinition: true, scopeLine: 144, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !354)
!352 = !DISubroutineType(types: !353)
!353 = !{!9, !9}
!354 = !{!355}
!355 = !DILocalVariable(name: "arg", arg: 1, scope: !351, file: !3, line: 143, type: !9)
!356 = !DILocation(line: 143, column: 23, scope: !351)
!357 = !DILocation(line: 146, column: 5, scope: !351)
!358 = !DILocation(line: 147, column: 15, scope: !359)
!359 = distinct !DILexicalBlock(scope: !351, file: !3, line: 146, column: 14)
!360 = !DILocation(line: 147, column: 32, scope: !359)
!361 = !DILocation(line: 147, column: 9, scope: !359)
!362 = !DILocation(line: 147, column: 38, scope: !359)
!363 = distinct !{!363, !361, !364}
!364 = !DILocation(line: 147, column: 46, scope: !359)
!365 = !DILocation(line: 149, column: 12, scope: !366)
!366 = distinct !DILexicalBlock(scope: !359, file: !3, line: 149, column: 12)
!367 = !DILocation(line: 149, column: 24, scope: !366)
!368 = !DILocation(line: 149, column: 12, scope: !359)
!369 = !DILocation(line: 150, column: 7, scope: !370)
!370 = distinct !DILexicalBlock(scope: !366, file: !3, line: 149, column: 30)
!371 = !{i32 4103}
!372 = !DILocation(line: 159, column: 3, scope: !351)
!373 = !DILocation(line: 154, column: 19, scope: !359)
!374 = !DILocation(line: 155, column: 2, scope: !359)
!375 = !{i32 4211}
!376 = !DILocation(line: 156, column: 2, scope: !359)
!377 = !DILocation(line: 157, column: 18, scope: !359)
!378 = distinct !{!378, !357, !379}
!379 = !DILocation(line: 158, column: 5, scope: !351)
!380 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !3, file: !3, line: 162, type: !381, isLocal: false, isDefinition: true, scopeLine: 163, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !383)
!381 = !DISubroutineType(types: !382)
!382 = !{null, !7}
!383 = !{!384, !385, !386}
!384 = !DILocalVariable(name: "loopNo", arg: 1, scope: !380, file: !3, line: 162, type: !7)
!385 = !DILocalVariable(name: "result", scope: !380, file: !3, line: 164, type: !11)
!386 = !DILocalVariable(name: "configure", scope: !380, file: !3, line: 166, type: !11)
!387 = !DILocation(line: 162, column: 36, scope: !380)
!388 = !DILocation(line: 164, column: 9, scope: !380)
!389 = !DILocation(line: 166, column: 21, scope: !380)
!390 = !DILocation(line: 166, column: 9, scope: !380)
!391 = !DILocation(line: 171, column: 5, scope: !380)
!392 = !DILocation(line: 173, column: 21, scope: !380)
!393 = !DILocation(line: 174, column: 22, scope: !380)
!394 = !DILocation(line: 176, column: 5, scope: !380)
!395 = !DILocation(line: 176, column: 33, scope: !380)
!396 = !DILocation(line: 176, column: 11, scope: !380)
!397 = !DILocation(line: 176, column: 27, scope: !380)
!398 = distinct !{!398, !394, !399}
!399 = !DILocation(line: 176, column: 41, scope: !380)
!400 = !DILocation(line: 177, column: 1, scope: !380)
!401 = distinct !DISubprogram(name: "deleteCGRA", scope: !3, file: !3, line: 179, type: !339, isLocal: false, isDefinition: true, scopeLine: 180, isOptimized: true, unit: !2, variables: !4)
!402 = !DILocation(line: 181, column: 3, scope: !401)
!403 = !DILocation(line: 183, column: 15, scope: !401)
!404 = !DILocation(line: 184, column: 20, scope: !401)
!405 = !DILocation(line: 186, column: 3, scope: !401)
!406 = !DILocation(line: 187, column: 16, scope: !401)
!407 = !DILocation(line: 187, column: 3, scope: !401)
!408 = !DILocation(line: 188, column: 1, scope: !401)
!409 = distinct !DISubprogram(name: "createCGRA", scope: !3, file: !3, line: 191, type: !339, isLocal: false, isDefinition: true, scopeLine: 192, isOptimized: true, unit: !2, variables: !410)
!410 = !{!411, !412}
!411 = !DILocalVariable(name: "result", scope: !409, file: !3, line: 193, type: !11)
!412 = !DILocalVariable(name: "i", scope: !409, file: !3, line: 194, type: !7)
!413 = !DILocation(line: 135, column: 10, scope: !338, inlinedAt: !414)
!414 = distinct !DILocation(line: 196, column: 3, scope: !409)
!415 = !DILocation(line: 193, column: 7, scope: !409)
!416 = !DILocation(line: 194, column: 12, scope: !409)
!417 = !DILocation(line: 135, column: 5, scope: !338, inlinedAt: !414)
!418 = !DILocation(line: 137, column: 13, scope: !338, inlinedAt: !414)
!419 = !DILocation(line: 136, column: 11, scope: !338, inlinedAt: !414)
!420 = !DILocation(line: 138, column: 5, scope: !338, inlinedAt: !414)
!421 = !DILocation(line: 139, column: 5, scope: !338, inlinedAt: !414)
!422 = !DILocation(line: 141, column: 1, scope: !338, inlinedAt: !414)
!423 = !DILocation(line: 197, column: 43, scope: !409)
!424 = !DILocation(line: 197, column: 42, scope: !409)
!425 = !DILocation(line: 197, column: 22, scope: !409)
!426 = !DILocation(line: 197, column: 13, scope: !409)
!427 = !DILocation(line: 198, column: 40, scope: !409)
!428 = !DILocation(line: 198, column: 22, scope: !409)
!429 = !DILocation(line: 198, column: 13, scope: !409)
!430 = !DILocation(line: 199, column: 22, scope: !409)
!431 = !DILocation(line: 199, column: 13, scope: !409)
!432 = !DILocation(line: 200, column: 22, scope: !409)
!433 = !DILocation(line: 200, column: 13, scope: !409)
!434 = !DILocation(line: 203, column: 16, scope: !435)
!435 = distinct !DILexicalBlock(scope: !436, file: !3, line: 203, column: 3)
!436 = distinct !DILexicalBlock(scope: !409, file: !3, line: 203, column: 3)
!437 = !DILocation(line: 203, column: 3, scope: !436)
!438 = !DILocation(line: 204, column: 7, scope: !435)
!439 = !DILocation(line: 203, column: 32, scope: !435)
!440 = !DILocation(line: 203, column: 19, scope: !435)
!441 = distinct !{!441, !437, !442}
!442 = !DILocation(line: 204, column: 29, scope: !436)
!443 = !DILocation(line: 206, column: 3, scope: !409)
!444 = !DILocation(line: 207, column: 12, scope: !409)
!445 = !DILocation(line: 208, column: 1, scope: !409)
