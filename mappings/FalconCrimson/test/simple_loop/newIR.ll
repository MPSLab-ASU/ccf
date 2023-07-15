; ModuleID = 'combinedIR.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%union.pthread_attr_t = type { i32, [32 x i8] }

@.str = private unnamed_addr constant [20 x i8] c"a[%d]=%d\09 b[%d]=%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"c[%d]=%d\0A\00", align 1
@totalLoops = global i32 0, align 4
@thread_cond_cpu = local_unnamed_addr global i32 1, align 4
@thread_cond_cgra = local_unnamed_addr global i32 0, align 4
@thread_exit = local_unnamed_addr global i32 0, align 4
@dynamicTCVal = local_unnamed_addr global i32 0, align 4
@initCGRA = common local_unnamed_addr global i32* null, align 4
@epilog = common local_unnamed_addr global i32* null, align 4
@prolog = common local_unnamed_addr global i32* null, align 4
@kernel = common local_unnamed_addr global i32* null, align 4
@prologPtr = common local_unnamed_addr global i32* null, align 4
@epilogPtr = common local_unnamed_addr global i32* null, align 4
@kernelPtr = common local_unnamed_addr global i32* null, align 4
@pth = common global i32 0, align 4
@ArrPtr = common local_unnamed_addr global i32* null, align 4
@configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1.3 = private unnamed_addr constant [16 x i8] c"/epilog_ins.bin\00", align 1
@.str.2.4 = private unnamed_addr constant [16 x i8] c"/prolog_ins.bin\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"/kernel_ins.bin\00", align 1
@.str.4 = private unnamed_addr constant [17 x i8] c"/CGRA_config.csv\00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.6 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@initializeParameters.s = internal constant [2 x i8] c", ", align 1
@.str.9 = private unnamed_addr constant [16 x i8] c"CGRA_config.csv\00", align 1
@.str.10 = private unnamed_addr constant [33 x i8] c"\0A*********CGRA ROWS %d*********\0A\00", align 1
@.str.11 = private unnamed_addr constant [34 x i8] c"\0A**********CGRA COLS %d*********\0A\00", align 1
@.str.12 = private unnamed_addr constant [32 x i8] c"\0A**********EPISIZE %d*********\0A\00", align 1
@.str.13 = private unnamed_addr constant [32 x i8] c"\0A**********PROSIZE %d*********\0A\00", align 1
@.str.14 = private unnamed_addr constant [33 x i8] c"\0A**********KERNSIZE %d*********\0A\00", align 1
@.str.15 = private unnamed_addr constant [37 x i8] c"\0A******SIZE OF UNISGNED INT %d*****\0A\00", align 1
@.str.16 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@.str.17 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.18 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.19 = private unnamed_addr constant [52 x i8] c"From FILE: PROLOGPC= %x, EPILOGPC=%x,  KernelPC=%x\0A\00", align 1
@configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.20 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.21 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.22 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.23 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.24 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@.str.26 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@str = private unnamed_addr constant [14 x i8] c"deleting cgra\00"
@str.30 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00"
@str.31 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00"
@gPtr1 = common local_unnamed_addr global [10000 x i32]* null
@gPtr2 = common local_unnamed_addr global [10000 x i32]* null
@gPtr3 = common local_unnamed_addr global [10000 x i32]* null

; Function Attrs: nounwind
define i32 @main() local_unnamed_addr #0 {
  call void @createCGRA()
  %1 = alloca [10000 x i32], align 4
  %2 = alloca [10000 x i32], align 4
  %3 = alloca [10000 x i32], align 4
  %4 = bitcast [10000 x i32]* %1 to i8*
  call void @llvm.lifetime.start(i64 40000, i8* nonnull %4) #6
  %5 = bitcast [10000 x i32]* %2 to i8*
  call void @llvm.lifetime.start(i64 40000, i8* nonnull %5) #6
  %6 = bitcast [10000 x i32]* %3 to i8*
  call void @llvm.lifetime.start(i64 40000, i8* nonnull %6) #6
  store [10000 x i32]* %1, [10000 x i32]** @gPtr1
  store [10000 x i32]* %2, [10000 x i32]** @gPtr2
  store [10000 x i32]* %3, [10000 x i32]** @gPtr3
  br label %7

; <label>:7:                                      ; preds = %7, %0
  %8 = phi i32 [ 0, %0 ], [ %10, %7 ]
  %9 = getelementptr inbounds [10000 x i32], [10000 x i32]* %1, i32 0, i32 %8
  store i32 %8, i32* %9, align 4, !tbaa !5
  %10 = add nuw nsw i32 %8, 1
  %11 = icmp eq i32 %10, 10000
  br i1 %11, label %12, label %7, !llvm.loop !9

; <label>:12:                                     ; preds = %7
  br label %13

; <label>:13:                                     ; preds = %13, %12
  %14 = phi i32 [ %17, %13 ], [ 0, %12 ]
  %15 = add nuw nsw i32 %14, 2
  %16 = getelementptr inbounds [10000 x i32], [10000 x i32]* %2, i32 0, i32 %14
  store i32 %15, i32* %16, align 4, !tbaa !5
  %17 = add nuw nsw i32 %14, 1
  %18 = icmp eq i32 %17, 10000
  br i1 %18, label %19, label %13, !llvm.loop !27

; <label>:19:                                     ; preds = %13
  br label %20

; <label>:20:                                     ; preds = %20, %19
  %21 = phi i32 [ %27, %20 ], [ 0, %19 ]
  %22 = getelementptr inbounds [10000 x i32], [10000 x i32]* %1, i32 0, i32 %21
  %23 = load i32, i32* %22, align 4, !tbaa !5
  %24 = getelementptr inbounds [10000 x i32], [10000 x i32]* %2, i32 0, i32 %21
  %25 = load i32, i32* %24, align 4, !tbaa !5
  %26 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i32 %21, i32 %23, i32 %21, i32 %25)
  %27 = add nuw nsw i32 %21, 1
  %28 = icmp eq i32 %27, 10
  br i1 %28, label %29, label %20, !llvm.loop !30

; <label>:29:                                     ; preds = %20
  br label %30

; <label>:30:                                     ; preds = %29
  call void @accelerateOnCGRA(i32 1)
  br label %31

; <label>:31:                                     ; preds = %30
  br label %32

; <label>:32:                                     ; preds = %32, %31
  %33 = phi i32 [ %37, %32 ], [ 0, %31 ]
  %34 = getelementptr inbounds [10000 x i32], [10000 x i32]* %3, i32 0, i32 %33
  %35 = load i32, i32* %34, align 4, !tbaa !5
  %36 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i32 %33, i32 %35)
  %37 = add nuw nsw i32 %33, 1
  %38 = icmp eq i32 %37, 10
  br i1 %38, label %39, label %32, !llvm.loop !33

; <label>:39:                                     ; preds = %32
  call void @llvm.lifetime.end(i64 40000, i8* nonnull %6) #6
  call void @llvm.lifetime.end(i64 40000, i8* nonnull %5) #6
  call void @llvm.lifetime.end(i64 40000, i8* nonnull %4) #6
  ret i32 0
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata, metadata) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #2

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #2

; Function Attrs: nounwind
define i32 @initializeParameters(i32) local_unnamed_addr #0 {
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
  %13 = alloca [20 x i8], align 1
  %14 = alloca [40 x i8], align 1
  %15 = alloca i32, align 4
  %16 = load i32*, i32** @initCGRA, align 4, !tbaa !36
  %17 = add i32 %0, -1
  %18 = mul i32 %17, 7
  %19 = getelementptr inbounds i32, i32* %16, i32 %18
  store i32 2011168768, i32* %19, align 4, !tbaa !5
  %20 = getelementptr inbounds i32, i32* %19, i32 1
  store i32 2011168768, i32* %20, align 4, !tbaa !5
  %21 = getelementptr inbounds i32, i32* %19, i32 2
  %22 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  %23 = bitcast i32* %21 to i8*
  call void @llvm.memset.p0i8.i64(i8* %23, i8 0, i64 20, i32 4, i1 false)
  call void @llvm.lifetime.start(i64 25, i8* nonnull %22) #6
  %24 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %24) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %24, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %25 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %22, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i32 %0) #6
  %26 = call i8* @strcat(i8* nonnull %24, i8* nonnull %22) #6
  %27 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %27) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %27, i8 0, i32 40, i32 1, i1 false)
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %28) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %28, i8 0, i32 40, i32 1, i1 false)
  %29 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %29) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %29, i8 0, i32 40, i32 1, i1 false)
  %30 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %30) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %30, i8 0, i32 40, i32 1, i1 false)
  %31 = getelementptr inbounds [40 x i8], [40 x i8]* %8, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %31) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %31, i8 0, i32 40, i32 1, i1 false)
  %32 = call i8* @strcat(i8* nonnull %27, i8* nonnull %24) #6
  %33 = call i32 @strlen(i8* nonnull %27)
  %34 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %33
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %34, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1.3, i32 0, i32 0), i32 16, i32 1, i1 false)
  %35 = call i8* @strcat(i8* nonnull %28, i8* nonnull %24) #6
  %36 = call i32 @strlen(i8* nonnull %28)
  %37 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %36
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %37, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2.4, i32 0, i32 0), i32 16, i32 1, i1 false)
  %38 = call i8* @strcat(i8* nonnull %29, i8* nonnull %24) #6
  %39 = call i32 @strlen(i8* nonnull %29)
  %40 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %39
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %40, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 16, i32 1, i1 false)
  %41 = call i8* @strcat(i8* nonnull %31, i8* nonnull %24) #6
  %42 = call i32 @strlen(i8* nonnull %31)
  %43 = getelementptr [40 x i8], [40 x i8]* %8, i32 0, i32 %42
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %43, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.4, i32 0, i32 0), i32 17, i32 1, i1 false)
  %44 = call %struct._IO_FILE* @fopen(i8* nonnull %27, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0))
  %45 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0))
  %46 = call %struct._IO_FILE* @fopen(i8* nonnull %29, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0))
  %47 = call %struct._IO_FILE* @fopen(i8* nonnull %31, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %48 = bitcast i32* %9 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %48) #6
  %49 = bitcast i32* %10 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %49) #6
  %50 = bitcast i32* %11 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %50) #6
  %51 = bitcast i32* %12 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %51) #6
  %52 = call i32 @fread(i8* nonnull %48, i32 4, i32 1, %struct._IO_FILE* %44)
  %53 = call i32 @fread(i8* nonnull %49, i32 4, i32 1, %struct._IO_FILE* %45)
  %54 = call i32 @fread(i8* nonnull %50, i32 4, i32 1, %struct._IO_FILE* %46)
  %55 = icmp eq %struct._IO_FILE* %47, null
  br i1 %55, label %128, label %56

; <label>:56:                                     ; preds = %1
  %57 = getelementptr inbounds [20 x i8], [20 x i8]* %13, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %57) #6
  %58 = call i8* @fgets(i8* nonnull %57, i32 20, %struct._IO_FILE* nonnull %47)
  %59 = icmp eq i8* %58, null
  br i1 %59, label %124, label %60

; <label>:60:                                     ; preds = %56
  br label %64

; <label>:61:                                     ; preds = %118
  %.lcssa3 = phi i32 [ %113, %118 ]
  %.lcssa2 = phi i32 [ %114, %118 ]
  %.lcssa1 = phi i32 [ %119, %118 ]
  %.lcssa = phi i32 [ %120, %118 ]
  %62 = call i8* @fgets(i8* nonnull %57, i32 20, %struct._IO_FILE* nonnull %47)
  %63 = icmp eq i8* %62, null
  br i1 %63, label %123, label %64

; <label>:64:                                     ; preds = %61, %60
  %65 = phi i32 [ %.lcssa2, %61 ], [ undef, %60 ]
  %66 = phi i32 [ %.lcssa, %61 ], [ undef, %60 ]
  %67 = phi i32 [ %.lcssa3, %61 ], [ 0, %60 ]
  %68 = phi i32 [ %.lcssa1, %61 ], [ 0, %60 ]
  %69 = call i8* @strtok(i8* nonnull %57, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @initializeParameters.s, i32 0, i32 0)) #6
  br label %70

; <label>:70:                                     ; preds = %118, %64
  %71 = phi i32 [ %121, %118 ], [ 0, %64 ]
  %72 = phi i32 [ %114, %118 ], [ %65, %64 ]
  %73 = phi i32 [ %120, %118 ], [ %66, %64 ]
  %74 = phi i8* [ %106, %118 ], [ %69, %64 ]
  %75 = phi i32 [ %113, %118 ], [ %67, %64 ]
  %76 = phi i32 [ %119, %118 ], [ %68, %64 ]
  %77 = icmp eq i32 %71, 0
  br i1 %77, label %78, label %104

; <label>:78:                                     ; preds = %70
  %79 = load i8, i8* %74, align 1, !tbaa !38
  %80 = zext i8 %79 to i32
  %81 = sub nsw i32 88, %80
  %82 = icmp eq i32 %81, 0
  br i1 %82, label %83, label %88

; <label>:83:                                     ; preds = %78
  %84 = getelementptr inbounds i8, i8* %74, i32 1
  %85 = load i8, i8* %84, align 1, !tbaa !38
  %86 = zext i8 %85 to i32
  %87 = sub nsw i32 0, %86
  br label %88

; <label>:88:                                     ; preds = %83, %78
  %89 = phi i32 [ %87, %83 ], [ %81, %78 ]
  %90 = icmp eq i32 %89, 0
  %91 = select i1 %90, i32 1, i32 %75
  %92 = sub nsw i32 89, %80
  %93 = icmp eq i32 %92, 0
  br i1 %93, label %94, label %99

; <label>:94:                                     ; preds = %88
  %95 = getelementptr inbounds i8, i8* %74, i32 1
  %96 = load i8, i8* %95, align 1, !tbaa !38
  %97 = zext i8 %96 to i32
  %98 = sub nsw i32 0, %97
  br label %99

; <label>:99:                                     ; preds = %94, %88
  %100 = phi i32 [ %98, %94 ], [ %92, %88 ]
  %101 = icmp eq i32 %100, 0
  %102 = select i1 %101, i32 1, i32 %76
  %103 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @initializeParameters.s, i32 0, i32 0)) #6
  br label %104

; <label>:104:                                    ; preds = %99, %70
  %105 = phi i32 [ 1, %99 ], [ %71, %70 ]
  %106 = phi i8* [ %103, %99 ], [ %74, %70 ]
  %107 = phi i32 [ %91, %99 ], [ %75, %70 ]
  %108 = phi i32 [ %102, %99 ], [ %76, %70 ]
  %109 = icmp eq i32 %107, 1
  br i1 %109, label %110, label %112

; <label>:110:                                    ; preds = %104
  %111 = call i32 @strtol(i8* nocapture nonnull %106, i8** null, i32 10) #6
  br label %112

; <label>:112:                                    ; preds = %110, %104
  %113 = phi i32 [ 0, %110 ], [ %107, %104 ]
  %114 = phi i32 [ %111, %110 ], [ %72, %104 ]
  %115 = icmp eq i32 %108, 1
  br i1 %115, label %116, label %118

; <label>:116:                                    ; preds = %112
  %117 = call i32 @strtol(i8* nocapture nonnull %106, i8** null, i32 10) #6
  br label %118

; <label>:118:                                    ; preds = %116, %112
  %119 = phi i32 [ 0, %116 ], [ %108, %112 ]
  %120 = phi i32 [ %117, %116 ], [ %73, %112 ]
  %121 = add nuw nsw i32 %105, 1
  %122 = icmp eq i32 %121, 2
  br i1 %122, label %61, label %70, !llvm.loop !39

; <label>:123:                                    ; preds = %61
  %.lcssa2.lcssa = phi i32 [ %.lcssa2, %61 ]
  %.lcssa.lcssa = phi i32 [ %.lcssa, %61 ]
  br label %124

; <label>:124:                                    ; preds = %123, %56
  %125 = phi i32 [ undef, %56 ], [ %.lcssa.lcssa, %123 ]
  %126 = phi i32 [ undef, %56 ], [ %.lcssa2.lcssa, %123 ]
  %127 = call i32 @fclose(%struct._IO_FILE* nonnull %47)
  call void @llvm.lifetime.end(i64 20, i8* nonnull %57) #6
  br label %129

; <label>:128:                                    ; preds = %1
  call void @perror(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.9, i32 0, i32 0)) #7
  br label %129

; <label>:129:                                    ; preds = %128, %124
  %130 = phi i32 [ %125, %124 ], [ undef, %128 ]
  %131 = phi i32 [ %126, %124 ], [ undef, %128 ]
  %132 = mul nsw i32 %131, %130
  %133 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.10, i32 0, i32 0), i32 %131)
  %134 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.11, i32 0, i32 0), i32 %130)
  %135 = load i32, i32* %9, align 4, !tbaa !5
  %136 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.12, i32 0, i32 0), i32 %135)
  %137 = load i32, i32* %10, align 4, !tbaa !5
  %138 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.13, i32 0, i32 0), i32 %137)
  %139 = load i32, i32* %11, align 4, !tbaa !5
  %140 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.14, i32 0, i32 0), i32 %139)
  %141 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.15, i32 0, i32 0), i32 4)
  %142 = shl i32 %135, 2
  %143 = call noalias i8* @malloc(i32 %142) #6
  store i8* %143, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !36
  %144 = shl i32 %137, 2
  %145 = call noalias i8* @malloc(i32 %144) #6
  store i8* %145, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !36
  %146 = shl i32 %139, 2
  %147 = call noalias i8* @malloc(i32 %146) #6
  store i8* %147, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !36
  %148 = call i32 @fread(i8* %143, i32 4, i32 %135, %struct._IO_FILE* %44)
  %149 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !36
  %150 = call i32 @fread(i8* %149, i32 4, i32 %137, %struct._IO_FILE* %45)
  %151 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !36
  %152 = call i32 @fread(i8* %151, i32 4, i32 %139, %struct._IO_FILE* %46)
  %153 = call i8* @strcat(i8* nonnull %30, i8* nonnull %24) #6
  %154 = call i32 @strlen(i8* nonnull %30)
  %155 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %154
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %155, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.16, i32 0, i32 0), i32 26, i32 1, i1 false)
  %156 = call %struct._IO_FILE* @fopen(i8* nonnull %30, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %157 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %156, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i32* nonnull %12) #6
  %158 = sdiv i32 %139, %132
  %159 = sdiv i32 %135, %132
  %160 = sdiv i32 %137, %132
  %161 = load i32*, i32** @initCGRA, align 4, !tbaa !36
  %162 = getelementptr inbounds i32, i32* %161, i32 %18
  %163 = getelementptr inbounds i32, i32* %162, i32 2
  store i32 %158, i32* %163, align 4, !tbaa !5
  %164 = getelementptr inbounds i32, i32* %162, i32 3
  store i32 %159, i32* %164, align 4, !tbaa !5
  %165 = getelementptr inbounds i32, i32* %162, i32 4
  store i32 %160, i32* %165, align 4, !tbaa !5
  %166 = load i32, i32* %12, align 4, !tbaa !5
  %167 = sdiv i32 %166, %132
  %168 = getelementptr inbounds i32, i32* %162, i32 6
  store i32 %167, i32* %168, align 4, !tbaa !5
  %169 = call i32 @fclose(%struct._IO_FILE* %44)
  %170 = call i32 @fclose(%struct._IO_FILE* %45)
  %171 = call i32 @fclose(%struct._IO_FILE* %46)
  %172 = call i32 @fclose(%struct._IO_FILE* %156)
  %173 = getelementptr inbounds [40 x i8], [40 x i8]* %14, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %173) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %173, i8 0, i32 40, i32 1, i1 false)
  %174 = call i8* @strcat(i8* nonnull %173, i8* nonnull %24) #6
  %175 = call i32 @strlen(i8* nonnull %173)
  %176 = getelementptr [40 x i8], [40 x i8]* %14, i32 0, i32 %175
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %176, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.17, i32 0, i32 0), i32 18, i32 1, i1 false)
  %177 = bitcast i32* %15 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %177) #6
  store i32 0, i32* %15, align 4, !tbaa !5
  %178 = call %struct._IO_FILE* @fopen(i8* nonnull %173, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %179 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %178, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i32* nonnull %15) #6
  %180 = call i32 @fclose(%struct._IO_FILE* %178)
  %181 = load i32, i32* %15, align 4, !tbaa !5
  %182 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.18, i32 0, i32 0), i32 %181)
  %183 = load i32, i32* %15, align 4, !tbaa !5
  %184 = load i32*, i32** @initCGRA, align 4, !tbaa !36
  %185 = getelementptr inbounds i32, i32* %184, i32 %18
  %186 = getelementptr inbounds i32, i32* %185, i32 5
  store i32 %183, i32* %186, align 4, !tbaa !5
  %187 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !36
  %188 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !36
  %189 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !36
  %190 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.19, i32 0, i32 0), i32 %187, i32 %188, i32 %189)
  %191 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !36
  %192 = load i32*, i32** @prologPtr, align 4, !tbaa !36
  %193 = getelementptr inbounds i32, i32* %192, i32 %17
  store i32 %191, i32* %193, align 4, !tbaa !219
  %194 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !36
  %195 = load i32*, i32** @epilogPtr, align 4, !tbaa !36
  %196 = getelementptr inbounds i32, i32* %195, i32 %17
  store i32 %194, i32* %196, align 4, !tbaa !219
  %197 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !36
  %198 = load i32*, i32** @kernelPtr, align 4, !tbaa !36
  %199 = getelementptr inbounds i32, i32* %198, i32 %17
  store i32 %197, i32* %199, align 4, !tbaa !219
  call void @llvm.lifetime.end(i64 4, i8* nonnull %177) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %173) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %51) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %50) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %49) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %48) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %31) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %30) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %29) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %28) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %27) #6
  call void @llvm.lifetime.end(i64 20, i8* nonnull %24) #6
  call void @llvm.lifetime.end(i64 25, i8* nonnull %22) #6
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture writeonly, i8* nocapture readonly, i32, i32, i1) #2

; Function Attrs: nounwind
declare i32 @sprintf(i8* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: nounwind
declare i8* @strcat(i8*, i8* nocapture readonly) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i32, i1) #2

; Function Attrs: nounwind readonly
declare i32 @strlen(i8* nocapture) #4

; Function Attrs: nounwind
declare noalias %struct._IO_FILE* @fopen(i8* nocapture readonly, i8* nocapture readonly) local_unnamed_addr #3

; Function Attrs: nounwind
declare i32 @fread(i8* nocapture, i32, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #3

; Function Attrs: nounwind
declare i8* @fgets(i8*, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #3

; Function Attrs: nounwind
declare i8* @strtok(i8*, i8* nocapture readonly) local_unnamed_addr #3

; Function Attrs: nounwind
declare i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #3

; Function Attrs: nounwind
declare i32 @fclose(%struct._IO_FILE* nocapture) local_unnamed_addr #3

; Function Attrs: nounwind
declare void @perror(i8* nocapture readonly) local_unnamed_addr #3

; Function Attrs: nounwind
declare noalias i8* @malloc(i32) local_unnamed_addr #3

declare i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #5

; Function Attrs: nounwind
define i32 @configureCGRA(i32) local_unnamed_addr #0 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  %5 = load i32*, i32** @initCGRA, align 4, !tbaa !36
  %6 = add i32 %0, -1
  %7 = mul i32 %6, 7
  %8 = getelementptr inbounds i32, i32* %5, i32 %7
  %9 = getelementptr inbounds i32, i32* %8, i32 5
  %10 = load i32, i32* %9, align 4, !tbaa !5
  %11 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  call void @llvm.lifetime.start(i64 25, i8* nonnull %11) #6
  %12 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %12) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %12, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %13 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %11, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), i32 %0) #6
  %14 = call i8* @strcat(i8* nonnull %12, i8* nonnull %11) #6
  %15 = icmp slt i32 %10, 1
  br i1 %15, label %16, label %22

; <label>:16:                                     ; preds = %1
  %17 = load i32, i32* @dynamicTCVal, align 4, !tbaa !5
  %18 = add nsw i32 %17, %10
  %19 = load i32*, i32** @initCGRA, align 4, !tbaa !36
  %20 = getelementptr inbounds i32, i32* %19, i32 %7
  %21 = getelementptr inbounds i32, i32* %20, i32 5
  store i32 %18, i32* %21, align 4, !tbaa !5
  br label %22

; <label>:22:                                     ; preds = %16, %1
  %23 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %23) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %23, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %24 = call i32 @strlen(i8* nonnull %23)
  %25 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %24
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %25, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.20, i32 0, i32 0), i32 14, i32 1, i1 false)
  %26 = call %struct._IO_FILE* @fopen(i8* nonnull %23, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.21, i32 0, i32 0))
  br label %27

; <label>:27:                                     ; preds = %27, %22
  %28 = phi i32 [ 0, %22 ], [ %34, %27 ]
  %29 = load i32*, i32** @initCGRA, align 4, !tbaa !36
  %30 = getelementptr inbounds i32, i32* %29, i32 %7
  %31 = getelementptr inbounds i32, i32* %30, i32 %28
  %32 = load i32, i32* %31, align 4, !tbaa !5
  %33 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.22, i32 0, i32 0), i32 %32)
  %34 = add nuw nsw i32 %28, 1
  %35 = icmp eq i32 %34, 7
  br i1 %35, label %36, label %27, !llvm.loop !221

; <label>:36:                                     ; preds = %27
  %37 = load i32*, i32** @epilogPtr, align 4, !tbaa !36
  %38 = getelementptr inbounds i32, i32* %37, i32 %6
  %39 = load i32, i32* %38, align 4, !tbaa !219
  %40 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i32 %39)
  %41 = load i32*, i32** @prologPtr, align 4, !tbaa !36
  %42 = getelementptr inbounds i32, i32* %41, i32 %6
  %43 = load i32, i32* %42, align 4, !tbaa !219
  %44 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i32 %43)
  %45 = load i32*, i32** @kernelPtr, align 4, !tbaa !36
  %46 = getelementptr inbounds i32, i32* %45, i32 %6
  %47 = load i32, i32* %46, align 4, !tbaa !219
  %48 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.23, i32 0, i32 0), i32 %47)
  %49 = call i32 @fclose(%struct._IO_FILE* %26)
  call void @llvm.lifetime.end(i64 40, i8* nonnull %23) #6
  call void @llvm.lifetime.end(i64 20, i8* nonnull %12) #6
  call void @llvm.lifetime.end(i64 25, i8* nonnull %11) #6
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: nounwind
define void @checkTotalLoops() local_unnamed_addr #0 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0))
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.24, i32 0, i32 0), i32* nonnull @totalLoops) #6
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3)
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #6
  ret void
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #0 {
  br label %2

; <label>:2:                                      ; preds = %15, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !5
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %11, label %5

; <label>:5:                                      ; preds = %2
  br label %6

; <label>:6:                                      ; preds = %6, %5
  %7 = tail call i32 @usleep(i32 1) #6
  %8 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !5
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %6, !llvm.loop !236

; <label>:10:                                     ; preds = %6
  br label %11

; <label>:11:                                     ; preds = %10, %2
  %12 = load i32, i32* @thread_exit, align 4, !tbaa !5
  %13 = icmp eq i32 %12, 1
  br i1 %13, label %14, label %15

; <label>:14:                                     ; preds = %11
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #6, !srcloc !244
  ret i8* null

; <label>:15:                                     ; preds = %11
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !5
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #6, !srcloc !245
  %16 = tail call i32 @usleep(i32 1) #6
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !5
  br label %2, !llvm.loop !246
}

declare i32 @usleep(i32) local_unnamed_addr #5

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #0 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.26, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !5
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !5
  br label %4

; <label>:4:                                      ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #6
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !5
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !248

; <label>:8:                                      ; preds = %4
  ret void
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #0 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0))
  store i32 1, i32* @thread_exit, align 4, !tbaa !5
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !5
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.30, i32 0, i32 0))
  %3 = load i32, i32* @pth, align 4, !tbaa !219
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #6
  ret void
}

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) #6

declare i32 @pthread_join(i32, i8**) local_unnamed_addr #5

; Function Attrs: nounwind
define void @createCGRA() local_unnamed_addr #0 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false) #6
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i32 0, i32 0)) #6
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.24, i32 0, i32 0), i32* nonnull @totalLoops) #6
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #6
  %6 = load i32, i32* @totalLoops, align 4, !tbaa !5
  %7 = mul i32 %6, 28
  %8 = tail call noalias i8* @malloc(i32 %7) #6
  store i8* %8, i8** bitcast (i32** @initCGRA to i8**), align 4, !tbaa !36
  %9 = shl i32 %6, 2
  %10 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %10, i8** bitcast (i32** @prologPtr to i8**), align 4, !tbaa !36
  %11 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %11, i8** bitcast (i32** @kernelPtr to i8**), align 4, !tbaa !36
  %12 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %12, i8** bitcast (i32** @epilogPtr to i8**), align 4, !tbaa !36
  %13 = icmp eq i32 %6, 0
  br i1 %13, label %22, label %14

; <label>:14:                                     ; preds = %0
  br label %15

; <label>:15:                                     ; preds = %15, %14
  %16 = phi i32 [ %18, %15 ], [ 1, %14 ]
  %17 = tail call i32 @initializeParameters(i32 %16)
  %18 = add i32 %16, 1
  %19 = load i32, i32* @totalLoops, align 4, !tbaa !5
  %20 = icmp ugt i32 %18, %19
  br i1 %20, label %21, label %15, !llvm.loop !257

; <label>:21:                                     ; preds = %15
  br label %22

; <label>:22:                                     ; preds = %21, %0
  %23 = tail call i32 @puts(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str.31, i32 0, i32 0))
  %24 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #6
  ret void
}

; Function Attrs: nounwind
declare i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #3

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readonly }
attributes #5 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nounwind }
attributes #7 = { cold }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2, !3, !4}

!0 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{i32 1, !"min_enum_size", i32 4}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10}
!10 = !DILocation(line: 15, column: 1, scope: !11)
!11 = distinct !DILexicalBlock(scope: !13, file: !12, line: 12, column: 1)
!12 = !DIFile(filename: "add.c", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!13 = distinct !DISubprogram(name: "main", scope: !12, file: !12, line: 4, type: !14, isLocal: false, isDefinition: true, scopeLine: 5, isOptimized: true, unit: !17, variables: !19)
!14 = !DISubroutineType(types: !15)
!15 = !{!16}
!16 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!17 = distinct !DICompileUnit(language: DW_LANG_C99, file: !12, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !18)
!18 = !{}
!19 = !{!20, !21, !25, !26}
!20 = !DILocalVariable(name: "i", scope: !13, file: !12, line: 7, type: !16)
!21 = !DILocalVariable(name: "a", scope: !13, file: !12, line: 9, type: !22)
!22 = !DICompositeType(tag: DW_TAG_array_type, baseType: !16, size: 320000, elements: !23)
!23 = !{!24}
!24 = !DISubrange(count: 10000)
!25 = !DILocalVariable(name: "b", scope: !13, file: !12, line: 9, type: !22)
!26 = !DILocalVariable(name: "c", scope: !13, file: !12, line: 9, type: !22)
!27 = distinct !{!27, !28}
!28 = !DILocation(line: 20, column: 1, scope: !29)
!29 = distinct !DILexicalBlock(scope: !13, file: !12, line: 17, column: 1)
!30 = distinct !{!30, !31}
!31 = !DILocation(line: 27, column: 1, scope: !32)
!32 = distinct !DILexicalBlock(scope: !13, file: !12, line: 23, column: 1)
!33 = distinct !{!33, !34}
!34 = !DILocation(line: 40, column: 1, scope: !35)
!35 = distinct !DILexicalBlock(scope: !13, file: !12, line: 37, column: 1)
!36 = !{!37, !37, i64 0}
!37 = !{!"any pointer", !7, i64 0}
!38 = !{!7, !7, i64 0}
!39 = distinct !{!39, !40}
!40 = !DILocation(line: 88, column: 13, scope: !41)
!41 = distinct !DILexicalBlock(scope: !43, file: !42, line: 67, column: 13)
!42 = !DIFile(filename: "/home/mahesh/ccf/scripts/CGRALib/cgra.c", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!43 = distinct !DILexicalBlock(scope: !44, file: !42, line: 65, column: 9)
!44 = distinct !DILexicalBlock(scope: !45, file: !42, line: 62, column: 5)
!45 = distinct !DILexicalBlock(scope: !46, file: !42, line: 61, column: 6)
!46 = distinct !DISubprogram(name: "initializeParameters", scope: !42, file: !42, line: 7, type: !47, isLocal: false, isDefinition: true, scopeLine: 8, flags: DIFlagPrototyped, isOptimized: true, unit: !50, variables: !107)
!47 = !DISubroutineType(types: !48)
!48 = !{!16, !49}
!49 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!50 = distinct !DICompileUnit(language: DW_LANG_C99, file: !42, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !18, retainedTypes: !51, globals: !68)
!51 = !{!52, !53, !55, !57, !60, !63, !49, !64, !65, !16, !66}
!52 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!53 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !54, line: 62, baseType: !49)
!54 = !DIFile(filename: "/home/mahesh/ccf/llvm/build/lib/clang/5.0.0/include/stddef.h", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!55 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !56, size: 32)
!56 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!57 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !58, size: 32)
!58 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !59)
!59 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!60 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !61, size: 32)
!61 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !62)
!62 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!63 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !49, size: 32)
!64 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!65 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 32)
!66 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !67, size: 32)
!67 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !62, size: 32)
!68 = !{!69, !72, !74, !76, !78, !80, !85, !87, !89, !91, !93, !96, !98, !100, !102}
!69 = !DIGlobalVariableExpression(var: !70)
!70 = distinct !DIGlobalVariable(name: "totalLoops", scope: !50, file: !71, line: 29, type: !49, isLocal: false, isDefinition: true)
!71 = !DIFile(filename: "/home/mahesh/ccf/scripts/CGRALib/cgra.h", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!72 = !DIGlobalVariableExpression(var: !73)
!73 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !50, file: !71, line: 32, type: !49, isLocal: false, isDefinition: true)
!74 = !DIGlobalVariableExpression(var: !75)
!75 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !50, file: !71, line: 33, type: !49, isLocal: false, isDefinition: true)
!76 = !DIGlobalVariableExpression(var: !77)
!77 = distinct !DIGlobalVariable(name: "thread_exit", scope: !50, file: !71, line: 34, type: !49, isLocal: false, isDefinition: true)
!78 = !DIGlobalVariableExpression(var: !79)
!79 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !50, file: !71, line: 36, type: !16, isLocal: false, isDefinition: true)
!80 = !DIGlobalVariableExpression(var: !81)
!81 = distinct !DIGlobalVariable(name: "s", scope: !46, file: !42, line: 57, type: !82, isLocal: true, isDefinition: true)
!82 = !DICompositeType(tag: DW_TAG_array_type, baseType: !61, size: 16, elements: !83)
!83 = !{!84}
!84 = !DISubrange(count: 2)
!85 = !DIGlobalVariableExpression(var: !86)
!86 = distinct !DIGlobalVariable(name: "prolog", scope: !50, file: !71, line: 23, type: !63, isLocal: false, isDefinition: true)
!87 = !DIGlobalVariableExpression(var: !88)
!88 = distinct !DIGlobalVariable(name: "epilog", scope: !50, file: !71, line: 23, type: !63, isLocal: false, isDefinition: true)
!89 = !DIGlobalVariableExpression(var: !90)
!90 = distinct !DIGlobalVariable(name: "kernel", scope: !50, file: !71, line: 23, type: !63, isLocal: false, isDefinition: true)
!91 = !DIGlobalVariableExpression(var: !92)
!92 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !50, file: !71, line: 24, type: !63, isLocal: false, isDefinition: true)
!93 = !DIGlobalVariableExpression(var: !94)
!94 = distinct !DIGlobalVariable(name: "prologPtr", scope: !50, file: !71, line: 26, type: !95, isLocal: false, isDefinition: true)
!95 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !64, size: 32)
!96 = !DIGlobalVariableExpression(var: !97)
!97 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !50, file: !71, line: 26, type: !95, isLocal: false, isDefinition: true)
!98 = !DIGlobalVariableExpression(var: !99)
!99 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !50, file: !71, line: 26, type: !95, isLocal: false, isDefinition: true)
!100 = !DIGlobalVariableExpression(var: !101)
!101 = distinct !DIGlobalVariable(name: "initCGRA", scope: !50, file: !71, line: 28, type: !65, isLocal: false, isDefinition: true)
!102 = !DIGlobalVariableExpression(var: !103)
!103 = distinct !DIGlobalVariable(name: "pth", scope: !50, file: !71, line: 31, type: !104, isLocal: false, isDefinition: true)
!104 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !105, line: 37, baseType: !106)
!105 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!106 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!107 = !{!108, !109, !110, !168, !169, !170, !171, !172, !176, !180, !181, !182, !183, !184, !185, !186, !187, !188, !189, !190, !191, !192, !193, !194, !195, !202, !203, !205, !206, !209, !210, !212, !213, !214, !215, !216, !217, !218}
!108 = !DILocalVariable(name: "loopID", arg: 1, scope: !46, file: !42, line: 7, type: !49)
!109 = !DILocalVariable(name: "i", scope: !46, file: !42, line: 9, type: !49)
!110 = !DILocalVariable(name: "epi", scope: !46, file: !42, line: 19, type: !111)
!111 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !112, size: 32)
!112 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !113, line: 48, baseType: !114)
!113 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdio.h", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!114 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !115, line: 245, size: 1216, elements: !116)
!115 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/libio.h", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!116 = !{!117, !118, !119, !120, !121, !122, !123, !124, !125, !126, !127, !128, !129, !137, !138, !139, !140, !143, !145, !147, !151, !154, !158, !159, !160, !161, !162, !163, !164}
!117 = !DIDerivedType(tag: DW_TAG_member, name: "_flags", scope: !114, file: !115, line: 246, baseType: !16, size: 32)
!118 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_ptr", scope: !114, file: !115, line: 251, baseType: !67, size: 32, offset: 32)
!119 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_end", scope: !114, file: !115, line: 252, baseType: !67, size: 32, offset: 64)
!120 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_base", scope: !114, file: !115, line: 253, baseType: !67, size: 32, offset: 96)
!121 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_base", scope: !114, file: !115, line: 254, baseType: !67, size: 32, offset: 128)
!122 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_ptr", scope: !114, file: !115, line: 255, baseType: !67, size: 32, offset: 160)
!123 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_end", scope: !114, file: !115, line: 256, baseType: !67, size: 32, offset: 192)
!124 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_base", scope: !114, file: !115, line: 257, baseType: !67, size: 32, offset: 224)
!125 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_end", scope: !114, file: !115, line: 258, baseType: !67, size: 32, offset: 256)
!126 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_base", scope: !114, file: !115, line: 260, baseType: !67, size: 32, offset: 288)
!127 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_backup_base", scope: !114, file: !115, line: 261, baseType: !67, size: 32, offset: 320)
!128 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_end", scope: !114, file: !115, line: 262, baseType: !67, size: 32, offset: 352)
!129 = !DIDerivedType(tag: DW_TAG_member, name: "_markers", scope: !114, file: !115, line: 264, baseType: !130, size: 32, offset: 384)
!130 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !131, size: 32)
!131 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_marker", file: !115, line: 160, size: 96, elements: !132)
!132 = !{!133, !134, !136}
!133 = !DIDerivedType(tag: DW_TAG_member, name: "_next", scope: !131, file: !115, line: 161, baseType: !130, size: 32)
!134 = !DIDerivedType(tag: DW_TAG_member, name: "_sbuf", scope: !131, file: !115, line: 162, baseType: !135, size: 32, offset: 32)
!135 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !114, size: 32)
!136 = !DIDerivedType(tag: DW_TAG_member, name: "_pos", scope: !131, file: !115, line: 166, baseType: !16, size: 32, offset: 64)
!137 = !DIDerivedType(tag: DW_TAG_member, name: "_chain", scope: !114, file: !115, line: 266, baseType: !135, size: 32, offset: 416)
!138 = !DIDerivedType(tag: DW_TAG_member, name: "_fileno", scope: !114, file: !115, line: 268, baseType: !16, size: 32, offset: 448)
!139 = !DIDerivedType(tag: DW_TAG_member, name: "_flags2", scope: !114, file: !115, line: 272, baseType: !16, size: 32, offset: 480)
!140 = !DIDerivedType(tag: DW_TAG_member, name: "_old_offset", scope: !114, file: !115, line: 274, baseType: !141, size: 32, offset: 512)
!141 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off_t", file: !142, line: 131, baseType: !64)
!142 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "/home/mahesh/git_repos/ullmann/test/simple_loop")
!143 = !DIDerivedType(tag: DW_TAG_member, name: "_cur_column", scope: !114, file: !115, line: 278, baseType: !144, size: 16, offset: 544)
!144 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!145 = !DIDerivedType(tag: DW_TAG_member, name: "_vtable_offset", scope: !114, file: !115, line: 279, baseType: !146, size: 8, offset: 560)
!146 = !DIBasicType(name: "signed char", size: 8, encoding: DW_ATE_signed_char)
!147 = !DIDerivedType(tag: DW_TAG_member, name: "_shortbuf", scope: !114, file: !115, line: 280, baseType: !148, size: 8, offset: 568)
!148 = !DICompositeType(tag: DW_TAG_array_type, baseType: !62, size: 8, elements: !149)
!149 = !{!150}
!150 = !DISubrange(count: 1)
!151 = !DIDerivedType(tag: DW_TAG_member, name: "_lock", scope: !114, file: !115, line: 284, baseType: !152, size: 32, offset: 576)
!152 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !153, size: 32)
!153 = !DIDerivedType(tag: DW_TAG_typedef, name: "_IO_lock_t", file: !115, line: 154, baseType: null)
!154 = !DIDerivedType(tag: DW_TAG_member, name: "_offset", scope: !114, file: !115, line: 293, baseType: !155, size: 64, offset: 640)
!155 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off64_t", file: !142, line: 132, baseType: !156)
!156 = !DIDerivedType(tag: DW_TAG_typedef, name: "__quad_t", file: !142, line: 55, baseType: !157)
!157 = !DIBasicType(name: "long long int", size: 64, encoding: DW_ATE_signed)
!158 = !DIDerivedType(tag: DW_TAG_member, name: "__pad1", scope: !114, file: !115, line: 302, baseType: !52, size: 32, offset: 704)
!159 = !DIDerivedType(tag: DW_TAG_member, name: "__pad2", scope: !114, file: !115, line: 303, baseType: !52, size: 32, offset: 736)
!160 = !DIDerivedType(tag: DW_TAG_member, name: "__pad3", scope: !114, file: !115, line: 304, baseType: !52, size: 32, offset: 768)
!161 = !DIDerivedType(tag: DW_TAG_member, name: "__pad4", scope: !114, file: !115, line: 305, baseType: !52, size: 32, offset: 800)
!162 = !DIDerivedType(tag: DW_TAG_member, name: "__pad5", scope: !114, file: !115, line: 306, baseType: !53, size: 32, offset: 832)
!163 = !DIDerivedType(tag: DW_TAG_member, name: "_mode", scope: !114, file: !115, line: 308, baseType: !16, size: 32, offset: 864)
!164 = !DIDerivedType(tag: DW_TAG_member, name: "_unused2", scope: !114, file: !115, line: 310, baseType: !165, size: 320, offset: 896)
!165 = !DICompositeType(tag: DW_TAG_array_type, baseType: !62, size: 320, elements: !166)
!166 = !{!167}
!167 = !DISubrange(count: 40)
!168 = !DILocalVariable(name: "pro", scope: !46, file: !42, line: 19, type: !111)
!169 = !DILocalVariable(name: "kern", scope: !46, file: !42, line: 19, type: !111)
!170 = !DILocalVariable(name: "count", scope: !46, file: !42, line: 19, type: !111)
!171 = !DILocalVariable(name: "arch", scope: !46, file: !42, line: 19, type: !111)
!172 = !DILocalVariable(name: "loopno", scope: !46, file: !42, line: 20, type: !173)
!173 = !DICompositeType(tag: DW_TAG_array_type, baseType: !62, size: 200, elements: !174)
!174 = !{!175}
!175 = !DISubrange(count: 25)
!176 = !DILocalVariable(name: "directoryPath", scope: !46, file: !42, line: 21, type: !177)
!177 = !DICompositeType(tag: DW_TAG_array_type, baseType: !62, size: 160, elements: !178)
!178 = !{!179}
!179 = !DISubrange(count: 20)
!180 = !DILocalVariable(name: "epifile", scope: !46, file: !42, line: 26, type: !165)
!181 = !DILocalVariable(name: "profile", scope: !46, file: !42, line: 27, type: !165)
!182 = !DILocalVariable(name: "kernfile", scope: !46, file: !42, line: 28, type: !165)
!183 = !DILocalVariable(name: "liveOutfile", scope: !46, file: !42, line: 29, type: !165)
!184 = !DILocalVariable(name: "archfile", scope: !46, file: !42, line: 30, type: !165)
!185 = !DILocalVariable(name: "episize", scope: !46, file: !42, line: 49, type: !16)
!186 = !DILocalVariable(name: "prosize", scope: !46, file: !42, line: 49, type: !16)
!187 = !DILocalVariable(name: "kernelsize", scope: !46, file: !42, line: 49, type: !16)
!188 = !DILocalVariable(name: "livevar_st_size", scope: !46, file: !42, line: 49, type: !16)
!189 = !DILocalVariable(name: "CGRA_XDim", scope: !46, file: !42, line: 50, type: !16)
!190 = !DILocalVariable(name: "CGRA_YDim", scope: !46, file: !42, line: 50, type: !16)
!191 = !DILocalVariable(name: "token", scope: !46, file: !42, line: 58, type: !67)
!192 = !DILocalVariable(name: "X_flag", scope: !46, file: !42, line: 59, type: !16)
!193 = !DILocalVariable(name: "Y_flag", scope: !46, file: !42, line: 59, type: !16)
!194 = !DILocalVariable(name: "line", scope: !44, file: !42, line: 63, type: !177)
!195 = !DILocalVariable(name: "__s1_len", scope: !196, file: !42, line: 71, type: !53)
!196 = distinct !DILexicalBlock(scope: !197, file: !42, line: 71, column: 25)
!197 = distinct !DILexicalBlock(scope: !198, file: !42, line: 71, column: 24)
!198 = distinct !DILexicalBlock(scope: !199, file: !42, line: 70, column: 17)
!199 = distinct !DILexicalBlock(scope: !200, file: !42, line: 69, column: 20)
!200 = distinct !DILexicalBlock(scope: !201, file: !42, line: 68, column: 13)
!201 = distinct !DILexicalBlock(scope: !41, file: !42, line: 67, column: 13)
!202 = !DILocalVariable(name: "__s2_len", scope: !196, file: !42, line: 71, type: !53)
!203 = !DILocalVariable(name: "__s2", scope: !204, file: !42, line: 71, type: !57)
!204 = distinct !DILexicalBlock(scope: !196, file: !42, line: 71, column: 25)
!205 = !DILocalVariable(name: "__result", scope: !204, file: !42, line: 71, type: !16)
!206 = !DILocalVariable(name: "__s1_len", scope: !207, file: !42, line: 73, type: !53)
!207 = distinct !DILexicalBlock(scope: !208, file: !42, line: 73, column: 25)
!208 = distinct !DILexicalBlock(scope: !198, file: !42, line: 73, column: 24)
!209 = !DILocalVariable(name: "__s2_len", scope: !207, file: !42, line: 73, type: !53)
!210 = !DILocalVariable(name: "__s2", scope: !211, file: !42, line: 73, type: !57)
!211 = distinct !DILexicalBlock(scope: !207, file: !42, line: 73, column: 25)
!212 = !DILocalVariable(name: "__result", scope: !211, file: !42, line: 73, type: !16)
!213 = !DILocalVariable(name: "CGRAsize", scope: !46, file: !42, line: 95, type: !16)
!214 = !DILocalVariable(name: "II", scope: !46, file: !42, line: 119, type: !16)
!215 = !DILocalVariable(name: "epiLength", scope: !46, file: !42, line: 120, type: !16)
!216 = !DILocalVariable(name: "prolength", scope: !46, file: !42, line: 121, type: !16)
!217 = !DILocalVariable(name: "loopitfile", scope: !46, file: !42, line: 134, type: !165)
!218 = !DILocalVariable(name: "kernelCount", scope: !46, file: !42, line: 138, type: !16)
!219 = !{!220, !220, i64 0}
!220 = !{!"long", !7, i64 0}
!221 = distinct !{!221, !222}
!222 = !DILocation(line: 176, column: 58, scope: !223)
!223 = distinct !DILexicalBlock(scope: !224, file: !42, line: 175, column: 3)
!224 = distinct !DISubprogram(name: "configureCGRA", scope: !42, file: !42, line: 155, type: !47, isLocal: false, isDefinition: true, scopeLine: 156, flags: DIFlagPrototyped, isOptimized: true, unit: !50, variables: !225)
!225 = !{!226, !227, !228, !229, !230, !231, !232, !235}
!226 = !DILocalVariable(name: "loopID", arg: 1, scope: !224, file: !42, line: 155, type: !49)
!227 = !DILocalVariable(name: "i", scope: !224, file: !42, line: 157, type: !49)
!228 = !DILocalVariable(name: "kernelCount", scope: !224, file: !42, line: 158, type: !16)
!229 = !DILocalVariable(name: "count", scope: !224, file: !42, line: 159, type: !111)
!230 = !DILocalVariable(name: "loopno", scope: !224, file: !42, line: 160, type: !173)
!231 = !DILocalVariable(name: "directoryPath", scope: !224, file: !42, line: 161, type: !177)
!232 = !DILocalVariable(name: "newTC", scope: !233, file: !42, line: 168, type: !16)
!233 = distinct !DILexicalBlock(scope: !234, file: !42, line: 167, column: 3)
!234 = distinct !DILexicalBlock(scope: !224, file: !42, line: 166, column: 6)
!235 = !DILocalVariable(name: "initCGRAfile", scope: !224, file: !42, line: 172, type: !165)
!236 = distinct !{!236, !237}
!237 = !DILocation(line: 199, column: 46, scope: !238)
!238 = distinct !DILexicalBlock(scope: !239, file: !42, line: 198, column: 14)
!239 = distinct !DISubprogram(name: "runOnCGRA", scope: !42, file: !42, line: 195, type: !240, isLocal: false, isDefinition: true, scopeLine: 196, flags: DIFlagPrototyped, isOptimized: true, unit: !50, variables: !242)
!240 = !DISubroutineType(types: !241)
!241 = !{!52, !52}
!242 = !{!243}
!243 = !DILocalVariable(name: "arg", arg: 1, scope: !239, file: !42, line: 195, type: !52)
!244 = !{i32 5735}
!245 = !{i32 5843}
!246 = distinct !{!246, !247}
!247 = !DILocation(line: 210, column: 5, scope: !239)
!248 = distinct !{!248, !249}
!249 = !DILocation(line: 228, column: 41, scope: !250)
!250 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !42, file: !42, line: 214, type: !251, isLocal: false, isDefinition: true, scopeLine: 215, flags: DIFlagPrototyped, isOptimized: true, unit: !50, variables: !253)
!251 = !DISubroutineType(types: !252)
!252 = !{null, !49}
!253 = !{!254, !255, !256}
!254 = !DILocalVariable(name: "loopNo", arg: 1, scope: !250, file: !42, line: 214, type: !49)
!255 = !DILocalVariable(name: "result", scope: !250, file: !42, line: 216, type: !16)
!256 = !DILocalVariable(name: "configure", scope: !250, file: !42, line: 218, type: !16)
!257 = distinct !{!257, !258}
!258 = !DILocation(line: 256, column: 29, scope: !259)
!259 = distinct !DILexicalBlock(scope: !260, file: !42, line: 255, column: 3)
!260 = distinct !DISubprogram(name: "createCGRA", scope: !42, file: !42, line: 243, type: !261, isLocal: false, isDefinition: true, scopeLine: 244, isOptimized: true, unit: !50, variables: !263)
!261 = !DISubroutineType(types: !262)
!262 = !{null}
!263 = !{!264, !265}
!264 = !DILocalVariable(name: "result", scope: !260, file: !42, line: 245, type: !16)
!265 = !DILocalVariable(name: "i", scope: !260, file: !42, line: 246, type: !49)
