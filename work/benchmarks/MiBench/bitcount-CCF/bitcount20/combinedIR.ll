; ModuleID = 'temp.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.bfile = type { %struct._IO_FILE*, i8, i8, i8, i8 }
%union.pthread_attr_t = type { i32, [32 x i8] }

@.str = private unnamed_addr constant [3 x i8] c"01\00", align 1
@thread_cond_cpu = local_unnamed_addr global i32 1, align 4
@thread_cond_cgra = local_unnamed_addr global i32 0, align 4
@thread_exit = local_unnamed_addr global i32 0, align 4
@dynamicTCVal = local_unnamed_addr global i32 0, align 4
@initCGRA = common local_unnamed_addr global [7 x i32] zeroinitializer, align 4
@epilog = common local_unnamed_addr global i32* null, align 4
@prolog = common local_unnamed_addr global i32* null, align 4
@kernel = common local_unnamed_addr global i32* null, align 4
@prologPtr = common local_unnamed_addr global i32 0, align 4
@epilogPtr = common local_unnamed_addr global i32 0, align 4
@kernelPtr = common local_unnamed_addr global i32 0, align 4
@pth = common global i32 0, align 4
@ArrPtr = common local_unnamed_addr global i32* null, align 4
@stderr = external local_unnamed_addr global %struct._IO_FILE*, align 4
@.str.7 = private unnamed_addr constant [29 x i8] c"Usage: bitcnts <iterations>\0A\00", align 1
@.str.8 = private unnamed_addr constant [33 x i8] c"Bit counter algorithm benchmark\0A\00", align 1
@main.text = internal unnamed_addr constant [7 x i8*] [i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0)], align 4
@.str.9 = private unnamed_addr constant [36 x i8] c"%-38s> Time: %7.3f sec.; Bits: %ld\0A\00", align 1
@main.pBitCntFunc = internal unnamed_addr constant [7 x i32 (i32)*] [i32 (i32)* @bit_count, i32 (i32)* @bitcount, i32 (i32)* @ntbl_bitcnt, i32 (i32)* @ntbl_bitcount, i32 (i32)* @BW_btbl_bitcount, i32 (i32)* @AR_btbl_bitcount, i32 (i32)* @bit_shifter], align 4
@.str.10 = private unnamed_addr constant [13 x i8] c"\0ABest  > %s\0A\00", align 1
@.str.11 = private unnamed_addr constant [12 x i8] c"Worst > %s\0A\00", align 1
@.str.1 = private unnamed_addr constant [29 x i8] c"Optimized 1 bit/loop counter\00", align 1
@.str.1.2 = private unnamed_addr constant [26 x i8] c"Ratko's mystery algorithm\00", align 1
@.str.2 = private unnamed_addr constant [31 x i8] c"Recursive bit count by nybbles\00", align 1
@.str.3 = private unnamed_addr constant [35 x i8] c"Non-recursive bit count by nybbles\00", align 1
@.str.4 = private unnamed_addr constant [38 x i8] c"Non-recursive bit count by bytes (BW)\00", align 1
@.str.5 = private unnamed_addr constant [38 x i8] c"Non-recursive bit count by bytes (AR)\00", align 1
@.str.6 = private unnamed_addr constant [21 x i8] c"Shift and count bits\00", align 1
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
@.str.19 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@str = private unnamed_addr constant [14 x i8] c"deleting cgra\00"
@str.23 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00"
@str.24 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00"

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

.preheader:                                       ; preds = %.preheader.preheader, %12
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

.preheader:                                       ; preds = %.preheader.preheader, %29
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
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %14
  %34 = phi i8* [ %15, %14 ], [ %30, %.loopexit.loopexit ]
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
  br i1 %12, label %.preheader2.preheader, label %65

.preheader2.preheader:                            ; preds = %7
  br label %.preheader2

.preheader2:                                      ; preds = %.preheader2.preheader, %.loopexit
  %13 = phi double [ %37, %.loopexit ], [ 0x7FEFFFFFFFFFFFFF, %.preheader2.preheader ]
  %14 = phi i32 [ %40, %.loopexit ], [ undef, %.preheader2.preheader ]
  %15 = phi i32 [ %36, %.loopexit ], [ undef, %.preheader2.preheader ]
  %16 = phi i32 [ %44, %.loopexit ], [ 0, %.preheader2.preheader ]
  %17 = phi double [ %39, %.loopexit ], [ 0.000000e+00, %.preheader2.preheader ]
  %18 = tail call i32 @clock() #5
  %19 = icmp eq i32 %16, 2
  %20 = tail call i32 @rand() #5
  br i1 %19, label %.preheader.preheader, label %54

.preheader.preheader:                             ; preds = %.preheader2
  br label %.preheader

; <label>:21:                                     ; preds = %54, %21
  %22 = phi i32 [ %20, %54 ], [ %28, %21 ]
  %23 = phi i32 [ 0, %54 ], [ %26, %21 ]
  %24 = phi i32 [ 0, %54 ], [ %27, %21 ]
  %25 = tail call i32 %56(i32 %22) #5
  %26 = add nsw i32 %25, %23
  %27 = add nuw nsw i32 %24, 1
  %28 = add nsw i32 %22, 13
  %29 = icmp eq i32 %27, %10
  br i1 %29, label %.loopexit.loopexit19, label %21, !llvm.loop !35

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit.loopexit19:                             ; preds = %21
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit19, %.loopexit.loopexit
  %30 = phi i32 [ %50, %.loopexit.loopexit ], [ %26, %.loopexit.loopexit19 ]
  %31 = tail call i32 @clock() #5
  %32 = sub nsw i32 %31, %18
  %33 = sitofp i32 %32 to double
  %34 = fdiv double %33, 1.000000e+06
  %35 = fcmp olt double %34, %13
  %36 = select i1 %35, i32 %16, i32 %15
  %37 = select i1 %35, double %34, double %13
  %38 = fcmp ogt double %34, %17
  %39 = select i1 %38, double %34, double %17
  %40 = select i1 %38, i32 %16, i32 %14
  %41 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %16
  %42 = load i8*, i8** %41, align 4, !tbaa !34
  %43 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* %42, double %34, i32 %30)
  %44 = add nuw nsw i32 %16, 1
  %45 = icmp eq i32 %44, 7
  br i1 %45, label %.loopexit3.loopexit, label %.preheader2, !llvm.loop !83

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %46 = phi i32 [ %52, %.preheader ], [ %20, %.preheader.preheader ]
  %47 = phi i32 [ %50, %.preheader ], [ 0, %.preheader.preheader ]
  %48 = phi i32 [ %51, %.preheader ], [ 0, %.preheader.preheader ]
  %49 = tail call i32 @ntbl_bitcnt(i32 %46) #5
  %50 = add nsw i32 %49, %47
  %51 = add nuw nsw i32 %48, 1
  %52 = add nsw i32 %46, 13
  %53 = icmp eq i32 %51, %10
  br i1 %53, label %.loopexit.loopexit, label %.preheader, !llvm.loop !85

; <label>:54:                                     ; preds = %.preheader2
  %55 = getelementptr inbounds [7 x i32 (i32)*], [7 x i32 (i32)*]* @main.pBitCntFunc, i32 0, i32 %16
  %56 = load i32 (i32)*, i32 (i32)** %55, align 4, !tbaa !34
  br label %21

.loopexit3.loopexit:                              ; preds = %.loopexit
  br label %.loopexit3

.loopexit3:                                       ; preds = %.loopexit3.loopexit, %65
  %57 = phi i32 [ %149, %65 ], [ %36, %.loopexit3.loopexit ]
  %58 = phi i32 [ %151, %65 ], [ %40, %.loopexit3.loopexit ]
  %59 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %57
  %60 = load i8*, i8** %59, align 4, !tbaa !34
  %61 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.10, i32 0, i32 0), i8* %60)
  %62 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %58
  %63 = load i8*, i8** %62, align 4, !tbaa !34
  %64 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.11, i32 0, i32 0), i8* %63)
  ret i32 0

; <label>:65:                                     ; preds = %7
  %66 = tail call i32 @clock() #5
  %67 = tail call i32 @rand() #5
  %68 = tail call i32 @clock() #5
  %69 = sub nsw i32 %68, %66
  %70 = sitofp i32 %69 to double
  %71 = fdiv double %70, 1.000000e+06
  %72 = fcmp olt double %71, 0x7FEFFFFFFFFFFFFF
  %73 = select i1 %72, double %71, double 0x7FEFFFFFFFFFFFFF
  %74 = fcmp ogt double %71, 0.000000e+00
  %75 = select i1 %74, double %71, double 0.000000e+00
  %76 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), double %71, i32 0)
  %77 = tail call i32 @clock() #5
  %78 = tail call i32 @rand() #5
  %79 = tail call i32 @clock() #5
  %80 = sub nsw i32 %79, %77
  %81 = sitofp i32 %80 to double
  %82 = fdiv double %81, 1.000000e+06
  %83 = fcmp olt double %82, %73
  %84 = zext i1 %83 to i32
  %85 = select i1 %83, double %82, double %73
  %86 = fcmp ogt double %82, %75
  %87 = select i1 %86, double %82, double %75
  %88 = zext i1 %86 to i32
  %89 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), double %82, i32 0)
  %90 = tail call i32 @clock() #5
  %91 = tail call i32 @rand() #5
  %92 = tail call i32 @clock() #5
  %93 = sub nsw i32 %92, %90
  %94 = sitofp i32 %93 to double
  %95 = fdiv double %94, 1.000000e+06
  %96 = fcmp olt double %95, %85
  %97 = select i1 %96, i32 2, i32 %84
  %98 = select i1 %96, double %95, double %85
  %99 = fcmp ogt double %95, %87
  %100 = select i1 %99, double %95, double %87
  %101 = select i1 %99, i32 2, i32 %88
  %102 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), double %95, i32 0)
  %103 = tail call i32 @clock() #5
  %104 = tail call i32 @rand() #5
  %105 = tail call i32 @clock() #5
  %106 = sub nsw i32 %105, %103
  %107 = sitofp i32 %106 to double
  %108 = fdiv double %107, 1.000000e+06
  %109 = fcmp olt double %108, %98
  %110 = select i1 %109, i32 3, i32 %97
  %111 = select i1 %109, double %108, double %98
  %112 = fcmp ogt double %108, %100
  %113 = select i1 %112, double %108, double %100
  %114 = select i1 %112, i32 3, i32 %101
  %115 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), double %108, i32 0)
  %116 = tail call i32 @clock() #5
  %117 = tail call i32 @rand() #5
  %118 = tail call i32 @clock() #5
  %119 = sub nsw i32 %118, %116
  %120 = sitofp i32 %119 to double
  %121 = fdiv double %120, 1.000000e+06
  %122 = fcmp olt double %121, %111
  %123 = select i1 %122, i32 4, i32 %110
  %124 = select i1 %122, double %121, double %111
  %125 = fcmp ogt double %121, %113
  %126 = select i1 %125, double %121, double %113
  %127 = select i1 %125, i32 4, i32 %114
  %128 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), double %121, i32 0)
  %129 = tail call i32 @clock() #5
  %130 = tail call i32 @rand() #5
  %131 = tail call i32 @clock() #5
  %132 = sub nsw i32 %131, %129
  %133 = sitofp i32 %132 to double
  %134 = fdiv double %133, 1.000000e+06
  %135 = fcmp olt double %134, %124
  %136 = select i1 %135, i32 5, i32 %123
  %137 = select i1 %135, double %134, double %124
  %138 = fcmp ogt double %134, %126
  %139 = select i1 %138, double %134, double %126
  %140 = select i1 %138, i32 5, i32 %127
  %141 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), double %134, i32 0)
  %142 = tail call i32 @clock() #5
  %143 = tail call i32 @rand() #5
  %144 = tail call i32 @clock() #5
  %145 = sub nsw i32 %144, %142
  %146 = sitofp i32 %145 to double
  %147 = fdiv double %146, 1.000000e+06
  %148 = fcmp olt double %147, %137
  %149 = select i1 %148, i32 6, i32 %136
  %150 = fcmp ogt double %147, %139
  %151 = select i1 %150, i32 6, i32 %140
  %152 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), double %147, i32 0)
  br label %.loopexit3
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

; Function Attrs: nounwind readnone
define i32 @ntbl_bitcnt(i32) #7 {
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
define i32 @bit_count(i32) #8 {
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %.loopexit, label %.preheader.preheader, !llvm.loop !90

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %3 = phi i32 [ %7, %.preheader ], [ %0, %.preheader.preheader ]
  %4 = phi i32 [ %5, %.preheader ], [ 0, %.preheader.preheader ]
  %5 = add nuw nsw i32 %4, 1
  %6 = add nsw i32 %3, -1
  %7 = and i32 %6, %3
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !llvm.loop !90

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %9 = phi i32 [ 0, %1 ], [ %5, %.loopexit.loopexit ]
  ret i32 %9
}

; Function Attrs: norecurse nounwind readnone
define i32 @bitcount(i32) #8 {
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

; Function Attrs: norecurse nounwind readnone
define i32 @ntbl_bitcount(i32) #8 {
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
define i32 @BW_btbl_bitcount(i32) #8 {
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
define i32 @AR_btbl_bitcount(i32) #8 {
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

; Function Attrs: norecurse nounwind readnone
define internal i32 @bit_shifter(i32) #8 {
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %3 = phi i32 [ %7, %.preheader ], [ 0, %.preheader.preheader ]
  %4 = phi i32 [ %8, %.preheader ], [ 0, %.preheader.preheader ]
  %5 = phi i32 [ %9, %.preheader ], [ %0, %.preheader.preheader ]
  %6 = and i32 %5, 1
  %7 = add nsw i32 %6, %3
  %8 = add nuw nsw i32 %4, 1
  %9 = ashr i32 %5, 1
  %10 = icmp ne i32 %9, 0
  %11 = icmp ult i32 %8, 32
  %12 = and i1 %11, %10
  br i1 %12, label %.preheader, label %.loopexit.loopexit, !llvm.loop !99

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %13 = phi i32 [ 0, %1 ], [ %7, %.loopexit.loopexit ]
  ret i32 %13
}

; Function Attrs: nounwind readnone
define i32 @btbl_bitcnt(i32) local_unnamed_addr #7 {
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
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 0), align 4, !tbaa !107
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 1), align 4, !tbaa !107
  %14 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  tail call void @llvm.memset.p0i8.i64(i8* bitcast (i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2) to i8*), i8 0, i64 20, i32 4, i1 false)
  call void @llvm.lifetime.start(i64 25, i8* nonnull %14) #5
  %15 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %15) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %15, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %16 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %14, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32 %0) #5
  %17 = call i8* @strcat(i8* nonnull %15, i8* nonnull %14) #5
  %18 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %18) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %18, i8 0, i32 40, i32 1, i1 false)
  %19 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %19) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %19, i8 0, i32 40, i32 1, i1 false)
  %20 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %20) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %20, i8 0, i32 40, i32 1, i1 false)
  %21 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %21) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %21, i8 0, i32 40, i32 1, i1 false)
  %22 = call i8* @strcat(i8* nonnull %18, i8* nonnull %15) #5
  %23 = call i32 @strlen(i8* nonnull %18)
  %24 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %23
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %24, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1.14, i32 0, i32 0), i32 16, i32 1, i1 false)
  %25 = call i8* @strcat(i8* nonnull %19, i8* nonnull %15) #5
  %26 = call i32 @strlen(i8* nonnull %19)
  %27 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %26
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %27, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2.15, i32 0, i32 0), i32 16, i32 1, i1 false)
  %28 = call i8* @strcat(i8* nonnull %20, i8* nonnull %15) #5
  %29 = call i32 @strlen(i8* nonnull %20)
  %30 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %29
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %30, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3.16, i32 0, i32 0), i32 16, i32 1, i1 false)
  %31 = call %struct._IO_FILE* @fopen(i8* nonnull %18, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0))
  %32 = call %struct._IO_FILE* @fopen(i8* nonnull %19, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0))
  %33 = call %struct._IO_FILE* @fopen(i8* nonnull %20, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0))
  %34 = bitcast i32* %8 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %34) #5
  %35 = bitcast i32* %9 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %35) #5
  %36 = bitcast i32* %10 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %36) #5
  %37 = bitcast i32* %11 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %37) #5
  %38 = call i32 @fread(i8* nonnull %34, i32 4, i32 1, %struct._IO_FILE* %31)
  %39 = call i32 @fread(i8* nonnull %35, i32 4, i32 1, %struct._IO_FILE* %32)
  %40 = call i32 @fread(i8* nonnull %36, i32 4, i32 1, %struct._IO_FILE* %33)
  %41 = load i32, i32* %8, align 4, !tbaa !107
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5.18, i32 0, i32 0), i32 %41)
  %43 = load i32, i32* %9, align 4, !tbaa !107
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6.19, i32 0, i32 0), i32 %43)
  %45 = load i32, i32* %10, align 4, !tbaa !107
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7.20, i32 0, i32 0), i32 %45)
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8.21, i32 0, i32 0), i32 4)
  %48 = shl i32 %41, 2
  %49 = call noalias i8* @malloc(i32 %48) #5
  store i8* %49, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !34
  %50 = shl i32 %43, 2
  %51 = call noalias i8* @malloc(i32 %50) #5
  store i8* %51, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !34
  %52 = shl i32 %45, 2
  %53 = call noalias i8* @malloc(i32 %52) #5
  store i8* %53, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !34
  %54 = call i32 @fread(i8* %49, i32 4, i32 %41, %struct._IO_FILE* %31)
  %55 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !34
  %56 = call i32 @fread(i8* %55, i32 4, i32 %43, %struct._IO_FILE* %32)
  %57 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !34
  %58 = call i32 @fread(i8* %57, i32 4, i32 %45, %struct._IO_FILE* %33)
  %59 = call i8* @strcat(i8* nonnull %21, i8* nonnull %15) #5
  %60 = call i32 @strlen(i8* nonnull %21)
  %61 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %60
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %61, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.9.22, i32 0, i32 0), i32 26, i32 1, i1 false)
  %62 = call %struct._IO_FILE* @fopen(i8* nonnull %21, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0))
  %63 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %62, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %11) #5
  %64 = sdiv i32 %45, 16
  %65 = sdiv i32 %41, 16
  %66 = sdiv i32 %43, 16
  store i32 %64, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2), align 4, !tbaa !107
  store i32 %65, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 3), align 4, !tbaa !107
  store i32 %66, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 4), align 4, !tbaa !107
  %67 = load i32, i32* %11, align 4, !tbaa !107
  %68 = sdiv i32 %67, 16
  store i32 %68, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 6), align 4, !tbaa !107
  %69 = call i32 @fclose(%struct._IO_FILE* %31)
  %70 = call i32 @fclose(%struct._IO_FILE* %32)
  %71 = call i32 @fclose(%struct._IO_FILE* %33)
  %72 = call i32 @fclose(%struct._IO_FILE* %62)
  %73 = getelementptr inbounds [40 x i8], [40 x i8]* %12, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %73) #5
  call void @llvm.memset.p0i8.i32(i8* nonnull %73, i8 0, i32 40, i32 1, i1 false)
  %74 = call i8* @strcat(i8* nonnull %73, i8* nonnull %15) #5
  %75 = call i32 @strlen(i8* nonnull %73)
  %76 = getelementptr [40 x i8], [40 x i8]* %12, i32 0, i32 %75
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %76, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.11.24, i32 0, i32 0), i32 18, i32 1, i1 false)
  %77 = bitcast i32* %13 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %77) #5
  store i32 0, i32* %13, align 4, !tbaa !107
  %78 = call %struct._IO_FILE* @fopen(i8* nonnull %73, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0))
  %79 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %78, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %13) #5
  %80 = call i32 @fclose(%struct._IO_FILE* %78)
  %81 = load i32, i32* %13, align 4, !tbaa !107
  %82 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %81)
  %83 = load i32, i32* %13, align 4, !tbaa !107
  %84 = icmp sgt i32 %83, 0
  %85 = load i32, i32* @dynamicTCVal, align 4
  %86 = select i1 %84, i32 0, i32 %85
  %87 = add nsw i32 %86, %83
  store i32 %87, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !tbaa !107
  %88 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !34
  %89 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !34
  %90 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !34
  %91 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.13.25, i32 0, i32 0), i32 %88, i32 %89, i32 %90)
  %92 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !34
  store i32 %92, i32* @prologPtr, align 4, !tbaa !109
  %93 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !34
  store i32 %93, i32* @epilogPtr, align 4, !tbaa !109
  %94 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !34
  store i32 %94, i32* @kernelPtr, align 4, !tbaa !109
  call void @llvm.lifetime.end(i64 4, i8* nonnull %77) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %73) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %37) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %36) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %35) #5
  call void @llvm.lifetime.end(i64 4, i8* nonnull %34) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %21) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %20) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %19) #5
  call void @llvm.lifetime.end(i64 40, i8* nonnull %18) #5
  call void @llvm.lifetime.end(i64 20, i8* nonnull %15) #5
  call void @llvm.lifetime.end(i64 25, i8* nonnull %14) #5
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
  %5 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  call void @llvm.lifetime.start(i64 25, i8* nonnull %5) #5
  %6 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %6) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %6, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %7 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %5, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32 %0) #5
  %8 = call i8* @strcat(i8* nonnull %6, i8* nonnull %5) #5
  %9 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %9) #5
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %9, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %10 = call i32 @strlen(i8* nonnull %9)
  %11 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %10
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %11, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.14, i32 0, i32 0), i32 14, i32 1, i1 false)
  %12 = call %struct._IO_FILE* @fopen(i8* nonnull %9, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.15, i32 0, i32 0))
  %13 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 0), align 4, !tbaa !107
  %14 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %13)
  %15 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 1), align 4, !tbaa !107
  %16 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %15)
  %17 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2), align 4, !tbaa !107
  %18 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %17)
  %19 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 3), align 4, !tbaa !107
  %20 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %19)
  %21 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 4), align 4, !tbaa !107
  %22 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %21)
  %23 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !tbaa !107
  %24 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %23)
  %25 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 6), align 4, !tbaa !107
  %26 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %25)
  %27 = load i32, i32* @epilogPtr, align 4, !tbaa !109
  %28 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %27)
  %29 = load i32, i32* @prologPtr, align 4, !tbaa !109
  %30 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %29)
  %31 = load i32, i32* @kernelPtr, align 4, !tbaa !109
  %32 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %12, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %31)
  %33 = call i32 @fclose(%struct._IO_FILE* %12)
  call void @llvm.lifetime.end(i64 40, i8* nonnull %9) #5
  call void @llvm.lifetime.end(i64 20, i8* nonnull %6) #5
  call void @llvm.lifetime.end(i64 25, i8* nonnull %5) #5
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #4

; Function Attrs: norecurse nounwind
define void @updateLoopTC(i32) local_unnamed_addr #10 {
  %2 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !tbaa !107
  %3 = icmp sgt i32 %2, 0
  br i1 %3, label %6, label %4

; <label>:4:                                      ; preds = %1
  %5 = load i32, i32* @dynamicTCVal, align 4, !tbaa !107
  store i32 %5, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !tbaa !107
  br label %6

; <label>:6:                                      ; preds = %4, %1
  ret void
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #2 {
  br label %2

; <label>:2:                                      ; preds = %11, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !107
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !107
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %.loopexit.loopexit, label %.preheader, !llvm.loop !111

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %8 = load i32, i32* @thread_exit, align 4, !tbaa !107
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #5, !srcloc !156
  ret i8* null

; <label>:11:                                     ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !107
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #5, !srcloc !157
  %12 = tail call i32 @usleep(i32 1) #5
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !107
  br label %2, !llvm.loop !158
}

declare i32 @usleep(i32) local_unnamed_addr #9

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #2 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.19, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !107
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !107
  br label %4

; <label>:4:                                      ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !107
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !160

; <label>:8:                                      ; preds = %4
  ret void
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #2 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0))
  store i32 1, i32* @thread_exit, align 4, !tbaa !107
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !107
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.23, i32 0, i32 0))
  %3 = load i32, i32* @pth, align 4, !tbaa !109
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #5
  ret void
}

declare i32 @pthread_join(i32, i8**) local_unnamed_addr #9

; Function Attrs: nounwind
define void @createCGRA() local_unnamed_addr #2 {
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 0), align 4, !tbaa !107
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 1), align 4, !tbaa !107
  tail call void @llvm.memset.p0i8.i64(i8* bitcast (i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2) to i8*), i8 0, i64 20, i32 4, i1 false)
  %1 = tail call i32 @initializeParameters(i32 1)
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str.24, i32 0, i32 0))
  %3 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #5
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
attributes #7 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { norecurse nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
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
!11 = !DIFile(filename: "bitstrng.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
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
!36 = !DILocation(line: 67, column: 27, scope: !37)
!37 = distinct !DILexicalBlock(scope: !39, file: !38, line: 66, column: 7)
!38 = !DIFile(filename: "bitcnts.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
!39 = distinct !DILexicalBlock(scope: !40, file: !38, line: 65, column: 5)
!40 = distinct !DILexicalBlock(scope: !41, file: !38, line: 58, column: 8)
!41 = distinct !DILexicalBlock(scope: !42, file: !38, line: 56, column: 31)
!42 = distinct !DILexicalBlock(scope: !43, file: !38, line: 56, column: 3)
!43 = distinct !DILexicalBlock(scope: !44, file: !38, line: 56, column: 3)
!44 = distinct !DISubprogram(name: "main", scope: !38, file: !38, line: 21, type: !45, isLocal: false, isDefinition: true, scopeLine: 22, flags: DIFlagPrototyped, isOptimized: true, unit: !48, variables: !64)
!45 = !DISubroutineType(types: !46)
!46 = !{!17, !17, !47}
!47 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 32)
!48 = distinct !DICompileUnit(language: DW_LANG_C99, file: !38, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19, retainedTypes: !49, globals: !52)
!49 = !{!50, !17, !47, !51}
!50 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!51 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!52 = !{!53, !61}
!53 = !DIGlobalVariableExpression(var: !54)
!54 = distinct !DIGlobalVariable(name: "pBitCntFunc", scope: !44, file: !38, line: 28, type: !55, isLocal: true, isDefinition: true)
!55 = !DICompositeType(tag: DW_TAG_array_type, baseType: !56, size: 224, elements: !59)
!56 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !57, size: 32)
!57 = !DISubroutineType(types: !58)
!58 = !{!17, !16}
!59 = !{!60}
!60 = !DISubrange(count: 7)
!61 = !DIGlobalVariableExpression(var: !62)
!62 = distinct !DIGlobalVariable(name: "text", scope: !44, file: !38, line: 38, type: !63, isLocal: true, isDefinition: true)
!63 = !DICompositeType(tag: DW_TAG_array_type, baseType: !14, size: 224, elements: !59)
!64 = !{!65, !66, !67, !72, !73, !74, !75, !76, !77, !78, !79, !80, !81, !82}
!65 = !DILocalVariable(name: "argc", arg: 1, scope: !44, file: !38, line: 21, type: !17)
!66 = !DILocalVariable(name: "argv", arg: 2, scope: !44, file: !38, line: 21, type: !47)
!67 = !DILocalVariable(name: "start", scope: !44, file: !38, line: 23, type: !68)
!68 = !DIDerivedType(tag: DW_TAG_typedef, name: "clock_t", file: !69, line: 59, baseType: !70)
!69 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/time.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
!70 = !DIDerivedType(tag: DW_TAG_typedef, name: "__clock_t", file: !71, line: 135, baseType: !16)
!71 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
!72 = !DILocalVariable(name: "stop", scope: !44, file: !38, line: 23, type: !68)
!73 = !DILocalVariable(name: "ct", scope: !44, file: !38, line: 24, type: !50)
!74 = !DILocalVariable(name: "cmin", scope: !44, file: !38, line: 24, type: !50)
!75 = !DILocalVariable(name: "cmax", scope: !44, file: !38, line: 24, type: !50)
!76 = !DILocalVariable(name: "i", scope: !44, file: !38, line: 25, type: !17)
!77 = !DILocalVariable(name: "cminix", scope: !44, file: !38, line: 25, type: !17)
!78 = !DILocalVariable(name: "cmaxix", scope: !44, file: !38, line: 25, type: !17)
!79 = !DILocalVariable(name: "j", scope: !44, file: !38, line: 26, type: !16)
!80 = !DILocalVariable(name: "n", scope: !44, file: !38, line: 26, type: !16)
!81 = !DILocalVariable(name: "seed", scope: !44, file: !38, line: 26, type: !16)
!82 = !DILocalVariable(name: "iterations", scope: !44, file: !38, line: 27, type: !17)
!83 = distinct !{!83, !84}
!84 = !DILocation(line: 82, column: 3, scope: !43)
!85 = distinct !{!85, !86, !89}
!86 = !DILocation(line: 62, column: 27, scope: !87)
!87 = distinct !DILexicalBlock(scope: !88, file: !38, line: 61, column: 7)
!88 = distinct !DILexicalBlock(scope: !40, file: !38, line: 59, column: 5)
!89 = !{!"llvm.loop.CGRA.enable"}
!90 = distinct !{!90, !91}
!91 = !DILocation(line: 18, column: 34, scope: !92)
!92 = distinct !DILexicalBlock(scope: !94, file: !93, line: 16, column: 13)
!93 = !DIFile(filename: "bitcnt_1.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
!94 = distinct !DISubprogram(name: "bit_count", scope: !93, file: !93, line: 9, type: !57, isLocal: false, isDefinition: true, scopeLine: 10, flags: DIFlagPrototyped, isOptimized: true, unit: !95, variables: !96)
!95 = distinct !DICompileUnit(language: DW_LANG_C99, file: !93, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19)
!96 = !{!97, !98}
!97 = !DILocalVariable(name: "x", arg: 1, scope: !94, file: !93, line: 9, type: !16)
!98 = !DILocalVariable(name: "n", scope: !94, file: !93, line: 11, type: !17)
!99 = distinct !{!99, !100}
!100 = !DILocation(line: 93, column: 22, scope: !101)
!101 = distinct !DILexicalBlock(scope: !102, file: !38, line: 92, column: 3)
!102 = distinct !DISubprogram(name: "bit_shifter", scope: !38, file: !38, line: 88, type: !57, isLocal: true, isDefinition: true, scopeLine: 89, flags: DIFlagPrototyped, isOptimized: true, unit: !48, variables: !103)
!103 = !{!104, !105, !106}
!104 = !DILocalVariable(name: "x", arg: 1, scope: !102, file: !38, line: 88, type: !16)
!105 = !DILocalVariable(name: "i", scope: !102, file: !38, line: 90, type: !17)
!106 = !DILocalVariable(name: "n", scope: !102, file: !38, line: 90, type: !17)
!107 = !{!108, !108, i64 0}
!108 = !{!"int", !6, i64 0}
!109 = !{!110, !110, i64 0}
!110 = !{!"long", !6, i64 0}
!111 = distinct !{!111, !112}
!112 = !DILocation(line: 193, column: 46, scope: !113)
!113 = distinct !DILexicalBlock(scope: !115, file: !114, line: 190, column: 14)
!114 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
!115 = distinct !DISubprogram(name: "runOnCGRA", scope: !114, file: !114, line: 187, type: !116, isLocal: false, isDefinition: true, scopeLine: 188, flags: DIFlagPrototyped, isOptimized: true, unit: !118, variables: !154)
!116 = !DISubroutineType(types: !117)
!117 = !{!51, !51}
!118 = distinct !DICompileUnit(language: DW_LANG_C99, file: !114, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19, retainedTypes: !119, globals: !122)
!119 = !{!120, !121, !16, !51}
!120 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !121, size: 32)
!121 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!122 = !{!123, !126, !128, !130, !132, !134, !136, !138, !140, !142, !144, !146, !149}
!123 = !DIGlobalVariableExpression(var: !124)
!124 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !118, file: !125, line: 29, type: !121, isLocal: false, isDefinition: true)
!125 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
!126 = !DIGlobalVariableExpression(var: !127)
!127 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !118, file: !125, line: 30, type: !121, isLocal: false, isDefinition: true)
!128 = !DIGlobalVariableExpression(var: !129)
!129 = distinct !DIGlobalVariable(name: "thread_exit", scope: !118, file: !125, line: 31, type: !121, isLocal: false, isDefinition: true)
!130 = !DIGlobalVariableExpression(var: !131)
!131 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !118, file: !125, line: 33, type: !17, isLocal: false, isDefinition: true)
!132 = !DIGlobalVariableExpression(var: !133)
!133 = distinct !DIGlobalVariable(name: "prolog", scope: !118, file: !125, line: 21, type: !120, isLocal: false, isDefinition: true)
!134 = !DIGlobalVariableExpression(var: !135)
!135 = distinct !DIGlobalVariable(name: "epilog", scope: !118, file: !125, line: 21, type: !120, isLocal: false, isDefinition: true)
!136 = !DIGlobalVariableExpression(var: !137)
!137 = distinct !DIGlobalVariable(name: "kernel", scope: !118, file: !125, line: 21, type: !120, isLocal: false, isDefinition: true)
!138 = !DIGlobalVariableExpression(var: !139)
!139 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !118, file: !125, line: 22, type: !120, isLocal: false, isDefinition: true)
!140 = !DIGlobalVariableExpression(var: !141)
!141 = distinct !DIGlobalVariable(name: "prologPtr", scope: !118, file: !125, line: 24, type: !16, isLocal: false, isDefinition: true)
!142 = !DIGlobalVariableExpression(var: !143)
!143 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !118, file: !125, line: 24, type: !16, isLocal: false, isDefinition: true)
!144 = !DIGlobalVariableExpression(var: !145)
!145 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !118, file: !125, line: 24, type: !16, isLocal: false, isDefinition: true)
!146 = !DIGlobalVariableExpression(var: !147)
!147 = distinct !DIGlobalVariable(name: "initCGRA", scope: !118, file: !125, line: 26, type: !148, isLocal: false, isDefinition: true)
!148 = !DICompositeType(tag: DW_TAG_array_type, baseType: !17, size: 224, elements: !59)
!149 = !DIGlobalVariableExpression(var: !150)
!150 = distinct !DIGlobalVariable(name: "pth", scope: !118, file: !125, line: 28, type: !151, isLocal: false, isDefinition: true)
!151 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !152, line: 37, baseType: !153)
!152 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount20")
!153 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!154 = !{!155}
!155 = !DILocalVariable(name: "arg", arg: 1, scope: !115, file: !114, line: 187, type: !51)
!156 = !{i32 5040}
!157 = !{i32 6075}
!158 = distinct !{!158, !159}
!159 = !DILocation(line: 227, column: 5, scope: !115)
!160 = distinct !{!160, !161}
!161 = !DILocation(line: 247, column: 41, scope: !162)
!162 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !114, file: !114, line: 231, type: !163, isLocal: false, isDefinition: true, scopeLine: 232, flags: DIFlagPrototyped, isOptimized: true, unit: !118, variables: !165)
!163 = !DISubroutineType(types: !164)
!164 = !{null, !121}
!165 = !{!166, !167, !168}
!166 = !DILocalVariable(name: "loopNo", arg: 1, scope: !162, file: !114, line: 231, type: !121)
!167 = !DILocalVariable(name: "result", scope: !162, file: !114, line: 233, type: !17)
!168 = !DILocalVariable(name: "configure", scope: !162, file: !114, line: 236, type: !17)
