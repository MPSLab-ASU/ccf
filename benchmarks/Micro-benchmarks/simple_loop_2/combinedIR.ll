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
@__const.configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1.2 = private unnamed_addr constant [16 x i8] c"/epilog_ins.bin\00", align 1
@.str.2 = private unnamed_addr constant [16 x i8] c"/prolog_ins.bin\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"/kernel_ins.bin\00", align 1
@.str.4 = private unnamed_addr constant [17 x i8] c"/CGRA_config.txt\00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.6 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.7 = private unnamed_addr constant [32 x i8] c"\0A**********EPISIZE %d*********\0A\00", align 1
@.str.8 = private unnamed_addr constant [32 x i8] c"\0A**********PROSIZE %d*********\0A\00", align 1
@.str.9 = private unnamed_addr constant [33 x i8] c"\0A**********KERNSIZE %d*********\0A\00", align 1
@.str.10 = private unnamed_addr constant [37 x i8] c"\0A******SIZE OF UNSIGNED LONG%d*****\0A\00", align 1
@.str.11 = private unnamed_addr constant [42 x i8] c"\0A******SIZE OF UNSIGNED LONG LONG%d*****\0A\00", align 1
@.str.12 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@str = private unnamed_addr constant [52 x i8] c"\0A*******************before while loop**************\00", align 1
@.str.14 = private unnamed_addr constant [39 x i8] c"\0A************XDIM and YDim are %d, %d\0A\00", align 1
@.str.15 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.16 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.17 = private unnamed_addr constant [55 x i8] c"From FILE: PROLOGPC= %lx, EPILOGPC=%lx,  KernelPC=%lx\0A\00", align 1
@__const.configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.18 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.19 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.20 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.21 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@__const.checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.22 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@.str.24 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@str.27 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00", align 1
@str.28 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00", align 1

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
  %9 = icmp ugt i32 %7, 3
  %10 = fadd float %8, 0x40444CCCC0000000
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
  %16 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %17 = add i32 %0, -1
  %18 = mul i32 %17, 7
  %19 = getelementptr inbounds i32, i32* %16, i32 %18
  store i32 2011168768, i32* %19, align 4, !tbaa !13
  %20 = getelementptr inbounds i32, i32* %19, i32 1
  store i32 2011168768, i32* %20, align 4, !tbaa !13
  %21 = getelementptr inbounds i32, i32* %19, i32 2
  %22 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  %23 = bitcast i32* %21 to i8*
  tail call void @llvm.memset.p0i8.i64(i8* nonnull align 4 dereferenceable(20) %23, i8 0, i64 20, i1 false)
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %22) #12
  %24 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %24) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %24, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false)
  %25 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %22, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #12
  %26 = call i8* @strcat(i8* nonnull %24, i8* nonnull %22) #12
  %27 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %27) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %27, i8 0, i32 40, i1 false)
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %28) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %28, i8 0, i32 40, i1 false)
  %29 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %29) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %29, i8 0, i32 40, i1 false)
  %30 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %30) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %30, i8 0, i32 40, i1 false)
  %31 = getelementptr inbounds [40 x i8], [40 x i8]* %8, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %31) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %31, i8 0, i32 40, i1 false)
  %32 = call i8* @strcat(i8* nonnull %27, i8* nonnull %24) #12
  %33 = call i32 @strlen(i8* nonnull %27)
  %34 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %33
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %34, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1.2, i32 0, i32 0), i32 16, i1 false)
  %35 = call i8* @strcat(i8* nonnull %28, i8* nonnull %24) #12
  %36 = call i32 @strlen(i8* nonnull %28)
  %37 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %36
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %37, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 16, i1 false)
  %38 = call i8* @strcat(i8* nonnull %29, i8* nonnull %24) #12
  %39 = call i32 @strlen(i8* nonnull %29)
  %40 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %39
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(16) %40, i8* nonnull align 1 dereferenceable(16) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 16, i1 false)
  %41 = call i8* @strcat(i8* nonnull %31, i8* nonnull %24) #12
  %42 = call i32 @strlen(i8* nonnull %31)
  %43 = getelementptr [40 x i8], [40 x i8]* %8, i32 0, i32 %42
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(17) %43, i8* nonnull align 1 dereferenceable(17) getelementptr inbounds ([17 x i8], [17 x i8]* @.str.4, i32 0, i32 0), i32 17, i1 false)
  %44 = call %struct._IO_FILE* @fopen(i8* nonnull %27, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0))
  %45 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0))
  %46 = call %struct._IO_FILE* @fopen(i8* nonnull %29, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0))
  %47 = call %struct._IO_FILE* @fopen(i8* nonnull %31, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %48 = bitcast i32* %9 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %48) #12
  %49 = bitcast i32* %10 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %49) #12
  %50 = bitcast i32* %11 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %50) #12
  %51 = bitcast i32* %12 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %51) #12
  %52 = call i32 @fread(i8* nonnull %48, i32 4, i32 1, %struct._IO_FILE* %44)
  %53 = call i32 @fread(i8* nonnull %49, i32 4, i32 1, %struct._IO_FILE* %45)
  %54 = call i32 @fread(i8* nonnull %50, i32 4, i32 1, %struct._IO_FILE* %46)
  %55 = load i32, i32* %9, align 4, !tbaa !13
  %56 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.7, i32 0, i32 0), i32 %55)
  %57 = load i32, i32* %10, align 4, !tbaa !13
  %58 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([32 x i8], [32 x i8]* @.str.8, i32 0, i32 0), i32 %57)
  %59 = load i32, i32* %11, align 4, !tbaa !13
  %60 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([33 x i8], [33 x i8]* @.str.9, i32 0, i32 0), i32 %59)
  %61 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([37 x i8], [37 x i8]* @.str.10, i32 0, i32 0), i32 4)
  %62 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([42 x i8], [42 x i8]* @.str.11, i32 0, i32 0), i32 8)
  %63 = shl i32 %55, 3
  %64 = call noalias i8* @malloc(i32 %63) #12
  store i8* %64, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !5
  %65 = shl i32 %57, 3
  %66 = call noalias i8* @malloc(i32 %65) #12
  store i8* %66, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !5
  %67 = shl i32 %59, 3
  %68 = call noalias i8* @malloc(i32 %67) #12
  store i8* %68, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !5
  %69 = call i32 @fread(i8* %64, i32 8, i32 %55, %struct._IO_FILE* %44)
  %70 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !5
  %71 = call i32 @fread(i8* %70, i32 8, i32 %57, %struct._IO_FILE* %45)
  %72 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !5
  %73 = call i32 @fread(i8* %72, i32 8, i32 %59, %struct._IO_FILE* %46)
  %74 = call i8* @strcat(i8* nonnull %30, i8* nonnull %24) #12
  %75 = call i32 @strlen(i8* nonnull %30)
  %76 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %75
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(26) %76, i8* nonnull align 1 dereferenceable(26) getelementptr inbounds ([26 x i8], [26 x i8]* @.str.12, i32 0, i32 0), i32 26, i1 false)
  %77 = call %struct._IO_FILE* @fopen(i8* nonnull %30, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %78 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %77, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %12) #12
  %79 = getelementptr inbounds [256 x i8], [256 x i8]* %13, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 256, i8* nonnull %79) #12
  %80 = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([52 x i8], [52 x i8]* @str, i32 0, i32 0))
  %81 = call i8* @fgets(i8* nonnull %79, i32 256, %struct._IO_FILE* %47)
  %82 = icmp eq i8* %81, null
  br i1 %82, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %94
  %83 = phi i32 [ %96, %94 ], [ 0, %.preheader.preheader ]
  %84 = phi i32 [ %95, %94 ], [ 0, %.preheader.preheader ]
  %85 = phi i32 [ %86, %94 ], [ 0, %.preheader.preheader ]
  %86 = add nuw nsw i32 %85, 1
  %87 = icmp eq i32 %85, 0
  br i1 %87, label %88, label %90

88:                                               ; preds = %.preheader
  %89 = call i32 @strtol(i8* nocapture nonnull %79, i8** null, i32 10) #12
  br label %94

90:                                               ; preds = %.preheader
  %91 = icmp eq i32 %86, 2
  br i1 %91, label %92, label %.loopexit.loopexit

92:                                               ; preds = %90
  %93 = call i32 @strtol(i8* nocapture nonnull %79, i8** null, i32 10) #12
  br label %94

94:                                               ; preds = %92, %88
  %95 = phi i32 [ %84, %88 ], [ %93, %92 ]
  %96 = phi i32 [ %89, %88 ], [ %83, %92 ]
  %97 = call i8* @fgets(i8* nonnull %79, i32 256, %struct._IO_FILE* %47)
  %98 = icmp eq i8* %97, null
  br i1 %98, label %.loopexit.loopexit, label %.preheader, !llvm.loop !15

.loopexit.loopexit:                               ; preds = %94, %90
  %.ph = phi i32 [ %95, %94 ], [ %84, %90 ]
  %.ph6 = phi i32 [ %96, %94 ], [ %83, %90 ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %99 = phi i32 [ 0, %1 ], [ %.ph, %.loopexit.loopexit ]
  %100 = phi i32 [ 0, %1 ], [ %.ph6, %.loopexit.loopexit ]
  %101 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([39 x i8], [39 x i8]* @.str.14, i32 0, i32 0), i32 %100, i32 %99)
  %102 = mul nsw i32 %100, %99
  %103 = sdiv i32 %59, %102
  %104 = sdiv i32 %55, %102
  %105 = sdiv i32 %57, %102
  %106 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %107 = getelementptr inbounds i32, i32* %106, i32 %18
  %108 = getelementptr inbounds i32, i32* %107, i32 2
  store i32 %103, i32* %108, align 4, !tbaa !13
  %109 = getelementptr inbounds i32, i32* %107, i32 3
  store i32 %104, i32* %109, align 4, !tbaa !13
  %110 = getelementptr inbounds i32, i32* %107, i32 4
  store i32 %105, i32* %110, align 4, !tbaa !13
  %111 = load i32, i32* %12, align 4, !tbaa !13
  %112 = sdiv i32 %111, %102
  %113 = getelementptr inbounds i32, i32* %107, i32 6
  store i32 %112, i32* %113, align 4, !tbaa !13
  %114 = call i32 @fclose(%struct._IO_FILE* %44)
  %115 = call i32 @fclose(%struct._IO_FILE* %45)
  %116 = call i32 @fclose(%struct._IO_FILE* %46)
  %117 = call i32 @fclose(%struct._IO_FILE* %77)
  %118 = call i32 @fclose(%struct._IO_FILE* %47)
  %119 = getelementptr inbounds [40 x i8], [40 x i8]* %14, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %119) #12
  call void @llvm.memset.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %119, i8 0, i32 40, i1 false)
  %120 = call i8* @strcat(i8* nonnull %119, i8* nonnull %24) #12
  %121 = call i32 @strlen(i8* nonnull %119)
  %122 = getelementptr [40 x i8], [40 x i8]* %14, i32 0, i32 %121
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(18) %122, i8* nonnull align 1 dereferenceable(18) getelementptr inbounds ([18 x i8], [18 x i8]* @.str.15, i32 0, i32 0), i32 18, i1 false)
  %123 = bitcast i32* %15 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %123) #12
  store i32 0, i32* %15, align 4, !tbaa !13
  %124 = call %struct._IO_FILE* @fopen(i8* nonnull %119, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %125 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %124, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32* nonnull %15) #12
  %126 = call i32 @fclose(%struct._IO_FILE* %124)
  %127 = load i32, i32* %15, align 4, !tbaa !13
  %128 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([16 x i8], [16 x i8]* @.str.16, i32 0, i32 0), i32 %127)
  %129 = load i32, i32* %15, align 4, !tbaa !13
  %130 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %131 = getelementptr inbounds i32, i32* %130, i32 %18
  %132 = getelementptr inbounds i32, i32* %131, i32 5
  store i32 %129, i32* %132, align 4, !tbaa !13
  %133 = load i32*, i32** @prolog, align 4, !tbaa !5
  %134 = ptrtoint i32* %133 to i32
  %135 = load i32*, i32** @epilog, align 4, !tbaa !5
  %136 = ptrtoint i32* %135 to i32
  %137 = load i32*, i32** @kernel, align 4, !tbaa !5
  %138 = ptrtoint i32* %137 to i32
  %139 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([55 x i8], [55 x i8]* @.str.17, i32 0, i32 0), i32 %134, i32 %136, i32 %138)
  %140 = load i32*, i32** @prolog, align 4, !tbaa !5
  %141 = ptrtoint i32* %140 to i32
  %142 = load i32*, i32** @prologPtr, align 4, !tbaa !5
  %143 = getelementptr inbounds i32, i32* %142, i32 %17
  store i32 %141, i32* %143, align 4, !tbaa !16
  %144 = load i32*, i32** @epilog, align 4, !tbaa !5
  %145 = ptrtoint i32* %144 to i32
  %146 = load i32*, i32** @epilogPtr, align 4, !tbaa !5
  %147 = getelementptr inbounds i32, i32* %146, i32 %17
  store i32 %145, i32* %147, align 4, !tbaa !16
  %148 = load i32*, i32** @kernel, align 4, !tbaa !5
  %149 = ptrtoint i32* %148 to i32
  %150 = load i32*, i32** @kernelPtr, align 4, !tbaa !5
  %151 = getelementptr inbounds i32, i32* %150, i32 %17
  store i32 %149, i32* %151, align 4, !tbaa !16
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %123) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %119) #12
  call void @llvm.lifetime.end.p0i8(i64 256, i8* nonnull %79) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %51) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %50) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %49) #12
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %48) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %31) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %30) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %29) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %28) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %27) #12
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %24) #12
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %22) #12
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #4

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #5

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i32, i1 immarg) #5

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @sprintf(i8* noalias nocapture noundef writeonly, i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nounwind willreturn
declare dso_local i8* @strcat(i8* noalias returned writeonly, i8* noalias nocapture readonly) local_unnamed_addr #6

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i1 immarg) #4

; Function Attrs: argmemonly nofree nounwind readonly willreturn
declare i32 @strlen(i8* nocapture) local_unnamed_addr #7

; Function Attrs: nofree nounwind
declare dso_local noalias noundef %struct._IO_FILE* @fopen(i8* nocapture noundef readonly, i8* nocapture noundef readonly) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fread(i8* nocapture noundef, i32 noundef, i32 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: inaccessiblememonly nofree nounwind willreturn
declare dso_local noalias noundef i8* @malloc(i32) local_unnamed_addr #8

declare dso_local i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #9

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #10

; Function Attrs: nofree nounwind
declare dso_local noundef i8* @fgets(i8* noundef, i32 noundef, %struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fclose(%struct._IO_FILE* nocapture noundef) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #5

; Function Attrs: nofree nounwind
define dso_local i32 @configureCGRA(i32 %0) local_unnamed_addr #0 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  %5 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %6 = add i32 %0, -1
  %7 = mul i32 %6, 7
  %8 = getelementptr inbounds i32, i32* %5, i32 %7
  %9 = getelementptr inbounds i32, i32* %8, i32 5
  %10 = load i32, i32* %9, align 4, !tbaa !13
  %11 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 25, i8* nonnull %11) #12
  %12 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 20, i8* nonnull %12) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(20) %12, i8* nonnull align 1 dereferenceable(20) getelementptr inbounds ([20 x i8], [20 x i8]* @__const.configureCGRA.directoryPath, i32 0, i32 0), i32 20, i1 false)
  %13 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %11, i8* nonnull dereferenceable(1) getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 %0) #12
  %14 = call i8* @strcat(i8* nonnull %12, i8* nonnull %11) #12
  %15 = icmp slt i32 %10, 1
  br i1 %15, label %16, label %22

16:                                               ; preds = %1
  %17 = load i32, i32* @dynamicTCVal, align 4, !tbaa !13
  %18 = add nsw i32 %17, %10
  %19 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %20 = getelementptr inbounds i32, i32* %19, i32 %7
  %21 = getelementptr inbounds i32, i32* %20, i32 5
  store i32 %18, i32* %21, align 4, !tbaa !13
  br label %22

22:                                               ; preds = %16, %1
  %23 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %23) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %23, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i1 false)
  %24 = call i32 @strlen(i8* nonnull %23)
  %25 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %24
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(14) %25, i8* nonnull align 1 dereferenceable(14) getelementptr inbounds ([14 x i8], [14 x i8]* @.str.18, i32 0, i32 0), i32 14, i1 false)
  %26 = call %struct._IO_FILE* @fopen(i8* nonnull %23, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.19, i32 0, i32 0))
  br label %27

27:                                               ; preds = %27, %22
  %28 = phi i32 [ 0, %22 ], [ %34, %27 ]
  %29 = load i32*, i32** @initCGRA, align 4, !tbaa !5
  %30 = getelementptr inbounds i32, i32* %29, i32 %7
  %31 = getelementptr inbounds i32, i32* %30, i32 %28
  %32 = load i32, i32* %31, align 4, !tbaa !13
  %33 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.20, i32 0, i32 0), i32 %32)
  %34 = add nuw nsw i32 %28, 1
  %35 = icmp eq i32 %34, 7
  br i1 %35, label %36, label %27, !llvm.loop !18

36:                                               ; preds = %27
  %37 = load i32*, i32** @epilogPtr, align 4, !tbaa !5
  %38 = getelementptr inbounds i32, i32* %37, i32 %6
  %39 = load i32, i32* %38, align 4, !tbaa !16
  %40 = zext i32 %39 to i64
  %41 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.21, i32 0, i32 0), i64 %40)
  %42 = load i32*, i32** @prologPtr, align 4, !tbaa !5
  %43 = getelementptr inbounds i32, i32* %42, i32 %6
  %44 = load i32, i32* %43, align 4, !tbaa !16
  %45 = zext i32 %44 to i64
  %46 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.21, i32 0, i32 0), i64 %45)
  %47 = load i32*, i32** @kernelPtr, align 4, !tbaa !5
  %48 = getelementptr inbounds i32, i32* %47, i32 %6
  %49 = load i32, i32* %48, align 4, !tbaa !16
  %50 = zext i32 %49 to i64
  %51 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.21, i32 0, i32 0), i64 %50)
  %52 = call i32 @fclose(%struct._IO_FILE* %26)
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %23) #12
  call void @llvm.lifetime.end.p0i8(i64 20, i8* nonnull %12) #12
  call void @llvm.lifetime.end.p0i8(i64 25, i8* nonnull %11) #12
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @fprintf(%struct._IO_FILE* nocapture noundef, i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define dso_local void @checkTotalLoops() local_unnamed_addr #3 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %2) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %2, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false)
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.22, i32 0, i32 0), i32* nonnull @totalLoops) #12
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3)
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %2) #12
  ret void
}

; Function Attrs: nounwind
define dso_local i8* @runOnCGRA(i8* nocapture readnone %0) #3 {
  br label %2

2:                                                ; preds = %11, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !13
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %5 = tail call i32 @usleep(i32 1) #12
  %6 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !13
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %.loopexit.loopexit, label %.preheader, !llvm.loop !19

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %8 = load i32, i32* @thread_exit, align 4, !tbaa !13
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %11

10:                                               ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #12, !srcloc !20
  ret i8* null

11:                                               ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !13
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #12, !srcloc !21
  %12 = tail call i32 @usleep(i32 1) #12
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !13
  br label %2, !llvm.loop !22
}

declare dso_local i32 @usleep(i32) local_unnamed_addr #9

; Function Attrs: nounwind
define dso_local void @accelerateOnCGRA(i32 %0) local_unnamed_addr #3 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @.str.24, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !13
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !13
  br label %4

4:                                                ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #12
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !13
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !23

8:                                                ; preds = %4
  ret void
}

; Function Attrs: nounwind
define dso_local void @deleteCGRA() local_unnamed_addr #3 {
  store i32 1, i32* @thread_exit, align 4, !tbaa !13
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !13
  %1 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([40 x i8], [40 x i8]* @str.27, i32 0, i32 0))
  %2 = load i32, i32* @pth, align 4, !tbaa !16
  %3 = tail call i32 @pthread_join(i32 %2, i8** null) #12
  ret void
}

declare dso_local i32 @pthread_join(i32, i8**) local_unnamed_addr #9

; Function Attrs: nounwind
define dso_local void @createCGRA() local_unnamed_addr #3 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %2) #12
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull align 1 dereferenceable(40) %2, i8* nonnull align 1 dereferenceable(40) getelementptr inbounds ([40 x i8], [40 x i8]* @__const.checkTotalLoops.myfile, i32 0, i32 0), i32 40, i1 false) #12
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0)) #12
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.22, i32 0, i32 0), i32* nonnull @totalLoops) #12
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3) #12
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %2) #12
  %6 = load i32, i32* @totalLoops, align 4, !tbaa !13
  %7 = mul i32 %6, 28
  %8 = tail call noalias i8* @malloc(i32 %7) #12
  store i8* %8, i8** bitcast (i32** @initCGRA to i8**), align 4, !tbaa !5
  %9 = shl i32 %6, 2
  %10 = tail call noalias i8* @malloc(i32 %9) #12
  store i8* %10, i8** bitcast (i32** @prologPtr to i8**), align 4, !tbaa !5
  %11 = tail call noalias i8* @malloc(i32 %9) #12
  store i8* %11, i8** bitcast (i32** @kernelPtr to i8**), align 4, !tbaa !5
  %12 = tail call noalias i8* @malloc(i32 %9) #12
  store i8* %12, i8** bitcast (i32** @epilogPtr to i8**), align 4, !tbaa !5
  %13 = icmp eq i32 %6, 0
  br i1 %13, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %0
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %14 = phi i32 [ %16, %.preheader ], [ 1, %.preheader.preheader ]
  %15 = tail call i32 @initializeParameters(i32 %14)
  %16 = add i32 %14, 1
  %17 = load i32, i32* @totalLoops, align 4, !tbaa !13
  %18 = icmp ugt i32 %16, %17
  br i1 %18, label %.loopexit.loopexit, label %.preheader, !llvm.loop !24

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %0
  %19 = tail call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([35 x i8], [35 x i8]* @str.28, i32 0, i32 0))
  tail call void asm sideeffect "mov r8,$0", "r"(i32 71) #12, !srcloc !25
  %20 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #12
  ret void
}

; Function Attrs: nounwind
declare dso_local i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #11

attributes #0 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "min-legal-vector-width"="0" "no-jump-tables"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #1 = { nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #2 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #3 = { nounwind "disable-tail-calls"="false" "frame-pointer"="none" "min-legal-vector-width"="0" "no-jump-tables"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #4 = { argmemonly nofree nosync nounwind willreturn writeonly }
attributes #5 = { argmemonly nofree nosync nounwind willreturn }
attributes #6 = { argmemonly nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #7 = { argmemonly nofree nounwind readonly willreturn }
attributes #8 = { inaccessiblememonly nofree nounwind willreturn "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #9 = { "disable-tail-calls"="false" "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+armv7-a,+d32,+dsp,+fp64,+neon,+vfp2,+vfp2sp,+vfp3,+vfp3d16,+vfp3d16sp,+vfp3sp,-thumb-mode" }
attributes #10 = { nofree nounwind }
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
!20 = !{i32 5270}
!21 = !{i32 5378}
!22 = distinct !{!22, !12}
!23 = distinct !{!23, !10, !12}
!24 = distinct !{!24, !10, !12}
!25 = !{i32 6673}
