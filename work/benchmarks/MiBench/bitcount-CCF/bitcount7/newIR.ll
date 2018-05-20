; ModuleID = 'combinedIR.bc'
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
@gVar2 = common local_unnamed_addr global i32 0
@gVar3 = common local_unnamed_addr global i32 0
@gVar4 = common local_unnamed_addr global i32 0
@gVar5 = common local_unnamed_addr global i32 0
@gVar6 = common local_unnamed_addr global i32 0
@gVar7 = common local_unnamed_addr global i32 0
@gVar8 = common local_unnamed_addr global i32 0
@gVar9 = common local_unnamed_addr global i32 0
@gVar10 = common local_unnamed_addr global i32 0
@gVar11 = common local_unnamed_addr global i32 0

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
  br i1 %16, label %.preheader.preheader, label %.loopexit

.preheader.preheader:                             ; preds = %14
  br label %.preheader

.preheader:                                       ; preds = %29, %.preheader.preheader
  %17 = phi i32 [ %19, %29 ], [ %2, %.preheader.preheader ]
  %18 = phi i8* [ %30, %29 ], [ %15, %.preheader.preheader ]
  %19 = add nsw i32 %17, -1
  %20 = lshr i32 %1, %19
  %21 = and i32 %20, 1
  %22 = or i32 %21, 48
  %23 = trunc i32 %22 to i8
  %24 = getelementptr inbounds i8, i8* %18, i32 1
  store i8 %23, i8* %18, align 1, !tbaa !5
  %25 = and i32 %19, 3
  %26 = icmp eq i32 %25, 0
  %27 = icmp ne i32 %19, 0
  %28 = and i1 %27, %26
  br i1 %28, label %32, label %29

; <label>:29:                                     ; preds = %32, %.preheader
  %30 = phi i8* [ %33, %32 ], [ %24, %.preheader ]
  %31 = icmp sgt i32 %17, 1
  br i1 %31, label %.preheader, label %.loopexit.loopexit, !llvm.loop !8

; <label>:32:                                     ; preds = %.preheader
  %33 = getelementptr inbounds i8, i8* %18, i32 2
  store i8 32, i8* %24, align 1, !tbaa !5
  br label %29

.loopexit.loopexit:                               ; preds = %29
  %.lcssa = phi i8* [ %30, %29 ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %14
  %34 = phi i8* [ %15, %14 ], [ %.lcssa, %.loopexit.loopexit ]
  store i8 0, i8* %34, align 1, !tbaa !5
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
  store %struct._IO_FILE* %7, %struct._IO_FILE** %8, align 4, !tbaa !27
  %9 = icmp eq %struct._IO_FILE* %7, null
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %6
  tail call void @free(i8* nonnull %3) #5
  br label %14

; <label>:11:                                     ; preds = %6
  %12 = getelementptr inbounds i8, i8* %3, i32 5
  store i8 0, i8* %12, align 1, !tbaa !30
  %13 = getelementptr inbounds i8, i8* %3, i32 7
  store i8 0, i8* %13, align 1, !tbaa !31
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
  %3 = load i8, i8* %2, align 1, !tbaa !30
  %4 = icmp eq i8 %3, 0
  br i1 %4, label %9, label %5

; <label>:5:                                      ; preds = %1
  %6 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 1
  %7 = load i8, i8* %6, align 4, !tbaa !32
  %8 = add i8 %3, -1
  br label %15

; <label>:9:                                      ; preds = %1
  %10 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 0
  %11 = load %struct._IO_FILE*, %struct._IO_FILE** %10, align 4, !tbaa !27
  %12 = tail call i32 @fgetc(%struct._IO_FILE* %11)
  %13 = trunc i32 %12 to i8
  %14 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 1
  store i8 %13, i8* %14, align 4, !tbaa !32
  store i8 8, i8* %2, align 1, !tbaa !30
  br label %15

; <label>:15:                                     ; preds = %9, %5
  %16 = phi i8 [ %13, %9 ], [ %7, %5 ]
  %17 = phi i8 [ 7, %9 ], [ %8, %5 ]
  store i8 %17, i8* %2, align 1, !tbaa !30
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
  %4 = load i8, i8* %3, align 1, !tbaa !31
  %5 = icmp eq i8 %4, 8
  %6 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 3
  br i1 %5, label %9, label %7

; <label>:7:                                      ; preds = %2
  %8 = add i8 %4, 1
  br label %15

; <label>:9:                                      ; preds = %2
  %10 = load i8, i8* %6, align 2, !tbaa !33
  %11 = zext i8 %10 to i32
  %12 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 0
  %13 = load %struct._IO_FILE*, %struct._IO_FILE** %12, align 4, !tbaa !27
  %14 = tail call i32 @fputc(i32 %11, %struct._IO_FILE* %13)
  store i8 0, i8* %3, align 1, !tbaa !31
  br label %15

; <label>:15:                                     ; preds = %9, %7
  %16 = phi i8 [ %8, %7 ], [ 1, %9 ]
  store i8 %16, i8* %3, align 1, !tbaa !31
  %17 = load i8, i8* %6, align 2, !tbaa !33
  %18 = zext i8 %17 to i32
  %19 = shl nuw nsw i32 %18, 1
  %20 = and i32 %0, 1
  %21 = or i32 %19, %20
  %22 = trunc i32 %21 to i8
  store i8 %22, i8* %6, align 2, !tbaa !33
  ret void
}

; Function Attrs: nounwind
declare i32 @fputc(i32, %struct._IO_FILE* nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define void @bfclose(%struct.bfile* nocapture) local_unnamed_addr #2 {
  %2 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 0
  %3 = load %struct._IO_FILE*, %struct._IO_FILE** %2, align 4, !tbaa !27
  %4 = tail call i32 @fclose(%struct._IO_FILE* %3)
  %5 = bitcast %struct.bfile* %0 to i8*
  tail call void @free(i8* %5) #5
  ret void
}

; Function Attrs: nounwind
declare i32 @fclose(%struct._IO_FILE* nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
define i32 @main(i32, i8** nocapture readonly) local_unnamed_addr #2 {
  call void @createCGRA()
  %3 = icmp slt i32 %0, 2
  br i1 %3, label %4, label %7

; <label>:4:                                      ; preds = %2
  %5 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !tbaa !34
  %6 = tail call i32 @fwrite(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.7, i32 0, i32 0), i32 28, i32 1, %struct._IO_FILE* %5) #11
  tail call void @exit(i32 -1) #12
  unreachable

; <label>:7:                                      ; preds = %2
  %8 = getelementptr inbounds i8*, i8** %1, i32 1
  %9 = load i8*, i8** %8, align 4, !tbaa !34
  %10 = tail call i32 @strtol(i8* nocapture nonnull %9, i8** null, i32 10) #5
  %11 = tail call i32 @puts(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.8, i32 0, i32 0))
  %12 = icmp sgt i32 %10, 0
  %13 = tail call i32 @clock() #5
  %14 = tail call i32 @rand() #5
  store i32 %10, i32* @gVar2
  store i32 %10, i32* @gVar5
  store i32 %10, i32* @gVar10
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
  br label %151

.preheader1:                                      ; preds = %bit_count.exit, %.preheader1.preheader
  %101 = phi i32 [ %114, %bit_count.exit ], [ %14, %.preheader1.preheader ]
  %102 = phi i32 [ %112, %bit_count.exit ], [ 0, %.preheader1.preheader ]
  %103 = phi i32 [ %113, %bit_count.exit ], [ 0, %.preheader1.preheader ]
  %104 = icmp eq i32 %101, 0
  br i1 %104, label %bit_count.exit, label %.preheader.i.preheader, !llvm.loop !35

.preheader.i.preheader:                           ; preds = %.preheader1
  br label %.preheader.i

.preheader.i:                                     ; preds = %.preheader.i, %.preheader.i.preheader
  %105 = phi i32 [ %109, %.preheader.i ], [ %101, %.preheader.i.preheader ]
  %106 = phi i32 [ %107, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  %107 = add nuw nsw i32 %106, 1
  %108 = add nsw i32 %105, -1
  %109 = and i32 %108, %105
  %110 = icmp eq i32 %109, 0
  br i1 %110, label %bit_count.exit.loopexit, label %.preheader.i, !llvm.loop !35

bit_count.exit.loopexit:                          ; preds = %.preheader.i
  %.lcssa5 = phi i32 [ %107, %.preheader.i ]
  br label %bit_count.exit

bit_count.exit:                                   ; preds = %bit_count.exit.loopexit, %.preheader1
  %111 = phi i32 [ 0, %.preheader1 ], [ %.lcssa5, %bit_count.exit.loopexit ]
  %112 = add nsw i32 %111, %102
  %113 = add nuw nsw i32 %103, 1
  %114 = add nsw i32 %101, 13
  %115 = icmp eq i32 %113, %10
  br i1 %115, label %116, label %.preheader1, !llvm.loop !46

; <label>:116:                                    ; preds = %bit_count.exit
  %.lcssa6 = phi i32 [ %112, %bit_count.exit ]
  %117 = tail call i32 @clock() #5
  %118 = sub nsw i32 %117, %13
  %119 = sitofp i32 %118 to double
  %120 = fdiv double %119, 1.000000e+06
  %121 = fcmp olt double %120, 0x7FEFFFFFFFFFFFFF
  %122 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), double %120, i32 %.lcssa6)
  %123 = tail call i32 @clock() #5
  %124 = tail call i32 @rand() #5
  %min.iters.check = icmp ult i32 %10, 4
  br i1 %min.iters.check, label %scalar.ph.preheader, label %min.iters.checked

scalar.ph.preheader:                              ; preds = %middle.block, %min.iters.checked, %116
  %.ph = phi i32 [ %124, %min.iters.checked ], [ %124, %116 ], [ %ind.end, %middle.block ]
  %.ph39 = phi i32 [ 0, %min.iters.checked ], [ 0, %116 ], [ %150, %middle.block ]
  %.ph40 = phi i32 [ 0, %min.iters.checked ], [ 0, %116 ], [ %n.vec, %middle.block ]
  store i32 %.ph, i32* @gVar7
  store i32 %.ph39, i32* @gVar8
  store i32 %.ph40, i32* @gVar9
  br label %125

; <label>:125:                                    ; preds = %scalar.ph.preheader
  call void @accelerateOnCGRA(i32 3)
  br label %.loopexit38.loopexit

min.iters.checked:                                ; preds = %116
  %n.mod.vf = and i32 %10, 3
  %n.vec = sub i32 %10, %n.mod.vf
  %cmp.zero = icmp eq i32 %n.vec, 0
  %126 = mul i32 %10, 13
  %127 = add i32 %124, %126
  %128 = mul nsw i32 %n.mod.vf, -13
  %ind.end = add i32 %127, %128
  br i1 %cmp.zero, label %scalar.ph.preheader, label %vector.ph

vector.ph:                                        ; preds = %min.iters.checked
  %.splatinsert = insertelement <4 x i32> undef, i32 %124, i32 0
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> undef, <4 x i32> zeroinitializer
  %induction = add <4 x i32> %.splat, <i32 0, i32 13, i32 26, i32 39>
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i32 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %vec.ind = phi <4 x i32> [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi <4 x i32> [ zeroinitializer, %vector.ph ], [ %148, %vector.body ]
  %129 = lshr <4 x i32> %vec.ind, <i32 1, i32 1, i32 1, i32 1>
  %130 = and <4 x i32> %129, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>
  %131 = and <4 x i32> %vec.ind, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>
  %132 = add nuw <4 x i32> %130, %131
  %133 = lshr <4 x i32> %132, <i32 2, i32 2, i32 2, i32 2>
  %134 = and <4 x i32> %133, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>
  %135 = and <4 x i32> %132, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>
  %136 = add nuw nsw <4 x i32> %134, %135
  %137 = lshr <4 x i32> %136, <i32 4, i32 4, i32 4, i32 4>
  %138 = and <4 x i32> %137, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>
  %139 = and <4 x i32> %136, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>
  %140 = add nuw nsw <4 x i32> %138, %139
  %141 = lshr <4 x i32> %140, <i32 8, i32 8, i32 8, i32 8>
  %142 = and <4 x i32> %141, <i32 983055, i32 983055, i32 983055, i32 983055>
  %143 = and <4 x i32> %140, <i32 983055, i32 983055, i32 983055, i32 983055>
  %144 = add nuw nsw <4 x i32> %142, %143
  %145 = lshr <4 x i32> %144, <i32 16, i32 16, i32 16, i32 16>
  %146 = and <4 x i32> %144, <i32 31, i32 31, i32 31, i32 31>
  %147 = add <4 x i32> %146, %vec.phi
  %148 = add <4 x i32> %147, %145
  %index.next = add i32 %index, 4
  %vec.ind.next = add <4 x i32> %vec.ind, <i32 52, i32 52, i32 52, i32 52>
  %149 = icmp eq i32 %index.next, %n.vec
  br i1 %149, label %middle.block, label %vector.body, !llvm.loop !91

middle.block:                                     ; preds = %vector.body
  %.lcssa4 = phi <4 x i32> [ %148, %vector.body ]
  %rdx.shuf = shufflevector <4 x i32> %.lcssa4, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  %bin.rdx = add <4 x i32> %.lcssa4, %rdx.shuf
  %rdx.shuf36 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  %bin.rdx37 = add <4 x i32> %bin.rdx, %rdx.shuf36
  %150 = extractelement <4 x i32> %bin.rdx37, i32 0
  %cmp.n = icmp eq i32 %n.mod.vf, 0
  br i1 %cmp.n, label %.loopexit38, label %scalar.ph.preheader

; <label>:151:                                    ; preds = %315, %15
  %152 = phi i32 [ %326, %315 ], [ %97, %15 ]
  %153 = phi i32 [ %328, %315 ], [ %99, %15 ]
  %154 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %152
  %155 = load i8*, i8** %154, align 4, !tbaa !34
  %156 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.10, i32 0, i32 0), i8* %155)
  %157 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %153
  %158 = load i8*, i8** %157, align 4, !tbaa !34
  %159 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.11, i32 0, i32 0), i8* %158)
  ret i32 0

.loopexit38.loopexit:                             ; preds = %125
  br label %160

; <label>:160:                                    ; preds = %.loopexit38.loopexit
  %161 = load i32, i32* @gVar11
  br label %.loopexit38

.loopexit38:                                      ; preds = %160, %middle.block
  %.lcssa31 = phi i32 [ %150, %middle.block ], [ %161, %160 ]
  %162 = fcmp ogt double %120, 0.000000e+00
  %163 = select i1 %121, double %120, double 0x7FEFFFFFFFFFFFFF
  %164 = select i1 %162, double %120, double 0.000000e+00
  %165 = tail call i32 @clock() #5
  %166 = sub nsw i32 %165, %123
  %167 = sitofp i32 %166 to double
  %168 = fdiv double %167, 1.000000e+06
  %169 = fcmp olt double %168, %163
  %170 = fcmp ogt double %168, %164
  %171 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), double %168, i32 %.lcssa31)
  %172 = tail call i32 @clock() #5
  %173 = tail call i32 @rand() #5
  br label %174

; <label>:174:                                    ; preds = %174, %.loopexit38
  %175 = phi i32 [ %173, %.loopexit38 ], [ %181, %174 ]
  %176 = phi i32 [ 0, %.loopexit38 ], [ %179, %174 ]
  %177 = phi i32 [ 0, %.loopexit38 ], [ %180, %174 ]
  %178 = tail call i32 @ntbl_bitcnt(i32 %175) #5
  %179 = add nsw i32 %178, %176
  %180 = add nuw nsw i32 %177, 1
  %181 = add nsw i32 %175, 13
  %182 = icmp eq i32 %180, %10
  br i1 %182, label %183, label %174, !llvm.loop !94

; <label>:183:                                    ; preds = %174
  %.lcssa3 = phi i32 [ %179, %174 ]
  %184 = zext i1 %169 to i32
  %185 = select i1 %170, double %168, double %164
  %186 = select i1 %169, double %168, double %163
  %187 = tail call i32 @clock() #5
  %188 = sub nsw i32 %187, %172
  %189 = sitofp i32 %188 to double
  %190 = fdiv double %189, 1.000000e+06
  %191 = fcmp olt double %190, %186
  %192 = fcmp ogt double %190, %185
  %193 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), double %190, i32 %.lcssa3)
  %194 = tail call i32 @clock() #5
  %195 = tail call i32 @rand() #5
  br label %196

; <label>:196:                                    ; preds = %196, %183
  %197 = phi i32 [ %195, %183 ], [ %247, %196 ]
  %198 = phi i32 [ 0, %183 ], [ %245, %196 ]
  %199 = phi i32 [ 0, %183 ], [ %246, %196 ]
  %200 = and i32 %197, 15
  %201 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %200
  %202 = load i8, i8* %201, align 1, !tbaa !5
  %203 = zext i8 %202 to i32
  %204 = lshr i32 %197, 4
  %205 = and i32 %204, 15
  %206 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %205
  %207 = load i8, i8* %206, align 1, !tbaa !5
  %208 = zext i8 %207 to i32
  %209 = lshr i32 %197, 8
  %210 = and i32 %209, 15
  %211 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %210
  %212 = load i8, i8* %211, align 1, !tbaa !5
  %213 = zext i8 %212 to i32
  %214 = lshr i32 %197, 12
  %215 = and i32 %214, 15
  %216 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %215
  %217 = load i8, i8* %216, align 1, !tbaa !5
  %218 = zext i8 %217 to i32
  %219 = lshr i32 %197, 16
  %220 = and i32 %219, 15
  %221 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %220
  %222 = load i8, i8* %221, align 1, !tbaa !5
  %223 = zext i8 %222 to i32
  %224 = lshr i32 %197, 20
  %225 = and i32 %224, 15
  %226 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %225
  %227 = load i8, i8* %226, align 1, !tbaa !5
  %228 = zext i8 %227 to i32
  %229 = lshr i32 %197, 24
  %230 = and i32 %229, 15
  %231 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %230
  %232 = load i8, i8* %231, align 1, !tbaa !5
  %233 = zext i8 %232 to i32
  %234 = lshr i32 %197, 28
  %235 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %234
  %236 = load i8, i8* %235, align 1, !tbaa !5
  %237 = zext i8 %236 to i32
  %238 = add i32 %198, %203
  %239 = add i32 %238, %208
  %240 = add i32 %239, %213
  %241 = add i32 %240, %218
  %242 = add i32 %241, %223
  %243 = add i32 %242, %228
  %244 = add i32 %243, %233
  %245 = add i32 %244, %237
  %246 = add nuw nsw i32 %199, 1
  %247 = add nsw i32 %197, 13
  %248 = icmp eq i32 %246, %10
  br i1 %248, label %249, label %196, !llvm.loop !95

; <label>:249:                                    ; preds = %196
  %.lcssa2 = phi i32 [ %245, %196 ]
  %250 = zext i1 %170 to i32
  %251 = select i1 %191, i32 2, i32 %184
  %252 = select i1 %192, double %190, double %185
  %253 = select i1 %191, double %190, double %186
  %254 = tail call i32 @clock() #5
  %255 = sub nsw i32 %254, %194
  %256 = sitofp i32 %255 to double
  %257 = fdiv double %256, 1.000000e+06
  %258 = fcmp olt double %257, %253
  %259 = fcmp ogt double %257, %252
  %260 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), double %257, i32 %.lcssa2)
  %261 = tail call i32 @clock() #5
  %262 = tail call i32 @rand() #5
  store i32 %262, i32* @gVar4
  br label %263

; <label>:263:                                    ; preds = %249
  call void @accelerateOnCGRA(i32 2)
  br label %264

; <label>:264:                                    ; preds = %263
  %265 = select i1 %192, i32 2, i32 %250
  %266 = select i1 %258, i32 3, i32 %251
  %267 = select i1 %259, double %257, double %252
  %268 = select i1 %258, double %257, double %253
  %269 = tail call i32 @clock() #5
  %270 = sub nsw i32 %269, %261
  %271 = sitofp i32 %270 to double
  %272 = fdiv double %271, 1.000000e+06
  %273 = fcmp olt double %272, %268
  %274 = fcmp ogt double %272, %267
  %gVar6 = load i32, i32* @gVar6
  %275 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), double %272, i32 %gVar6)
  %276 = tail call i32 @clock() #5
  %277 = tail call i32 @rand() #5
  store i32 %277, i32* @gVar1
  %278 = sub i32 %10, 0
  %279 = sdiv i32 %278, 1
  store i32 %279, i32* @dynamicTCVal
  br label %280

; <label>:280:                                    ; preds = %264
  call void @accelerateOnCGRA(i32 1)
  br label %281

; <label>:281:                                    ; preds = %280
  %282 = select i1 %259, i32 3, i32 %265
  %283 = select i1 %273, i32 4, i32 %266
  %284 = select i1 %274, double %272, double %267
  %285 = select i1 %273, double %272, double %268
  %286 = tail call i32 @clock() #5
  %287 = sub nsw i32 %286, %276
  %288 = sitofp i32 %287 to double
  %289 = fdiv double %288, 1.000000e+06
  %290 = fcmp olt double %289, %285
  %291 = fcmp ogt double %289, %284
  %gVar3 = load i32, i32* @gVar3
  %292 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), double %289, i32 %gVar3)
  %293 = tail call i32 @clock() #5
  %294 = tail call i32 @rand() #5
  br label %295

; <label>:295:                                    ; preds = %.loopexit, %281
  %296 = phi i32 [ %294, %281 ], [ %313, %.loopexit ]
  %297 = phi i32 [ 0, %281 ], [ %311, %.loopexit ]
  %298 = phi i32 [ 0, %281 ], [ %312, %.loopexit ]
  %299 = icmp eq i32 %296, 0
  br i1 %299, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %295
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %300 = phi i32 [ %304, %.preheader ], [ 0, %.preheader.preheader ]
  %301 = phi i32 [ %305, %.preheader ], [ 0, %.preheader.preheader ]
  %302 = phi i32 [ %306, %.preheader ], [ %296, %.preheader.preheader ]
  %303 = and i32 %302, 1
  %304 = add nsw i32 %303, %300
  %305 = add nuw nsw i32 %301, 1
  %306 = ashr i32 %302, 1
  %307 = icmp ne i32 %306, 0
  %308 = icmp ult i32 %305, 32
  %309 = and i1 %308, %307
  br i1 %309, label %.preheader, label %.loopexit.loopexit, !llvm.loop !96

.loopexit.loopexit:                               ; preds = %.preheader
  %.lcssa = phi i32 [ %304, %.preheader ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %295
  %310 = phi i32 [ 0, %295 ], [ %.lcssa, %.loopexit.loopexit ]
  %311 = add nsw i32 %310, %297
  %312 = add nuw nsw i32 %298, 1
  %313 = add nsw i32 %296, 13
  %314 = icmp eq i32 %312, %10
  br i1 %314, label %315, label %295, !llvm.loop !104

; <label>:315:                                    ; preds = %.loopexit
  %.lcssa1 = phi i32 [ %311, %.loopexit ]
  %316 = select i1 %274, i32 4, i32 %282
  %317 = select i1 %290, i32 5, i32 %283
  %318 = select i1 %291, double %289, double %284
  %319 = select i1 %290, double %289, double %285
  %320 = select i1 %291, i32 5, i32 %316
  %321 = tail call i32 @clock() #5
  %322 = sub nsw i32 %321, %293
  %323 = sitofp i32 %322 to double
  %324 = fdiv double %323, 1.000000e+06
  %325 = fcmp olt double %324, %319
  %326 = select i1 %325, i32 6, i32 %317
  %327 = fcmp ogt double %324, %318
  %328 = select i1 %327, i32 6, i32 %320
  %329 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), double %324, i32 %.lcssa1)
  br label %151
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
  br i1 %2, label %.loopexit, label %.preheader.preheader, !llvm.loop !35

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %3 = phi i32 [ %7, %.preheader ], [ %0, %.preheader.preheader ]
  %4 = phi i32 [ %5, %.preheader ], [ 0, %.preheader.preheader ]
  %5 = add nuw nsw i32 %4, 1
  %6 = add nsw i32 %3, -1
  %7 = and i32 %6, %3
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !llvm.loop !35

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
  %14 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %15 = add i32 %0, -1
  %16 = mul i32 %15, 7
  %17 = getelementptr inbounds i32, i32* %14, i32 %16
  store i32 2011168768, i32* %17, align 4, !tbaa !105
  %18 = getelementptr inbounds i32, i32* %17, i32 1
  store i32 2011168768, i32* %18, align 4, !tbaa !105
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
  %48 = load i32, i32* %8, align 4, !tbaa !105
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5.18, i32 0, i32 0), i32 %48)
  %50 = load i32, i32* %9, align 4, !tbaa !105
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6.19, i32 0, i32 0), i32 %50)
  %52 = load i32, i32* %10, align 4, !tbaa !105
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7.20, i32 0, i32 0), i32 %52)
  %54 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8.21, i32 0, i32 0), i32 4)
  %55 = shl i32 %48, 2
  %56 = call noalias i8* @malloc(i32 %55) #5
  store i8* %56, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !34
  %57 = shl i32 %50, 2
  %58 = call noalias i8* @malloc(i32 %57) #5
  store i8* %58, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !34
  %59 = shl i32 %52, 2
  %60 = call noalias i8* @malloc(i32 %59) #5
  store i8* %60, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !34
  %61 = call i32 @fread(i8* %56, i32 4, i32 %48, %struct._IO_FILE* %38)
  %62 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !34
  %63 = call i32 @fread(i8* %62, i32 4, i32 %50, %struct._IO_FILE* %39)
  %64 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !34
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
  %74 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %75 = getelementptr inbounds i32, i32* %74, i32 %16
  %76 = getelementptr inbounds i32, i32* %75, i32 2
  store i32 %71, i32* %76, align 4, !tbaa !105
  %77 = getelementptr inbounds i32, i32* %75, i32 3
  store i32 %72, i32* %77, align 4, !tbaa !105
  %78 = getelementptr inbounds i32, i32* %75, i32 4
  store i32 %73, i32* %78, align 4, !tbaa !105
  %79 = load i32, i32* %11, align 4, !tbaa !105
  %80 = sdiv i32 %79, 16
  %81 = getelementptr inbounds i32, i32* %75, i32 6
  store i32 %80, i32* %81, align 4, !tbaa !105
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
  store i32 0, i32* %13, align 4, !tbaa !105
  %91 = call %struct._IO_FILE* @fopen(i8* nonnull %86, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0))
  %92 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %91, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %13) #5
  %93 = call i32 @fclose(%struct._IO_FILE* %91)
  %94 = load i32, i32* %13, align 4, !tbaa !105
  %95 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %94)
  %96 = load i32, i32* %13, align 4, !tbaa !105
  %97 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %98 = getelementptr inbounds i32, i32* %97, i32 %16
  %99 = getelementptr inbounds i32, i32* %98, i32 5
  store i32 %96, i32* %99, align 4, !tbaa !105
  %100 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !34
  %101 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !34
  %102 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !34
  %103 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.13.25, i32 0, i32 0), i32 %100, i32 %101, i32 %102)
  %104 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !34
  %105 = load i32*, i32** @prologPtr, align 4, !tbaa !34
  %106 = getelementptr inbounds i32, i32* %105, i32 %15
  store i32 %104, i32* %106, align 4, !tbaa !107
  %107 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !34
  %108 = load i32*, i32** @epilogPtr, align 4, !tbaa !34
  %109 = getelementptr inbounds i32, i32* %108, i32 %15
  store i32 %107, i32* %109, align 4, !tbaa !107
  %110 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !34
  %111 = load i32*, i32** @kernelPtr, align 4, !tbaa !34
  %112 = getelementptr inbounds i32, i32* %111, i32 %15
  store i32 %110, i32* %112, align 4, !tbaa !107
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
  %5 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %6 = add i32 %0, -1
  %7 = mul i32 %6, 7
  %8 = getelementptr inbounds i32, i32* %5, i32 %7
  %9 = getelementptr inbounds i32, i32* %8, i32 5
  %10 = load i32, i32* %9, align 4, !tbaa !105
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
  %17 = load i32, i32* @dynamicTCVal, align 4, !tbaa !105
  %18 = add nsw i32 %17, %10
  %19 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %20 = getelementptr inbounds i32, i32* %19, i32 %7
  %21 = getelementptr inbounds i32, i32* %20, i32 5
  store i32 %18, i32* %21, align 4, !tbaa !105
  br label %22

; <label>:22:                                     ; preds = %16, %1
  %23 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %23) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %23, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %24 = call i32 @strlen(i8* nonnull %23)
  %25 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %24
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %25, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.14, i32 0, i32 0), i32 14, i32 1, i1 false)
  %26 = call %struct._IO_FILE* @fopen(i8* nonnull %23, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.15, i32 0, i32 0))
  %27 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %28 = getelementptr inbounds i32, i32* %27, i32 %7
  %29 = load i32, i32* %28, align 4, !tbaa !105
  %30 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %29)
  %31 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %32 = getelementptr inbounds i32, i32* %31, i32 %7
  %33 = getelementptr inbounds i32, i32* %32, i32 1
  %34 = load i32, i32* %33, align 4, !tbaa !105
  %35 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %34)
  %36 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %37 = getelementptr inbounds i32, i32* %36, i32 %7
  %38 = getelementptr inbounds i32, i32* %37, i32 2
  %39 = load i32, i32* %38, align 4, !tbaa !105
  %40 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %39)
  %41 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %42 = getelementptr inbounds i32, i32* %41, i32 %7
  %43 = getelementptr inbounds i32, i32* %42, i32 3
  %44 = load i32, i32* %43, align 4, !tbaa !105
  %45 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %44)
  %46 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %47 = getelementptr inbounds i32, i32* %46, i32 %7
  %48 = getelementptr inbounds i32, i32* %47, i32 4
  %49 = load i32, i32* %48, align 4, !tbaa !105
  %50 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %49)
  %51 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %52 = getelementptr inbounds i32, i32* %51, i32 %7
  %53 = getelementptr inbounds i32, i32* %52, i32 5
  %54 = load i32, i32* %53, align 4, !tbaa !105
  %55 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %54)
  %56 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %57 = getelementptr inbounds i32, i32* %56, i32 %7
  %58 = getelementptr inbounds i32, i32* %57, i32 6
  %59 = load i32, i32* %58, align 4, !tbaa !105
  %60 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %59)
  %61 = load i32*, i32** @epilogPtr, align 4, !tbaa !34
  %62 = getelementptr inbounds i32, i32* %61, i32 %6
  %63 = load i32, i32* %62, align 4, !tbaa !107
  %64 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %63)
  %65 = load i32*, i32** @prologPtr, align 4, !tbaa !34
  %66 = getelementptr inbounds i32, i32* %65, i32 %6
  %67 = load i32, i32* %66, align 4, !tbaa !107
  %68 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %67)
  %69 = load i32*, i32** @kernelPtr, align 4, !tbaa !34
  %70 = getelementptr inbounds i32, i32* %69, i32 %6
  %71 = load i32, i32* %70, align 4, !tbaa !107
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
  %2 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %3 = getelementptr inbounds i32, i32* %2, i32 5
  %4 = load i32, i32* %3, align 4, !tbaa !105
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %8, label %6

; <label>:6:                                      ; preds = %1
  %7 = load i32, i32* @dynamicTCVal, align 4, !tbaa !105
  store i32 %7, i32* %3, align 4, !tbaa !105
  br label %8

; <label>:8:                                      ; preds = %6, %1
  ret void
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #2 {
  br label %2

; <label>:2:                                      ; preds = %11, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !105
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !105
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %.loopexit.loopexit, label %.preheader, !llvm.loop !109

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %8 = load i32, i32* @thread_exit, align 4, !tbaa !105
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #5, !srcloc !157
  ret i8* null

; <label>:11:                                     ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !105
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #5, !srcloc !158
  %12 = tail call i32 @usleep(i32 1) #5
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !105
  br label %2, !llvm.loop !159
}

declare i32 @usleep(i32) local_unnamed_addr #9

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #2 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.20, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !105
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !105
  br label %4

; <label>:4:                                      ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !105
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !161

; <label>:8:                                      ; preds = %4
  ret void
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #2 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0))
  store i32 1, i32* @thread_exit, align 4, !tbaa !105
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !105
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.24, i32 0, i32 0))
  %3 = load i32, i32* @pth, align 4, !tbaa !107
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
  %6 = load i32, i32* @totalLoops, align 4, !tbaa !105
  %7 = mul i32 %6, 28
  %8 = tail call noalias i8* @malloc(i32 %7) #5
  store i8* %8, i8** bitcast (i32** @initCGRA to i8**), align 4, !tbaa !34
  %9 = shl i32 %6, 2
  %10 = tail call noalias i8* @malloc(i32 %9) #5
  store i8* %10, i8** bitcast (i32** @prologPtr to i8**), align 4, !tbaa !34
  %11 = tail call noalias i8* @malloc(i32 %9) #5
  store i8* %11, i8** bitcast (i32** @kernelPtr to i8**), align 4, !tbaa !34
  %12 = tail call noalias i8* @malloc(i32 %9) #5
  store i8* %12, i8** bitcast (i32** @epilogPtr to i8**), align 4, !tbaa !34
  %13 = icmp eq i32 %6, 0
  br i1 %13, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %0
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %14 = phi i32 [ %16, %.preheader ], [ 1, %.preheader.preheader ]
  %15 = tail call i32 @initializeParameters(i32 %14)
  %16 = add i32 %14, 1
  %17 = load i32, i32* @totalLoops, align 4, !tbaa !105
  %18 = icmp ugt i32 %16, %17
  br i1 %18, label %.loopexit.loopexit, label %.preheader, !llvm.loop !170

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
!8 = distinct !{!8, !9}
!9 = !DILocation(line: 43, column: 7, scope: !10)
!10 = distinct !DISubprogram(name: "bitstring", scope: !11, file: !11, line: 31, type: !12, isLocal: false, isDefinition: true, scopeLine: 32, flags: DIFlagPrototyped, isOptimized: true, unit: !18, variables: !20)
!11 = !DIFile(filename: "bitstrng.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!12 = !DISubroutineType(types: !13)
!13 = !{null, !14, !16, !17, !17}
!14 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !15, size: 32)
!15 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!16 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!17 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!18 = distinct !DICompileUnit(language: DW_LANG_C99, file: !11, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19)
!19 = !{}
!20 = !{!21, !22, !23, !24, !25, !26}
!21 = !DILocalVariable(name: "str", arg: 1, scope: !10, file: !11, line: 31, type: !14)
!22 = !DILocalVariable(name: "byze", arg: 2, scope: !10, file: !11, line: 31, type: !16)
!23 = !DILocalVariable(name: "biz", arg: 3, scope: !10, file: !11, line: 31, type: !17)
!24 = !DILocalVariable(name: "strwid", arg: 4, scope: !10, file: !11, line: 31, type: !17)
!25 = !DILocalVariable(name: "i", scope: !10, file: !11, line: 33, type: !17)
!26 = !DILocalVariable(name: "j", scope: !10, file: !11, line: 33, type: !17)
!27 = !{!28, !29, i64 0}
!28 = !{!"", !29, i64 0, !6, i64 4, !6, i64 5, !6, i64 6, !6, i64 7}
!29 = !{!"any pointer", !6, i64 0}
!30 = !{!28, !6, i64 5}
!31 = !{!28, !6, i64 7}
!32 = !{!28, !6, i64 4}
!33 = !{!28, !6, i64 6}
!34 = !{!29, !29, i64 0}
!35 = distinct !{!35, !36}
!36 = !DILocation(line: 18, column: 34, scope: !37)
!37 = distinct !DILexicalBlock(scope: !39, file: !38, line: 16, column: 13)
!38 = !DIFile(filename: "bitcnt_1.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!39 = distinct !DISubprogram(name: "bit_count", scope: !38, file: !38, line: 9, type: !40, isLocal: false, isDefinition: true, scopeLine: 10, flags: DIFlagPrototyped, isOptimized: true, unit: !42, variables: !43)
!40 = !DISubroutineType(types: !41)
!41 = !{!17, !16}
!42 = distinct !DICompileUnit(language: DW_LANG_C99, file: !38, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19)
!43 = !{!44, !45}
!44 = !DILocalVariable(name: "x", arg: 1, scope: !39, file: !38, line: 9, type: !16)
!45 = !DILocalVariable(name: "n", scope: !39, file: !38, line: 11, type: !17)
!46 = distinct !{!46, !47, !90}
!47 = !DILocation(line: 60, column: 27, scope: !48)
!48 = distinct !DILexicalBlock(scope: !50, file: !49, line: 59, column: 5)
!49 = !DIFile(filename: "bitcnts.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!50 = distinct !DILexicalBlock(scope: !51, file: !49, line: 56, column: 31)
!51 = distinct !DILexicalBlock(scope: !52, file: !49, line: 56, column: 3)
!52 = distinct !DILexicalBlock(scope: !53, file: !49, line: 56, column: 3)
!53 = distinct !DISubprogram(name: "main", scope: !49, file: !49, line: 21, type: !54, isLocal: false, isDefinition: true, scopeLine: 22, flags: DIFlagPrototyped, isOptimized: true, unit: !57, variables: !71)
!54 = !DISubroutineType(types: !55)
!55 = !{!17, !17, !56}
!56 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 32)
!57 = distinct !DICompileUnit(language: DW_LANG_C99, file: !49, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19, retainedTypes: !58, globals: !61)
!58 = !{!59, !17, !56, !60}
!59 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!60 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!61 = !{!62, !68}
!62 = !DIGlobalVariableExpression(var: !63)
!63 = distinct !DIGlobalVariable(name: "pBitCntFunc", scope: !53, file: !49, line: 28, type: !64, isLocal: true, isDefinition: true)
!64 = !DICompositeType(tag: DW_TAG_array_type, baseType: !65, size: 224, elements: !66)
!65 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !40, size: 32)
!66 = !{!67}
!67 = !DISubrange(count: 7)
!68 = !DIGlobalVariableExpression(var: !69)
!69 = distinct !DIGlobalVariable(name: "text", scope: !53, file: !49, line: 38, type: !70, isLocal: true, isDefinition: true)
!70 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 224, elements: !66)
!71 = !{!72, !73, !74, !79, !80, !81, !82, !83, !84, !85, !86, !87, !88, !89}
!72 = !DILocalVariable(name: "argc", arg: 1, scope: !53, file: !49, line: 21, type: !17)
!73 = !DILocalVariable(name: "argv", arg: 2, scope: !53, file: !49, line: 21, type: !56)
!74 = !DILocalVariable(name: "start", scope: !53, file: !49, line: 23, type: !75)
!75 = !DIDerivedType(tag: DW_TAG_typedef, name: "clock_t", file: !76, line: 59, baseType: !77)
!76 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/time.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!77 = !DIDerivedType(tag: DW_TAG_typedef, name: "__clock_t", file: !78, line: 135, baseType: !16)
!78 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!79 = !DILocalVariable(name: "stop", scope: !53, file: !49, line: 23, type: !75)
!80 = !DILocalVariable(name: "ct", scope: !53, file: !49, line: 24, type: !59)
!81 = !DILocalVariable(name: "cmin", scope: !53, file: !49, line: 24, type: !59)
!82 = !DILocalVariable(name: "cmax", scope: !53, file: !49, line: 24, type: !59)
!83 = !DILocalVariable(name: "i", scope: !53, file: !49, line: 25, type: !17)
!84 = !DILocalVariable(name: "cminix", scope: !53, file: !49, line: 25, type: !17)
!85 = !DILocalVariable(name: "cmaxix", scope: !53, file: !49, line: 25, type: !17)
!86 = !DILocalVariable(name: "j", scope: !53, file: !49, line: 26, type: !16)
!87 = !DILocalVariable(name: "n", scope: !53, file: !49, line: 26, type: !16)
!88 = !DILocalVariable(name: "seed", scope: !53, file: !49, line: 26, type: !16)
!89 = !DILocalVariable(name: "iterations", scope: !53, file: !49, line: 27, type: !17)
!90 = !{!"llvm.loop.CGRA.disable"}
!91 = distinct !{!91, !47, !92, !93, !90}
!92 = !{!"llvm.loop.vectorize.width", i32 1}
!93 = !{!"llvm.loop.interleave.count", i32 1}
!94 = distinct !{!94, !47, !90}
!95 = distinct !{!95, !47, !90}
!96 = distinct !{!96, !97}
!97 = !DILocation(line: 85, column: 22, scope: !98)
!98 = distinct !DILexicalBlock(scope: !99, file: !49, line: 84, column: 3)
!99 = distinct !DISubprogram(name: "bit_shifter", scope: !49, file: !49, line: 80, type: !40, isLocal: true, isDefinition: true, scopeLine: 81, flags: DIFlagPrototyped, isOptimized: true, unit: !57, variables: !100)
!100 = !{!101, !102, !103}
!101 = !DILocalVariable(name: "x", arg: 1, scope: !99, file: !49, line: 80, type: !16)
!102 = !DILocalVariable(name: "i", scope: !99, file: !49, line: 82, type: !17)
!103 = !DILocalVariable(name: "n", scope: !99, file: !49, line: 82, type: !17)
!104 = distinct !{!104, !47, !90}
!105 = !{!106, !106, i64 0}
!106 = !{!"int", !6, i64 0}
!107 = !{!108, !108, i64 0}
!108 = !{!"long", !6, i64 0}
!109 = distinct !{!109, !110}
!110 = !DILocation(line: 223, column: 46, scope: !111)
!111 = distinct !DILexicalBlock(scope: !113, file: !112, line: 220, column: 14)
!112 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!113 = distinct !DISubprogram(name: "runOnCGRA", scope: !112, file: !112, line: 217, type: !114, isLocal: false, isDefinition: true, scopeLine: 218, flags: DIFlagPrototyped, isOptimized: true, unit: !116, variables: !155)
!114 = !DISubroutineType(types: !115)
!115 = !{!60, !60}
!116 = distinct !DICompileUnit(language: DW_LANG_C99, file: !112, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19, retainedTypes: !117, globals: !121)
!117 = !{!118, !119, !16, !60, !120}
!118 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !119, size: 32)
!119 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!120 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 32)
!121 = !{!122, !125, !127, !129, !131, !133, !135, !137, !139, !141, !144, !146, !148, !150}
!122 = !DIGlobalVariableExpression(var: !123)
!123 = distinct !DIGlobalVariable(name: "totalLoops", scope: !116, file: !124, line: 27, type: !119, isLocal: false, isDefinition: true)
!124 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!125 = !DIGlobalVariableExpression(var: !126)
!126 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !116, file: !124, line: 30, type: !119, isLocal: false, isDefinition: true)
!127 = !DIGlobalVariableExpression(var: !128)
!128 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !116, file: !124, line: 31, type: !119, isLocal: false, isDefinition: true)
!129 = !DIGlobalVariableExpression(var: !130)
!130 = distinct !DIGlobalVariable(name: "thread_exit", scope: !116, file: !124, line: 32, type: !119, isLocal: false, isDefinition: true)
!131 = !DIGlobalVariableExpression(var: !132)
!132 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !116, file: !124, line: 34, type: !17, isLocal: false, isDefinition: true)
!133 = !DIGlobalVariableExpression(var: !134)
!134 = distinct !DIGlobalVariable(name: "prolog", scope: !116, file: !124, line: 21, type: !118, isLocal: false, isDefinition: true)
!135 = !DIGlobalVariableExpression(var: !136)
!136 = distinct !DIGlobalVariable(name: "epilog", scope: !116, file: !124, line: 21, type: !118, isLocal: false, isDefinition: true)
!137 = !DIGlobalVariableExpression(var: !138)
!138 = distinct !DIGlobalVariable(name: "kernel", scope: !116, file: !124, line: 21, type: !118, isLocal: false, isDefinition: true)
!139 = !DIGlobalVariableExpression(var: !140)
!140 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !116, file: !124, line: 22, type: !118, isLocal: false, isDefinition: true)
!141 = !DIGlobalVariableExpression(var: !142)
!142 = distinct !DIGlobalVariable(name: "prologPtr", scope: !116, file: !124, line: 24, type: !143, isLocal: false, isDefinition: true)
!143 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 32)
!144 = !DIGlobalVariableExpression(var: !145)
!145 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !116, file: !124, line: 24, type: !143, isLocal: false, isDefinition: true)
!146 = !DIGlobalVariableExpression(var: !147)
!147 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !116, file: !124, line: 24, type: !143, isLocal: false, isDefinition: true)
!148 = !DIGlobalVariableExpression(var: !149)
!149 = distinct !DIGlobalVariable(name: "initCGRA", scope: !116, file: !124, line: 26, type: !120, isLocal: false, isDefinition: true)
!150 = !DIGlobalVariableExpression(var: !151)
!151 = distinct !DIGlobalVariable(name: "pth", scope: !116, file: !124, line: 29, type: !152, isLocal: false, isDefinition: true)
!152 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !153, line: 37, baseType: !154)
!153 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount7")
!154 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!155 = !{!156}
!156 = !DILocalVariable(name: "arg", arg: 1, scope: !113, file: !112, line: 217, type: !60)
!157 = !{i32 6148}
!158 = !{i32 7183}
!159 = distinct !{!159, !160}
!160 = !DILocation(line: 257, column: 5, scope: !113)
!161 = distinct !{!161, !162}
!162 = !DILocation(line: 277, column: 41, scope: !163)
!163 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !112, file: !112, line: 261, type: !164, isLocal: false, isDefinition: true, scopeLine: 262, flags: DIFlagPrototyped, isOptimized: true, unit: !116, variables: !166)
!164 = !DISubroutineType(types: !165)
!165 = !{null, !119}
!166 = !{!167, !168, !169}
!167 = !DILocalVariable(name: "loopNo", arg: 1, scope: !163, file: !112, line: 261, type: !119)
!168 = !DILocalVariable(name: "result", scope: !163, file: !112, line: 263, type: !17)
!169 = !DILocalVariable(name: "configure", scope: !163, file: !112, line: 266, type: !17)
!170 = distinct !{!170, !171}
!171 = !DILocation(line: 314, column: 29, scope: !172)
!172 = distinct !DILexicalBlock(scope: !173, file: !112, line: 313, column: 3)
!173 = distinct !DISubprogram(name: "createCGRA", scope: !112, file: !112, line: 293, type: !174, isLocal: false, isDefinition: true, scopeLine: 294, isOptimized: true, unit: !116, variables: !176)
!174 = !DISubroutineType(types: !175)
!175 = !{null}
!176 = !{!177, !178}
!177 = !DILocalVariable(name: "result", scope: !173, file: !112, line: 303, type: !17)
!178 = !DILocalVariable(name: "i", scope: !173, file: !112, line: 304, type: !119)
