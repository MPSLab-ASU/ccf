; ModuleID = 'temp.bc'
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
  store i32 %10, i32* @gVar4
  br i1 %12, label %.preheader2.preheader, label %112

.preheader2.preheader:                            ; preds = %7
  %13 = mul i32 %10, 13
  %min.iters.check = icmp ult i32 %10, 4
  %n.mod.vf = and i32 %10, 3
  %n.vec = sub i32 %10, %n.mod.vf
  %cmp.zero = icmp eq i32 %n.vec, 0
  %14 = mul nsw i32 %n.mod.vf, -13
  %cmp.n = icmp eq i32 %n.mod.vf, 0
  br label %.preheader2

.preheader2:                                      ; preds = %.loopexit, %.preheader2.preheader
  %15 = phi double [ %66, %.loopexit ], [ 0x7FEFFFFFFFFFFFFF, %.preheader2.preheader ]
  %16 = phi i32 [ %69, %.loopexit ], [ undef, %.preheader2.preheader ]
  %17 = phi i32 [ %65, %.loopexit ], [ undef, %.preheader2.preheader ]
  %18 = phi i32 [ %73, %.loopexit ], [ 0, %.preheader2.preheader ]
  %19 = phi double [ %68, %.loopexit ], [ 0.000000e+00, %.preheader2.preheader ]
  %20 = tail call i32 @clock() #5
  %21 = icmp eq i32 %18, 1
  %22 = tail call i32 @rand() #5
  br i1 %21, label %.preheader.preheader, label %101

.preheader.preheader:                             ; preds = %.preheader2
  br i1 %min.iters.check, label %.preheader.preheader23, label %min.iters.checked

.preheader.preheader23:                           ; preds = %middle.block, %min.iters.checked, %.preheader.preheader
  %.ph = phi i32 [ %22, %min.iters.checked ], [ %22, %.preheader.preheader ], [ %ind.end, %middle.block ]
  %.ph24 = phi i32 [ 0, %min.iters.checked ], [ 0, %.preheader.preheader ], [ %47, %middle.block ]
  %.ph25 = phi i32 [ 0, %min.iters.checked ], [ 0, %.preheader.preheader ], [ %n.vec, %middle.block ]
  store i32 %.ph, i32* @gVar1
  store i32 %.ph24, i32* @gVar2
  store i32 %.ph25, i32* @gVar3
  %23 = sub i32 %10, %.ph25
  %24 = sdiv i32 %23, 1
  store i32 %24, i32* @dynamicTCVal
  br label %.preheader

min.iters.checked:                                ; preds = %.preheader.preheader
  %25 = add i32 %13, %22
  %ind.end = add i32 %25, %14
  br i1 %cmp.zero, label %.preheader.preheader23, label %vector.ph

vector.ph:                                        ; preds = %min.iters.checked
  %.splatinsert = insertelement <4 x i32> undef, i32 %22, i32 0
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> undef, <4 x i32> zeroinitializer
  %induction = add <4 x i32> %.splat, <i32 0, i32 13, i32 26, i32 39>
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i32 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %vec.ind = phi <4 x i32> [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi <4 x i32> [ zeroinitializer, %vector.ph ], [ %45, %vector.body ]
  %26 = lshr <4 x i32> %vec.ind, <i32 1, i32 1, i32 1, i32 1>
  %27 = and <4 x i32> %26, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>
  %28 = and <4 x i32> %vec.ind, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>
  %29 = add nuw <4 x i32> %27, %28
  %30 = lshr <4 x i32> %29, <i32 2, i32 2, i32 2, i32 2>
  %31 = and <4 x i32> %30, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>
  %32 = and <4 x i32> %29, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>
  %33 = add nuw nsw <4 x i32> %31, %32
  %34 = lshr <4 x i32> %33, <i32 4, i32 4, i32 4, i32 4>
  %35 = and <4 x i32> %34, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>
  %36 = and <4 x i32> %33, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>
  %37 = add nuw nsw <4 x i32> %35, %36
  %38 = lshr <4 x i32> %37, <i32 8, i32 8, i32 8, i32 8>
  %39 = and <4 x i32> %38, <i32 983055, i32 983055, i32 983055, i32 983055>
  %40 = and <4 x i32> %37, <i32 983055, i32 983055, i32 983055, i32 983055>
  %41 = add nuw nsw <4 x i32> %39, %40
  %42 = lshr <4 x i32> %41, <i32 16, i32 16, i32 16, i32 16>
  %43 = and <4 x i32> %41, <i32 31, i32 31, i32 31, i32 31>
  %44 = add <4 x i32> %43, %vec.phi
  %45 = add <4 x i32> %44, %42
  %index.next = add i32 %index, 4
  %vec.ind.next = add <4 x i32> %vec.ind, <i32 52, i32 52, i32 52, i32 52>
  %46 = icmp eq i32 %index.next, %n.vec
  br i1 %46, label %middle.block, label %vector.body, !llvm.loop !35

middle.block:                                     ; preds = %vector.body
  %rdx.shuf = shufflevector <4 x i32> %45, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>
  %bin.rdx = add <4 x i32> %45, %rdx.shuf
  %rdx.shuf21 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>
  %bin.rdx22 = add <4 x i32> %bin.rdx, %rdx.shuf21
  %47 = extractelement <4 x i32> %bin.rdx22, i32 0
  br i1 %cmp.n, label %.loopexit, label %.preheader.preheader23

; <label>:48:                                     ; preds = %101, %48
  %49 = phi i32 [ %22, %101 ], [ %55, %48 ]
  %50 = phi i32 [ 0, %101 ], [ %53, %48 ]
  %51 = phi i32 [ 0, %101 ], [ %54, %48 ]
  %52 = tail call i32 %103(i32 %49) #5
  %53 = add nsw i32 %52, %50
  %54 = add nuw nsw i32 %51, 1
  %55 = add nsw i32 %49, 13
  %56 = icmp eq i32 %54, %10
  br i1 %56, label %.loopexit.loopexit26, label %48, !llvm.loop !86

.loopexit.loopexit:                               ; preds = %.preheader
  br label %57

; <label>:57:                                     ; preds = %.loopexit.loopexit
  %58 = load i32, i32* @gVar5
  br label %.loopexit

.loopexit.loopexit26:                             ; preds = %48
  br label %.loopexit

.loopexit:                                        ; preds = %57, %.loopexit.loopexit26, %middle.block
  %59 = phi i32 [ %47, %middle.block ], [ %58, %57 ], [ %53, %.loopexit.loopexit26 ]
  %60 = tail call i32 @clock() #5
  %61 = sub nsw i32 %60, %20
  %62 = sitofp i32 %61 to double
  %63 = fdiv double %62, 1.000000e+06
  %64 = fcmp olt double %63, %15
  %65 = select i1 %64, i32 %18, i32 %17
  %66 = select i1 %64, double %63, double %15
  %67 = fcmp ogt double %63, %19
  %68 = select i1 %67, double %63, double %19
  %69 = select i1 %67, i32 %18, i32 %16
  %70 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %18
  %71 = load i8*, i8** %70, align 4, !tbaa !34
  %72 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* %71, double %63, i32 %59)
  %73 = add nuw nsw i32 %18, 1
  %74 = icmp eq i32 %73, 7
  br i1 %74, label %.loopexit3.loopexit, label %.preheader2, !llvm.loop !90

.preheader:                                       ; preds = %.preheader, %.preheader.preheader23
  %75 = phi i32 [ %99, %.preheader ], [ %.ph, %.preheader.preheader23 ]
  %76 = phi i32 [ %97, %.preheader ], [ %.ph24, %.preheader.preheader23 ]
  %77 = phi i32 [ %98, %.preheader ], [ %.ph25, %.preheader.preheader23 ]
  %78 = lshr i32 %75, 1
  %79 = and i32 %78, 1431655765
  %80 = and i32 %75, 1431655765
  %81 = add nuw i32 %79, %80
  %82 = lshr i32 %81, 2
  %83 = and i32 %82, 858993459
  %84 = and i32 %81, 858993459
  %85 = add nuw nsw i32 %83, %84
  %86 = lshr i32 %85, 4
  %87 = and i32 %86, 117901063
  %88 = and i32 %85, 117901063
  %89 = add nuw nsw i32 %87, %88
  %90 = lshr i32 %89, 8
  %91 = and i32 %90, 983055
  %92 = and i32 %89, 983055
  %93 = add nuw nsw i32 %91, %92
  %94 = lshr i32 %93, 16
  %95 = and i32 %93, 31
  %96 = add i32 %95, %76
  %97 = add i32 %96, %94
  %98 = add nuw nsw i32 %77, 1
  %99 = add nsw i32 %75, 13
  %100 = icmp eq i32 %98, %10
  br i1 %100, label %.loopexit.loopexit, label %.preheader, !llvm.loop !92

; <label>:101:                                    ; preds = %.preheader2
  %102 = getelementptr inbounds [7 x i32 (i32)*], [7 x i32 (i32)*]* @main.pBitCntFunc, i32 0, i32 %18
  %103 = load i32 (i32)*, i32 (i32)** %102, align 4, !tbaa !34
  br label %48

.loopexit3.loopexit:                              ; preds = %.loopexit
  br label %.loopexit3

.loopexit3:                                       ; preds = %112, %.loopexit3.loopexit
  %104 = phi i32 [ %196, %112 ], [ %65, %.loopexit3.loopexit ]
  %105 = phi i32 [ %198, %112 ], [ %69, %.loopexit3.loopexit ]
  %106 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %104
  %107 = load i8*, i8** %106, align 4, !tbaa !34
  %108 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.10, i32 0, i32 0), i8* %107)
  %109 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %105
  %110 = load i8*, i8** %109, align 4, !tbaa !34
  %111 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.11, i32 0, i32 0), i8* %110)
  ret i32 0

; <label>:112:                                    ; preds = %7
  %113 = tail call i32 @clock() #5
  %114 = tail call i32 @rand() #5
  %115 = tail call i32 @clock() #5
  %116 = sub nsw i32 %115, %113
  %117 = sitofp i32 %116 to double
  %118 = fdiv double %117, 1.000000e+06
  %119 = fcmp olt double %118, 0x7FEFFFFFFFFFFFFF
  %120 = select i1 %119, double %118, double 0x7FEFFFFFFFFFFFFF
  %121 = fcmp ogt double %118, 0.000000e+00
  %122 = select i1 %121, double %118, double 0.000000e+00
  %123 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), double %118, i32 0)
  %124 = tail call i32 @clock() #5
  %125 = tail call i32 @rand() #5
  %126 = tail call i32 @clock() #5
  %127 = sub nsw i32 %126, %124
  %128 = sitofp i32 %127 to double
  %129 = fdiv double %128, 1.000000e+06
  %130 = fcmp olt double %129, %120
  %131 = zext i1 %130 to i32
  %132 = select i1 %130, double %129, double %120
  %133 = fcmp ogt double %129, %122
  %134 = select i1 %133, double %129, double %122
  %135 = zext i1 %133 to i32
  %136 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), double %129, i32 0)
  %137 = tail call i32 @clock() #5
  %138 = tail call i32 @rand() #5
  %139 = tail call i32 @clock() #5
  %140 = sub nsw i32 %139, %137
  %141 = sitofp i32 %140 to double
  %142 = fdiv double %141, 1.000000e+06
  %143 = fcmp olt double %142, %132
  %144 = select i1 %143, i32 2, i32 %131
  %145 = select i1 %143, double %142, double %132
  %146 = fcmp ogt double %142, %134
  %147 = select i1 %146, double %142, double %134
  %148 = select i1 %146, i32 2, i32 %135
  %149 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), double %142, i32 0)
  %150 = tail call i32 @clock() #5
  %151 = tail call i32 @rand() #5
  %152 = tail call i32 @clock() #5
  %153 = sub nsw i32 %152, %150
  %154 = sitofp i32 %153 to double
  %155 = fdiv double %154, 1.000000e+06
  %156 = fcmp olt double %155, %145
  %157 = select i1 %156, i32 3, i32 %144
  %158 = select i1 %156, double %155, double %145
  %159 = fcmp ogt double %155, %147
  %160 = select i1 %159, double %155, double %147
  %161 = select i1 %159, i32 3, i32 %148
  %162 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), double %155, i32 0)
  %163 = tail call i32 @clock() #5
  %164 = tail call i32 @rand() #5
  %165 = tail call i32 @clock() #5
  %166 = sub nsw i32 %165, %163
  %167 = sitofp i32 %166 to double
  %168 = fdiv double %167, 1.000000e+06
  %169 = fcmp olt double %168, %158
  %170 = select i1 %169, i32 4, i32 %157
  %171 = select i1 %169, double %168, double %158
  %172 = fcmp ogt double %168, %160
  %173 = select i1 %172, double %168, double %160
  %174 = select i1 %172, i32 4, i32 %161
  %175 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), double %168, i32 0)
  %176 = tail call i32 @clock() #5
  %177 = tail call i32 @rand() #5
  %178 = tail call i32 @clock() #5
  %179 = sub nsw i32 %178, %176
  %180 = sitofp i32 %179 to double
  %181 = fdiv double %180, 1.000000e+06
  %182 = fcmp olt double %181, %171
  %183 = select i1 %182, i32 5, i32 %170
  %184 = select i1 %182, double %181, double %171
  %185 = fcmp ogt double %181, %173
  %186 = select i1 %185, double %181, double %173
  %187 = select i1 %185, i32 5, i32 %174
  %188 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), double %181, i32 0)
  %189 = tail call i32 @clock() #5
  %190 = tail call i32 @rand() #5
  %191 = tail call i32 @clock() #5
  %192 = sub nsw i32 %191, %189
  %193 = sitofp i32 %192 to double
  %194 = fdiv double %193, 1.000000e+06
  %195 = fcmp olt double %194, %184
  %196 = select i1 %195, i32 6, i32 %183
  %197 = fcmp ogt double %194, %186
  %198 = select i1 %197, i32 6, i32 %187
  %199 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), double %194, i32 0)
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

; Function Attrs: norecurse nounwind readnone
define i32 @bitcount(i32) #7 {
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
define i32 @bit_count(i32) #7 {
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %.loopexit, label %.preheader.preheader, !llvm.loop !95

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %3 = phi i32 [ %7, %.preheader ], [ %0, %.preheader.preheader ]
  %4 = phi i32 [ %5, %.preheader ], [ 0, %.preheader.preheader ]
  %5 = add nuw nsw i32 %4, 1
  %6 = add nsw i32 %3, -1
  %7 = and i32 %6, %3
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !llvm.loop !95

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %9 = phi i32 [ 0, %1 ], [ %5, %.loopexit.loopexit ]
  ret i32 %9
}

; Function Attrs: nounwind readnone
define i32 @ntbl_bitcnt(i32) #8 {
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
define i32 @ntbl_bitcount(i32) #7 {
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
define i32 @BW_btbl_bitcount(i32) #7 {
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
define i32 @AR_btbl_bitcount(i32) #7 {
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
define internal i32 @bit_shifter(i32) #7 {
  %2 = icmp eq i32 %0, 0
  br i1 %2, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %1
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
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
  br i1 %12, label %.preheader, label %.loopexit.loopexit, !llvm.loop !104

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %13 = phi i32 [ 0, %1 ], [ %7, %.loopexit.loopexit ]
  ret i32 %13
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
  store i32 2011168768, i32* %17, align 4, !tbaa !112
  %18 = getelementptr inbounds i32, i32* %17, i32 1
  store i32 2011168768, i32* %18, align 4, !tbaa !112
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
  %48 = load i32, i32* %8, align 4, !tbaa !112
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5.18, i32 0, i32 0), i32 %48)
  %50 = load i32, i32* %9, align 4, !tbaa !112
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6.19, i32 0, i32 0), i32 %50)
  %52 = load i32, i32* %10, align 4, !tbaa !112
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
  store i32 %71, i32* %76, align 4, !tbaa !112
  %77 = getelementptr inbounds i32, i32* %75, i32 3
  store i32 %72, i32* %77, align 4, !tbaa !112
  %78 = getelementptr inbounds i32, i32* %75, i32 4
  store i32 %73, i32* %78, align 4, !tbaa !112
  %79 = load i32, i32* %11, align 4, !tbaa !112
  %80 = sdiv i32 %79, 16
  %81 = getelementptr inbounds i32, i32* %75, i32 6
  store i32 %80, i32* %81, align 4, !tbaa !112
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
  store i32 0, i32* %13, align 4, !tbaa !112
  %91 = call %struct._IO_FILE* @fopen(i8* nonnull %86, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0))
  %92 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %91, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %13) #5
  %93 = call i32 @fclose(%struct._IO_FILE* %91)
  %94 = load i32, i32* %13, align 4, !tbaa !112
  %95 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %94)
  %96 = load i32, i32* %13, align 4, !tbaa !112
  %97 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %98 = getelementptr inbounds i32, i32* %97, i32 %16
  %99 = getelementptr inbounds i32, i32* %98, i32 5
  store i32 %96, i32* %99, align 4, !tbaa !112
  %100 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !34
  %101 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !34
  %102 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !34
  %103 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.13.25, i32 0, i32 0), i32 %100, i32 %101, i32 %102)
  %104 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !34
  %105 = load i32*, i32** @prologPtr, align 4, !tbaa !34
  %106 = getelementptr inbounds i32, i32* %105, i32 %15
  store i32 %104, i32* %106, align 4, !tbaa !114
  %107 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !34
  %108 = load i32*, i32** @epilogPtr, align 4, !tbaa !34
  %109 = getelementptr inbounds i32, i32* %108, i32 %15
  store i32 %107, i32* %109, align 4, !tbaa !114
  %110 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !34
  %111 = load i32*, i32** @kernelPtr, align 4, !tbaa !34
  %112 = getelementptr inbounds i32, i32* %111, i32 %15
  store i32 %110, i32* %112, align 4, !tbaa !114
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
  %10 = load i32, i32* %9, align 4, !tbaa !112
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
  %17 = load i32, i32* @dynamicTCVal, align 4, !tbaa !112
  %18 = add nsw i32 %17, %10
  %19 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %20 = getelementptr inbounds i32, i32* %19, i32 %7
  %21 = getelementptr inbounds i32, i32* %20, i32 5
  store i32 %18, i32* %21, align 4, !tbaa !112
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
  %29 = load i32, i32* %28, align 4, !tbaa !112
  %30 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %29)
  %31 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %32 = getelementptr inbounds i32, i32* %31, i32 %7
  %33 = getelementptr inbounds i32, i32* %32, i32 1
  %34 = load i32, i32* %33, align 4, !tbaa !112
  %35 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %34)
  %36 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %37 = getelementptr inbounds i32, i32* %36, i32 %7
  %38 = getelementptr inbounds i32, i32* %37, i32 2
  %39 = load i32, i32* %38, align 4, !tbaa !112
  %40 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %39)
  %41 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %42 = getelementptr inbounds i32, i32* %41, i32 %7
  %43 = getelementptr inbounds i32, i32* %42, i32 3
  %44 = load i32, i32* %43, align 4, !tbaa !112
  %45 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %44)
  %46 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %47 = getelementptr inbounds i32, i32* %46, i32 %7
  %48 = getelementptr inbounds i32, i32* %47, i32 4
  %49 = load i32, i32* %48, align 4, !tbaa !112
  %50 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %49)
  %51 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %52 = getelementptr inbounds i32, i32* %51, i32 %7
  %53 = getelementptr inbounds i32, i32* %52, i32 5
  %54 = load i32, i32* %53, align 4, !tbaa !112
  %55 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %54)
  %56 = load i32*, i32** @initCGRA, align 4, !tbaa !34
  %57 = getelementptr inbounds i32, i32* %56, i32 %7
  %58 = getelementptr inbounds i32, i32* %57, i32 6
  %59 = load i32, i32* %58, align 4, !tbaa !112
  %60 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %59)
  %61 = load i32*, i32** @epilogPtr, align 4, !tbaa !34
  %62 = getelementptr inbounds i32, i32* %61, i32 %6
  %63 = load i32, i32* %62, align 4, !tbaa !114
  %64 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %63)
  %65 = load i32*, i32** @prologPtr, align 4, !tbaa !34
  %66 = getelementptr inbounds i32, i32* %65, i32 %6
  %67 = load i32, i32* %66, align 4, !tbaa !114
  %68 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %67)
  %69 = load i32*, i32** @kernelPtr, align 4, !tbaa !34
  %70 = getelementptr inbounds i32, i32* %69, i32 %6
  %71 = load i32, i32* %70, align 4, !tbaa !114
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
  %4 = load i32, i32* %3, align 4, !tbaa !112
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %8, label %6

; <label>:6:                                      ; preds = %1
  %7 = load i32, i32* @dynamicTCVal, align 4, !tbaa !112
  store i32 %7, i32* %3, align 4, !tbaa !112
  br label %8

; <label>:8:                                      ; preds = %6, %1
  ret void
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #2 {
  br label %2

; <label>:2:                                      ; preds = %11, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !112
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !112
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %.loopexit.loopexit, label %.preheader, !llvm.loop !116

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %8 = load i32, i32* @thread_exit, align 4, !tbaa !112
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #5, !srcloc !164
  ret i8* null

; <label>:11:                                     ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !112
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #5, !srcloc !165
  %12 = tail call i32 @usleep(i32 1) #5
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !112
  br label %2, !llvm.loop !166
}

declare i32 @usleep(i32) local_unnamed_addr #9

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #2 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.20, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !112
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !112
  br label %4

; <label>:4:                                      ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #5
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !112
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !168

; <label>:8:                                      ; preds = %4
  ret void
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #2 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0))
  store i32 1, i32* @thread_exit, align 4, !tbaa !112
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !112
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.24, i32 0, i32 0))
  %3 = load i32, i32* @pth, align 4, !tbaa !114
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
  %6 = load i32, i32* @totalLoops, align 4, !tbaa !112
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
  %17 = load i32, i32* @totalLoops, align 4, !tbaa !112
  %18 = icmp ugt i32 %16, %17
  br i1 %18, label %.loopexit.loopexit, label %.preheader, !llvm.loop !177

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
!11 = !DIFile(filename: "bitstrng.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
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
!35 = distinct !{!35, !36, !83, !84, !85}
!36 = !DILocation(line: 62, column: 27, scope: !37)
!37 = distinct !DILexicalBlock(scope: !39, file: !38, line: 61, column: 7)
!38 = !DIFile(filename: "bitcnts.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
!39 = distinct !DILexicalBlock(scope: !40, file: !38, line: 59, column: 5)
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
!69 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/time.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
!70 = !DIDerivedType(tag: DW_TAG_typedef, name: "__clock_t", file: !71, line: 135, baseType: !16)
!71 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
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
!83 = !{!"llvm.loop.vectorize.width", i32 1}
!84 = !{!"llvm.loop.interleave.count", i32 1}
!85 = !{!"llvm.loop.CGRA.disable"}
!86 = distinct !{!86, !87}
!87 = !DILocation(line: 67, column: 27, scope: !88)
!88 = distinct !DILexicalBlock(scope: !89, file: !38, line: 66, column: 7)
!89 = distinct !DILexicalBlock(scope: !40, file: !38, line: 65, column: 5)
!90 = distinct !{!90, !91}
!91 = !DILocation(line: 82, column: 3, scope: !43)
!92 = distinct !{!92, !36, !93, !94, !83, !84}
!93 = !{!"llvm.loop.CGRA.enable"}
!94 = !{!"llvm.loop.unroll.runtime.disable"}
!95 = distinct !{!95, !96}
!96 = !DILocation(line: 18, column: 34, scope: !97)
!97 = distinct !DILexicalBlock(scope: !99, file: !98, line: 16, column: 13)
!98 = !DIFile(filename: "bitcnt_1.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
!99 = distinct !DISubprogram(name: "bit_count", scope: !98, file: !98, line: 9, type: !57, isLocal: false, isDefinition: true, scopeLine: 10, flags: DIFlagPrototyped, isOptimized: true, unit: !100, variables: !101)
!100 = distinct !DICompileUnit(language: DW_LANG_C99, file: !98, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19)
!101 = !{!102, !103}
!102 = !DILocalVariable(name: "x", arg: 1, scope: !99, file: !98, line: 9, type: !16)
!103 = !DILocalVariable(name: "n", scope: !99, file: !98, line: 11, type: !17)
!104 = distinct !{!104, !105}
!105 = !DILocation(line: 93, column: 22, scope: !106)
!106 = distinct !DILexicalBlock(scope: !107, file: !38, line: 92, column: 3)
!107 = distinct !DISubprogram(name: "bit_shifter", scope: !38, file: !38, line: 88, type: !57, isLocal: true, isDefinition: true, scopeLine: 89, flags: DIFlagPrototyped, isOptimized: true, unit: !48, variables: !108)
!108 = !{!109, !110, !111}
!109 = !DILocalVariable(name: "x", arg: 1, scope: !107, file: !38, line: 88, type: !16)
!110 = !DILocalVariable(name: "i", scope: !107, file: !38, line: 90, type: !17)
!111 = !DILocalVariable(name: "n", scope: !107, file: !38, line: 90, type: !17)
!112 = !{!113, !113, i64 0}
!113 = !{!"int", !6, i64 0}
!114 = !{!115, !115, i64 0}
!115 = !{!"long", !6, i64 0}
!116 = distinct !{!116, !117}
!117 = !DILocation(line: 223, column: 46, scope: !118)
!118 = distinct !DILexicalBlock(scope: !120, file: !119, line: 220, column: 14)
!119 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.c", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
!120 = distinct !DISubprogram(name: "runOnCGRA", scope: !119, file: !119, line: 217, type: !121, isLocal: false, isDefinition: true, scopeLine: 218, flags: DIFlagPrototyped, isOptimized: true, unit: !123, variables: !162)
!121 = !DISubroutineType(types: !122)
!122 = !{!51, !51}
!123 = distinct !DICompileUnit(language: DW_LANG_C99, file: !119, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !19, retainedTypes: !124, globals: !128)
!124 = !{!125, !126, !16, !51, !127}
!125 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !126, size: 32)
!126 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!127 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 32)
!128 = !{!129, !132, !134, !136, !138, !140, !142, !144, !146, !148, !151, !153, !155, !157}
!129 = !DIGlobalVariableExpression(var: !130)
!130 = distinct !DIGlobalVariable(name: "totalLoops", scope: !123, file: !131, line: 27, type: !126, isLocal: false, isDefinition: true)
!131 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
!132 = !DIGlobalVariableExpression(var: !133)
!133 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !123, file: !131, line: 30, type: !126, isLocal: false, isDefinition: true)
!134 = !DIGlobalVariableExpression(var: !135)
!135 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !123, file: !131, line: 31, type: !126, isLocal: false, isDefinition: true)
!136 = !DIGlobalVariableExpression(var: !137)
!137 = distinct !DIGlobalVariable(name: "thread_exit", scope: !123, file: !131, line: 32, type: !126, isLocal: false, isDefinition: true)
!138 = !DIGlobalVariableExpression(var: !139)
!139 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !123, file: !131, line: 34, type: !17, isLocal: false, isDefinition: true)
!140 = !DIGlobalVariableExpression(var: !141)
!141 = distinct !DIGlobalVariable(name: "prolog", scope: !123, file: !131, line: 21, type: !125, isLocal: false, isDefinition: true)
!142 = !DIGlobalVariableExpression(var: !143)
!143 = distinct !DIGlobalVariable(name: "epilog", scope: !123, file: !131, line: 21, type: !125, isLocal: false, isDefinition: true)
!144 = !DIGlobalVariableExpression(var: !145)
!145 = distinct !DIGlobalVariable(name: "kernel", scope: !123, file: !131, line: 21, type: !125, isLocal: false, isDefinition: true)
!146 = !DIGlobalVariableExpression(var: !147)
!147 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !123, file: !131, line: 22, type: !125, isLocal: false, isDefinition: true)
!148 = !DIGlobalVariableExpression(var: !149)
!149 = distinct !DIGlobalVariable(name: "prologPtr", scope: !123, file: !131, line: 24, type: !150, isLocal: false, isDefinition: true)
!150 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 32)
!151 = !DIGlobalVariableExpression(var: !152)
!152 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !123, file: !131, line: 24, type: !150, isLocal: false, isDefinition: true)
!153 = !DIGlobalVariableExpression(var: !154)
!154 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !123, file: !131, line: 24, type: !150, isLocal: false, isDefinition: true)
!155 = !DIGlobalVariableExpression(var: !156)
!156 = distinct !DIGlobalVariable(name: "initCGRA", scope: !123, file: !131, line: 26, type: !127, isLocal: false, isDefinition: true)
!157 = !DIGlobalVariableExpression(var: !158)
!158 = distinct !DIGlobalVariable(name: "pth", scope: !123, file: !131, line: 29, type: !159, isLocal: false, isDefinition: true)
!159 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !160, line: 37, baseType: !161)
!160 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/shail/Results-extra/MiBench/bitcount-CCF/bitcount19")
!161 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!162 = !{!163}
!163 = !DILocalVariable(name: "arg", arg: 1, scope: !120, file: !119, line: 217, type: !51)
!164 = !{i32 6148}
!165 = !{i32 7183}
!166 = distinct !{!166, !167}
!167 = !DILocation(line: 257, column: 5, scope: !120)
!168 = distinct !{!168, !169}
!169 = !DILocation(line: 277, column: 41, scope: !170)
!170 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !119, file: !119, line: 261, type: !171, isLocal: false, isDefinition: true, scopeLine: 262, flags: DIFlagPrototyped, isOptimized: true, unit: !123, variables: !173)
!171 = !DISubroutineType(types: !172)
!172 = !{null, !126}
!173 = !{!174, !175, !176}
!174 = !DILocalVariable(name: "loopNo", arg: 1, scope: !170, file: !119, line: 261, type: !126)
!175 = !DILocalVariable(name: "result", scope: !170, file: !119, line: 263, type: !17)
!176 = !DILocalVariable(name: "configure", scope: !170, file: !119, line: 266, type: !17)
!177 = distinct !{!177, !178}
!178 = !DILocation(line: 314, column: 29, scope: !179)
!179 = distinct !DILexicalBlock(scope: !180, file: !119, line: 313, column: 3)
!180 = distinct !DISubprogram(name: "createCGRA", scope: !119, file: !119, line: 293, type: !181, isLocal: false, isDefinition: true, scopeLine: 294, isOptimized: true, unit: !123, variables: !183)
!181 = !DISubroutineType(types: !182)
!182 = !{null}
!183 = !{!184, !185}
!184 = !DILocalVariable(name: "result", scope: !180, file: !119, line: 303, type: !17)
!185 = !DILocalVariable(name: "i", scope: !180, file: !119, line: 304, type: !126)
