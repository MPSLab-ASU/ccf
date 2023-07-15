; ModuleID = 'temp.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-unknown-linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%union.pthread_attr_t = type { i32, [32 x i8] }

@.str = private unnamed_addr constant [22 x i8] c"iterations:%d\09, c:%f\0A\00", align 1
@totalLoops = dso_local global i32 0, align 4
@thread_cond_cpu = dso_local local_unnamed_addr global i32 1, align 4
@thread_cond_cgra = dso_local local_unnamed_addr global i32 0, align 4
@thread_exit = dso_local local_unnamed_addr global i32 0, align 4
@dynamicTCVal = dso_local local_unnamed_addr global i32 0, align 4
@initCGRA = dso_local local_unnamed_addr global i32* null, align 4
@epilog = dso_local local_unnamed_addr global i32* null, align 4
@prolog = dso_local local_unnamed_addr global i32* null, align 4
@kernel = dso_local local_unnamed_addr global i32* null, align 4
@prologPtr = dso_local local_unnamed_addr global i32* null, align 4
@epilogPtr = dso_local local_unnamed_addr global i32* null, align 4
@kernelPtr = dso_local local_unnamed_addr global i32* null, align 4
@pth = dso_local global i32 0, align 4
@ArrPtr = dso_local local_unnamed_addr global i32* null, align 4
@str = private unnamed_addr constant [34 x i8] c"from cgra.c Initialize Parameters\00", align 1
@__const.configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
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
@.str.13 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@str.35 = private unnamed_addr constant [52 x i8] c"\0A*******************before while loop**************\00", align 1
@.str.15 = private unnamed_addr constant [39 x i8] c"\0A************XDIM and YDim are %d, %d\0A\00", align 1
@.str.16 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.17 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.18 = private unnamed_addr constant [55 x i8] c"From FILE: PROLOGPC= %lx, EPILOGPC=%lx,  KernelPC=%lx\0A\00", align 1
@str.36 = private unnamed_addr constant [30 x i8] c"Entering cgra.c configureCGRA\00", align 1
@__const.configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.20 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.21 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.22 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.23 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@str.37 = private unnamed_addr constant [32 x i8] c"Entering cgra.c checkTotalLoops\00", align 1
@__const.checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.25 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@str.38 = private unnamed_addr constant [26 x i8] c"Entering cgra.c runOnCGRA\00", align 1
@str.39 = private unnamed_addr constant [33 x i8] c"Entering cgra.c accelerateOnCGRA\00", align 1
@.str.29 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@str.40 = private unnamed_addr constant [14 x i8] c"deleting cgra\00", align 1
@str.41 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00", align 1
@str.42 = private unnamed_addr constant [26 x i8] c"Exiting cgra.c deleteCGRA\00", align 1
@str.43 = private unnamed_addr constant [27 x i8] c"Entering cgra.c createCGRA\00", align 1
@str.44 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00", align 1

; Function Attrs: nofree nounwind
define dso_local i32 @main(i32 %0, i8** nocapture readonly %1) local_unnamed_addr #0 {
  %3 = getelementptr inbounds i8*, i8** %1, i32 1
  %4 = load i8*, i8** %3, align 4, !tbaa !5
  %5 = tail call i32 @strtol(i8* nocapture nonnull %4, i8** null, i32 10) #12
  %6 = icmp sgt i32 %5, 0
  br i1 %6, label %.preheader.preheader, label %16

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %7 = phi i32 [ %12, %.preheader ], [ 0, %.preheader.preheader ]
  %8 = phi float [ %11, %.preheader ], [ 0.000000e+00, %.preheader.preheader ]
  %9 = icmp ult i32 %7, 3
  %10 = fadd float %8, 4.000000e+01
  %11 = select i1 %9, float %10, float %8
  %12 = add nuw nsw i32 %7, 1
  %13 = icmp eq i32 %12, %5
  br i1 %13, label %14, label %.preheader, !llvm.loop !9

14:                                               ; preds = %.preheader
  %15 = fpext float %11 to double
  br label %16

16:                                               ; preds = %14, %2
  %17 = phi double [ 0.000000e+00, %2 ], [ %15, %14 ]
  %18 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([22 x i8], [22 x i8]* @.str, i32 0, i32 0), i32 %5, double %17)
  ret i32 0
}

; Function Attrs: nofree nounwind willreturn
declare dso_local i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #1

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local i32 @initializeParameters(i32 %0) local_unnamed_addr #3 {
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
  %16 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([34 x i8], [34 x i8]* @str, i32 0, i32 0))
  %17 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %18 = add i32 %0, -1
  %19 = mul i32 %18, 7
  %20 = getelementptr inbounds i32, i32* %17, i32 %19
  store i32 2011168768, i32* %20, align 4, !tbaa !13
  %21 = getelementptr inbounds i32, i32* %20, i32 1
  store i32 2011168768, i32* %21, align 4, !tbaa !13
  %22 = getelementptr inbounds i32, i32* %20, i32 2
  %23 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  %24 = bitcast i32* %22 to i8*
  tail call void @llvm.memset.p0i8.i64(i8* nonnull align 4 dereferenceable(20) %24, i8 0, i64 20, i1 false)
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %23) #12
  %25 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %25) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %25, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false)
  %26 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %23, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #12
  %27 = call i8* @strcat(i8* nonnull %25, i8* nonnull %23) #12
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %28) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %28, i8 0, i32 40, i1 false)
  %29 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %29) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %29, i8 0, i32 40, i1 false)
  %30 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %30) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %30, i8 0, i32 40, i1 false)
  %31 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %31) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %31, i8 0, i32 40, i1 false)
  %32 = getelementptr inbounds [40 x i8], [40 x i8]* %8, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %32) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %32, i8 0, i32 40, i1 false)
  %33 = call i8* @strcat(i8* nonnull %28, i8* nonnull %25) #12
  %34 = call i32 @strlen(i8* nonnull %28)
  %35 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %34
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %35, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 16, i1 false)
  %36 = call i8* @strcat(i8* nonnull %29, i8* nonnull %25) #12
  %37 = call i32 @strlen(i8* nonnull %29)
  %38 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %37
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %38, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 16, i1 false)
  %39 = call i8* @strcat(i8* nonnull %30, i8* nonnull %25) #12
  %40 = call i32 @strlen(i8* nonnull %30)
  %41 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %40
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %41, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.4, i32 0, i32 0), i32 16, i1 false)
  %42 = call i8* @strcat(i8* nonnull %32, i8* nonnull %25) #12
  %43 = call i32 @strlen(i8* nonnull %32)
  %44 = getelementptr [40 x i8], [40 x i8]* %8, i32 0, i32 %43
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(17) %44, i8* nonnull align 1 dereferenceable(17) getelementptr inbounds ([17 x i8], [17 x i8]* @.str.5, i32 0, i32 0), i32 17, i1 false)
  %45 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0))
  %46 = call %struct._IO_FILE* @fopen(i8* nonnull %29, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0))
  %47 = call %struct._IO_FILE* @fopen(i8* nonnull %30, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.6, i32 0, i32 0))
  %48 = call %struct._IO_FILE* @fopen(i8* nonnull %32, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0))
  %49 = bitcast i32* %9 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %49) #12
  %50 = bitcast i32* %10 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %50) #12
  %51 = bitcast i32* %11 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %51) #12
  %52 = bitcast i32* %12 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %52) #12
  %53 = call i32 @fread(i8* nonnull %49, i32 4, i32 1, %struct._IO_FILE* %45)
  %54 = call i32 @fread(i8* nonnull %50, i32 4, i32 1, %struct._IO_FILE* %46)
  %55 = call i32 @fread(i8* nonnull %51, i32 4, i32 1, %struct._IO_FILE* %47)
  %56 = load i32, i32* %9, align 4, !tbaa !13
  %57 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.8, i32 0, i32 0), i32 %56)
  %58 = load i32, i32* %10, align 4, !tbaa !13
  %59 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.9, i32 0, i32 0), i32 %58)
  %60 = load i32, i32* %11, align 4, !tbaa !13
  %61 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @.str.10, i32 0, i32 0), i32 %60)
  %62 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([37 x i8], [37 x i8]* @.str.11, i32 0, i32 0), i32 4)
  %63 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([42 x i8], [42 x i8]* @.str.12, i32 0, i32 0), i32 8)
  %64 = shl i32 %56, 3
  %65 = call noalias i8* @malloc(i32 %64) #12
  store i8* %65, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !5
  %66 = shl i32 %58, 3
  %67 = call noalias i8* @malloc(i32 %66) #12
  store i8* %67, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !5
  %68 = shl i32 %60, 3
  %69 = call noalias i8* @malloc(i32 %68) #12
  store i8* %69, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !5
  %70 = call i32 @fread(i8* %65, i32 8, i32 %56, %struct._IO_FILE* %45)
  %71 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !5
  %72 = call i32 @fread(i8* %71, i32 8, i32 %58, %struct._IO_FILE* %46)
  %73 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !5
  %74 = call i32 @fread(i8* %73, i32 8, i32 %60, %struct._IO_FILE* %47)
  %75 = call i8* @strcat(i8* nonnull %31, i8* nonnull %25) #12
  %76 = call i32 @strlen(i8* nonnull %31)
  %77 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %76
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(26) %77, i8* nonnull align 1 dereferenceable(26) getelementptr inbounds ([26 x i8], [26 x i8]* @.str.13, i32 0, i32 0), i32 26, i1 false)
  %78 = call %struct._IO_FILE* @fopen(i8* nonnull %31, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0))
  %79 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %78, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %12) #12
  %80 = getelementptr inbounds [256 x i8], [256 x i8]* %13, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %80) #12
  %81 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([52 x i8], [52 x i8]* @str.35, i32 0, i32 0))
  %82 = call i8* @fgets(i8* nonnull %80, i32 256, %struct._IO_FILE* %48)
  %83 = icmp eq i8* %82, null
  br i1 %83, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %95
  %84 = phi i32 [ %97, %95 ], [ 0, %.preheader.preheader ]
  %85 = phi i32 [ %96, %95 ], [ 0, %.preheader.preheader ]
  %86 = phi i32 [ %87, %95 ], [ 0, %.preheader.preheader ]
  %87 = add nuw nsw i32 %86, 1
  %88 = icmp eq i32 %86, 0
  br i1 %88, label %89, label %91

89:                                               ; preds = %.preheader
  %90 = call i32 @strtol(i8* nocapture nonnull %80, i8** null, i32 10) #12
  br label %95

91:                                               ; preds = %.preheader
  %92 = icmp eq i32 %87, 2
  br i1 %92, label %93, label %.loopexit.loopexit

93:                                               ; preds = %91
  %94 = call i32 @strtol(i8* nocapture nonnull %80, i8** null, i32 10) #12
  br label %95

95:                                               ; preds = %93, %89
  %96 = phi i32 [ %85, %89 ], [ %94, %93 ]
  %97 = phi i32 [ %90, %89 ], [ %84, %93 ]
  %98 = call i8* @fgets(i8* nonnull %80, i32 256, %struct._IO_FILE* %48)
  %99 = icmp eq i8* %98, null
  br i1 %99, label %.loopexit.loopexit, label %.preheader, !llvm.loop !15

.loopexit.loopexit:                               ; preds = %95, %91
  %.ph = phi i32 [ %96, %95 ], [ %85, %91 ]
  %.ph6 = phi i32 [ %97, %95 ], [ %84, %91 ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %100 = phi i32 [ 0, %1 ], [ %.ph, %.loopexit.loopexit ]
  %101 = phi i32 [ 0, %1 ], [ %.ph6, %.loopexit.loopexit ]
  %102 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([39 x i8], [39 x i8]* @.str.15, i32 0, i32 0), i32 %101, i32 %100)
  %103 = mul nsw i32 %101, %100
  %104 = sdiv i32 %60, %103
  %105 = sdiv i32 %56, %103
  %106 = sdiv i32 %58, %103
  %107 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %108 = getelementptr inbounds i32, i32* %107, i32 %19
  %109 = getelementptr inbounds i32, i32* %108, i32 2
  store i32 %104, i32* %109, align 4, !tbaa !13
  %110 = getelementptr inbounds i32, i32* %108, i32 3
  store i32 %105, i32* %110, align 4, !tbaa !13
  %111 = getelementptr inbounds i32, i32* %108, i32 4
  store i32 %106, i32* %111, align 4, !tbaa !13
  %112 = load i32, i32* %12, align 4, !tbaa !13
  %113 = sdiv i32 %112, %103
  %114 = getelementptr inbounds i32, i32* %108, i32 6
  store i32 %113, i32* %114, align 4, !tbaa !13
  %115 = call i32 @fclose(%struct._IO_FILE* %45)
  %116 = call i32 @fclose(%struct._IO_FILE* %46)
  %117 = call i32 @fclose(%struct._IO_FILE* %47)
  %118 = call i32 @fclose(%struct._IO_FILE* %78)
  %119 = call i32 @fclose(%struct._IO_FILE* %48)
  %120 = getelementptr inbounds [40 x i8], [40 x i8]* %14, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %120) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %120, i8 0, i32 40, i1 false)
  %121 = call i8* @strcat(i8* nonnull %120, i8* nonnull %25) #12
  %122 = call i32 @strlen(i8* nonnull %120)
  %123 = getelementptr [40 x i8], [40 x i8]* %14, i32 0, i32 %122
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(18) %123, i8* nonnull align 1 dereferenceable(18) getelementptr inbounds ([18 x i8], [18 x i8]* @.str.16, i32 0, i32 0), i32 18, i1 false)
  %124 = bitcast i32* %15 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %124) #12
  store i32 0, i32* %15, align 4, !tbaa !13
  %125 = call %struct._IO_FILE* @fopen(i8* nonnull %120, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0))
  %126 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %125, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %15) #12
  %127 = call i32 @fclose(%struct._IO_FILE* %125)
  %128 = load i32, i32* %15, align 4, !tbaa !13
  %129 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.17, i32 0, i32 0), i32 %128)
  %130 = load i32, i32* %15, align 4, !tbaa !13
  %131 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %132 = getelementptr inbounds i32, i32* %131, i32 %19
  %133 = getelementptr inbounds i32, i32* %132, i32 5
  store i32 %130, i32* %133, align 4, !tbaa !13
  %134 = load i32*, i32** @prolog, align 4, !tbaa !5
  %135 = ptrtoint i32* %134 to i32
  %136 = load i32*, i32** @epilog, align 4, !tbaa !5
  %137 = ptrtoint i32* %136 to i32
  %138 = load i32*, i32** @kernel, align 4, !tbaa !5
  %139 = ptrtoint i32* %138 to i32
  %140 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([55 x i8], [55 x i8]* @.str.18, i32 0, i32 0), i32 %135, i32 %137, i32 %139)
  %141 = load i32*, i32** @prolog, align 4, !tbaa !5
  %142 = ptrtoint i32* %141 to i32
  %143 = load i32*, i32** @prologPtr, align 4, !tbaa !5
  %144 = getelementptr inbounds i32, i32* %143, i32 %18
  store i32 %142, i32* %144, align 4, !tbaa !16
  %145 = load i32*, i32** @epilog, align 4, !tbaa !5
  %146 = ptrtoint i32* %145 to i32
  %147 = load i32*, i32** @epilogPtr, align 4, !tbaa !5
  %148 = getelementptr inbounds i32, i32* %147, i32 %18
  store i32 %146, i32* %148, align 4, !tbaa !16
  %149 = load i32*, i32** @kernel, align 4, !tbaa !5
  %150 = ptrtoint i32* %149 to i32
  %151 = load i32*, i32** @kernelPtr, align 4, !tbaa !5
  %152 = getelementptr inbounds i32, i32* %151, i32 %18
  store i32 %150, i32* %152, align 4, !tbaa !16
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %124) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %120) #12
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %80) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %52) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %51) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %50) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %49) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %32) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %31) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %30) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %29) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %28) #12
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %25) #12
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %23) #12
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #4

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #5

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #6

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i32, i1 immarg) #6

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @sprintf(i8* noalias nocapture noundef writeonly, i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nounwind willreturn
declare dso_local i8* @strcat(i8* noalias returned writeonly, i8* noalias nocapture readonly) local_unnamed_addr #7

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i1 immarg) #5

; Function Attrs: argmemonly nofree nounwind readonly willreturn
declare i32 @strlen(i8* nocapture) local_unnamed_addr #8

; Function Attrs: nofree nounwind
declare dso_local noalias noundef %struct._IO_FILE* @fopen(i8* nocapture noundef readonly, i8* nocapture noundef readonly) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fread(i8* nocapture noundef, i32 noundef, i32 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: inaccessiblememonly nofree nounwind willreturn
declare dso_local noalias noundef i8* @malloc(i32) local_unnamed_addr #9

declare dso_local i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #10

; Function Attrs: nofree nounwind
declare dso_local noundef i8* @fgets(i8* noundef, i32 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fclose(%struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #6

; Function Attrs: nofree nounwind
define dso_local i32 @configureCGRA(i32 %0) local_unnamed_addr #0 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  %5 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([30 x i8], [30 x i8]* @str.36, i32 0, i32 0))
  %6 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %7 = add i32 %0, -1
  %8 = mul i32 %7, 7
  %9 = getelementptr inbounds i32, i32* %6, i32 %8
  %10 = getelementptr inbounds i32, i32* %9, i32 5
  %11 = load i32, i32* %10, align 4, !tbaa !13
  %12 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %12) #12
  %13 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %13) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %13, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false)
  %14 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %12, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #12
  %15 = call i8* @strcat(i8* nonnull %13, i8* nonnull %12) #12
  %16 = icmp slt i32 %11, 1
  br i1 %16, label %17, label %23

17:                                               ; preds = %1
  %18 = load i32, i32* @dynamicTCVal, align 4, !tbaa !13
  %19 = add nsw i32 %18, %11
  %20 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %21 = getelementptr inbounds i32, i32* %20, i32 %8
  %22 = getelementptr inbounds i32, i32* %21, i32 5
  store i32 %19, i32* %22, align 4, !tbaa !13
  br label %23

23:                                               ; preds = %17, %1
  %24 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %24) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %24, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i1 false)
  %25 = call i32 @strlen(i8* nonnull %24)
  %26 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %25
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(14) %26, i8* nonnull align 1 dereferenceable(14) getelementptr inbounds ([14 x i8], [14 x i8]* @.str.20, i32 0, i32 0), i32 14, i1 false)
  %27 = call %struct._IO_FILE* @fopen(i8* nonnull %24, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.21, i32 0, i32 0))
  br label %28

28:                                               ; preds = %28, %23
  %29 = phi i32 [ 0, %23 ], [ %35, %28 ]
  %30 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %31 = getelementptr inbounds i32, i32* %30, i32 %8
  %32 = getelementptr inbounds i32, i32* %31, i32 %29
  %33 = load i32, i32* %32, align 4, !tbaa !13
  %34 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.22, i32 0, i32 0), i32 %33)
  %35 = add nuw nsw i32 %29, 1
  %36 = icmp eq i32 %35, 7
  br i1 %36, label %37, label %28, !llvm.loop !18

37:                                               ; preds = %28
  %38 = load i32*, i32** @epilogPtr, align 4, !tbaa !5
  %39 = getelementptr inbounds i32, i32* %38, i32 %7
  %40 = load i32, i32* %39, align 4, !tbaa !16
  %41 = zext i32 %40 to i64
  %42 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i64 %41)
  %43 = load i32*, i32** @prologPtr, align 4, !tbaa !5
  %44 = getelementptr inbounds i32, i32* %43, i32 %7
  %45 = load i32, i32* %44, align 4, !tbaa !16
  %46 = zext i32 %45 to i64
  %47 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i64 %46)
  %48 = load i32*, i32** @kernelPtr, align 4, !tbaa !5
  %49 = getelementptr inbounds i32, i32* %48, i32 %7
  %50 = load i32, i32* %49, align 4, !tbaa !16
  %51 = zext i32 %50 to i64
  %52 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i64 %51)
  %53 = call i32 @fclose(%struct._IO_FILE* %27)
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %24) #12
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %13) #12
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %12) #12
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fprintf(%struct._IO_FILE* nocapture noundef, i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @checkTotalLoops() local_unnamed_addr #3 {
  %1 = alloca [40 x i8], align 1
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @str.37, i32 0, i32 0))
  %3 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %3) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %3, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false)
  %4 = call %struct._IO_FILE* @fopen(i8* nonnull %3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0))
  %5 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.25, i32 0, i32 0), i32* nonnull @totalLoops) #12
  %6 = tail call i32 @fclose(%struct._IO_FILE* %4)
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %3) #12
  ret void
}

; Function Attrs: nounwind
define dso_local i8* @runOnCGRA(i8* nocapture readnone %0) #3 {
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([26 x i8], [26 x i8]* @str.38, i32 0, i32 0))
  br label %3

3:                                                ; preds = %12, %1
  %4 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !13
  %5 = icmp eq i32 %4, 1
  br i1 %5, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %3
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %6 = tail call i32 @usleep(i32 1) #12
  %7 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !13
  %8 = icmp eq i32 %7, 1
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !llvm.loop !19

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %3
  %9 = load i32, i32* @thread_exit, align 4, !tbaa !13
  %10 = icmp eq i32 %9, 1
  br i1 %10, label %11, label %12

11:                                               ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #12, !srcloc !20
  ret i8* null

12:                                               ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !13
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #12, !srcloc !21
  %13 = tail call i32 @usleep(i32 1) #12
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !13
  br label %3, !llvm.loop !22
}

declare dso_local i32 @usleep(i32) local_unnamed_addr #10

; Function Attrs: nounwind
define dso_local void @accelerateOnCGRA(i32 %0) local_unnamed_addr #3 {
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @str.39, i32 0, i32 0))
  %3 = tail call i32 @configureCGRA(i32 %0)
  %4 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @.str.29, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !13
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !13
  br label %5

5:                                                ; preds = %5, %1
  %6 = tail call i32 @usleep(i32 1) #12
  %7 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !13
  %8 = icmp eq i32 %7, 1
  br i1 %8, label %9, label %5, !llvm.loop !23

9:                                                ; preds = %5
  ret void
}

; Function Attrs: nounwind
define dso_local void @deleteCGRA() local_unnamed_addr #3 {
  %1 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([14 x i8], [14 x i8]* @str.40, i32 0, i32 0))
  store i32 1, i32* @thread_exit, align 4, !tbaa !13
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !13
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([40 x i8], [40 x i8]* @str.41, i32 0, i32 0))
  %3 = load i32, i32* @pth, align 4, !tbaa !16
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #12
  %5 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([26 x i8], [26 x i8]* @str.42, i32 0, i32 0))
  ret void
}

declare dso_local i32 @pthread_join(i32, i8**) local_unnamed_addr #10

; Function Attrs: nounwind
define dso_local void @createCGRA() local_unnamed_addr #3 {
  %1 = alloca [40 x i8], align 1
  %2 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([27 x i8], [27 x i8]* @str.43, i32 0, i32 0))
  %3 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @str.37, i32 0, i32 0)) #12
  %4 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %4) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %4, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false) #12
  %5 = call %struct._IO_FILE* @fopen(i8* nonnull %4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i32 0, i32 0)) #12
  %6 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %5, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.25, i32 0, i32 0), i32* nonnull @totalLoops) #12
  %7 = tail call i32 @fclose(%struct._IO_FILE* %5) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %4) #12
  %8 = load i32, i32* @totalLoops, align 4, !tbaa !13
  %9 = mul i32 %8, 28
  %10 = tail call noalias i8* @malloc(i32 %9) #12
  store i8* %10, i8** bitcast (i32** @initCGRA to i8**), align 4, !tbaa !5
  %11 = shl i32 %8, 2
  %12 = tail call noalias i8* @malloc(i32 %11) #12
  store i8* %12, i8** bitcast (i32** @prologPtr to i8**), align 4, !tbaa !5
  %13 = tail call noalias i8* @malloc(i32 %11) #12
  store i8* %13, i8** bitcast (i32** @kernelPtr to i8**), align 4, !tbaa !5
  %14 = tail call noalias i8* @malloc(i32 %11) #12
  store i8* %14, i8** bitcast (i32** @epilogPtr to i8**), align 4, !tbaa !5
  %15 = icmp eq i32 %8, 0
  br i1 %15, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %0
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %16 = phi i32 [ %18, %.preheader ], [ 1, %.preheader.preheader ]
  %17 = tail call i32 @initializeParameters(i32 %16)
  %18 = add i32 %16, 1
  %19 = load i32, i32* @totalLoops, align 4, !tbaa !13
  %20 = icmp ugt i32 %18, %19
  br i1 %20, label %.loopexit.loopexit, label %.preheader, !llvm.loop !24

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %0
  %21 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @str.44, i32 0, i32 0))
  tail call void asm sideeffect "mov r8,$0", "r"(i32 71) #12, !srcloc !25
  %22 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #12
  ret void
}

; Function Attrs: nounwind
declare dso_local i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #11

attributes #0 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "min-legal-vector-width"="0" "no-jump-tables"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #1 = { nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #2 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #3 = { nounwind "disable-tail-calls"="false" "frame-pointer"="none" "min-legal-vector-width"="0" "no-jump-tables"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #4 = { nofree nounwind }
attributes #5 = { argmemonly nofree nosync nounwind willreturn writeonly }
attributes #6 = { argmemonly nofree nosync nounwind willreturn }
attributes #7 = { argmemonly nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #8 = { argmemonly nofree nounwind readonly willreturn }
attributes #9 = { inaccessiblememonly nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #10 = { "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #11 = { nounwind "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #12 = { nounwind }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2, !3, !4}

!0 = !{!"clang version 13.0.0 (https://github.com/llvm/llvm-project.git 0d4534237de38fcf1b2a71bcb0f74cb5675fe7e1)"}
!1 = !{i32 7, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{i32 1, !"min_enum_size", i32 4}
!5 = !{!6, !6, i64 0}
!6 = !{!"any pointer", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10, !11, !12}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!"llvm.loop.CGRA.enable"}
!12 = !{!"llvm.loop.unroll.disable"}
!13 = !{!14, !14, i64 0}
!14 = !{!"int", !7, i64 0}
!15 = distinct !{!15, !10, !12}
!16 = !{!17, !17, i64 0}
!17 = !{!"long", !7, i64 0}
!18 = distinct !{!18, !10, !12}
!19 = distinct !{!19, !10, !12}
!20 = !{i32 5320}
!21 = !{i32 5428}
!22 = distinct !{!22, !12}
!23 = distinct !{!23, !10, !12}
!24 = distinct !{!24, !10, !12}
!25 = !{i32 6755}
