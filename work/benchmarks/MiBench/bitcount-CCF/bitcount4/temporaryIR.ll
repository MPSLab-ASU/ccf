; ModuleID = 'temp1.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.bfile = type { %struct._IO_FILE*, i8, i8, i8, i8 }
%union.pthread_attr_t = type { i32, [32 x i8] }

@.str = private unnamed_addr constant [3 x i8] c"01\00", align 1
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
@stderr = external local_unnamed_addr global %struct._IO_FILE*, align 4
@.str.7 = private unnamed_addr constant [29 x i8] c"Usage: bitcnts <iterations>\0A\00", align 1
@.str.8 = private unnamed_addr constant [33 x i8] c"Bit counter algorithm benchmark\0A\00", align 1
@.str.9 = private unnamed_addr constant [36 x i8] c"%-38s> Time: %7.3f sec.; Bits: %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [29 x i8] c"Optimized 1 bit/loop counter\00", align 1
@.str.1.2 = private unnamed_addr constant [26 x i8] c"Ratko's mystery algorithm\00", align 1
@.str.2 = private unnamed_addr constant [31 x i8] c"Recursive bit count by nybbles\00", align 1
@.str.3 = private unnamed_addr constant [35 x i8] c"Non-recursive bit count by nybbles\00", align 1
@.str.4 = private unnamed_addr constant [38 x i8] c"Non-recursive bit count by bytes (BW)\00", align 1
@.str.5 = private unnamed_addr constant [38 x i8] c"Non-recursive bit count by bytes (AR)\00", align 1
@.str.6 = private unnamed_addr constant [21 x i8] c"Shift and count bits\00", align 1
@main.text = internal unnamed_addr constant [7 x i8*] [i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0)], align 4
@.str.10 = private unnamed_addr constant [13 x i8] c"\0ABest  > %s\0A\00", align 1
@.str.11 = private unnamed_addr constant [12 x i8] c"Worst > %s\0A\00", align 1
@bits = internal unnamed_addr constant [256 x i8] c"\00\01\01\02\01\02\02\03\01\02\02\03\02\03\03\04\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\04\05\05\06\05\06\06\07\05\06\06\07\06\07\07\08", align 1
@configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
@.str.13 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1.14 = private unnamed_addr constant [16 x i8] c"/epilog_ins.bin\00", align 1
@.str.2.15 = private unnamed_addr constant [16 x i8] c"/prolog_ins.bin\00", align 1
@.str.3.16 = private unnamed_addr constant [16 x i8] c"/kernel_ins.bin\00", align 1
@.str.4.17 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.5.18 = private unnamed_addr constant [32 x i8] c"\0A**********EPISIZE %d*********\0A\00", align 1
@.str.6.19 = private unnamed_addr constant [32 x i8] c"\0A**********PROSIZE %d*********\0A\00", align 1
@.str.7.20 = private unnamed_addr constant [33 x i8] c"\0A**********KERNSIZE %d*********\0A\00", align 1
@.str.8.21 = private unnamed_addr constant [37 x i8] c"\0A******SIZE OF UNISGNED INT %d*****\0A\00", align 1
@.str.9.22 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@.str.10.23 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.11.24 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.12 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.13.25 = private unnamed_addr constant [52 x i8] c"From FILE: PROLOGPC= %x, EPILOGPC=%x,  KernelPC=%x\0A\00", align 1
@configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.14 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.15 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.17 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.18 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@.str.20 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@str = private unnamed_addr constant [14 x i8] c"deleting cgra\00"
@str.24 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00"
@str.25 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00"
@gVar1 = common local_unnamed_addr global i32 0
@gPtr1 = common local_unnamed_addr global i8* null
@gVar2 = common local_unnamed_addr global i32 0
@gPtr2 = common local_unnamed_addr global i8* null
@dmy_CGRAst_1 = common local_unnamed_addr global i8 0

; Function Attrs: nounwind readonly
define i32 @bstr_i(i8* readonly) local_unnamed_addr #0 {
  %2 = icmp eq i8* %0, null
  br i1 %2, label %.loopexit, label %3

; <label>:3:                                      ; preds = %1
  %4 = load i8, i8* %0, align 1, !tbaa !5
  %5 = icmp eq i8 %4, 0
  br i1 %5, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %3
  br label %.preheader

.preheader:                                       ; preds = %12, %.preheader.preheader
  %6 = phi i8 [ %18, %12 ], [ %4, %.preheader.preheader ]
  %7 = phi i8* [ %13, %12 ], [ %0, %.preheader.preheader ]
  %8 = phi i32 [ %17, %12 ], [ 0, %.preheader.preheader ]
  %9 = zext i8 %6 to i32
  %10 = tail call i8* @memchr(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %9, i32 3)
  %11 = icmp eq i8* %10, null
  br i1 %11, label %.loopexit.loopexit, label %12

; <label>:12:                                     ; preds = %.preheader
  %13 = getelementptr inbounds i8, i8* %7, i32 1
  %14 = shl i32 %8, 1
  %15 = and i8 %6, 1
  %16 = zext i8 %15 to i32
  %17 = or i32 %14, %16
  %18 = load i8, i8* %13, align 1, !tbaa !5
  %19 = icmp eq i8 %18, 0
  br i1 %19, label %.loopexit.loopexit, label %.preheader

.loopexit.loopexit:                               ; preds = %12, %.preheader
  %.ph = phi i32 [ %8, %.preheader ], [ %17, %12 ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %3, %1
  %20 = phi i32 [ 0, %1 ], [ 0, %3 ], [ %.ph, %.loopexit.loopexit ]
  ret i32 %20
}

; Function Attrs: nounwind readonly
declare i8* @memchr(i8*, i32, i32) local_unnamed_addr #1

; Function Attrs: nounwind
define void @bitstring(i8* nocapture, i32, i32, i32) local_unnamed_addr #2 {
  %5 = ashr i32 %2, 2
  %6 = and i32 %2, 3
  %7 = icmp eq i32 %6, 0
  %8 = zext i1 %7 to i32
  %sum = add i32 %5, %2
  %9 = sub i32 %3, %sum
  %10 = add i32 %9, %8
  %11 = icmp sgt i32 %10, 0
  br i1 %11, label %12, label %14

; <label>:12:                                     ; preds = %4
  tail call void @llvm.memset.p0i8.i32(i8* %0, i8 32, i32 %10, i32 1, i1 false)
  %13 = getelementptr i8, i8* %0, i32 %10
  br label %14

; <label>:14:                                     ; preds = %12, %4
  %15 = phi i8* [ %0, %4 ], [ %13, %12 ]
  %16 = icmp sgt i32 %2, 0
  store i8* %15, i8** @gPtr1
  br i1 %16, label %.preheader.preheader, label %.loopexit

.preheader.preheader:                             ; preds = %14
  store i32 %2, i32* @gVar1
  store i32 %1, i32* @gVar2
  %17 = sub i32 1, %2
  %18 = sdiv i32 %17, -1
  %19 = add i32 %18, 1
  store i32 %19, i32* @dynamicTCVal
  br label %20

; <label>:20:                                     ; preds = %.preheader.preheader
  call void @accelerateOnCGRA(i32 1)
  br label %.loopexit.loopexit

.loopexit.loopexit:                               ; preds = %20
  br label %21

; <label>:21:                                     ; preds = %.loopexit.loopexit
  %22 = load i8*, i8** @gPtr2
  br label %.loopexit

.loopexit:                                        ; preds = %21, %14
  %23 = phi i8* [ %15, %14 ], [ %22, %21 ]
  store i8 0, i8* %23, align 1, !tbaa !5
  ret void
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i32, i1) #3

; Function Attrs: nounwind
define noalias %struct.bfile* @bfopen(i8* nocapture readonly, i8* nocapture readonly) local_unnamed_addr #2 {
  %3 = tail call noalias i8* @malloc(i32 8) #5
  %4 = bitcast i8* %3 to %struct.bfile*
  %5 = icmp eq i8* %3, null
  br i1 %5, label %14, label %6

; <label>:6:                                      ; preds = %2
  %7 = tail call %struct._IO_FILE* @fopen(i8* %0, i8* %1)
  %8 = bitcast i8* %3 to %struct._IO_FILE**
  store %struct._IO_FILE* %7, %struct._IO_FILE** %8, align 4, !tbaa !8
  %9 = icmp eq %struct._IO_FILE* %7, null
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %6
  tail call void @free(i8* nonnull %3) #5
  br label %14

; <label>:11:                                     ; preds = %6
  %12 = getelementptr inbounds i8, i8* %3, i32 5
  store i8 0, i8* %12, align 1, !tbaa !11
  %13 = getelementptr inbounds i8, i8* %3, i32 7
  store i8 0, i8* %13, align 1, !tbaa !12
  br label %14

; <label>:14:                                     ; preds = %11, %10, %2
  %15 = phi %struct.bfile* [ null, %10 ], [ %4, %11 ], [ null, %2 ]
  ret %struct.bfile* %15
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i32) local_unnamed_addr #4

; Function Attrs: nounwind
declare noalias %struct._IO_FILE* @fopen(i8* nocapture readonly, i8* nocapture readonly) local_unnamed_addr #4

; Function Attrs: nounwind
declare void @free(i8* nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define i32 @bfread(%struct.bfile* nocapture) local_unnamed_addr #2 {
  %2 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 2
  %3 = load i8, i8* %2, align 1, !tbaa !11
  %4 = icmp eq i8 %3, 0
  br i1 %4, label %9, label %5

; <label>:5:                                      ; preds = %1
  %6 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 1
  %7 = load i8, i8* %6, align 4, !tbaa !13
  %8 = add i8 %3, -1
  br label %15

; <label>:9:                                      ; preds = %1
  %10 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 0
  %11 = load %struct._IO_FILE*, %struct._IO_FILE** %10, align 4, !tbaa !8
  %12 = tail call i32 @fgetc(%struct._IO_FILE* %11)
  %13 = trunc i32 %12 to i8
  %14 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 1
  store i8 %13, i8* %14, align 4, !tbaa !13
  store i8 8, i8* %2, align 1, !tbaa !11
  br label %15

; <label>:15:                                     ; preds = %9, %5
  %16 = phi i8 [ %13, %9 ], [ %7, %5 ]
  %17 = phi i8 [ 7, %9 ], [ %8, %5 ]
  store i8 %17, i8* %2, align 1, !tbaa !11
  %18 = zext i8 %16 to i32
  %19 = zext i8 %17 to i32
  %20 = shl i32 1, %19
  %21 = and i32 %20, %18
  %22 = icmp ne i32 %21, 0
  %23 = zext i1 %22 to i32
  ret i32 %23
}

; Function Attrs: nounwind
declare i32 @fgetc(%struct._IO_FILE* nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define void @bfwrite(i32, %struct.bfile* nocapture) local_unnamed_addr #2 {
  %3 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 4
  %4 = load i8, i8* %3, align 1, !tbaa !12
  %5 = icmp eq i8 %4, 8
  %6 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 3
  br i1 %5, label %9, label %7

; <label>:7:                                      ; preds = %2
  %8 = add i8 %4, 1
  br label %15

; <label>:9:                                      ; preds = %2
  %10 = load i8, i8* %6, align 2, !tbaa !14
  %11 = zext i8 %10 to i32
  %12 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 0
  %13 = load %struct._IO_FILE*, %struct._IO_FILE** %12, align 4, !tbaa !8
  %14 = tail call i32 @fputc(i32 %11, %struct._IO_FILE* %13)
  store i8 0, i8* %3, align 1, !tbaa !12
  br label %15

; <label>:15:                                     ; preds = %9, %7
  %16 = phi i8 [ %8, %7 ], [ 1, %9 ]
  store i8 %16, i8* %3, align 1, !tbaa !12
  %17 = load i8, i8* %6, align 2, !tbaa !14
  %18 = zext i8 %17 to i32
  %19 = shl nuw nsw i32 %18, 1
  %20 = and i32 %0, 1
  %21 = or i32 %19, %20
  %22 = trunc i32 %21 to i8
  store i8 %22, i8* %6, align 2, !tbaa !14
  ret void
}

; Function Attrs: nounwind
declare i32 @fputc(i32, %struct._IO_FILE* nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define void @bfclose(%struct.bfile* nocapture) local_unnamed_addr #2 {
  %2 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 0
  %3 = load %struct._IO_FILE*, %struct._IO_FILE** %2, align 4, !tbaa !8
  %4 = tail call i32 @fclose(%struct._IO_FILE* %3)
  %5 = bitcast %struct.bfile* %0 to i8*
  tail call void @free(i8* %5) #5
  ret void
}

; Function Attrs: nounwind
declare i32 @fclose(%struct._IO_FILE* nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define i32 @main(i32, i8** nocapture readonly) local_unnamed_addr #2 {
  %3 = icmp slt i32 %0, 2
  br i1 %3, label %4, label %7

; <label>:4:                                      ; preds = %2
  %5 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !15
  %6 = tail call i32 @fwrite(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.7, i32 0, i32 0), i32 28, i32 1, %struct._IO_FILE* %5) #11
  tail call void @exit(i32 -1) #12
  unreachable

; <label>:7:                                      ; preds = %2
  %8 = getelementptr inbounds i8*, i8** %1, i32 1
  %9 = load i8*, i8** %8, align 4, !tbaa !15
  %10 = tail call i32 @strtol(i8* nocapture nonnull %9, i8** null, i32 10) #5
  %11 = tail call i32 @puts(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.8, i32 0, i32 0))
  %12 = icmp sgt i32 %10, 0
  %13 = tail call i32 @clock() #5
  %14 = tail call i32 @rand() #5
  br i1 %12, label %.preheader1.preheader, label %15

.preheader1.preheader:                            ; preds = %7
  br label %.preheader1

; <label>:15:                                     ; preds = %7
  %16 = tail call i32 @clock() #5
  %17 = sub nsw i32 %16, %13
  %18 = sitofp i32 %17 to double
  %19 = fdiv double %18, 1.000000e+06
  %20 = fcmp olt double %19, 0x7FEFFFFFFFFFFFFF
  %21 = select i1 %20, double %19, double 0x7FEFFFFFFFFFFFFF
  %22 = fcmp ogt double %19, 0.000000e+00
  %23 = select i1 %22, double %19, double 0.000000e+00
  %24 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), double %19, i32 0)
  %25 = tail call i32 @clock() #5
  %26 = tail call i32 @rand() #5
  %27 = tail call i32 @clock() #5
  %28 = sub nsw i32 %27, %25
  %29 = sitofp i32 %28 to double
  %30 = fdiv double %29, 1.000000e+06
  %31 = fcmp olt double %30, %21
  %32 = zext i1 %31 to i32
  %33 = select i1 %31, double %30, double %21
  %34 = fcmp ogt double %30, %23
  %35 = select i1 %34, double %30, double %23
  %36 = zext i1 %34 to i32
  %37 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), double %30, i32 0)
  %38 = tail call i32 @clock() #5
  %39 = tail call i32 @rand() #5
  %40 = tail call i32 @clock() #5
  %41 = sub nsw i32 %40, %38
  %42 = sitofp i32 %41 to double
  %43 = fdiv double %42, 1.000000e+06
  %44 = fcmp olt double %43, %33
  %45 = select i1 %44, i32 2, i32 %32
  %46 = select i1 %44, double %43, double %33
  %47 = fcmp ogt double %43, %35
  %48 = select i1 %47, double %43, double %35
  %49 = select i1 %47, i32 2, i32 %36
  %50 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), double %43, i32 0)
  %51 = tail call i32 @clock() #5
  %52 = tail call i32 @rand() #5
  %53 = tail call i32 @clock() #5
  %54 = sub nsw i32 %53, %51
  %55 = sitofp i32 %54 to double
  %56 = fdiv double %55, 1.000000e+06
  %57 = fcmp olt double %56, %46
  %58 = select i1 %57, i32 3, i32 %45
  %59 = select i1 %57, double %56, double %46
  %60 = fcmp ogt double %56, %48
  %61 = select i1 %60, double %56, double %48
  %62 = select i1 %60, i32 3, i32 %49
  %63 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), double %56, i32 0)
  %64 = tail call i32 @clock() #5
  %65 = tail call i32 @rand() #5
  %66 = tail call i32 @clock() #5
  %67 = sub nsw i32 %66, %64
  %68 = sitofp i32 %67 to double
  %69 = fdiv double %68, 1.000000e+06
  %70 = fcmp olt double %69, %59
  %71 = select i1 %70, i32 4, i32 %58
  %72 = select i1 %70, double %69, double %59
  %73 = fcmp ogt double %69, %61
  %74 = select i1 %73, double %69, double %61
  %75 = select i1 %73, i32 4, i32 %62
  %76 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), double %69, i32 0)
  %77 = tail call i32 @clock() #5
  %78 = tail call i32 @rand() #5
  %79 = tail call i32 @clock() #5
  %80 = sub nsw i32 %79, %77
  %81 = sitofp i32 %80 to double
  %82 = fdiv double %81, 1.000000e+06
  %83 = fcmp olt double %82, %72
  %84 = select i1 %83, i32 5, i32 %71
  %85 = select i1 %83, double %82, double %72
  %86 = fcmp ogt double %82, %74
  %87 = select i1 %86, double %82, double %74
  %88 = select i1 %86, i32 5, i32 %75
  %89 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), double %82, i32 0)
  %90 = tail call i32 @clock() #5
  %91 = tail call i32 @rand() #5
  %92 = tail call i32 @clock() #5
  %93 = sub nsw i32 %92, %90
  %94 = sitofp i32 %93 to double
  %95 = fdiv double %94, 1.000000e+06
  %96 = fcmp olt double %95, %85
  %97 = select i1 %96, i32 6, i32 %84
  %98 = fcmp ogt double %95, %87
  %99 = select i1 %98, i32 6, i32 %88
  %100 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), double %95, i32 0)
  br label %150

.preheader1:                                      ; preds = %bit_count.exit, %.preheader1.preheader
  %101 = phi i32 [ %114, %bit_count.exit ], [ %14, %.preheader1.preheader ]
  %102 = phi i32 [ %112, %bit_count.exit ], [ 0, %.preheader1.preheader ]
  %103 = phi i32 [ %113, %bit_count.exit ], [ 0, %.preheader1.preheader ]
  %104 = icmp eq i32 %101, 0
  br i1 %104, label %bit_count.exit, label %.preheader.i.preheader, !llvm.loop !16

.preheader.i.preheader:                           ; preds = %.preheader1
  br label %.preheader.i

.preheader.i:                                     ; preds = %.preheader.i, %.preheader.i.preheader
  %105 = phi i32 [ %109, %.preheader.i ], [ %101, %.preheader.i.preheader ]
  %106 = phi i32 [ %107, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %107 = add nuw nsw i32 %106, 1
  %108 = add nsw i32 %105, -1
  %109 = and i32 %108, %105
  %110 = icmp eq i32 %109, 0
  br i1 %110, label %bit_count.exit.loopexit, label %.preheader.i, !llvm.loop !16

bit_count.exit.loopexit:                          ; preds = %.preheader.i
  %.lcssa8 = phi i32 [ %107, %.preheader.i ]
  br label %bit_count.exit

bit_count.exit:                                   ; preds = %bit_count.exit.loopexit, %.preheader1
  %111 = phi i32 [ 0, %.preheader1 ], [ %.lcssa8, %bit_count.exit.loopexit ]
  %112 = add nsw i32 %111, %102
  %113 = add nuw nsw i32 %103, 1
  %114 = add nsw i32 %101, 13
  %115 = icmp eq i32 %113, %10
  br i1 %115, label %116, label %.preheader1, !llvm.loop !30

; <label>:116:                                    ; preds = %bit_count.exit
  %.lcssa9 = phi i32 [ %112, %bit_count.exit ]
  %117 = tail call i32 @clock() #5
  %118 = sub nsw i32 %117, %13
  %119 = sitofp i32 %118 to double
  %120 = fdiv double %119, 1.000000e+06
  %121 = fcmp olt double %120, 0x7FEFFFFFFFFFFFFF
  %122 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), double %120, i32 %.lcssa9)
  %123 = tail call i32 @clock() #5
  %124 = tail call i32 @rand() #5
  %min.iters.check = icmp ult i32 %10, 4
  br i1 %min.iters.check, label %scalar.ph.preheader, label %min.iters.checked

scalar.ph.preheader:                              ; preds = %middle.block, %min.iters.checked, %116
  %.ph = phi i32 [ %124, %min.iters.checked ], [ %124, %116 ], [ %ind.end, %middle.block ]
  %.ph39 = phi i32 [ 0, %min.iters.checked ], [ 0, %116 ], [ %149, %middle.block ]
  %.ph40 = phi i32 [ 0, %min.iters.checked ], [ 0, %116 ], [ %n.vec, %middle.block ]
  br label %scalar.ph

min.iters.checked:                                ; preds = %116
  %n.mod.vf = and i32 %10, 3
  %n.vec = sub i32 %10, %n.mod.vf
  %cmp.zero = icmp eq i32 %n.vec, 0
  %125 = mul i32 %10, 13
  %126 = add i32 %124, %125
  %127 = mul nsw i32 %n.mod.vf, -13
  %ind.end = add i32 %126, %127
  br i1 %cmp.zero, label %scalar.ph.preheader, label %vector.ph

vector.ph:                                        ; preds = %min.iters.checked
  %.splatinsert = insertelement <4 x i32> undef, i32 %124, i32 0
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> undef, <4 x i32> zeroinitializer
  %induction = add <4 x i32> %.splat, <i32 0, i32 13, i32 26, i32 39>
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i32 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %vec.ind = phi <4 x i32> [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi <4 x i32> [ zeroinitializer, %vector.ph ], [ %147, %vector.body ]
  %128 = lshr <4 x i32> %vec.ind, <i32 1, i32 1, i32 1, i32 1>
  %129 = and <4 x i32> %128, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>
  %130 = and <4 x i32> %vec.ind, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>
  %131 = add nuw <4 x i32> %129, %130
  %132 = lshr <4 x i32> %131, <i32 2, i32 2, i32 2, i32 2>
  %133 = and <4 x i32> %132, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>
  %134 = and <4 x i32> %131, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>
  %135 = add nuw nsw <4 x i32> %133, %134
  %136 = lshr <4 x i32> %135, <i32 4, i32 4, i32 4, i32 4>
  %137 = and <4 x i32> %136, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>
  %138 = and <4 x i32> %135, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>
  %139 = add nuw nsw <4 x i32> %137, %138
  %140 = lshr <4 x i32> %139, <i32 8, i32 8, i32 8, i32 8>
  %141 = and <4 x i32> %140, <i32 983055, i32 983055, i32 983055, i32 983055>
  %142 = and <4 x i32> %139, <i32 983055, i32 983055, i32 983055, i32 983055>
  %143 = add nuw nsw <4 x i32> %141, %142
  %144 = lshr <4 x i32> %143, <i32 16, i32 16, i32 16, i32 16>
  %145 = and <4 x i32> %143, <i32 31, i32 31, i32 31, i32 31>
  %146 = add <4 x i32> %145, %vec.phi
  %147 = add <4 x i32> %146, %144
  %index.next = add i32 %index, 4
  %vec.ind.next = add <4 x i32> %vec.ind, <i32 52, i32 52, i32 52, i32 52>
  %148 = icmp eq i32 %index.next, %n.vec
  br i1 %148, label %middle.block, label %vector.body, !llvm.loop !76

middle.block:                                     ; preds = %vector.body
  %.lcssa7 = phi <4 x i32> [ %147, %vector.body ]
  %rdx.shuf = shufflevector <4 x i32> %.lcssa7, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  %bin.rdx = add <4 x i32> %.lcssa7, %rdx.shuf
  %rdx.shuf36 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  %bin.rdx37 = add <4 x i32> %bin.rdx, %rdx.shuf36
  %149 = extractelement <4 x i32> %bin.rdx37, i32 0
  %cmp.n = icmp eq i32 %n.mod.vf, 0
  br i1 %cmp.n, label %.loopexit38, label %scalar.ph.preheader

; <label>:150:                                    ; preds = %392, %15
  %151 = phi i32 [ %403, %392 ], [ %97, %15 ]
  %152 = phi i32 [ %405, %392 ], [ %99, %15 ]
  %153 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %151
  %154 = load i8*, i8** %153, align 4, !tbaa !15
  %155 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.10, i32 0, i32 0), i8* %154)
  %156 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %152
  %157 = load i8*, i8** %156, align 4, !tbaa !15
  %158 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.11, i32 0, i32 0), i8* %157)
  ret i32 0

scalar.ph:                                        ; preds = %scalar.ph, %scalar.ph.preheader
  %159 = phi i32 [ %183, %scalar.ph ], [ %.ph, %scalar.ph.preheader ]
  %160 = phi i32 [ %181, %scalar.ph ], [ %.ph39, %scalar.ph.preheader ]
  %161 = phi i32 [ %182, %scalar.ph ], [ %.ph40, %scalar.ph.preheader ]
  %162 = lshr i32 %159, 1
  %163 = and i32 %162, 1431655765
  %164 = and i32 %159, 1431655765
  %165 = add nuw i32 %163, %164
  %166 = lshr i32 %165, 2
  %167 = and i32 %166, 858993459
  %168 = and i32 %165, 858993459
  %169 = add nuw nsw i32 %167, %168
  %170 = lshr i32 %169, 4
  %171 = and i32 %170, 117901063
  %172 = and i32 %169, 117901063
  %173 = add nuw nsw i32 %171, %172
  %174 = lshr i32 %173, 8
  %175 = and i32 %174, 983055
  %176 = and i32 %173, 983055
  %177 = add nuw nsw i32 %175, %176
  %178 = lshr i32 %177, 16
  %179 = and i32 %177, 31
  %180 = add i32 %179, %160
  %181 = add i32 %180, %178
  %182 = add nuw nsw i32 %161, 1
  %183 = add nsw i32 %159, 13
  %184 = icmp eq i32 %182, %10
  br i1 %184, label %.loopexit38.loopexit, label %scalar.ph, !llvm.loop !79

.loopexit38.loopexit:                             ; preds = %scalar.ph
  %.lcssa6 = phi i32 [ %181, %scalar.ph ]
  br label %.loopexit38

.loopexit38:                                      ; preds = %.loopexit38.loopexit, %middle.block
  %.lcssa31 = phi i32 [ %149, %middle.block ], [ %.lcssa6, %.loopexit38.loopexit ]
  %185 = fcmp ogt double %120, 0.000000e+00
  %186 = select i1 %121, double %120, double 0x7FEFFFFFFFFFFFFF
  %187 = select i1 %185, double %120, double 0.000000e+00
  %188 = tail call i32 @clock() #5
  %189 = sub nsw i32 %188, %123
  %190 = sitofp i32 %189 to double
  %191 = fdiv double %190, 1.000000e+06
  %192 = fcmp olt double %191, %186
  %193 = fcmp ogt double %191, %187
  %194 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), double %191, i32 %.lcssa31)
  %195 = tail call i32 @clock() #5
  %196 = tail call i32 @rand() #5
  br label %197

; <label>:197:                                    ; preds = %197, %.loopexit38
  %198 = phi i32 [ %196, %.loopexit38 ], [ %204, %197 ]
  %199 = phi i32 [ 0, %.loopexit38 ], [ %202, %197 ]
  %200 = phi i32 [ 0, %.loopexit38 ], [ %203, %197 ]
  %201 = tail call i32 @ntbl_bitcnt(i32 %198) #5
  %202 = add nsw i32 %201, %199
  %203 = add nuw nsw i32 %200, 1
  %204 = add nsw i32 %198, 13
  %205 = icmp eq i32 %203, %10
  br i1 %205, label %206, label %197, !llvm.loop !30

; <label>:206:                                    ; preds = %197
  %.lcssa5 = phi i32 [ %202, %197 ]
  %207 = zext i1 %192 to i32
  %208 = select i1 %193, double %191, double %187
  %209 = select i1 %192, double %191, double %186
  %210 = tail call i32 @clock() #5
  %211 = sub nsw i32 %210, %195
  %212 = sitofp i32 %211 to double
  %213 = fdiv double %212, 1.000000e+06
  %214 = fcmp olt double %213, %209
  %215 = fcmp ogt double %213, %208
  %216 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), double %213, i32 %.lcssa5)
  %217 = tail call i32 @clock() #5
  %218 = tail call i32 @rand() #5
  br label %219

; <label>:219:                                    ; preds = %219, %206
  %220 = phi i32 [ %218, %206 ], [ %270, %219 ]
  %221 = phi i32 [ 0, %206 ], [ %268, %219 ]
  %222 = phi i32 [ 0, %206 ], [ %269, %219 ]
  %223 = and i32 %220, 15
  %224 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %223
  %225 = load i8, i8* %224, align 1, !tbaa !5
  %226 = zext i8 %225 to i32
  %227 = lshr i32 %220, 4
  %228 = and i32 %227, 15
  %229 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %228
  %230 = load i8, i8* %229, align 1, !tbaa !5
  %231 = zext i8 %230 to i32
  %232 = lshr i32 %220, 8
  %233 = and i32 %232, 15
  %234 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %233
  %235 = load i8, i8* %234, align 1, !tbaa !5
  %236 = zext i8 %235 to i32
  %237 = lshr i32 %220, 12
  %238 = and i32 %237, 15
  %239 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %238
  %240 = load i8, i8* %239, align 1, !tbaa !5
  %241 = zext i8 %240 to i32
  %242 = lshr i32 %220, 16
  %243 = and i32 %242, 15
  %244 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %243
  %245 = load i8, i8* %244, align 1, !tbaa !5
  %246 = zext i8 %245 to i32
  %247 = lshr i32 %220, 20
  %248 = and i32 %247, 15
  %249 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %248
  %250 = load i8, i8* %249, align 1, !tbaa !5
  %251 = zext i8 %250 to i32
  %252 = lshr i32 %220, 24
  %253 = and i32 %252, 15
  %254 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %253
  %255 = load i8, i8* %254, align 1, !tbaa !5
  %256 = zext i8 %255 to i32
  %257 = lshr i32 %220, 28
  %258 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %257
  %259 = load i8, i8* %258, align 1, !tbaa !5
  %260 = zext i8 %259 to i32
  %261 = add i32 %221, %226
  %262 = add i32 %261, %231
  %263 = add i32 %262, %236
  %264 = add i32 %263, %241
  %265 = add i32 %264, %246
  %266 = add i32 %265, %251
  %267 = add i32 %266, %256
  %268 = add i32 %267, %260
  %269 = add nuw nsw i32 %222, 1
  %270 = add nsw i32 %220, 13
  %271 = icmp eq i32 %269, %10
  br i1 %271, label %272, label %219, !llvm.loop !30

; <label>:272:                                    ; preds = %219
  %.lcssa4 = phi i32 [ %268, %219 ]
  %273 = zext i1 %193 to i32
  %274 = select i1 %214, i32 2, i32 %207
  %275 = select i1 %215, double %213, double %208
  %276 = select i1 %214, double %213, double %209
  %277 = tail call i32 @clock() #5
  %278 = sub nsw i32 %277, %217
  %279 = sitofp i32 %278 to double
  %280 = fdiv double %279, 1.000000e+06
  %281 = fcmp olt double %280, %276
  %282 = fcmp ogt double %280, %275
  %283 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), double %280, i32 %.lcssa4)
  %284 = tail call i32 @clock() #5
  %285 = tail call i32 @rand() #5
  br label %286

; <label>:286:                                    ; preds = %286, %272
  %287 = phi i32 [ %285, %272 ], [ %313, %286 ]
  %288 = phi i32 [ 0, %272 ], [ %311, %286 ]
  %289 = phi i32 [ 0, %272 ], [ %312, %286 ]
  %290 = lshr i32 %287, 8
  %291 = lshr i32 %287, 16
  %292 = lshr i32 %287, 24
  %293 = and i32 %287, 255
  %294 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %293
  %295 = load i8, i8* %294, align 1, !tbaa !5
  %296 = zext i8 %295 to i32
  %297 = and i32 %290, 255
  %298 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %297
  %299 = load i8, i8* %298, align 1, !tbaa !5
  %300 = zext i8 %299 to i32
  %301 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %292
  %302 = load i8, i8* %301, align 1, !tbaa !5
  %303 = zext i8 %302 to i32
  %304 = and i32 %291, 255
  %305 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %304
  %306 = load i8, i8* %305, align 1, !tbaa !5
  %307 = zext i8 %306 to i32
  %308 = add i32 %288, %296
  %309 = add i32 %308, %300
  %310 = add i32 %309, %303
  %311 = add i32 %310, %307
  %312 = add nuw nsw i32 %289, 1
  %313 = add nsw i32 %287, 13
  %314 = icmp eq i32 %312, %10
  br i1 %314, label %315, label %286, !llvm.loop !30

; <label>:315:                                    ; preds = %286
  %.lcssa3 = phi i32 [ %311, %286 ]
  %316 = select i1 %215, i32 2, i32 %273
  %317 = select i1 %281, i32 3, i32 %274
  %318 = select i1 %282, double %280, double %275
  %319 = select i1 %281, double %280, double %276
  %320 = tail call i32 @clock() #5
  %321 = sub nsw i32 %320, %284
  %322 = sitofp i32 %321 to double
  %323 = fdiv double %322, 1.000000e+06
  %324 = fcmp olt double %323, %319
  %325 = fcmp ogt double %323, %318
  %326 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), double %323, i32 %.lcssa3)
  %327 = tail call i32 @clock() #5
  %328 = tail call i32 @rand() #5
  br label %329

; <label>:329:                                    ; preds = %329, %315
  %330 = phi i32 [ %328, %315 ], [ %356, %329 ]
  %331 = phi i32 [ 0, %315 ], [ %354, %329 ]
  %332 = phi i32 [ 0, %315 ], [ %355, %329 ]
  %333 = lshr i32 %330, 8
  %334 = lshr i32 %330, 16
  %335 = lshr i32 %330, 24
  %336 = and i32 %330, 255
  %337 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %336
  %338 = load i8, i8* %337, align 1, !tbaa !5
  %339 = zext i8 %338 to i32
  %340 = and i32 %333, 255
  %341 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %340
  %342 = load i8, i8* %341, align 1, !tbaa !5
  %343 = zext i8 %342 to i32
  %344 = and i32 %334, 255
  %345 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %344
  %346 = load i8, i8* %345, align 1, !tbaa !5
  %347 = zext i8 %346 to i32
  %348 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %335
  %349 = load i8, i8* %348, align 1, !tbaa !5
  %350 = zext i8 %349 to i32
  %351 = add i32 %331, %339
  %352 = add i32 %351, %343
  %353 = add i32 %352, %347
  %354 = add i32 %353, %350
  %355 = add nuw nsw i32 %332, 1
  %356 = add nsw i32 %330, 13
  %357 = icmp eq i32 %355, %10
  br i1 %357, label %358, label %329, !llvm.loop !30

; <label>:358:                                    ; preds = %329
  %.lcssa2 = phi i32 [ %354, %329 ]
  %359 = select i1 %282, i32 3, i32 %316
  %360 = select i1 %324, i32 4, i32 %317
  %361 = select i1 %325, double %323, double %318
  %362 = select i1 %324, double %323, double %319
  %363 = tail call i32 @clock() #5
  %364 = sub nsw i32 %363, %327
  %365 = sitofp i32 %364 to double
  %366 = fdiv double %365, 1.000000e+06
  %367 = fcmp olt double %366, %362
  %368 = fcmp ogt double %366, %361
  %369 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), double %366, i32 %.lcssa2)
  %370 = tail call i32 @clock() #5
  %371 = tail call i32 @rand() #5
  br label %372

; <label>:372:                                    ; preds = %.loopexit, %358
  %373 = phi i32 [ %371, %358 ], [ %390, %.loopexit ]
  %374 = phi i32 [ 0, %358 ], [ %388, %.loopexit ]
  %375 = phi i32 [ 0, %358 ], [ %389, %.loopexit ]
  %376 = icmp eq i32 %373, 0
  br i1 %376, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %372
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %377 = phi i32 [ %381, %.preheader ], [ 0, %.preheader.preheader ]
  %378 = phi i32 [ %382, %.preheader ], [ 0, %.preheader.preheader ]
  %379 = phi i32 [ %383, %.preheader ], [ %373, %.preheader.preheader ]
  %380 = and i32 %379, 1
  %381 = add nsw i32 %380, %377
  %382 = add nuw nsw i32 %378, 1
  %383 = ashr i32 %379, 1
  %384 = icmp ne i32 %383, 0
  %385 = icmp ult i32 %382, 32
  %386 = and i1 %385, %384
  br i1 %386, label %.preheader, label %.loopexit.loopexit, !llvm.loop !81

.loopexit.loopexit:                               ; preds = %.preheader
  %.lcssa = phi i32 [ %381, %.preheader ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %372
  %387 = phi i32 [ 0, %372 ], [ %.lcssa, %.loopexit.loopexit ]
  %388 = add nsw i32 %387, %374
  %389 = add nuw nsw i32 %375, 1
  %390 = add nsw i32 %373, 13
  %391 = icmp eq i32 %389, %10
  br i1 %391, label %392, label %372, !llvm.loop !30

; <label>:392:                                    ; preds = %.loopexit
  %.lcssa1 = phi i32 [ %388, %.loopexit ]
  %393 = select i1 %325, i32 4, i32 %359
  %394 = select i1 %367, i32 5, i32 %360
  %395 = select i1 %368, double %366, double %361
  %396 = select i1 %367, double %366, double %362
  %397 = select i1 %368, i32 5, i32 %393
  %398 = tail call i32 @clock() #5
  %399 = sub nsw i32 %398, %370
  %400 = sitofp i32 %399 to double
  %401 = fdiv double %400, 1.000000e+06
  %402 = fcmp olt double %401, %396
  %403 = select i1 %402, i32 6, i32 %394
  %404 = fcmp ogt double %401, %395
  %405 = select i1 %404, i32 6, i32 %397
  %406 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), double %401, i32 %.lcssa1)
  br label %150
}

; Function Attrs: nounwind
declare i32 @fwrite(i8* nocapture, i32, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #5

; Function Attrs: noreturn nounwind
declare void @exit(i32) local_unnamed_addr #6

; Function Attrs: nounwind
declare i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #4

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #4

; Function Attrs: nounwind
declare i32 @clock() local_unnamed_addr #4

; Function Attrs: nounwind
declare i32 @rand() local_unnamed_addr #4

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #4

; Function Attrs: norecurse nounwind readnone
define i32 @bit_count(i32) local_unnamed_addr #7 {
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %.loopexit, label %.preheader.preheader, !llvm.loop !16

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %3 = phi i32 [ %7, %.preheader ], [ %0, %.preheader.preheader ]
  %4 = phi i32 [ %5, %.preheader ], [ 0, %.preheader.preheader ]
  %5 = add nuw nsw i32 %4, 1
  %6 = add nsw i32 %3, -1
  %7 = and i32 %6, %3
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !llvm.loop !16

.loopexit.loopexit:                               ; preds = %.preheader
  %.lcssa = phi i32 [ %5, %.preheader ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %9 = phi i32 [ 0, %1 ], [ %.lcssa, %.loopexit.loopexit ]
  ret i32 %9
}

; Function Attrs: norecurse nounwind readnone
define i32 @bitcount(i32) local_unnamed_addr #7 {
  %2 = lshr i32 %0, 1
  %3 = and i32 %2, 1431655765
  %4 = and i32 %0, 1431655765
  %5 = add nuw i32 %3, %4
  %6 = lshr i32 %5, 2
  %7 = and i32 %6, 858993459
  %8 = and i32 %5, 858993459
  %9 = add nuw nsw i32 %7, %8
  %10 = lshr i32 %9, 4
  %11 = and i32 %10, 117901063
  %12 = and i32 %9, 117901063
  %13 = add nuw nsw i32 %11, %12
  %14 = lshr i32 %13, 8
  %15 = and i32 %14, 983055
  %16 = and i32 %13, 983055
  %17 = add nuw nsw i32 %15, %16
  %18 = lshr i32 %17, 16
  %19 = and i32 %17, 31
  %20 = add nuw nsw i32 %18, %19
  ret i32 %20
}

; Function Attrs: nounwind readnone
define i32 @ntbl_bitcnt(i32) local_unnamed_addr #8 {
  %2 = and i32 %0, 15
  %3 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %2
  %4 = load i8, i8* %3, align 1, !tbaa !5
  %5 = zext i8 %4 to i32
  %6 = ashr i32 %0, 4
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %11, label %8

; <label>:8:                                      ; preds = %1
  %9 = tail call i32 @ntbl_bitcnt(i32 %6)
  %10 = add nsw i32 %9, %5
  ret i32 %10

; <label>:11:                                     ; preds = %1
  ret i32 %5
}

; Function Attrs: norecurse nounwind readnone
define i32 @ntbl_bitcount(i32) local_unnamed_addr #7 {
  %2 = and i32 %0, 15
  %3 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %2
  %4 = load i8, i8* %3, align 1, !tbaa !5
  %5 = zext i8 %4 to i32
  %6 = lshr i32 %0, 4
  %7 = and i32 %6, 15
  %8 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %7
  %9 = load i8, i8* %8, align 1, !tbaa !5
  %10 = zext i8 %9 to i32
  %11 = add nuw nsw i32 %10, %5
  %12 = lshr i32 %0, 8
  %13 = and i32 %12, 15
  %14 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %13
  %15 = load i8, i8* %14, align 1, !tbaa !5
  %16 = zext i8 %15 to i32
  %17 = add nuw nsw i32 %11, %16
  %18 = lshr i32 %0, 12
  %19 = and i32 %18, 15
  %20 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %19
  %21 = load i8, i8* %20, align 1, !tbaa !5
  %22 = zext i8 %21 to i32
  %23 = add nuw nsw i32 %17, %22
  %24 = lshr i32 %0, 16
  %25 = and i32 %24, 15
  %26 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %25
  %27 = load i8, i8* %26, align 1, !tbaa !5
  %28 = zext i8 %27 to i32
  %29 = add nuw nsw i32 %23, %28
  %30 = lshr i32 %0, 20
  %31 = and i32 %30, 15
  %32 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %31
  %33 = load i8, i8* %32, align 1, !tbaa !5
  %34 = zext i8 %33 to i32
  %35 = add nuw nsw i32 %29, %34
  %36 = lshr i32 %0, 24
  %37 = and i32 %36, 15
  %38 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %37
  %39 = load i8, i8* %38, align 1, !tbaa !5
  %40 = zext i8 %39 to i32
  %41 = add nuw nsw i32 %35, %40
  %42 = lshr i32 %0, 28
  %43 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %42
  %44 = load i8, i8* %43, align 1, !tbaa !5
  %45 = zext i8 %44 to i32
  %46 = add nsw i32 %41, %45
  ret i32 %46
}

; Function Attrs: norecurse nounwind readnone
define i32 @BW_btbl_bitcount(i32) local_unnamed_addr #7 {
  %2 = lshr i32 %0, 8
  %3 = lshr i32 %0, 16
  %4 = lshr i32 %0, 24
  %5 = and i32 %0, 255
  %6 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %5
  %7 = load i8, i8* %6, align 1, !tbaa !5
  %8 = zext i8 %7 to i32
  %9 = and i32 %2, 255
  %10 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %9
  %11 = load i8, i8* %10, align 1, !tbaa !5
  %12 = zext i8 %11 to i32
  %13 = add nuw nsw i32 %12, %8
  %14 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %4
  %15 = load i8, i8* %14, align 1, !tbaa !5
  %16 = zext i8 %15 to i32
  %17 = add nuw nsw i32 %13, %16
  %18 = and i32 %3, 255
  %19 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %18
  %20 = load i8, i8* %19, align 1, !tbaa !5
  %21 = zext i8 %20 to i32
  %22 = add nuw nsw i32 %17, %21
  ret i32 %22
}

; Function Attrs: norecurse nounwind readnone
define i32 @AR_btbl_bitcount(i32) local_unnamed_addr #7 {
  %2 = lshr i32 %0, 8
  %3 = lshr i32 %0, 16
  %4 = lshr i32 %0, 24
  %5 = and i32 %0, 255
  %6 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %5
  %7 = load i8, i8* %6, align 1, !tbaa !5
  %8 = zext i8 %7 to i32
  %9 = and i32 %2, 255
  %10 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %9
  %11 = load i8, i8* %10, align 1, !tbaa !5
  %12 = zext i8 %11 to i32
  %13 = add nuw nsw i32 %12, %8
  %14 = and i32 %3, 255
  %15 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %14
  %16 = load i8, i8* %15, align 1, !tbaa !5
  %17 = zext i8 %16 to i32
  %18 = add nuw nsw i32 %13, %17
  %19 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %4
  %20 = load i8, i8* %19, align 1, !tbaa !5
  %21 = zext i8 %20 to i32
  %22 = add nuw nsw i32 %18, %21
  ret i32 %22
}

; Function Attrs: nounwind readnone
define i32 @btbl_bitcnt(i32) local_unnamed_addr #8 {
  %2 = and i32 %0, 255
  %3 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %2
  %4 = load i8, i8* %3, align 1, !tbaa !5
  %5 = zext i8 %4 to i32
  %6 = ashr i32 %0, 8
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %11, label %8

; <label>:8:                                      ; preds = %1
  %9 = tail call i32 @btbl_bitcnt(i32 %6)
  %10 = add nsw i32 %9, %5
  ret i32 %10

; <label>:11:                                     ; preds = %1
  ret i32 %5
}

; Function Attrs: nounwind
define i32 @initializeParameters(i32) local_unnamed_addr #2 {
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
  %14 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %15 = add i32 %0, -1
  %16 = mul i32 %15, 7
  %17 = getelementptr inbounds i32, i32* %14, i32 %16
  store i32 2011168768, i32* %17, align 4, !tbaa !89
  %18 = getelementptr inbounds i32, i32* %17, i32 1
  store i32 2011168768, i32* %18, align 4, !tbaa !89
  %19 = getelementptr inbounds i32, i32* %17, i32 2
  %20 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  %21 = bitcast i32* %19 to i8*
  tail call void @llvm.memset.p0i8.i64(i8* %21, i8 0, i64 20, i32 4, i1 false)
  call void @llvm.lifetime.start(i64 25, i8* nonnull %20) #5
  %22 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %22) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %22, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %23 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %20, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32 %0) #5
  %24 = call i8* @strcat(i8* nonnull %22, i8* nonnull %20) #5
  %25 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %25) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %25, i8 0, i32 40, i32 1, i1 false)
  %26 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %26) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %26, i8 0, i32 40, i32 1, i1 false)
  %27 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %27) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %27, i8 0, i32 40, i32 1, i1 false)
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %28) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %28, i8 0, i32 40, i32 1, i1 false)
  %29 = call i8* @strcat(i8* nonnull %25, i8* nonnull %22) #5
  %30 = call i32 @strlen(i8* nonnull %25)
  %31 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %30
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %31, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1.14, i32 0, i32 0), i32 16, i32 1, i1 false)
  %32 = call i8* @strcat(i8* nonnull %26, i8* nonnull %22) #5
  %33 = call i32 @strlen(i8* nonnull %26)
  %34 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %33
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %34, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2.15, i32 0, i32 0), i32 16, i32 1, i1 false)
  %35 = call i8* @strcat(i8* nonnull %27, i8* nonnull %22) #5
  %36 = call i32 @strlen(i8* nonnull %27)
  %37 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %36
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %37, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3.16, i32 0, i32 0), i32 16, i32 1, i1 false)
  %38 = call %struct._IO_FILE* @fopen(i8* nonnull %25, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0))
  %39 = call %struct._IO_FILE* @fopen(i8* nonnull %26, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0))
  %40 = call %struct._IO_FILE* @fopen(i8* nonnull %27, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0))
  %41 = bitcast i32* %8 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %41) #5
  %42 = bitcast i32* %9 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %42) #5
  %43 = bitcast i32* %10 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %43) #5
  %44 = bitcast i32* %11 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %44) #5
  %45 = call i32 @fread(i8* nonnull %41, i32 4, i32 1, %struct._IO_FILE* %38)
  %46 = call i32 @fread(i8* nonnull %42, i32 4, i32 1, %struct._IO_FILE* %39)
  %47 = call i32 @fread(i8* nonnull %43, i32 4, i32 1, %struct._IO_FILE* %40)
  %48 = load i32, i32* %8, align 4, !tbaa !89
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5.18, i32 0, i32 0), i32 %48)
  %50 = load i32, i32* %9, align 4, !tbaa !89
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6.19, i32 0, i32 0), i32 %50)
  %52 = load i32, i32* %10, align 4, !tbaa !89
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7.20, i32 0, i32 0), i32 %52)
  %54 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8.21, i32 0, i32 0), i32 4)
  %55 = shl i32 %48, 2
  %56 = call noalias i8* @malloc(i32 %55) #5
  store i8* %56, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !15
  %57 = shl i32 %50, 2
  %58 = call noalias i8* @malloc(i32 %57) #5
  store i8* %58, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !15
  %59 = shl i32 %52, 2
  %60 = call noalias i8* @malloc(i32 %59) #5
  store i8* %60, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !15
  %61 = call i32 @fread(i8* %56, i32 4, i32 %48, %struct._IO_FILE* %38)
  %62 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !15
  %63 = call i32 @fread(i8* %62, i32 4, i32 %50, %struct._IO_FILE* %39)
  %64 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !15
  %65 = call i32 @fread(i8* %64, i32 4, i32 %52, %struct._IO_FILE* %40)
  %66 = call i8* @strcat(i8* nonnull %28, i8* nonnull %22) #5
  %67 = call i32 @strlen(i8* nonnull %28)
  %68 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %67
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %68, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.9.22, i32 0, i32 0), i32 26, i32 1, i1 false)
  %69 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0))
  %70 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %69, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %11) #5
  %71 = sdiv i32 %52, 16
  %72 = sdiv i32 %48, 16
  %73 = sdiv i32 %50, 16
  %74 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %75 = getelementptr inbounds i32, i32* %74, i32 %16
  %76 = getelementptr inbounds i32, i32* %75, i32 2
  store i32 %71, i32* %76, align 4, !tbaa !89
  %77 = getelementptr inbounds i32, i32* %75, i32 3
  store i32 %72, i32* %77, align 4, !tbaa !89
  %78 = getelementptr inbounds i32, i32* %75, i32 4
  store i32 %73, i32* %78, align 4, !tbaa !89
  %79 = load i32, i32* %11, align 4, !tbaa !89
  %80 = sdiv i32 %79, 16
  %81 = getelementptr inbounds i32, i32* %75, i32 6
  store i32 %80, i32* %81, align 4, !tbaa !89
  %82 = call i32 @fclose(%struct._IO_FILE* %38)
  %83 = call i32 @fclose(%struct._IO_FILE* %39)
  %84 = call i32 @fclose(%struct._IO_FILE* %40)
  %85 = call i32 @fclose(%struct._IO_FILE* %69)
  %86 = getelementptr inbounds [40 x i8], [40 x i8]* %12, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %86) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %86, i8 0, i32 40, i32 1, i1 false)
  %87 = call i8* @strcat(i8* nonnull %86, i8* nonnull %22) #5
  %88 = call i32 @strlen(i8* nonnull %86)
  %89 = getelementptr [40 x i8], [40 x i8]* %12, i32 0, i32 %88
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %89, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.11.24, i32 0, i32 0), i32 18, i32 1, i1 false)
  %90 = bitcast i32* %13 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %90) #5
  store i32 0, i32* %13, align 4, !tbaa !89
  %91 = call %struct._IO_FILE* @fopen(i8* nonnull %86, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0))
  %92 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %91, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %13) #5
  %93 = call i32 @fclose(%struct._IO_FILE* %91)
  %94 = load i32, i32* %13, align 4, !tbaa !89
  %95 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %94)
  %96 = load i32, i32* %13, align 4, !tbaa !89
  %97 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %98 = getelementptr inbounds i32, i32* %97, i32 %16
  %99 = getelementptr inbounds i32, i32* %98, i32 5
  store i32 %96, i32* %99, align 4, !tbaa !89
  %100 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !15
  %101 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !15
  %102 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !15
  %103 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.13.25, i32 0, i32 0), i32 %100, i32 %101, i32 %102)
  %104 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !15
  %105 = load i32*, i32** @prologPtr, align 4, !tbaa !15
  %106 = getelementptr inbounds i32, i32* %105, i32 %15
  store i32 %104, i32* %106, align 4, !tbaa !91
  %107 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !15
  %108 = load i32*, i32** @epilogPtr, align 4, !tbaa !15
  %109 = getelementptr inbounds i32, i32* %108, i32 %15
  store i32 %107, i32* %109, align 4, !tbaa !91
  %110 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !15
  %111 = load i32*, i32** @kernelPtr, align 4, !tbaa !15
  %112 = getelementptr inbounds i32, i32* %111, i32 %15
  store i32 %110, i32* %112, align 4, !tbaa !91
  call void @llvm.lifetime.end(i64 4, i8* nonnull %90) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %86) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %44) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %43) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %42) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %41) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %28) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %27) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %26) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %25) #5
  call void @llvm.lifetime.end(i64 20, i8* nonnull %22) #5
  call void @llvm.lifetime.end(i64 25, i8* nonnull %20) #5
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #3

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #3

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture writeonly, i8* nocapture readonly, i32, i32, i1) #3

; Function Attrs: nounwind
declare i32 @sprintf(i8* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #4

; Function Attrs: nounwind
declare i8* @strcat(i8*, i8* nocapture readonly) local_unnamed_addr #4

; Function Attrs: nounwind readonly
declare i32 @strlen(i8* nocapture) local_unnamed_addr #1

; Function Attrs: nounwind
declare i32 @fread(i8* nocapture, i32, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #4

declare i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #9

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #3

; Function Attrs: nounwind
define i32 @configureCGRA(i32) local_unnamed_addr #2 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  %5 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %6 = add i32 %0, -1
  %7 = mul i32 %6, 7
  %8 = getelementptr inbounds i32, i32* %5, i32 %7
  %9 = getelementptr inbounds i32, i32* %8, i32 5
  %10 = load i32, i32* %9, align 4, !tbaa !89
  %11 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  call void @llvm.lifetime.start(i64 25, i8* nonnull %11) #5
  %12 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %12) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %12, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %13 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %11, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32 %0) #5
  %14 = call i8* @strcat(i8* nonnull %12, i8* nonnull %11) #5
  %15 = icmp slt i32 %10, 1
  br i1 %15, label %16, label %22

; <label>:16:                                     ; preds = %1
  %17 = load i32, i32* @dynamicTCVal, align 4, !tbaa !89
  %18 = add nsw i32 %17, %10
  %19 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %20 = getelementptr inbounds i32, i32* %19, i32 %7
  %21 = getelementptr inbounds i32, i32* %20, i32 5
  store i32 %18, i32* %21, align 4, !tbaa !89
  br label %22

; <label>:22:                                     ; preds = %16, %1
  %23 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %23) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %23, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %24 = call i32 @strlen(i8* nonnull %23)
  %25 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %24
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %25, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.14, i32 0, i32 0), i32 14, i32 1, i1 false)
  %26 = call %struct._IO_FILE* @fopen(i8* nonnull %23, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.15, i32 0, i32 0))
  %27 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %28 = getelementptr inbounds i32, i32* %27, i32 %7
  %29 = load i32, i32* %28, align 4, !tbaa !89
  %30 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %29)
  %31 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %32 = getelementptr inbounds i32, i32* %31, i32 %7
  %33 = getelementptr inbounds i32, i32* %32, i32 1
  %34 = load i32, i32* %33, align 4, !tbaa !89
  %35 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %34)
  %36 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %37 = getelementptr inbounds i32, i32* %36, i32 %7
  %38 = getelementptr inbounds i32, i32* %37, i32 2
  %39 = load i32, i32* %38, align 4, !tbaa !89
  %40 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %39)
  %41 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %42 = getelementptr inbounds i32, i32* %41, i32 %7
  %43 = getelementptr inbounds i32, i32* %42, i32 3
  %44 = load i32, i32* %43, align 4, !tbaa !89
  %45 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %44)
  %46 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %47 = getelementptr inbounds i32, i32* %46, i32 %7
  %48 = getelementptr inbounds i32, i32* %47, i32 4
  %49 = load i32, i32* %48, align 4, !tbaa !89
  %50 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %49)
  %51 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %52 = getelementptr inbounds i32, i32* %51, i32 %7
  %53 = getelementptr inbounds i32, i32* %52, i32 5
  %54 = load i32, i32* %53, align 4, !tbaa !89
  %55 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %54)
  %56 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %57 = getelementptr inbounds i32, i32* %56, i32 %7
  %58 = getelementptr inbounds i32, i32* %57, i32 6
  %59 = load i32, i32* %58, align 4, !tbaa !89
  %60 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %59)
  %61 = load i32*, i32** @epilogPtr, align 4, !tbaa !15
  %62 = getelementptr inbounds i32, i32* %61, i32 %6
  %63 = load i32, i32* %62, align 4, !tbaa !91
  %64 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %63)
  %65 = load i32*, i32** @prologPtr, align 4, !tbaa !15
  %66 = getelementptr inbounds i32, i32* %65, i32 %6
  %67 = load i32, i32* %66, align 4, !tbaa !91
  %68 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %67)
  %69 = load i32*, i32** @kernelPtr, align 4, !tbaa !15
  %70 = getelementptr inbounds i32, i32* %69, i32 %6
  %71 = load i32, i32* %70, align 4, !tbaa !91
  %72 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %71)
  %73 = call i32 @fclose(%struct._IO_FILE* %26)
  call void @llvm.lifetime.end(i64 40, i8* nonnull %23) #5
  call void @llvm.lifetime.end(i64 20, i8* nonnull %12) #5
  call void @llvm.lifetime.end(i64 25, i8* nonnull %11) #5
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #4

; Function Attrs: nounwind
define void @checkTotalLoops() local_unnamed_addr #2 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0))
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.18, i32 0, i32 0), i32* nonnull @totalLoops) #5
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3)
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #5
  ret void
}

; Function Attrs: norecurse nounwind
define void @updateLoopTC(i32) local_unnamed_addr #10 {
  %2 = load i32*, i32** @initCGRA, align 4, !tbaa !15
  %3 = getelementptr inbounds i32, i32* %2, i32 5
  %4 = load i32, i32* %3, align 4, !tbaa !89
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %8, label %6

; <label>:6:                                      ; preds = %1
  %7 = load i32, i32* @dynamicTCVal, align 4, !tbaa !89
  store i32 %7, i32* %3, align 4, !tbaa !89
  br label %8

; <label>:8:                                      ; preds = %6, %1
  ret void
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #2 {
  br label %2

; <label>:2:                                      ; preds = %11, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !89
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !89
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %.loopexit.loopexit, label %.preheader, !llvm.loop !93

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %8 = load i32, i32* @thread_exit, align 4, !tbaa !89
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #5, !srcloc !141
  ret i8* null

; <label>:11:                                     ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !89
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #5, !srcloc !142
  %12 = tail call i32 @usleep(i32 1) #5
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !89
  br label %2, !llvm.loop !143
}

declare i32 @usleep(i32) local_unnamed_addr #9

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #2 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.20, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !89
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !89
  br label %4

; <label>:4:                                      ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !89
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !145

; <label>:8:                                      ; preds = %4
  ret void
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #2 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0))
  store i32 1, i32* @thread_exit, align 4, !tbaa !89
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !89
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.24, i32 0, i32 0))
  %3 = load i32, i32* @pth, align 4, !tbaa !91
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #5
  ret void
}

declare i32 @pthread_join(i32, i8**) local_unnamed_addr #9

; Function Attrs: nounwind
define void @createCGRA() local_unnamed_addr #2 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false) #5
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0)) #5
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.18, i32 0, i32 0), i32* nonnull @totalLoops) #5
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #5
  %6 = load i32, i32* @totalLoops, align 4, !tbaa !89
  %7 = mul i32 %6, 28
  %8 = tail call noalias i8* @malloc(i32 %7) #5
  store i8* %8, i8** bitcast (i32** @initCGRA to i8**), align 4, !tbaa !15
  %9 = shl i32 %6, 2
  %10 = tail call noalias i8* @malloc(i32 %9) #5
  store i8* %10, i8** bitcast (i32** @prologPtr to i8**), align 4, !tbaa !15
  %11 = tail call noalias i8* @malloc(i32 %9) #5
  store i8* %11, i8** bitcast (i32** @kernelPtr to i8**), align 4, !tbaa !15
  %12 = tail call noalias i8* @malloc(i32 %9) #5
  store i8* %12, i8** bitcast (i32** @epilogPtr to i8**), align 4, !tbaa !15
  %13 = icmp eq i32 %6, 0
  br i1 %13, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %0
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %14 = phi i32 [ %16, %.preheader ], [ 1, %.preheader.preheader ]
  %15 = tail call i32 @initializeParameters(i32 %14)
  %16 = add i32 %14, 1
  %17 = load i32, i32* @totalLoops, align 4, !tbaa !89
  %18 = icmp ugt i32 %16, %17
  br i1 %18, label %.loopexit.loopexit, label %.preheader, !llvm.loop !154

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %0
  %19 = tail call i32 @puts(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str.25, i32 0, i32 0))
  %20 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #5
  ret void
}

; Function Attrs: nounwind
declare i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #4

attributes #0 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readonly }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind }
attributes #6 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { cold }
attributes #12 = { noreturn nounwind }

!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3, !4}

!0 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{i32 1, !"min_enum_size", i32 4}
!5 = !{!6, !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = !{!9, !10, i64 0}
!9 = !{!"", !10, i64 0, !6, i64 4, !6, i64 5, !6, i64 6, !6, i64 7}
!10 = !{!"any pointer", !6, i64 0}
!11 = !{!9, !6, i64 5}
!12 = !{!9, !6, i64 7}
!13 = !{!9, !6, i64 4}
!14 = !{!9, !6, i64 6}
!15 = !{!10, !10, i64 0}
!16 = distinct !{!16, !17}
!17 = !DILocation(line: 18, column: 34, scope: !18)
!18 = distinct !DILexicalBlock(scope: !20, file: !19, line: 16, column: 13)
!19 = !DIFile(filename: "bitcnt_1.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount4")
!20 = distinct !DISubprogram(name: "bit_count", scope: !19, file: !19, line: 9, type: !21, isLocal: false, isDefinition: true, scopeLine: 10, flags: DIFlagPrototyped, isOptimized: true, unit: !25, variables: !27)
!21 = !DISubroutineType(types: !22)
!22 = !{!23, !24}
!23 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!24 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!25 = distinct !DICompileUnit(language: DW_LANG_C99, file: !19, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !26)
!26 = !{}
!27 = !{!28, !29}
!28 = !DILocalVariable(name: "x", arg: 1, scope: !20, file: !19, line: 9, type: !24)
!29 = !DILocalVariable(name: "n", scope: !20, file: !19, line: 11, type: !23)
!30 = distinct !{!30, !31}
!31 = !DILocation(line: 60, column: 27, scope: !32)
!32 = distinct !DILexicalBlock(scope: !34, file: !33, line: 59, column: 5)
!33 = !DIFile(filename: "bitcnts.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount4")
!34 = distinct !DILexicalBlock(scope: !35, file: !33, line: 56, column: 31)
!35 = distinct !DILexicalBlock(scope: !36, file: !33, line: 56, column: 3)
!36 = distinct !DILexicalBlock(scope: !37, file: !33, line: 56, column: 3)
!37 = distinct !DISubprogram(name: "main", scope: !33, file: !33, line: 21, type: !38, isLocal: false, isDefinition: true, scopeLine: 22, flags: DIFlagPrototyped, isOptimized: true, unit: !43, variables: !57)
!38 = !DISubroutineType(types: !39)
!39 = !{!23, !23, !40}
!40 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !41, size: 32)
!41 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !42, size: 32)
!42 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!43 = distinct !DICompileUnit(language: DW_LANG_C99, file: !33, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !26, retainedTypes: !44, globals: !47)
!44 = !{!45, !23, !40, !46}
!45 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!46 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!47 = !{!48, !54}
!48 = !DIGlobalVariableExpression(var: !49)
!49 = distinct !DIGlobalVariable(name: "pBitCntFunc", scope: !37, file: !33, line: 28, type: !50, isLocal: true, isDefinition: true)
!50 = !DICompositeType(tag: DW_TAG_array_type, baseType: !51, size: 224, elements: !52)
!51 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !21, size: 32)
!52 = !{!53}
!53 = !DISubrange(count: 7)
!54 = !DIGlobalVariableExpression(var: !55)
!55 = distinct !DIGlobalVariable(name: "text", scope: !37, file: !33, line: 38, type: !56, isLocal: true, isDefinition: true)
!56 = !DICompositeType(tag: DW_TAG_array_type, baseType: !41, size: 224, elements: !52)
!57 = !{!58, !59, !60, !65, !66, !67, !68, !69, !70, !71, !72, !73, !74, !75}
!58 = !DILocalVariable(name: "argc", arg: 1, scope: !37, file: !33, line: 21, type: !23)
!59 = !DILocalVariable(name: "argv", arg: 2, scope: !37, file: !33, line: 21, type: !40)
!60 = !DILocalVariable(name: "start", scope: !37, file: !33, line: 23, type: !61)
!61 = !DIDerivedType(tag: DW_TAG_typedef, name: "clock_t", file: !62, line: 59, baseType: !63)
!62 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/time.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount4")
!63 = !DIDerivedType(tag: DW_TAG_typedef, name: "__clock_t", file: !64, line: 135, baseType: !24)
!64 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount4")
!65 = !DILocalVariable(name: "stop", scope: !37, file: !33, line: 23, type: !61)
!66 = !DILocalVariable(name: "ct", scope: !37, file: !33, line: 24, type: !45)
!67 = !DILocalVariable(name: "cmin", scope: !37, file: !33, line: 24, type: !45)
!68 = !DILocalVariable(name: "cmax", scope: !37, file: !33, line: 24, type: !45)
!69 = !DILocalVariable(name: "i", scope: !37, file: !33, line: 25, type: !23)
!70 = !DILocalVariable(name: "cminix", scope: !37, file: !33, line: 25, type: !23)
!71 = !DILocalVariable(name: "cmaxix", scope: !37, file: !33, line: 25, type: !23)
!72 = !DILocalVariable(name: "j", scope: !37, file: !33, line: 26, type: !24)
!73 = !DILocalVariable(name: "n", scope: !37, file: !33, line: 26, type: !24)
!74 = !DILocalVariable(name: "seed", scope: !37, file: !33, line: 26, type: !24)
!75 = !DILocalVariable(name: "iterations", scope: !37, file: !33, line: 27, type: !23)
!76 = distinct !{!76, !31, !77, !78}
!77 = !{!"llvm.loop.vectorize.width", i32 1}
!78 = !{!"llvm.loop.interleave.count", i32 1}
!79 = distinct !{!79, !31, !80, !77, !78}
!80 = !{!"llvm.loop.unroll.runtime.disable"}
!81 = distinct !{!81, !82}
!82 = !DILocation(line: 85, column: 22, scope: !83)
!83 = distinct !DILexicalBlock(scope: !84, file: !33, line: 84, column: 3)
!84 = distinct !DISubprogram(name: "bit_shifter", scope: !33, file: !33, line: 80, type: !21, isLocal: true, isDefinition: true, scopeLine: 81, flags: DIFlagPrototyped, isOptimized: true, unit: !43, variables: !85)
!85 = !{!86, !87, !88}
!86 = !DILocalVariable(name: "x", arg: 1, scope: !84, file: !33, line: 80, type: !24)
!87 = !DILocalVariable(name: "i", scope: !84, file: !33, line: 82, type: !23)
!88 = !DILocalVariable(name: "n", scope: !84, file: !33, line: 82, type: !23)
!89 = !{!90, !90, i64 0}
!90 = !{!"int", !6, i64 0}
!91 = !{!92, !92, i64 0}
!92 = !{!"long", !6, i64 0}
!93 = distinct !{!93, !94}
!94 = !DILocation(line: 223, column: 46, scope: !95)
!95 = distinct !DILexicalBlock(scope: !97, file: !96, line: 220, column: 14)
!96 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount4")
!97 = distinct !DISubprogram(name: "runOnCGRA", scope: !96, file: !96, line: 217, type: !98, isLocal: false, isDefinition: true, scopeLine: 218, flags: DIFlagPrototyped, isOptimized: true, unit: !100, variables: !139)
!98 = !DISubroutineType(types: !99)
!99 = !{!46, !46}
!100 = distinct !DICompileUnit(language: DW_LANG_C99, file: !96, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !26, retainedTypes: !101, globals: !105)
!101 = !{!102, !103, !24, !46, !104}
!102 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !103, size: 32)
!103 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!104 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !23, size: 32)
!105 = !{!106, !109, !111, !113, !115, !117, !119, !121, !123, !125, !128, !130, !132, !134}
!106 = !DIGlobalVariableExpression(var: !107)
!107 = distinct !DIGlobalVariable(name: "totalLoops", scope: !100, file: !108, line: 27, type: !103, isLocal: false, isDefinition: true)
!108 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount4")
!109 = !DIGlobalVariableExpression(var: !110)
!110 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !100, file: !108, line: 30, type: !103, isLocal: false, isDefinition: true)
!111 = !DIGlobalVariableExpression(var: !112)
!112 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !100, file: !108, line: 31, type: !103, isLocal: false, isDefinition: true)
!113 = !DIGlobalVariableExpression(var: !114)
!114 = distinct !DIGlobalVariable(name: "thread_exit", scope: !100, file: !108, line: 32, type: !103, isLocal: false, isDefinition: true)
!115 = !DIGlobalVariableExpression(var: !116)
!116 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !100, file: !108, line: 34, type: !23, isLocal: false, isDefinition: true)
!117 = !DIGlobalVariableExpression(var: !118)
!118 = distinct !DIGlobalVariable(name: "prolog", scope: !100, file: !108, line: 21, type: !102, isLocal: false, isDefinition: true)
!119 = !DIGlobalVariableExpression(var: !120)
!120 = distinct !DIGlobalVariable(name: "epilog", scope: !100, file: !108, line: 21, type: !102, isLocal: false, isDefinition: true)
!121 = !DIGlobalVariableExpression(var: !122)
!122 = distinct !DIGlobalVariable(name: "kernel", scope: !100, file: !108, line: 21, type: !102, isLocal: false, isDefinition: true)
!123 = !DIGlobalVariableExpression(var: !124)
!124 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !100, file: !108, line: 22, type: !102, isLocal: false, isDefinition: true)
!125 = !DIGlobalVariableExpression(var: !126)
!126 = distinct !DIGlobalVariable(name: "prologPtr", scope: !100, file: !108, line: 24, type: !127, isLocal: false, isDefinition: true)
!127 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !24, size: 32)
!128 = !DIGlobalVariableExpression(var: !129)
!129 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !100, file: !108, line: 24, type: !127, isLocal: false, isDefinition: true)
!130 = !DIGlobalVariableExpression(var: !131)
!131 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !100, file: !108, line: 24, type: !127, isLocal: false, isDefinition: true)
!132 = !DIGlobalVariableExpression(var: !133)
!133 = distinct !DIGlobalVariable(name: "initCGRA", scope: !100, file: !108, line: 26, type: !104, isLocal: false, isDefinition: true)
!134 = !DIGlobalVariableExpression(var: !135)
!135 = distinct !DIGlobalVariable(name: "pth", scope: !100, file: !108, line: 29, type: !136, isLocal: false, isDefinition: true)
!136 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !137, line: 37, baseType: !138)
!137 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount4")
!138 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!139 = !{!140}
!140 = !DILocalVariable(name: "arg", arg: 1, scope: !97, file: !96, line: 217, type: !46)
!141 = !{i32 6148}
!142 = !{i32 7183}
!143 = distinct !{!143, !144}
!144 = !DILocation(line: 257, column: 5, scope: !97)
!145 = distinct !{!145, !146}
!146 = !DILocation(line: 277, column: 41, scope: !147)
!147 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !96, file: !96, line: 261, type: !148, isLocal: false, isDefinition: true, scopeLine: 262, flags: DIFlagPrototyped, isOptimized: true, unit: !100, variables: !150)
!148 = !DISubroutineType(types: !149)
!149 = !{null, !103}
!150 = !{!151, !152, !153}
!151 = !DILocalVariable(name: "loopNo", arg: 1, scope: !147, file: !96, line: 261, type: !103)
!152 = !DILocalVariable(name: "result", scope: !147, file: !96, line: 263, type: !23)
!153 = !DILocalVariable(name: "configure", scope: !147, file: !96, line: 266, type: !23)
!154 = distinct !{!154, !155}
!155 = !DILocation(line: 314, column: 29, scope: !156)
!156 = distinct !DILexicalBlock(scope: !157, file: !96, line: 313, column: 3)
!157 = distinct !DISubprogram(name: "createCGRA", scope: !96, file: !96, line: 293, type: !158, isLocal: false, isDefinition: true, scopeLine: 294, isOptimized: true, unit: !100, variables: !160)
!158 = !DISubroutineType(types: !159)
!159 = !{null}
!160 = !{!161, !162}
!161 = !DILocalVariable(name: "result", scope: !157, file: !96, line: 303, type: !23)
!162 = !DILocalVariable(name: "i", scope: !157, file: !96, line: 304, type: !103)
