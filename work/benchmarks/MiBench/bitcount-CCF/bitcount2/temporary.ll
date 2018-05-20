; ModuleID = 'temp.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.bfile = type { %struct._IO_FILE*, i8, i8, i8, i8 }
%union.anon = type { i32 }
%union.pthread_attr_t = type { i32, [32 x i8] }

@.str = private unnamed_addr constant [3 x i8] c"01\00", align 1
@thread_cond_cpu = local_unnamed_addr global i32 1, align 4, !dbg !0
@thread_cond_cgra = local_unnamed_addr global i32 0, align 4, !dbg !11
@thread_exit = local_unnamed_addr global i32 0, align 4, !dbg !14
@dynamicTCVal = local_unnamed_addr global i32 0, align 4, !dbg !16
@initCGRA = common local_unnamed_addr global [7 x i32] zeroinitializer, align 4, !dbg !27
@epilog = common local_unnamed_addr global i32* null, align 4, !dbg !21
@prolog = common local_unnamed_addr global i32* null, align 4, !dbg !19
@kernel = common local_unnamed_addr global i32* null, align 4, !dbg !23
@pth = common global i32 0, align 4, !dbg !32
@ArrPtr = common local_unnamed_addr global i32* null, align 4, !dbg !25
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
@main.text = internal unnamed_addr constant [7 x i8*] [i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0)], align 4, !dbg !37
@.str.10 = private unnamed_addr constant [13 x i8] c"\0ABest  > %s\0A\00", align 1
@.str.11 = private unnamed_addr constant [12 x i8] c"Worst > %s\0A\00", align 1
@bits = internal unnamed_addr constant [256 x i8] c"\00\01\01\02\01\02\02\03\01\02\02\03\02\03\03\04\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\01\02\02\03\02\03\03\04\02\03\03\04\03\04\04\05\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\02\03\03\04\03\04\04\05\03\04\04\05\04\05\05\06\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\03\04\04\05\04\05\05\06\04\05\05\06\05\06\06\07\04\05\05\06\05\06\06\07\05\06\06\07\06\07\07\08", align 1, !dbg !76
@initializeParameters.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
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
@initializeParameters.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.13.25 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.14 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.15 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.16 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@str = private unnamed_addr constant [44 x i8] c"1:CGRA is waiting for signal from processor\00"
@str.26 = private unnamed_addr constant [43 x i8] c"HELLO FROM CGRA_MAIN THREAD THE ARRAY IS :\00"
@str.27 = private unnamed_addr constant [44 x i8] c"BYEEEE FROM CGRA_MAIN THREAD THE ARRAY IS :\00"
@str.28 = private unnamed_addr constant [34 x i8] c"Core Waiting For Signal From CGRA\00"
@str.29 = private unnamed_addr constant [26 x i8] c"Signal From CGRA Received\00"
@str.30 = private unnamed_addr constant [14 x i8] c"deleting cgra\00"
@str.31 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00"
@str.32 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00"

; Function Attrs: nounwind readonly
define i32 @bstr_i(i8* readonly) local_unnamed_addr #0 !dbg !110 {
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !114, metadata !117), !dbg !118
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !116, metadata !117), !dbg !119
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !116, metadata !117), !dbg !119
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !114, metadata !117), !dbg !118
  %2 = icmp eq i8* %0, null, !dbg !120
  br i1 %2, label %.loopexit, label %3, !dbg !121

; <label>:3:                                      ; preds = %1
  %4 = load i8, i8* %0, align 1, !dbg !122, !tbaa !123
  %5 = icmp eq i8 %4, 0, !dbg !122
  br i1 %5, label %.loopexit, label %.preheader.preheader, !dbg !126

.preheader.preheader:                             ; preds = %3
  br label %.preheader, !dbg !122

.preheader:                                       ; preds = %12, %.preheader.preheader
  %6 = phi i8 [ %18, %12 ], [ %4, %.preheader.preheader ]
  %7 = phi i8* [ %13, %12 ], [ %0, %.preheader.preheader ]
  %8 = phi i32 [ %17, %12 ], [ 0, %.preheader.preheader ]
  %9 = zext i8 %6 to i32, !dbg !122
  %10 = tail call i8* @memchr(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %9, i32 3), !dbg !127
  %11 = icmp eq i8* %10, null, !dbg !126
  br i1 %11, label %.loopexit.loopexit, label %12, !dbg !128

; <label>:12:                                     ; preds = %.preheader
  %13 = getelementptr inbounds i8, i8* %7, i32 1, !dbg !129
  tail call void @llvm.dbg.value(metadata i8* %13, i64 0, metadata !114, metadata !117), !dbg !118
  %14 = shl i32 %8, 1, !dbg !131
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !116, metadata !117), !dbg !119
  %15 = and i8 %6, 1, !dbg !132
  %16 = zext i8 %15 to i32, !dbg !132
  %17 = or i32 %14, %16, !dbg !133
  tail call void @llvm.dbg.value(metadata i32 %17, i64 0, metadata !116, metadata !117), !dbg !119
  tail call void @llvm.dbg.value(metadata i32 %17, i64 0, metadata !116, metadata !117), !dbg !119
  tail call void @llvm.dbg.value(metadata i8* %13, i64 0, metadata !114, metadata !117), !dbg !118
  %18 = load i8, i8* %13, align 1, !dbg !122, !tbaa !123
  %19 = icmp eq i8 %18, 0, !dbg !122
  br i1 %19, label %.loopexit.loopexit, label %.preheader, !dbg !126

.loopexit.loopexit:                               ; preds = %12, %.preheader
  %.ph = phi i32 [ %8, %.preheader ], [ %17, %12 ]
  br label %.loopexit, !dbg !134

.loopexit:                                        ; preds = %.loopexit.loopexit, %3, %1
  %20 = phi i32 [ 0, %1 ], [ 0, %3 ], [ %.ph, %.loopexit.loopexit ]
  ret i32 %20, !dbg !134
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata, metadata) #1

; Function Attrs: nounwind readonly
declare i8* @memchr(i8*, i32, i32) local_unnamed_addr #2

; Function Attrs: nounwind
define void @bitstring(i8* nocapture, i32, i32, i32) local_unnamed_addr #3 !dbg !135 {
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !139, metadata !117), !dbg !145
  tail call void @llvm.dbg.value(metadata i32 %1, i64 0, metadata !140, metadata !117), !dbg !146
  tail call void @llvm.dbg.value(metadata i32 %2, i64 0, metadata !141, metadata !117), !dbg !147
  tail call void @llvm.dbg.value(metadata i32 %3, i64 0, metadata !142, metadata !117), !dbg !148
  %5 = ashr i32 %2, 2, !dbg !149
  %6 = and i32 %2, 3, !dbg !150
  %7 = icmp eq i32 %6, 0, !dbg !150
  %8 = zext i1 %7 to i32, !dbg !150
  %sum = add i32 %5, %2, !dbg !151
  %9 = sub i32 %3, %sum, !dbg !152
  %10 = add i32 %9, %8, !dbg !152
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !144, metadata !117), !dbg !153
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !143, metadata !117), !dbg !154
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !139, metadata !117), !dbg !145
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !143, metadata !117), !dbg !154
  %11 = icmp sgt i32 %10, 0, !dbg !155
  br i1 %11, label %12, label %14, !dbg !158

; <label>:12:                                     ; preds = %4
  tail call void @llvm.memset.p0i8.i32(i8* %0, i8 32, i32 %10, i32 1, i1 false), !dbg !159
  %13 = getelementptr i8, i8* %0, i32 %10, !dbg !160
  br label %14, !dbg !161

; <label>:14:                                     ; preds = %12, %4
  %15 = phi i8* [ %0, %4 ], [ %13, %12 ]
  tail call void @llvm.dbg.value(metadata i8* %15, i64 0, metadata !139, metadata !117), !dbg !145
  tail call void @llvm.dbg.value(metadata i32 %2, i64 0, metadata !141, metadata !117), !dbg !147
  %16 = icmp sgt i32 %2, 0, !dbg !161
  br i1 %16, label %.preheader.preheader, label %.loopexit, !dbg !162

.preheader.preheader:                             ; preds = %14
  br label %.preheader, !dbg !163

.preheader:                                       ; preds = %29, %.preheader.preheader
  %17 = phi i32 [ %19, %29 ], [ %2, %.preheader.preheader ]
  %18 = phi i8* [ %30, %29 ], [ %15, %.preheader.preheader ]
  %19 = add nsw i32 %17, -1, !dbg !163
  %20 = lshr i32 %1, %19, !dbg !164
  %21 = and i32 %20, 1, !dbg !166
  %22 = or i32 %21, 48, !dbg !167
  %23 = trunc i32 %22 to i8, !dbg !168
  %24 = getelementptr inbounds i8, i8* %18, i32 1, !dbg !169
  tail call void @llvm.dbg.value(metadata i8* %24, i64 0, metadata !139, metadata !117), !dbg !145
  store i8 %23, i8* %18, align 1, !dbg !170, !tbaa !123
  %25 = and i32 %19, 3, !dbg !171
  %26 = icmp eq i32 %25, 0, !dbg !171
  %27 = icmp ne i32 %19, 0, !dbg !173
  %28 = and i1 %27, %26, !dbg !174
  br i1 %28, label %32, label %29, !dbg !174

; <label>:29:                                     ; preds = %32, %.preheader
  %30 = phi i8* [ %33, %32 ], [ %24, %.preheader ]
  tail call void @llvm.dbg.value(metadata i8* %30, i64 0, metadata !139, metadata !117), !dbg !145
  tail call void @llvm.dbg.value(metadata i32 %19, i64 0, metadata !141, metadata !117), !dbg !147
  %31 = icmp sgt i32 %17, 1, !dbg !161
  br i1 %31, label %.preheader, label %.loopexit.loopexit, !dbg !162, !llvm.loop !175

; <label>:32:                                     ; preds = %.preheader
  %33 = getelementptr inbounds i8, i8* %18, i32 2, !dbg !177
  tail call void @llvm.dbg.value(metadata i8* %33, i64 0, metadata !139, metadata !117), !dbg !145
  store i8 32, i8* %24, align 1, !dbg !178, !tbaa !123
  br label %29, !dbg !179

.loopexit.loopexit:                               ; preds = %29
  br label %.loopexit, !dbg !180

.loopexit:                                        ; preds = %.loopexit.loopexit, %14
  %34 = phi i8* [ %15, %14 ], [ %30, %.loopexit.loopexit ]
  store i8 0, i8* %34, align 1, !dbg !180, !tbaa !123
  ret void, !dbg !181
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i32, i1) #4

; Function Attrs: nounwind
define noalias %struct.bfile* @bfopen(i8* nocapture readonly, i8* nocapture readonly) local_unnamed_addr #3 !dbg !182 {
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !254, metadata !117), !dbg !257
  tail call void @llvm.dbg.value(metadata i8* %1, i64 0, metadata !255, metadata !117), !dbg !258
  %3 = tail call noalias i8* @malloc(i32 8) #6, !dbg !259
  %4 = bitcast i8* %3 to %struct.bfile*, !dbg !259
  tail call void @llvm.dbg.value(metadata %struct.bfile* %4, i64 0, metadata !256, metadata !117), !dbg !260
  %5 = icmp eq i8* %3, null, !dbg !261
  br i1 %5, label %14, label %6, !dbg !263

; <label>:6:                                      ; preds = %2
  %7 = tail call %struct._IO_FILE* @fopen(i8* %0, i8* %1), !dbg !264
  %8 = bitcast i8* %3 to %struct._IO_FILE**, !dbg !265
  store %struct._IO_FILE* %7, %struct._IO_FILE** %8, align 4, !dbg !266, !tbaa !267
  %9 = icmp eq %struct._IO_FILE* %7, null, !dbg !270
  br i1 %9, label %10, label %11, !dbg !272

; <label>:10:                                     ; preds = %6
  tail call void @free(i8* nonnull %3) #6, !dbg !273
  br label %14, !dbg !275

; <label>:11:                                     ; preds = %6
  %12 = getelementptr inbounds i8, i8* %3, i32 5, !dbg !276
  store i8 0, i8* %12, align 1, !dbg !277, !tbaa !278
  %13 = getelementptr inbounds i8, i8* %3, i32 7, !dbg !279
  store i8 0, i8* %13, align 1, !dbg !280, !tbaa !281
  br label %14, !dbg !282

; <label>:14:                                     ; preds = %11, %10, %2
  %15 = phi %struct.bfile* [ null, %10 ], [ %4, %11 ], [ null, %2 ]
  ret %struct.bfile* %15, !dbg !283
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i32) local_unnamed_addr #5

; Function Attrs: nounwind
declare noalias %struct._IO_FILE* @fopen(i8* nocapture readonly, i8* nocapture readonly) local_unnamed_addr #5

; Function Attrs: nounwind
declare void @free(i8* nocapture) local_unnamed_addr #5

; Function Attrs: nounwind
define i32 @bfread(%struct.bfile* nocapture) local_unnamed_addr #3 !dbg !284 {
  tail call void @llvm.dbg.value(metadata %struct.bfile* %0, i64 0, metadata !288, metadata !117), !dbg !289
  %2 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 2, !dbg !290
  %3 = load i8, i8* %2, align 1, !dbg !290, !tbaa !278
  %4 = icmp eq i8 %3, 0, !dbg !292
  br i1 %4, label %9, label %5, !dbg !293

; <label>:5:                                      ; preds = %1
  %6 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 1
  %7 = load i8, i8* %6, align 4, !tbaa !294
  %8 = add i8 %3, -1, !dbg !293
  br label %15, !dbg !293

; <label>:9:                                      ; preds = %1
  %10 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 0, !dbg !295
  %11 = load %struct._IO_FILE*, %struct._IO_FILE** %10, align 4, !dbg !295, !tbaa !267
  %12 = tail call i32 @fgetc(%struct._IO_FILE* %11), !dbg !297
  %13 = trunc i32 %12 to i8, !dbg !298
  %14 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 1, !dbg !299
  store i8 %13, i8* %14, align 4, !dbg !300, !tbaa !294
  store i8 8, i8* %2, align 1, !dbg !301, !tbaa !278
  br label %15, !dbg !302

; <label>:15:                                     ; preds = %9, %5
  %16 = phi i8 [ %13, %9 ], [ %7, %5 ], !dbg !303
  %17 = phi i8 [ 7, %9 ], [ %8, %5 ]
  store i8 %17, i8* %2, align 1, !dbg !304, !tbaa !278
  %18 = zext i8 %16 to i32, !dbg !305
  %19 = zext i8 %17 to i32, !dbg !306
  %20 = shl i32 1, %19, !dbg !307
  %21 = and i32 %20, %18, !dbg !308
  %22 = icmp ne i32 %21, 0, !dbg !309
  %23 = zext i1 %22 to i32, !dbg !309
  ret i32 %23, !dbg !310
}

; Function Attrs: nounwind
declare i32 @fgetc(%struct._IO_FILE* nocapture) local_unnamed_addr #5

; Function Attrs: nounwind
define void @bfwrite(i32, %struct.bfile* nocapture) local_unnamed_addr #3 !dbg !311 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !315, metadata !117), !dbg !317
  tail call void @llvm.dbg.value(metadata %struct.bfile* %1, i64 0, metadata !316, metadata !117), !dbg !318
  %3 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 4, !dbg !319
  %4 = load i8, i8* %3, align 1, !dbg !319, !tbaa !281
  %5 = icmp eq i8 %4, 8, !dbg !321
  %6 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 3
  br i1 %5, label %9, label %7, !dbg !322

; <label>:7:                                      ; preds = %2
  %8 = add i8 %4, 1, !dbg !322
  br label %15, !dbg !322

; <label>:9:                                      ; preds = %2
  %10 = load i8, i8* %6, align 2, !dbg !323, !tbaa !325
  %11 = zext i8 %10 to i32, !dbg !326
  %12 = getelementptr inbounds %struct.bfile, %struct.bfile* %1, i32 0, i32 0, !dbg !327
  %13 = load %struct._IO_FILE*, %struct._IO_FILE** %12, align 4, !dbg !327, !tbaa !267
  %14 = tail call i32 @fputc(i32 %11, %struct._IO_FILE* %13), !dbg !328
  store i8 0, i8* %3, align 1, !dbg !329, !tbaa !281
  br label %15, !dbg !330

; <label>:15:                                     ; preds = %9, %7
  %16 = phi i8 [ %8, %7 ], [ 1, %9 ]
  store i8 %16, i8* %3, align 1, !dbg !331, !tbaa !281
  %17 = load i8, i8* %6, align 2, !dbg !332, !tbaa !325
  %18 = zext i8 %17 to i32, !dbg !332
  %19 = shl nuw nsw i32 %18, 1, !dbg !332
  %20 = and i32 %0, 1, !dbg !333
  %21 = or i32 %19, %20, !dbg !334
  %22 = trunc i32 %21 to i8, !dbg !334
  store i8 %22, i8* %6, align 2, !dbg !334, !tbaa !325
  ret void, !dbg !335
}

; Function Attrs: nounwind
declare i32 @fputc(i32, %struct._IO_FILE* nocapture) local_unnamed_addr #5

; Function Attrs: nounwind
define void @bfclose(%struct.bfile* nocapture) local_unnamed_addr #3 !dbg !336 {
  tail call void @llvm.dbg.value(metadata %struct.bfile* %0, i64 0, metadata !340, metadata !117), !dbg !341
  %2 = getelementptr inbounds %struct.bfile, %struct.bfile* %0, i32 0, i32 0, !dbg !342
  %3 = load %struct._IO_FILE*, %struct._IO_FILE** %2, align 4, !dbg !342, !tbaa !267
  %4 = tail call i32 @fclose(%struct._IO_FILE* %3), !dbg !343
  %5 = bitcast %struct.bfile* %0 to i8*, !dbg !344
  tail call void @free(i8* %5) #6, !dbg !345
  ret void, !dbg !346
}

; Function Attrs: nounwind
declare i32 @fclose(%struct._IO_FILE* nocapture) local_unnamed_addr #5

; Function Attrs: nounwind
define i32 @main(i32, i8** nocapture readonly) local_unnamed_addr #3 !dbg !39 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !57, metadata !117), !dbg !347
  tail call void @llvm.dbg.value(metadata i8** %1, i64 0, metadata !58, metadata !117), !dbg !348
  tail call void @llvm.dbg.value(metadata double 0x7FEFFFFFFFFFFFFF, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata double 0.000000e+00, i64 0, metadata !67, metadata !117), !dbg !350
  %3 = icmp slt i32 %0, 2, !dbg !351
  br i1 %3, label %4, label %7, !dbg !353

; <label>:4:                                      ; preds = %2
  %5 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 4, !dbg !354, !tbaa !356
  %6 = tail call i32 @fwrite(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.7, i32 0, i32 0), i32 28, i32 1, %struct._IO_FILE* %5) #10, !dbg !357
  tail call void @exit(i32 -1) #11, !dbg !358
  unreachable, !dbg !358

; <label>:7:                                      ; preds = %2
  %8 = getelementptr inbounds i8*, i8** %1, i32 1, !dbg !359
  %9 = load i8*, i8** %8, align 4, !dbg !359, !tbaa !356
  tail call void @llvm.dbg.value(metadata i8* %9, i64 0, metadata !360, metadata !117) #6, !dbg !368
  %10 = tail call i32 @strtol(i8* nocapture nonnull %9, i8** null, i32 10) #6, !dbg !370
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !74, metadata !117), !dbg !371
  %11 = tail call i32 @puts(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.8, i32 0, i32 0)), !dbg !372
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double 0x7FEFFFFFFFFFFFFF, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 undef, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 undef, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double 0.000000e+00, i64 0, metadata !67, metadata !117), !dbg !350
  %12 = icmp sgt i32 %10, 0
  %13 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %14 = tail call i32 @rand() #6, !dbg !383
  br i1 %12, label %.preheader1.preheader, label %15, !dbg !385

.preheader1.preheader:                            ; preds = %7
  br label %.preheader1, !dbg !386

; <label>:15:                                     ; preds = %7
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %16 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !64, metadata !117), !dbg !395
  %17 = sub nsw i32 %16, %13, !dbg !396
  %18 = sitofp i32 %17 to double, !dbg !397
  %19 = fdiv double %18, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !65, metadata !117), !dbg !399
  %20 = fcmp olt double %19, 0x7FEFFFFFFFFFFFFF, !dbg !400
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %21 = select i1 %20, double %19, double 0x7FEFFFFFFFFFFFFF, !dbg !402
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %22 = fcmp ogt double %19, 0.000000e+00, !dbg !403
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %23 = select i1 %22, double %19, double 0.000000e+00, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %24 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), double %19, i32 0), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %25 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %26 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %27 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !64, metadata !117), !dbg !395
  %28 = sub nsw i32 %27, %25, !dbg !396
  %29 = sitofp i32 %28 to double, !dbg !397
  %30 = fdiv double %29, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !65, metadata !117), !dbg !399
  %31 = fcmp olt double %30, %21, !dbg !400
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %32 = zext i1 %31 to i32, !dbg !402
  %33 = select i1 %31, double %30, double %21, !dbg !402
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %34 = fcmp ogt double %30, %23, !dbg !403
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %35 = select i1 %34, double %30, double %23, !dbg !405
  %36 = zext i1 %34 to i32, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %37 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), double %30, i32 0), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %38 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %39 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %40 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !64, metadata !117), !dbg !395
  %41 = sub nsw i32 %40, %38, !dbg !396
  %42 = sitofp i32 %41 to double, !dbg !397
  %43 = fdiv double %42, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !65, metadata !117), !dbg !399
  %44 = fcmp olt double %43, %33, !dbg !400
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %45 = select i1 %44, i32 2, i32 %32, !dbg !402
  %46 = select i1 %44, double %43, double %33, !dbg !402
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %47 = fcmp ogt double %43, %35, !dbg !403
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %48 = select i1 %47, double %43, double %35, !dbg !405
  %49 = select i1 %47, i32 2, i32 %36, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %50 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), double %43, i32 0), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %51 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %52 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %53 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !64, metadata !117), !dbg !395
  %54 = sub nsw i32 %53, %51, !dbg !396
  %55 = sitofp i32 %54 to double, !dbg !397
  %56 = fdiv double %55, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !65, metadata !117), !dbg !399
  %57 = fcmp olt double %56, %46, !dbg !400
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %58 = select i1 %57, i32 3, i32 %45, !dbg !402
  %59 = select i1 %57, double %56, double %46, !dbg !402
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %60 = fcmp ogt double %56, %48, !dbg !403
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %61 = select i1 %60, double %56, double %48, !dbg !405
  %62 = select i1 %60, i32 3, i32 %49, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %63 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), double %56, i32 0), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %64 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %65 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %66 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !64, metadata !117), !dbg !395
  %67 = sub nsw i32 %66, %64, !dbg !396
  %68 = sitofp i32 %67 to double, !dbg !397
  %69 = fdiv double %68, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !65, metadata !117), !dbg !399
  %70 = fcmp olt double %69, %59, !dbg !400
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %71 = select i1 %70, i32 4, i32 %58, !dbg !402
  %72 = select i1 %70, double %69, double %59, !dbg !402
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %73 = fcmp ogt double %69, %61, !dbg !403
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %74 = select i1 %73, double %69, double %61, !dbg !405
  %75 = select i1 %73, i32 4, i32 %62, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %76 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), double %69, i32 0), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %77 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %78 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %79 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !64, metadata !117), !dbg !395
  %80 = sub nsw i32 %79, %77, !dbg !396
  %81 = sitofp i32 %80 to double, !dbg !397
  %82 = fdiv double %81, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !65, metadata !117), !dbg !399
  %83 = fcmp olt double %82, %72, !dbg !400
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %84 = select i1 %83, i32 5, i32 %71, !dbg !402
  %85 = select i1 %83, double %82, double %72, !dbg !402
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %86 = fcmp ogt double %82, %74, !dbg !403
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %87 = select i1 %86, double %82, double %74, !dbg !405
  %88 = select i1 %86, i32 5, i32 %75, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %89 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), double %82, i32 0), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %90 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %91 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %92 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %16, i64 0, metadata !64, metadata !117), !dbg !395
  %93 = sub nsw i32 %92, %90, !dbg !396
  %94 = sitofp i32 %93 to double, !dbg !397
  %95 = fdiv double %94, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !65, metadata !117), !dbg !399
  %96 = fcmp olt double %95, %85, !dbg !400
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %97 = select i1 %96, i32 6, i32 %84, !dbg !402
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %98 = fcmp ogt double %95, %87, !dbg !403
  tail call void @llvm.dbg.value(metadata double %19, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %99 = select i1 %98, i32 6, i32 %88, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  %100 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), double %95, i32 0), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %21, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %23, i64 0, metadata !67, metadata !117), !dbg !350
  br label %150, !dbg !407

.preheader1:                                      ; preds = %bit_count.exit, %.preheader1.preheader
  %101 = phi i32 [ %114, %bit_count.exit ], [ %14, %.preheader1.preheader ]
  %102 = phi i32 [ %112, %bit_count.exit ], [ 0, %.preheader1.preheader ]
  %103 = phi i32 [ %113, %bit_count.exit ], [ 0, %.preheader1.preheader ]
  tail call void @llvm.dbg.value(metadata i32 %101, i64 0, metadata !390, metadata !117), !dbg !408
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !391, metadata !117), !dbg !409
  %104 = icmp eq i32 %101, 0, !dbg !386
  br i1 %104, label %bit_count.exit, label %.preheader.i.preheader, !dbg !410, !llvm.loop !411

.preheader.i.preheader:                           ; preds = %.preheader1
  br label %.preheader.i, !dbg !414

.preheader.i:                                     ; preds = %.preheader.i, %.preheader.i.preheader
  %105 = phi i32 [ %109, %.preheader.i ], [ %101, %.preheader.i.preheader ]
  %106 = phi i32 [ %107, %.preheader.i ], [ 0, %.preheader.i.preheader ]
  tail call void @llvm.dbg.value(metadata i32 %106, i64 0, metadata !391, metadata !117), !dbg !409
  tail call void @llvm.dbg.value(metadata i32 %105, i64 0, metadata !390, metadata !117), !dbg !408
  %107 = add nuw nsw i32 %106, 1, !dbg !414
  tail call void @llvm.dbg.value(metadata i32 %107, i64 0, metadata !391, metadata !117), !dbg !409
  %108 = add nsw i32 %105, -1, !dbg !415
  %109 = and i32 %108, %105, !dbg !416
  tail call void @llvm.dbg.value(metadata i32 %109, i64 0, metadata !390, metadata !117), !dbg !408
  %110 = icmp eq i32 %109, 0, !dbg !417
  br i1 %110, label %bit_count.exit.loopexit, label %.preheader.i, !dbg !418, !llvm.loop !411

bit_count.exit.loopexit:                          ; preds = %.preheader.i
  br label %bit_count.exit, !dbg !419

bit_count.exit:                                   ; preds = %bit_count.exit.loopexit, %.preheader1
  %111 = phi i32 [ 0, %.preheader1 ], [ %107, %bit_count.exit.loopexit ]
  tail call void @llvm.dbg.value(metadata i32 %111, i64 0, metadata !391, metadata !117), !dbg !409
  %112 = add nsw i32 %111, %102, !dbg !419
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  %113 = add nuw nsw i32 %103, 1, !dbg !420
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %114 = add nsw i32 %101, 13, !dbg !421
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %115 = icmp eq i32 %113, %10, !dbg !423
  br i1 %115, label %116, label %.preheader1, !dbg !424, !llvm.loop !425

; <label>:116:                                    ; preds = %bit_count.exit
  %117 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %117, i64 0, metadata !64, metadata !117), !dbg !395
  %118 = sub nsw i32 %117, %13, !dbg !396
  %119 = sitofp i32 %118 to double, !dbg !397
  %120 = fdiv double %119, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !65, metadata !117), !dbg !399
  %121 = fcmp olt double %120, 0x7FEFFFFFFFFFFFFF, !dbg !400
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %122 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i32 0, i32 0), double %120, i32 %112), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %123 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %124 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %min.iters.check = icmp ult i32 %10, 4, !dbg !424
  br i1 %min.iters.check, label %scalar.ph.preheader, label %min.iters.checked, !dbg !424

scalar.ph.preheader:                              ; preds = %middle.block, %min.iters.checked, %116
  %.ph = phi i32 [ %124, %min.iters.checked ], [ %124, %116 ], [ %ind.end, %middle.block ]
  %.ph39 = phi i32 [ 0, %min.iters.checked ], [ 0, %116 ], [ %149, %middle.block ]
  %.ph40 = phi i32 [ 0, %min.iters.checked ], [ 0, %116 ], [ %n.vec, %middle.block ]
  br label %scalar.ph, !dbg !427

min.iters.checked:                                ; preds = %116
  %n.mod.vf = and i32 %10, 3, !dbg !424
  %n.vec = sub i32 %10, %n.mod.vf, !dbg !424
  %cmp.zero = icmp eq i32 %n.vec, 0, !dbg !424
  %125 = mul i32 %10, 13, !dbg !424
  %126 = add i32 %124, %125, !dbg !424
  %127 = mul nsw i32 %n.mod.vf, -13, !dbg !424
  %ind.end = add i32 %126, %127, !dbg !424
  br i1 %cmp.zero, label %scalar.ph.preheader, label %vector.ph, !dbg !424

vector.ph:                                        ; preds = %min.iters.checked
  %.splatinsert = insertelement <4 x i32> undef, i32 %124, i32 0, !dbg !424
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> undef, <4 x i32> zeroinitializer, !dbg !424
  %induction = add <4 x i32> %.splat, <i32 0, i32 13, i32 26, i32 39>, !dbg !424
  br label %vector.body, !dbg !424

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i32 [ 0, %vector.ph ], [ %index.next, %vector.body ], !dbg !420
  %vec.ind = phi <4 x i32> [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi <4 x i32> [ zeroinitializer, %vector.ph ], [ %147, %vector.body ]
  %128 = lshr <4 x i32> %vec.ind, <i32 1, i32 1, i32 1, i32 1>, !dbg !427
  %129 = and <4 x i32> %128, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>, !dbg !427
  %130 = and <4 x i32> %vec.ind, <i32 1431655765, i32 1431655765, i32 1431655765, i32 1431655765>, !dbg !432
  %131 = add nuw <4 x i32> %129, %130, !dbg !433
  %132 = lshr <4 x i32> %131, <i32 2, i32 2, i32 2, i32 2>, !dbg !434
  %133 = and <4 x i32> %132, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>, !dbg !434
  %134 = and <4 x i32> %131, <i32 858993459, i32 858993459, i32 858993459, i32 858993459>, !dbg !435
  %135 = add nuw nsw <4 x i32> %133, %134, !dbg !436
  %136 = lshr <4 x i32> %135, <i32 4, i32 4, i32 4, i32 4>, !dbg !437
  %137 = and <4 x i32> %136, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>, !dbg !437
  %138 = and <4 x i32> %135, <i32 117901063, i32 117901063, i32 117901063, i32 117901063>, !dbg !438
  %139 = add nuw nsw <4 x i32> %137, %138, !dbg !439
  %140 = lshr <4 x i32> %139, <i32 8, i32 8, i32 8, i32 8>, !dbg !440
  %141 = and <4 x i32> %140, <i32 983055, i32 983055, i32 983055, i32 983055>, !dbg !440
  %142 = and <4 x i32> %139, <i32 983055, i32 983055, i32 983055, i32 983055>, !dbg !441
  %143 = add nuw nsw <4 x i32> %141, %142, !dbg !442
  %144 = lshr <4 x i32> %143, <i32 16, i32 16, i32 16, i32 16>, !dbg !443
  %145 = and <4 x i32> %143, <i32 31, i32 31, i32 31, i32 31>, !dbg !444
  %146 = add <4 x i32> %145, %vec.phi, !dbg !445
  %147 = add <4 x i32> %146, %144, !dbg !419
  %index.next = add i32 %index, 4, !dbg !420
  %vec.ind.next = add <4 x i32> %vec.ind, <i32 52, i32 52, i32 52, i32 52>, !dbg !420
  %148 = icmp eq i32 %index.next, %n.vec, !dbg !420
  br i1 %148, label %middle.block, label %vector.body, !dbg !420, !llvm.loop !446

middle.block:                                     ; preds = %vector.body
  %rdx.shuf = shufflevector <4 x i32> %147, <4 x i32> undef, <4 x i32> <i32 2, i32 3, i32 undef, i32 undef>, !dbg !419
  %bin.rdx = add <4 x i32> %147, %rdx.shuf, !dbg !419
  %rdx.shuf36 = shufflevector <4 x i32> %bin.rdx, <4 x i32> undef, <4 x i32> <i32 1, i32 undef, i32 undef, i32 undef>, !dbg !419
  %bin.rdx37 = add <4 x i32> %bin.rdx, %rdx.shuf36, !dbg !419
  %149 = extractelement <4 x i32> %bin.rdx37, i32 0, !dbg !419
  %cmp.n = icmp eq i32 %n.mod.vf, 0
  br i1 %cmp.n, label %.loopexit38, label %scalar.ph.preheader, !dbg !424

; <label>:150:                                    ; preds = %392, %15
  %151 = phi i32 [ %403, %392 ], [ %97, %15 ]
  %152 = phi i32 [ %405, %392 ], [ %99, %15 ]
  %153 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %151, !dbg !407
  %154 = load i8*, i8** %153, align 4, !dbg !407, !tbaa !356
  %155 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.10, i32 0, i32 0), i8* %154), !dbg !449
  %156 = getelementptr inbounds [7 x i8*], [7 x i8*]* @main.text, i32 0, i32 %152, !dbg !450
  %157 = load i8*, i8** %156, align 4, !dbg !450, !tbaa !356
  %158 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.11, i32 0, i32 0), i8* %157), !dbg !451
  ret i32 0, !dbg !452

scalar.ph:                                        ; preds = %scalar.ph, %scalar.ph.preheader
  %159 = phi i32 [ %183, %scalar.ph ], [ %.ph, %scalar.ph.preheader ]
  %160 = phi i32 [ %181, %scalar.ph ], [ %.ph39, %scalar.ph.preheader ]
  %161 = phi i32 [ %182, %scalar.ph ], [ %.ph40, %scalar.ph.preheader ]
  tail call void @llvm.dbg.value(metadata i32 %159, i64 0, metadata !430, metadata !117), !dbg !453
  %162 = lshr i32 %159, 1, !dbg !427
  %163 = and i32 %162, 1431655765, !dbg !427
  %164 = and i32 %159, 1431655765, !dbg !432
  %165 = add nuw i32 %163, %164, !dbg !433
  tail call void @llvm.dbg.value(metadata i32 %165, i64 0, metadata !430, metadata !117), !dbg !453
  %166 = lshr i32 %165, 2, !dbg !434
  %167 = and i32 %166, 858993459, !dbg !434
  %168 = and i32 %165, 858993459, !dbg !435
  %169 = add nuw nsw i32 %167, %168, !dbg !436
  tail call void @llvm.dbg.value(metadata i32 %169, i64 0, metadata !430, metadata !117), !dbg !453
  %170 = lshr i32 %169, 4, !dbg !437
  %171 = and i32 %170, 117901063, !dbg !437
  %172 = and i32 %169, 117901063, !dbg !438
  %173 = add nuw nsw i32 %171, %172, !dbg !439
  tail call void @llvm.dbg.value(metadata i32 %173, i64 0, metadata !430, metadata !117), !dbg !453
  %174 = lshr i32 %173, 8, !dbg !440
  %175 = and i32 %174, 983055, !dbg !440
  %176 = and i32 %173, 983055, !dbg !441
  %177 = add nuw nsw i32 %175, %176, !dbg !442
  tail call void @llvm.dbg.value(metadata i32 %177, i64 0, metadata !430, metadata !117), !dbg !453
  %178 = lshr i32 %177, 16, !dbg !443
  %179 = and i32 %177, 31, !dbg !444
  tail call void @llvm.dbg.value(metadata i32 %180, i64 0, metadata !430, metadata !117), !dbg !453
  %180 = add i32 %179, %160, !dbg !445
  %181 = add i32 %180, %178, !dbg !419
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  %182 = add nuw nsw i32 %161, 1, !dbg !420
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %183 = add nsw i32 %159, 13, !dbg !421
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %184 = icmp eq i32 %182, %10, !dbg !423
  br i1 %184, label %.loopexit38.loopexit, label %scalar.ph, !dbg !424, !llvm.loop !454

.loopexit38.loopexit:                             ; preds = %scalar.ph
  br label %.loopexit38, !dbg !403

.loopexit38:                                      ; preds = %.loopexit38.loopexit, %middle.block
  %.lcssa31 = phi i32 [ %149, %middle.block ], [ %181, %.loopexit38.loopexit ]
  %185 = fcmp ogt double %120, 0.000000e+00, !dbg !403
  %186 = select i1 %121, double %120, double 0x7FEFFFFFFFFFFFFF, !dbg !402
  %187 = select i1 %185, double %120, double 0.000000e+00, !dbg !405
  %188 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %117, i64 0, metadata !64, metadata !117), !dbg !395
  %189 = sub nsw i32 %188, %123, !dbg !396
  %190 = sitofp i32 %189 to double, !dbg !397
  %191 = fdiv double %190, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !65, metadata !117), !dbg !399
  %192 = fcmp olt double %191, %186, !dbg !400
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %193 = fcmp ogt double %191, %187, !dbg !403
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %194 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.1.2, i32 0, i32 0), double %191, i32 %.lcssa31), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %195 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %196 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  br label %197, !dbg !424

; <label>:197:                                    ; preds = %197, %.loopexit38
  %198 = phi i32 [ %196, %.loopexit38 ], [ %204, %197 ]
  %199 = phi i32 [ 0, %.loopexit38 ], [ %202, %197 ]
  %200 = phi i32 [ 0, %.loopexit38 ], [ %203, %197 ]
  %201 = tail call i32 @ntbl_bitcnt(i32 %198) #6, !dbg !456
  %202 = add nsw i32 %201, %199, !dbg !419
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  %203 = add nuw nsw i32 %200, 1, !dbg !420
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %204 = add nsw i32 %198, 13, !dbg !421
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %205 = icmp eq i32 %203, %10, !dbg !423
  br i1 %205, label %206, label %197, !dbg !424, !llvm.loop !425

; <label>:206:                                    ; preds = %197
  %207 = zext i1 %192 to i32, !dbg !402
  %208 = select i1 %193, double %191, double %187, !dbg !405
  %209 = select i1 %192, double %191, double %186, !dbg !402
  %210 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %117, i64 0, metadata !64, metadata !117), !dbg !395
  %211 = sub nsw i32 %210, %195, !dbg !396
  %212 = sitofp i32 %211 to double, !dbg !397
  %213 = fdiv double %212, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !65, metadata !117), !dbg !399
  %214 = fcmp olt double %213, %209, !dbg !400
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %215 = fcmp ogt double %213, %208, !dbg !403
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %216 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.2, i32 0, i32 0), double %213, i32 %202), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %217 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %218 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  br label %219, !dbg !424

; <label>:219:                                    ; preds = %219, %206
  %220 = phi i32 [ %218, %206 ], [ %270, %219 ]
  %221 = phi i32 [ 0, %206 ], [ %268, %219 ]
  %222 = phi i32 [ 0, %206 ], [ %269, %219 ]
  tail call void @llvm.dbg.value(metadata i32 %220, i64 0, metadata !457, metadata !117), !dbg !460
  %223 = and i32 %220, 15, !dbg !462
  %224 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %223, !dbg !463
  %225 = load i8, i8* %224, align 1, !dbg !463, !tbaa !123
  %226 = zext i8 %225 to i32, !dbg !463
  %227 = lshr i32 %220, 4, !dbg !464
  %228 = and i32 %227, 15, !dbg !464
  %229 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %228, !dbg !465
  %230 = load i8, i8* %229, align 1, !dbg !465, !tbaa !123
  %231 = zext i8 %230 to i32, !dbg !465
  %232 = lshr i32 %220, 8, !dbg !466
  %233 = and i32 %232, 15, !dbg !466
  %234 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %233, !dbg !467
  %235 = load i8, i8* %234, align 1, !dbg !467, !tbaa !123
  %236 = zext i8 %235 to i32, !dbg !467
  %237 = lshr i32 %220, 12, !dbg !468
  %238 = and i32 %237, 15, !dbg !468
  %239 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %238, !dbg !469
  %240 = load i8, i8* %239, align 1, !dbg !469, !tbaa !123
  %241 = zext i8 %240 to i32, !dbg !469
  %242 = lshr i32 %220, 16, !dbg !470
  %243 = and i32 %242, 15, !dbg !470
  %244 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %243, !dbg !471
  %245 = load i8, i8* %244, align 1, !dbg !471, !tbaa !123
  %246 = zext i8 %245 to i32, !dbg !471
  %247 = lshr i32 %220, 20, !dbg !472
  %248 = and i32 %247, 15, !dbg !472
  %249 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %248, !dbg !473
  %250 = load i8, i8* %249, align 1, !dbg !473, !tbaa !123
  %251 = zext i8 %250 to i32, !dbg !473
  %252 = lshr i32 %220, 24, !dbg !474
  %253 = and i32 %252, 15, !dbg !474
  %254 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %253, !dbg !475
  %255 = load i8, i8* %254, align 1, !dbg !475, !tbaa !123
  %256 = zext i8 %255 to i32, !dbg !475
  %257 = lshr i32 %220, 28, !dbg !476
  %258 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %257, !dbg !477
  %259 = load i8, i8* %258, align 1, !dbg !477, !tbaa !123
  %260 = zext i8 %259 to i32, !dbg !477
  %261 = add i32 %221, %226, !dbg !478
  %262 = add i32 %261, %231, !dbg !479
  %263 = add i32 %262, %236, !dbg !480
  %264 = add i32 %263, %241, !dbg !481
  %265 = add i32 %264, %246, !dbg !482
  %266 = add i32 %265, %251, !dbg !483
  %267 = add i32 %266, %256, !dbg !484
  %268 = add i32 %267, %260, !dbg !419
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  %269 = add nuw nsw i32 %222, 1, !dbg !420
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %270 = add nsw i32 %220, 13, !dbg !421
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %271 = icmp eq i32 %269, %10, !dbg !423
  br i1 %271, label %272, label %219, !dbg !424, !llvm.loop !425

; <label>:272:                                    ; preds = %219
  %273 = zext i1 %193 to i32, !dbg !405
  %274 = select i1 %214, i32 2, i32 %207, !dbg !402
  %275 = select i1 %215, double %213, double %208, !dbg !405
  %276 = select i1 %214, double %213, double %209, !dbg !402
  %277 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %117, i64 0, metadata !64, metadata !117), !dbg !395
  %278 = sub nsw i32 %277, %217, !dbg !396
  %279 = sitofp i32 %278 to double, !dbg !397
  %280 = fdiv double %279, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !65, metadata !117), !dbg !399
  %281 = fcmp olt double %280, %276, !dbg !400
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %282 = fcmp ogt double %280, %275, !dbg !403
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %283 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), double %280, i32 %268), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %284 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %285 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  br label %286, !dbg !424

; <label>:286:                                    ; preds = %286, %272
  %287 = phi i32 [ %285, %272 ], [ %313, %286 ]
  %288 = phi i32 [ 0, %272 ], [ %311, %286 ]
  %289 = phi i32 [ 0, %272 ], [ %312, %286 ]
  tail call void @llvm.dbg.value(metadata i32 %287, i64 0, metadata !485, metadata !117), !dbg !496
  tail call void @llvm.dbg.declare(metadata %union.anon* undef, metadata !488, metadata !117), !dbg !498
  %290 = lshr i32 %287, 8, !dbg !499
  %291 = lshr i32 %287, 16, !dbg !499
  %292 = lshr i32 %287, 24, !dbg !499
  %293 = and i32 %287, 255, !dbg !500
  %294 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %293, !dbg !500
  %295 = load i8, i8* %294, align 1, !dbg !500, !tbaa !123
  %296 = zext i8 %295 to i32, !dbg !500
  %297 = and i32 %290, 255, !dbg !501
  %298 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %297, !dbg !501
  %299 = load i8, i8* %298, align 1, !dbg !501, !tbaa !123
  %300 = zext i8 %299 to i32, !dbg !501
  %301 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %292, !dbg !502
  %302 = load i8, i8* %301, align 1, !dbg !502, !tbaa !123
  %303 = zext i8 %302 to i32, !dbg !502
  %304 = and i32 %291, 255, !dbg !503
  %305 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %304, !dbg !503
  %306 = load i8, i8* %305, align 1, !dbg !503, !tbaa !123
  %307 = zext i8 %306 to i32, !dbg !503
  %308 = add i32 %288, %296, !dbg !504
  %309 = add i32 %308, %300, !dbg !505
  %310 = add i32 %309, %303, !dbg !506
  %311 = add i32 %310, %307, !dbg !419
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  %312 = add nuw nsw i32 %289, 1, !dbg !420
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %313 = add nsw i32 %287, 13, !dbg !421
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %314 = icmp eq i32 %312, %10, !dbg !423
  br i1 %314, label %315, label %286, !dbg !424, !llvm.loop !425

; <label>:315:                                    ; preds = %286
  %316 = select i1 %215, i32 2, i32 %273, !dbg !405
  %317 = select i1 %281, i32 3, i32 %274, !dbg !402
  %318 = select i1 %282, double %280, double %275, !dbg !405
  %319 = select i1 %281, double %280, double %276, !dbg !402
  %320 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %117, i64 0, metadata !64, metadata !117), !dbg !395
  %321 = sub nsw i32 %320, %284, !dbg !396
  %322 = sitofp i32 %321 to double, !dbg !397
  %323 = fdiv double %322, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !65, metadata !117), !dbg !399
  %324 = fcmp olt double %323, %319, !dbg !400
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %325 = fcmp ogt double %323, %318, !dbg !403
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %326 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.4, i32 0, i32 0), double %323, i32 %311), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %327 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %328 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  br label %329, !dbg !424

; <label>:329:                                    ; preds = %329, %315
  %330 = phi i32 [ %328, %315 ], [ %356, %329 ]
  %331 = phi i32 [ 0, %315 ], [ %354, %329 ]
  %332 = phi i32 [ 0, %315 ], [ %355, %329 ]
  %333 = lshr i32 %330, 8
  %334 = lshr i32 %330, 16
  %335 = lshr i32 %330, 24
  tail call void @llvm.dbg.declare(metadata i32* undef, metadata !507, metadata !117), !dbg !512
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !514
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !514
  %336 = and i32 %330, 255, !dbg !515
  %337 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %336, !dbg !515
  %338 = load i8, i8* %337, align 1, !dbg !515, !tbaa !123
  %339 = zext i8 %338 to i32, !dbg !515
  tail call void @llvm.dbg.value(metadata i32 %339, i64 0, metadata !511, metadata !117), !dbg !516
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !514
  %340 = and i32 %333, 255, !dbg !517
  %341 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %340, !dbg !517
  %342 = load i8, i8* %341, align 1, !dbg !517, !tbaa !123
  %343 = zext i8 %342 to i32, !dbg !517
  tail call void @llvm.dbg.value(metadata i32 %351, i64 0, metadata !511, metadata !117), !dbg !516
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !514
  %344 = and i32 %334, 255, !dbg !518
  %345 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %344, !dbg !518
  %346 = load i8, i8* %345, align 1, !dbg !518, !tbaa !123
  %347 = zext i8 %346 to i32, !dbg !518
  tail call void @llvm.dbg.value(metadata i32 %352, i64 0, metadata !511, metadata !117), !dbg !516
  %348 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %335, !dbg !519
  %349 = load i8, i8* %348, align 1, !dbg !519, !tbaa !123
  %350 = zext i8 %349 to i32, !dbg !519
  tail call void @llvm.dbg.value(metadata i32 %353, i64 0, metadata !511, metadata !117), !dbg !516
  %351 = add i32 %331, %339, !dbg !520
  %352 = add i32 %351, %343, !dbg !521
  %353 = add i32 %352, %347, !dbg !522
  %354 = add i32 %353, %350, !dbg !419
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  %355 = add nuw nsw i32 %332, 1, !dbg !420
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %356 = add nsw i32 %330, 13, !dbg !421
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %357 = icmp eq i32 %355, %10, !dbg !423
  br i1 %357, label %358, label %329, !dbg !424, !llvm.loop !425

; <label>:358:                                    ; preds = %329
  %359 = select i1 %282, i32 3, i32 %316, !dbg !405
  %360 = select i1 %324, i32 4, i32 %317, !dbg !402
  %361 = select i1 %325, double %323, double %318, !dbg !405
  %362 = select i1 %324, double %323, double %319, !dbg !402
  %363 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %117, i64 0, metadata !64, metadata !117), !dbg !395
  %364 = sub nsw i32 %363, %327, !dbg !396
  %365 = sitofp i32 %364 to double, !dbg !397
  %366 = fdiv double %365, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !65, metadata !117), !dbg !399
  %367 = fcmp olt double %366, %362, !dbg !400
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %368 = fcmp ogt double %366, %361, !dbg !403
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %369 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([38 x i8], [38 x i8]* @.str.5, i32 0, i32 0), double %366, i32 %354), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %370 = tail call i32 @clock() #6, !dbg !376
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !59, metadata !117), !dbg !380
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  %371 = tail call i32 @rand() #6, !dbg !383
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %14, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !71, metadata !117), !dbg !382
  br label %372, !dbg !424

; <label>:372:                                    ; preds = %.loopexit, %358
  %373 = phi i32 [ %371, %358 ], [ %390, %.loopexit ]
  %374 = phi i32 [ 0, %358 ], [ %388, %.loopexit ]
  %375 = phi i32 [ 0, %358 ], [ %389, %.loopexit ]
  tail call void @llvm.dbg.value(metadata i32 %373, i64 0, metadata !523, metadata !117), !dbg !528
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !527, metadata !117), !dbg !530
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !526, metadata !117), !dbg !531
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !527, metadata !117), !dbg !530
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !526, metadata !117), !dbg !531
  tail call void @llvm.dbg.value(metadata i32 %373, i64 0, metadata !523, metadata !117), !dbg !528
  %376 = icmp eq i32 %373, 0, !dbg !532
  br i1 %376, label %.loopexit, label %.preheader.preheader, !dbg !535

.preheader.preheader:                             ; preds = %372
  br label %.preheader, !dbg !536

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %377 = phi i32 [ %381, %.preheader ], [ 0, %.preheader.preheader ]
  %378 = phi i32 [ %382, %.preheader ], [ 0, %.preheader.preheader ]
  %379 = phi i32 [ %383, %.preheader ], [ %373, %.preheader.preheader ]
  %380 = and i32 %379, 1, !dbg !536
  %381 = add nsw i32 %380, %377, !dbg !537
  tail call void @llvm.dbg.value(metadata i32 %381, i64 0, metadata !527, metadata !117), !dbg !530
  %382 = add nuw nsw i32 %378, 1, !dbg !538
  tail call void @llvm.dbg.value(metadata i32 %382, i64 0, metadata !526, metadata !117), !dbg !531
  %383 = ashr i32 %379, 1, !dbg !539
  tail call void @llvm.dbg.value(metadata i32 %383, i64 0, metadata !523, metadata !117), !dbg !528
  tail call void @llvm.dbg.value(metadata i32 %381, i64 0, metadata !527, metadata !117), !dbg !530
  tail call void @llvm.dbg.value(metadata i32 %382, i64 0, metadata !526, metadata !117), !dbg !531
  tail call void @llvm.dbg.value(metadata i32 %383, i64 0, metadata !523, metadata !117), !dbg !528
  %384 = icmp ne i32 %383, 0, !dbg !532
  %385 = icmp ult i32 %382, 32, !dbg !540
  %386 = and i1 %385, %384, !dbg !541
  br i1 %386, label %.preheader, label %.loopexit.loopexit, !dbg !535, !llvm.loop !542

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit, !dbg !419

.loopexit:                                        ; preds = %.loopexit.loopexit, %372
  %387 = phi i32 [ 0, %372 ], [ %381, %.loopexit.loopexit ]
  %388 = add nsw i32 %387, %374, !dbg !419
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  %389 = add nuw nsw i32 %375, 1, !dbg !420
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %390 = add nsw i32 %373, 13, !dbg !421
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %114, i64 0, metadata !73, metadata !117), !dbg !422
  tail call void @llvm.dbg.value(metadata i32 %112, i64 0, metadata !72, metadata !117), !dbg !381
  tail call void @llvm.dbg.value(metadata i32 %113, i64 0, metadata !71, metadata !117), !dbg !382
  %391 = icmp eq i32 %389, %10, !dbg !423
  br i1 %391, label %392, label %372, !dbg !424, !llvm.loop !425

; <label>:392:                                    ; preds = %.loopexit
  %393 = select i1 %325, i32 4, i32 %359, !dbg !405
  %394 = select i1 %367, i32 5, i32 %360, !dbg !402
  %395 = select i1 %368, double %366, double %361, !dbg !405
  %396 = select i1 %367, double %366, double %362, !dbg !402
  %397 = select i1 %368, i32 5, i32 %393, !dbg !405
  %398 = tail call i32 @clock() #6, !dbg !394
  tail call void @llvm.dbg.value(metadata i32 %117, i64 0, metadata !64, metadata !117), !dbg !395
  %399 = sub nsw i32 %398, %370, !dbg !396
  %400 = sitofp i32 %399 to double, !dbg !397
  %401 = fdiv double %400, 1.000000e+06, !dbg !398
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !65, metadata !117), !dbg !399
  %402 = fcmp olt double %401, %396, !dbg !400
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %403 = select i1 %402, i32 6, i32 %394, !dbg !402
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  %404 = fcmp ogt double %401, %395, !dbg !403
  tail call void @llvm.dbg.value(metadata double %120, i64 0, metadata !67, metadata !117), !dbg !350
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  %405 = select i1 %404, i32 6, i32 %397, !dbg !405
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  %406 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @.str.9, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.6, i32 0, i32 0), double %401, i32 %388), !dbg !406
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %186, i64 0, metadata !66, metadata !117), !dbg !349
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !70, metadata !117), !dbg !374
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !69, metadata !117), !dbg !375
  tail call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !68, metadata !117), !dbg !373
  tail call void @llvm.dbg.value(metadata double %187, i64 0, metadata !67, metadata !117), !dbg !350
  br label %150, !dbg !407
}

; Function Attrs: nounwind
declare i32 @fwrite(i8* nocapture, i32, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #6

; Function Attrs: noreturn nounwind
declare void @exit(i32) local_unnamed_addr #7

; Function Attrs: nounwind
declare i32 @strtol(i8* readonly, i8** nocapture, i32) local_unnamed_addr #5

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #5

; Function Attrs: nounwind
declare i32 @clock() local_unnamed_addr #5

; Function Attrs: nounwind
declare i32 @rand() local_unnamed_addr #5

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #5

; Function Attrs: nounwind readnone
define i32 @bit_count(i32) local_unnamed_addr #8 !dbg !388 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !390, metadata !117), !dbg !545
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !391, metadata !117), !dbg !546
  %2 = icmp eq i32 %0, 0, !dbg !547
  br i1 %2, label %.loopexit, label %.preheader.preheader, !dbg !548, !llvm.loop !411

.preheader.preheader:                             ; preds = %1
  br label %.preheader, !dbg !549

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %3 = phi i32 [ %7, %.preheader ], [ %0, %.preheader.preheader ]
  %4 = phi i32 [ %5, %.preheader ], [ 0, %.preheader.preheader ]
  tail call void @llvm.dbg.value(metadata i32 %4, i64 0, metadata !391, metadata !117), !dbg !546
  tail call void @llvm.dbg.value(metadata i32 %3, i64 0, metadata !390, metadata !117), !dbg !545
  %5 = add nuw nsw i32 %4, 1, !dbg !549
  tail call void @llvm.dbg.value(metadata i32 %5, i64 0, metadata !391, metadata !117), !dbg !546
  %6 = add nsw i32 %3, -1, !dbg !550
  %7 = and i32 %6, %3, !dbg !551
  tail call void @llvm.dbg.value(metadata i32 %7, i64 0, metadata !390, metadata !117), !dbg !545
  %8 = icmp eq i32 %7, 0, !dbg !552
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !dbg !553, !llvm.loop !411

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit, !dbg !554

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %9 = phi i32 [ 0, %1 ], [ %5, %.loopexit.loopexit ]
  tail call void @llvm.dbg.value(metadata i32 %9, i64 0, metadata !391, metadata !117), !dbg !546
  ret i32 %9, !dbg !554
}

; Function Attrs: nounwind readnone
define i32 @bitcount(i32) local_unnamed_addr #8 !dbg !428 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !430, metadata !117), !dbg !555
  %2 = lshr i32 %0, 1, !dbg !556
  %3 = and i32 %2, 1431655765, !dbg !556
  %4 = and i32 %0, 1431655765, !dbg !557
  %5 = add nuw i32 %3, %4, !dbg !558
  tail call void @llvm.dbg.value(metadata i32 %5, i64 0, metadata !430, metadata !117), !dbg !555
  %6 = lshr i32 %5, 2, !dbg !559
  %7 = and i32 %6, 858993459, !dbg !559
  %8 = and i32 %5, 858993459, !dbg !560
  %9 = add nuw nsw i32 %7, %8, !dbg !561
  tail call void @llvm.dbg.value(metadata i32 %9, i64 0, metadata !430, metadata !117), !dbg !555
  %10 = lshr i32 %9, 4, !dbg !562
  %11 = and i32 %10, 117901063, !dbg !562
  %12 = and i32 %9, 117901063, !dbg !563
  %13 = add nuw nsw i32 %11, %12, !dbg !564
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !430, metadata !117), !dbg !555
  %14 = lshr i32 %13, 8, !dbg !565
  %15 = and i32 %14, 983055, !dbg !565
  %16 = and i32 %13, 983055, !dbg !566
  %17 = add nuw nsw i32 %15, %16, !dbg !567
  tail call void @llvm.dbg.value(metadata i32 %17, i64 0, metadata !430, metadata !117), !dbg !555
  %18 = lshr i32 %17, 16, !dbg !568
  %19 = and i32 %17, 31, !dbg !569
  %20 = add nuw nsw i32 %18, %19, !dbg !570
  tail call void @llvm.dbg.value(metadata i32 %20, i64 0, metadata !430, metadata !117), !dbg !555
  ret i32 %20, !dbg !571
}

; Function Attrs: nounwind readnone
define i32 @ntbl_bitcnt(i32) local_unnamed_addr #8 !dbg !572 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !574, metadata !117), !dbg !576
  %2 = and i32 %0, 15, !dbg !577
  %3 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %2, !dbg !578
  %4 = load i8, i8* %3, align 1, !dbg !578, !tbaa !123
  %5 = zext i8 %4 to i32, !dbg !578
  tail call void @llvm.dbg.value(metadata i32 %5, i64 0, metadata !575, metadata !117), !dbg !579
  %6 = ashr i32 %0, 4, !dbg !580
  tail call void @llvm.dbg.value(metadata i32 %6, i64 0, metadata !574, metadata !117), !dbg !576
  %7 = icmp eq i32 %6, 0, !dbg !582
  br i1 %7, label %11, label %8, !dbg !583

; <label>:8:                                      ; preds = %1
  %9 = tail call i32 @ntbl_bitcnt(i32 %6), !dbg !584
  %10 = add nsw i32 %9, %5, !dbg !585
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !575, metadata !117), !dbg !579
  ret i32 %10, !dbg !586

; <label>:11:                                     ; preds = %1
  tail call void @llvm.dbg.value(metadata i32 %5, i64 0, metadata !575, metadata !117), !dbg !579
  ret i32 %5, !dbg !586
}

; Function Attrs: nounwind readnone
define i32 @ntbl_bitcount(i32) local_unnamed_addr #8 !dbg !458 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !457, metadata !117), !dbg !587
  %2 = and i32 %0, 15, !dbg !588
  %3 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %2, !dbg !589
  %4 = load i8, i8* %3, align 1, !dbg !589, !tbaa !123
  %5 = zext i8 %4 to i32, !dbg !589
  %6 = lshr i32 %0, 4, !dbg !590
  %7 = and i32 %6, 15, !dbg !590
  %8 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %7, !dbg !591
  %9 = load i8, i8* %8, align 1, !dbg !591, !tbaa !123
  %10 = zext i8 %9 to i32, !dbg !591
  %11 = add nuw nsw i32 %10, %5, !dbg !592
  %12 = lshr i32 %0, 8, !dbg !593
  %13 = and i32 %12, 15, !dbg !593
  %14 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %13, !dbg !594
  %15 = load i8, i8* %14, align 1, !dbg !594, !tbaa !123
  %16 = zext i8 %15 to i32, !dbg !594
  %17 = add nuw nsw i32 %11, %16, !dbg !595
  %18 = lshr i32 %0, 12, !dbg !596
  %19 = and i32 %18, 15, !dbg !596
  %20 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %19, !dbg !597
  %21 = load i8, i8* %20, align 1, !dbg !597, !tbaa !123
  %22 = zext i8 %21 to i32, !dbg !597
  %23 = add nuw nsw i32 %17, %22, !dbg !598
  %24 = lshr i32 %0, 16, !dbg !599
  %25 = and i32 %24, 15, !dbg !599
  %26 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %25, !dbg !600
  %27 = load i8, i8* %26, align 1, !dbg !600, !tbaa !123
  %28 = zext i8 %27 to i32, !dbg !600
  %29 = add nuw nsw i32 %23, %28, !dbg !601
  %30 = lshr i32 %0, 20, !dbg !602
  %31 = and i32 %30, 15, !dbg !602
  %32 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %31, !dbg !603
  %33 = load i8, i8* %32, align 1, !dbg !603, !tbaa !123
  %34 = zext i8 %33 to i32, !dbg !603
  %35 = add nuw nsw i32 %29, %34, !dbg !604
  %36 = lshr i32 %0, 24, !dbg !605
  %37 = and i32 %36, 15, !dbg !605
  %38 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %37, !dbg !606
  %39 = load i8, i8* %38, align 1, !dbg !606, !tbaa !123
  %40 = zext i8 %39 to i32, !dbg !606
  %41 = add nuw nsw i32 %35, %40, !dbg !607
  %42 = lshr i32 %0, 28, !dbg !608
  %43 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %42, !dbg !609
  %44 = load i8, i8* %43, align 1, !dbg !609, !tbaa !123
  %45 = zext i8 %44 to i32, !dbg !609
  %46 = add nsw i32 %41, %45, !dbg !610
  ret i32 %46, !dbg !611
}

; Function Attrs: nounwind readnone
define i32 @BW_btbl_bitcount(i32) local_unnamed_addr #8 !dbg !486 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !485, metadata !117), !dbg !612
  tail call void @llvm.dbg.declare(metadata %union.anon* undef, metadata !488, metadata !117), !dbg !613
  %2 = lshr i32 %0, 8, !dbg !614
  %3 = lshr i32 %0, 16, !dbg !614
  %4 = lshr i32 %0, 24, !dbg !614
  %5 = and i32 %0, 255, !dbg !615
  %6 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %5, !dbg !615
  %7 = load i8, i8* %6, align 1, !dbg !615, !tbaa !123
  %8 = zext i8 %7 to i32, !dbg !615
  %9 = and i32 %2, 255, !dbg !616
  %10 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %9, !dbg !616
  %11 = load i8, i8* %10, align 1, !dbg !616, !tbaa !123
  %12 = zext i8 %11 to i32, !dbg !616
  %13 = add nuw nsw i32 %12, %8, !dbg !617
  %14 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %4, !dbg !618
  %15 = load i8, i8* %14, align 1, !dbg !618, !tbaa !123
  %16 = zext i8 %15 to i32, !dbg !618
  %17 = add nuw nsw i32 %13, %16, !dbg !619
  %18 = and i32 %3, 255, !dbg !620
  %19 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %18, !dbg !620
  %20 = load i8, i8* %19, align 1, !dbg !620, !tbaa !123
  %21 = zext i8 %20 to i32, !dbg !620
  %22 = add nuw nsw i32 %17, %21, !dbg !621
  ret i32 %22, !dbg !622
}

; Function Attrs: nounwind readnone
define i32 @AR_btbl_bitcount(i32) local_unnamed_addr #8 !dbg !508 {
  %2 = lshr i32 %0, 8
  %3 = lshr i32 %0, 16
  %4 = lshr i32 %0, 24
  tail call void @llvm.dbg.declare(metadata i32* undef, metadata !507, metadata !117), !dbg !623
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !624
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !624
  %5 = and i32 %0, 255, !dbg !625
  %6 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %5, !dbg !625
  %7 = load i8, i8* %6, align 1, !dbg !625, !tbaa !123
  %8 = zext i8 %7 to i32, !dbg !625
  tail call void @llvm.dbg.value(metadata i32 %8, i64 0, metadata !511, metadata !117), !dbg !626
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !624
  %9 = and i32 %2, 255, !dbg !627
  %10 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %9, !dbg !627
  %11 = load i8, i8* %10, align 1, !dbg !627, !tbaa !123
  %12 = zext i8 %11 to i32, !dbg !627
  %13 = add nuw nsw i32 %12, %8, !dbg !628
  tail call void @llvm.dbg.value(metadata i32 %13, i64 0, metadata !511, metadata !117), !dbg !626
  tail call void @llvm.dbg.value(metadata i8* undef, i64 0, metadata !510, metadata !117), !dbg !624
  %14 = and i32 %3, 255, !dbg !629
  %15 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %14, !dbg !629
  %16 = load i8, i8* %15, align 1, !dbg !629, !tbaa !123
  %17 = zext i8 %16 to i32, !dbg !629
  %18 = add nuw nsw i32 %13, %17, !dbg !630
  tail call void @llvm.dbg.value(metadata i32 %18, i64 0, metadata !511, metadata !117), !dbg !626
  %19 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %4, !dbg !631
  %20 = load i8, i8* %19, align 1, !dbg !631, !tbaa !123
  %21 = zext i8 %20 to i32, !dbg !631
  %22 = add nuw nsw i32 %18, %21, !dbg !632
  tail call void @llvm.dbg.value(metadata i32 %22, i64 0, metadata !511, metadata !117), !dbg !626
  ret i32 %22, !dbg !633
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind readnone
define i32 @btbl_bitcnt(i32) local_unnamed_addr #8 !dbg !634 {
  tail call void @llvm.dbg.declare(metadata [3 x i8]* undef, metadata !636, metadata !638), !dbg !639
  tail call void @llvm.dbg.declare(metadata i32* undef, metadata !636, metadata !117), !dbg !639
  %2 = and i32 %0, 255, !dbg !640
  %3 = getelementptr inbounds [256 x i8], [256 x i8]* @bits, i32 0, i32 %2, !dbg !641
  %4 = load i8, i8* %3, align 1, !dbg !641, !tbaa !123
  %5 = zext i8 %4 to i32, !dbg !641
  tail call void @llvm.dbg.value(metadata i32 %5, i64 0, metadata !637, metadata !117), !dbg !642
  %6 = ashr i32 %0, 8, !dbg !643
  %7 = icmp eq i32 %6, 0, !dbg !645
  br i1 %7, label %11, label %8, !dbg !646

; <label>:8:                                      ; preds = %1
  %9 = tail call i32 @btbl_bitcnt(i32 %6), !dbg !647
  %10 = add nsw i32 %9, %5, !dbg !648
  tail call void @llvm.dbg.value(metadata i32 %10, i64 0, metadata !637, metadata !117), !dbg !642
  ret i32 %10, !dbg !649

; <label>:11:                                     ; preds = %1
  tail call void @llvm.dbg.value(metadata i32 %5, i64 0, metadata !637, metadata !117), !dbg !642
  ret i32 %5, !dbg !649
}

; Function Attrs: nounwind
define i32 @initializeParameters(i32) local_unnamed_addr #3 !dbg !650 {
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
  %14 = alloca [40 x i8], align 1
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !654, metadata !117), !dbg !725
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !655, metadata !117), !dbg !726
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 0), align 4, !dbg !727, !tbaa !728
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 1), align 4, !dbg !730, !tbaa !728
  %15 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0, !dbg !731
  tail call void @llvm.memset.p0i8.i64(i8* bitcast (i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2) to i8*), i8 0, i64 20, i32 4, i1 false), !dbg !732
  call void @llvm.lifetime.start(i64 25, i8* nonnull %15) #6, !dbg !731
  tail call void @llvm.dbg.declare(metadata [25 x i8]* %2, metadata !700, metadata !117), !dbg !733
  %16 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0, !dbg !734
  call void @llvm.lifetime.start(i64 20, i8* nonnull %16) #6, !dbg !734
  tail call void @llvm.dbg.declare(metadata [20 x i8]* %3, metadata !704, metadata !117), !dbg !735
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %16, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @initializeParameters.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false), !dbg !735
  %17 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %15, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32 %0) #6, !dbg !736
  %18 = call i8* @strcat(i8* nonnull %16, i8* nonnull %15) #6, !dbg !737
  %19 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0, !dbg !738
  call void @llvm.lifetime.start(i64 40, i8* nonnull %19) #6, !dbg !738
  call void @llvm.dbg.declare(metadata [40 x i8]* %4, metadata !708, metadata !117), !dbg !739
  call void @llvm.memset.p0i8.i32(i8* nonnull %19, i8 0, i32 40, i32 1, i1 false), !dbg !739
  %20 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0, !dbg !740
  call void @llvm.lifetime.start(i64 40, i8* nonnull %20) #6, !dbg !740
  call void @llvm.dbg.declare(metadata [40 x i8]* %5, metadata !709, metadata !117), !dbg !741
  call void @llvm.memset.p0i8.i32(i8* nonnull %20, i8 0, i32 40, i32 1, i1 false), !dbg !741
  %21 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0, !dbg !742
  call void @llvm.lifetime.start(i64 40, i8* nonnull %21) #6, !dbg !742
  call void @llvm.dbg.declare(metadata [40 x i8]* %6, metadata !710, metadata !117), !dbg !743
  call void @llvm.memset.p0i8.i32(i8* nonnull %21, i8 0, i32 40, i32 1, i1 false), !dbg !743
  %22 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0, !dbg !744
  call void @llvm.lifetime.start(i64 40, i8* nonnull %22) #6, !dbg !744
  call void @llvm.dbg.declare(metadata [40 x i8]* %7, metadata !711, metadata !117), !dbg !745
  call void @llvm.memset.p0i8.i32(i8* nonnull %22, i8 0, i32 40, i32 1, i1 false), !dbg !745
  %23 = call i8* @strcat(i8* nonnull %19, i8* nonnull %16) #6, !dbg !746
  %24 = call i32 @strlen(i8* nonnull %19), !dbg !747
  %25 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %24, !dbg !747
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %25, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1.14, i32 0, i32 0), i32 16, i32 1, i1 false), !dbg !747
  %26 = call i8* @strcat(i8* nonnull %20, i8* nonnull %16) #6, !dbg !748
  %27 = call i32 @strlen(i8* nonnull %20), !dbg !749
  %28 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %27, !dbg !749
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %28, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2.15, i32 0, i32 0), i32 16, i32 1, i1 false), !dbg !749
  %29 = call i8* @strcat(i8* nonnull %21, i8* nonnull %16) #6, !dbg !750
  %30 = call i32 @strlen(i8* nonnull %21), !dbg !751
  %31 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %30, !dbg !751
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %31, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3.16, i32 0, i32 0), i32 16, i32 1, i1 false), !dbg !751
  %32 = call %struct._IO_FILE* @fopen(i8* nonnull %19, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0)), !dbg !752
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %32, i64 0, metadata !656, metadata !117), !dbg !753
  %33 = call %struct._IO_FILE* @fopen(i8* nonnull %20, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0)), !dbg !754
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %33, i64 0, metadata !697, metadata !117), !dbg !755
  %34 = call %struct._IO_FILE* @fopen(i8* nonnull %21, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4.17, i32 0, i32 0)), !dbg !756
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %34, i64 0, metadata !698, metadata !117), !dbg !757
  %35 = bitcast i32* %8 to i8*, !dbg !758
  call void @llvm.lifetime.start(i64 4, i8* nonnull %35) #6, !dbg !758
  %36 = bitcast i32* %9 to i8*, !dbg !758
  call void @llvm.lifetime.start(i64 4, i8* nonnull %36) #6, !dbg !758
  %37 = bitcast i32* %10 to i8*, !dbg !758
  call void @llvm.lifetime.start(i64 4, i8* nonnull %37) #6, !dbg !758
  %38 = bitcast i32* %11 to i8*, !dbg !758
  call void @llvm.lifetime.start(i64 4, i8* nonnull %38) #6, !dbg !758
  %39 = call i32 @fread(i8* nonnull %35, i32 4, i32 1, %struct._IO_FILE* %32), !dbg !759
  %40 = call i32 @fread(i8* nonnull %36, i32 4, i32 1, %struct._IO_FILE* %33), !dbg !760
  %41 = call i32 @fread(i8* nonnull %37, i32 4, i32 1, %struct._IO_FILE* %34), !dbg !761
  %42 = load i32, i32* %8, align 4, !dbg !762, !tbaa !728
  call void @llvm.dbg.value(metadata i32 %42, i64 0, metadata !712, metadata !117), !dbg !763
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5.18, i32 0, i32 0), i32 %42), !dbg !764
  %44 = load i32, i32* %9, align 4, !dbg !765, !tbaa !728
  call void @llvm.dbg.value(metadata i32 %44, i64 0, metadata !713, metadata !117), !dbg !766
  %45 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6.19, i32 0, i32 0), i32 %44), !dbg !767
  %46 = load i32, i32* %10, align 4, !dbg !768, !tbaa !728
  call void @llvm.dbg.value(metadata i32 %46, i64 0, metadata !714, metadata !117), !dbg !769
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7.20, i32 0, i32 0), i32 %46), !dbg !770
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8.21, i32 0, i32 0), i32 4), !dbg !771
  call void @llvm.dbg.value(metadata i32 %42, i64 0, metadata !712, metadata !117), !dbg !763
  %49 = shl i32 %42, 2, !dbg !772
  %50 = call noalias i8* @malloc(i32 %49) #6, !dbg !773
  store i8* %50, i8** bitcast (i32** @epilog to i8**), align 4, !dbg !774, !tbaa !356
  call void @llvm.dbg.value(metadata i32 %44, i64 0, metadata !713, metadata !117), !dbg !766
  %51 = shl i32 %44, 2, !dbg !775
  %52 = call noalias i8* @malloc(i32 %51) #6, !dbg !776
  store i8* %52, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !777, !tbaa !356
  call void @llvm.dbg.value(metadata i32 %46, i64 0, metadata !714, metadata !117), !dbg !769
  %53 = shl i32 %46, 2, !dbg !778
  %54 = call noalias i8* @malloc(i32 %53) #6, !dbg !779
  store i8* %54, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !780, !tbaa !356
  call void @llvm.dbg.value(metadata i32 %42, i64 0, metadata !712, metadata !117), !dbg !763
  %55 = call i32 @fread(i8* %50, i32 4, i32 %42, %struct._IO_FILE* %32), !dbg !781
  %56 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !dbg !782, !tbaa !356
  call void @llvm.dbg.value(metadata i32 %44, i64 0, metadata !713, metadata !117), !dbg !766
  %57 = call i32 @fread(i8* %56, i32 4, i32 %44, %struct._IO_FILE* %33), !dbg !783
  %58 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !dbg !784, !tbaa !356
  call void @llvm.dbg.value(metadata i32 %46, i64 0, metadata !714, metadata !117), !dbg !769
  %59 = call i32 @fread(i8* %58, i32 4, i32 %46, %struct._IO_FILE* %34), !dbg !785
  %60 = call i8* @strcat(i8* nonnull %22, i8* nonnull %16) #6, !dbg !786
  %61 = call i32 @strlen(i8* nonnull %22), !dbg !787
  %62 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %61, !dbg !787
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %62, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.9.22, i32 0, i32 0), i32 26, i32 1, i1 false), !dbg !787
  %63 = call %struct._IO_FILE* @fopen(i8* nonnull %22, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0)), !dbg !788
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %63, i64 0, metadata !699, metadata !117), !dbg !789
  call void @llvm.dbg.value(metadata i32* %11, i64 0, metadata !715, metadata !790), !dbg !791
  %64 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %63, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %11) #6, !dbg !792
  call void @llvm.dbg.value(metadata i32 %46, i64 0, metadata !714, metadata !117), !dbg !769
  %65 = sdiv i32 %46, 16, !dbg !793
  call void @llvm.dbg.value(metadata i32 %65, i64 0, metadata !716, metadata !117), !dbg !794
  call void @llvm.dbg.value(metadata i32 %42, i64 0, metadata !712, metadata !117), !dbg !763
  %66 = sdiv i32 %42, 16, !dbg !795
  call void @llvm.dbg.value(metadata i32 %66, i64 0, metadata !717, metadata !117), !dbg !796
  call void @llvm.dbg.value(metadata i32 %44, i64 0, metadata !713, metadata !117), !dbg !766
  %67 = sdiv i32 %44, 16, !dbg !797
  call void @llvm.dbg.value(metadata i32 %67, i64 0, metadata !718, metadata !117), !dbg !798
  store i32 %65, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2), align 4, !dbg !799, !tbaa !728
  store i32 %66, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 3), align 4, !dbg !800, !tbaa !728
  store i32 %67, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 4), align 4, !dbg !801, !tbaa !728
  %68 = load i32, i32* %11, align 4, !dbg !802, !tbaa !728
  call void @llvm.dbg.value(metadata i32 %68, i64 0, metadata !715, metadata !117), !dbg !791
  %69 = sdiv i32 %68, 16, !dbg !803
  store i32 %69, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 6), align 4, !dbg !804, !tbaa !728
  %70 = call i32 @fclose(%struct._IO_FILE* %32), !dbg !805
  %71 = call i32 @fclose(%struct._IO_FILE* %33), !dbg !806
  %72 = call i32 @fclose(%struct._IO_FILE* %34), !dbg !807
  %73 = call i32 @fclose(%struct._IO_FILE* %63), !dbg !808
  %74 = getelementptr inbounds [40 x i8], [40 x i8]* %12, i32 0, i32 0, !dbg !809
  call void @llvm.lifetime.start(i64 40, i8* nonnull %74) #6, !dbg !809
  call void @llvm.dbg.declare(metadata [40 x i8]* %12, metadata !719, metadata !117), !dbg !810
  call void @llvm.memset.p0i8.i32(i8* nonnull %74, i8 0, i32 40, i32 1, i1 false), !dbg !810
  %75 = call i8* @strcat(i8* nonnull %74, i8* nonnull %16) #6, !dbg !811
  %76 = call i32 @strlen(i8* nonnull %74), !dbg !812
  %77 = getelementptr [40 x i8], [40 x i8]* %12, i32 0, i32 %76, !dbg !812
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %77, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.11.24, i32 0, i32 0), i32 18, i32 1, i1 false), !dbg !812
  %78 = bitcast i32* %13 to i8*, !dbg !813
  call void @llvm.lifetime.start(i64 4, i8* nonnull %78) #6, !dbg !813
  call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !720, metadata !117), !dbg !814
  store i32 0, i32* %13, align 4, !dbg !814, !tbaa !728
  %79 = call %struct._IO_FILE* @fopen(i8* nonnull %74, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.23, i32 0, i32 0)), !dbg !815
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %79, i64 0, metadata !699, metadata !117), !dbg !789
  call void @llvm.dbg.value(metadata i32* %13, i64 0, metadata !720, metadata !790), !dbg !814
  %80 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %79, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.13, i32 0, i32 0), i32* nonnull %13) #6, !dbg !816
  %81 = call i32 @fclose(%struct._IO_FILE* %79), !dbg !817
  %82 = load i32, i32* %13, align 4, !dbg !818, !tbaa !728
  call void @llvm.dbg.value(metadata i32 %82, i64 0, metadata !720, metadata !117), !dbg !814
  %83 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %82), !dbg !819
  %84 = load i32, i32* %13, align 4, !dbg !820, !tbaa !728
  call void @llvm.dbg.value(metadata i32 %84, i64 0, metadata !720, metadata !117), !dbg !814
  %85 = icmp sgt i32 %84, 0, !dbg !821
  %86 = load i32, i32* @dynamicTCVal, align 4, !dbg !822
  %87 = select i1 %85, i32 0, i32 %86, !dbg !823
  %88 = add nsw i32 %87, %84, !dbg !823
  store i32 %88, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !tbaa !728
  %89 = getelementptr inbounds [40 x i8], [40 x i8]* %14, i32 0, i32 0, !dbg !824
  call void @llvm.lifetime.start(i64 40, i8* nonnull %89) #6, !dbg !824
  call void @llvm.dbg.declare(metadata [40 x i8]* %14, metadata !724, metadata !117), !dbg !825
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %89, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @initializeParameters.initCGRAfile, i32 0, i32 0), i32 40, i32 1, i1 false), !dbg !825
  %90 = call i32 @strlen(i8* nonnull %89), !dbg !826
  %91 = getelementptr [40 x i8], [40 x i8]* %14, i32 0, i32 %90, !dbg !826
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %91, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.13.25, i32 0, i32 0), i32 14, i32 1, i1 false), !dbg !826
  %92 = call %struct._IO_FILE* @fopen(i8* nonnull %89, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.14, i32 0, i32 0)), !dbg !827
  call void @llvm.dbg.value(metadata %struct._IO_FILE* %92, i64 0, metadata !699, metadata !117), !dbg !789
  call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !655, metadata !117), !dbg !726
  %93 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 0), align 4, !dbg !828, !tbaa !728
  %94 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.15, i32 0, i32 0), i32 %93), !dbg !831
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  %95 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 1), align 4, !dbg !828, !tbaa !728
  %96 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.15, i32 0, i32 0), i32 %95), !dbg !831
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  %97 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2), align 4, !dbg !828, !tbaa !728
  %98 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.15, i32 0, i32 0), i32 %97), !dbg !831
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  %99 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 3), align 4, !dbg !828, !tbaa !728
  %100 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.15, i32 0, i32 0), i32 %99), !dbg !831
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  %101 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 4), align 4, !dbg !828, !tbaa !728
  %102 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.15, i32 0, i32 0), i32 %101), !dbg !831
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  %103 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !dbg !828, !tbaa !728
  %104 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.15, i32 0, i32 0), i32 %103), !dbg !831
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  %105 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 6), align 4, !dbg !828, !tbaa !728
  %106 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.15, i32 0, i32 0), i32 %105), !dbg !831
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  call void @llvm.dbg.value(metadata i32 1, i64 0, metadata !655, metadata !117), !dbg !726
  %107 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !dbg !832, !tbaa !356
  %108 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.16, i32 0, i32 0), i32 %107), !dbg !833
  %109 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !dbg !834, !tbaa !356
  %110 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.16, i32 0, i32 0), i32 %109), !dbg !835
  %111 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !dbg !836, !tbaa !356
  %112 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %92, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.16, i32 0, i32 0), i32 %111), !dbg !837
  %113 = call i32 @fclose(%struct._IO_FILE* %92), !dbg !838
  call void @llvm.lifetime.end(i64 40, i8* nonnull %89) #6, !dbg !839
  call void @llvm.lifetime.end(i64 4, i8* nonnull %78) #6, !dbg !839
  call void @llvm.lifetime.end(i64 40, i8* nonnull %74) #6, !dbg !839
  call void @llvm.lifetime.end(i64 4, i8* nonnull %38) #6, !dbg !839
  call void @llvm.lifetime.end(i64 4, i8* nonnull %37) #6, !dbg !839
  call void @llvm.lifetime.end(i64 4, i8* nonnull %36) #6, !dbg !839
  call void @llvm.lifetime.end(i64 4, i8* nonnull %35) #6, !dbg !839
  call void @llvm.lifetime.end(i64 40, i8* nonnull %22) #6, !dbg !839
  call void @llvm.lifetime.end(i64 40, i8* nonnull %21) #6, !dbg !839
  call void @llvm.lifetime.end(i64 40, i8* nonnull %20) #6, !dbg !839
  call void @llvm.lifetime.end(i64 40, i8* nonnull %19) #6, !dbg !839
  call void @llvm.lifetime.end(i64 20, i8* nonnull %16) #6, !dbg !839
  call void @llvm.lifetime.end(i64 25, i8* nonnull %15) #6, !dbg !839
  ret i32 0, !dbg !840
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #4

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #4

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture writeonly, i8* nocapture readonly, i32, i32, i1) #4

; Function Attrs: nounwind
declare i32 @sprintf(i8* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #5

; Function Attrs: nounwind
declare i8* @strcat(i8*, i8* nocapture readonly) local_unnamed_addr #5

; Function Attrs: nounwind readonly
declare i32 @strlen(i8* nocapture) local_unnamed_addr #2

; Function Attrs: nounwind
declare i32 @fread(i8* nocapture, i32, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #5

declare i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #9

; Function Attrs: nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #5

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #4

; Function Attrs: nounwind
define void @updateLoopTC(i32) local_unnamed_addr #3 !dbg !841 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !845, metadata !117), !dbg !848
  %2 = load i32, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !dbg !849, !tbaa !728
  %3 = icmp sgt i32 %2, 0, !dbg !851
  br i1 %3, label %6, label %4, !dbg !852

; <label>:4:                                      ; preds = %1
  %5 = load i32, i32* @dynamicTCVal, align 4, !dbg !853, !tbaa !728
  store i32 %5, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 5), align 4, !dbg !854, !tbaa !728
  br label %6, !dbg !855

; <label>:6:                                      ; preds = %4, %1
  ret void, !dbg !855
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #3 !dbg !856 {
  tail call void @llvm.dbg.value(metadata i8* %0, i64 0, metadata !860, metadata !117), !dbg !861
  br label %2, !dbg !862

; <label>:2:                                      ; preds = %12, %1
  %3 = tail call i32 @puts(i8* getelementptr inbounds ([44 x i8], [44 x i8]* @str, i32 0, i32 0)), !dbg !863
  %4 = load i32, i32* @thread_cond_cgra, align 4, !dbg !865, !tbaa !728
  %5 = icmp eq i32 %4, 1, !dbg !866
  br i1 %5, label %.loopexit, label %.preheader.preheader, !dbg !867

.preheader.preheader:                             ; preds = %2
  br label %.preheader, !dbg !868

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %6 = tail call i32 @usleep(i32 1) #6, !dbg !868
  %7 = load i32, i32* @thread_cond_cgra, align 4, !dbg !865, !tbaa !728
  %8 = icmp eq i32 %7, 1, !dbg !866
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !dbg !867, !llvm.loop !869

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit, !dbg !871

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %9 = load i32, i32* @thread_exit, align 4, !dbg !871, !tbaa !728
  %10 = icmp eq i32 %9, 1, !dbg !873
  br i1 %10, label %11, label %12, !dbg !874

; <label>:11:                                     ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #6, !dbg !875, !srcloc !877
  ret i8* null, !dbg !878

; <label>:12:                                     ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !dbg !879, !tbaa !728
  %13 = tail call i32 @puts(i8* getelementptr inbounds ([43 x i8], [43 x i8]* @str.26, i32 0, i32 0)), !dbg !880
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #6, !dbg !881, !srcloc !882
  %14 = tail call i32 @usleep(i32 1) #6, !dbg !883
  %15 = tail call i32 @puts(i8* getelementptr inbounds ([44 x i8], [44 x i8]* @str.27, i32 0, i32 0)), !dbg !884
  store i32 1, i32* @thread_cond_cpu, align 4, !dbg !885, !tbaa !728
  br label %2, !dbg !862, !llvm.loop !886
}

declare i32 @usleep(i32) local_unnamed_addr #9

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #3 !dbg !888 {
  tail call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !890, metadata !117), !dbg !893
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !891, metadata !117), !dbg !894
  %2 = tail call i32 @initializeParameters(i32 %0), !dbg !895
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !892, metadata !117), !dbg !896
  store i32 0, i32* @thread_cond_cpu, align 4, !dbg !897, !tbaa !728
  store i32 1, i32* @thread_cond_cgra, align 4, !dbg !898, !tbaa !728
  %3 = tail call i32 @puts(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str.28, i32 0, i32 0)), !dbg !899
  %4 = load i32, i32* @thread_cond_cpu, align 4, !dbg !900, !tbaa !728
  %5 = icmp eq i32 %4, 1, !dbg !901
  br i1 %5, label %.loopexit, label %.preheader.preheader, !dbg !902

.preheader.preheader:                             ; preds = %1
  br label %.preheader, !dbg !903

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %6 = tail call i32 @usleep(i32 1) #6, !dbg !903
  %7 = load i32, i32* @thread_cond_cpu, align 4, !dbg !900, !tbaa !728
  %8 = icmp eq i32 %7, 1, !dbg !901
  br i1 %8, label %.loopexit.loopexit, label %.preheader, !dbg !902, !llvm.loop !904

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit, !dbg !906

.loopexit:                                        ; preds = %.loopexit.loopexit, %1
  %9 = tail call i32 @puts(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @str.29, i32 0, i32 0)), !dbg !906
  ret void, !dbg !907
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #3 !dbg !908 {
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.30, i32 0, i32 0)), !dbg !911
  store i32 1, i32* @thread_exit, align 4, !dbg !912, !tbaa !728
  store i32 1, i32* @thread_cond_cgra, align 4, !dbg !913, !tbaa !728
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.31, i32 0, i32 0)), !dbg !914
  %3 = load i32, i32* @pth, align 4, !dbg !915, !tbaa !916
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #6, !dbg !918
  ret void, !dbg !919
}

declare i32 @pthread_join(i32, i8**) local_unnamed_addr #9

; Function Attrs: nounwind
define void @createCGRA() local_unnamed_addr #3 !dbg !920 {
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 0), align 4, !dbg !923, !tbaa !728
  store i32 2011168768, i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 1), align 4, !dbg !924, !tbaa !728
  tail call void @llvm.dbg.value(metadata i32 0, i64 0, metadata !922, metadata !117), !dbg !925
  tail call void @llvm.memset.p0i8.i64(i8* bitcast (i32* getelementptr inbounds ([7 x i32], [7 x i32]* @initCGRA, i32 0, i32 2) to i8*), i8 0, i64 20, i32 4, i1 false), !dbg !926
  %1 = tail call i32 @puts(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str.32, i32 0, i32 0)), !dbg !927
  %2 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #6, !dbg !928
  tail call void @llvm.dbg.value(metadata i32 %2, i64 0, metadata !922, metadata !117), !dbg !925
  ret void, !dbg !929
}

; Function Attrs: nounwind
declare i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #5

attributes #0 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { nounwind readonly }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind }
attributes #5 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nounwind }
attributes #7 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { cold }
attributes #11 = { noreturn nounwind }

!llvm.dbg.cu = !{!85, !87, !89, !46, !78, !92, !100, !103, !2}
!llvm.ident = !{!105, !105, !105, !105, !105, !105, !105, !105, !105}
!llvm.module.flags = !{!106, !107, !108, !109}

!0 = !DIGlobalVariableExpression(var: !1)
!1 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !2, file: !13, line: 26, type: !7, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !10)
!3 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!4 = !{}
!5 = !{!6, !8, !9}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 32)
!7 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!8 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!10 = !{!0, !11, !14, !16, !19, !21, !23, !25, !27, !32}
!11 = !DIGlobalVariableExpression(var: !12)
!12 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !2, file: !13, line: 27, type: !7, isLocal: false, isDefinition: true)
!13 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!14 = !DIGlobalVariableExpression(var: !15)
!15 = distinct !DIGlobalVariable(name: "thread_exit", scope: !2, file: !13, line: 28, type: !7, isLocal: false, isDefinition: true)
!16 = !DIGlobalVariableExpression(var: !17)
!17 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !2, file: !13, line: 30, type: !18, isLocal: false, isDefinition: true)
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = !DIGlobalVariableExpression(var: !20)
!20 = distinct !DIGlobalVariable(name: "prolog", scope: !2, file: !13, line: 20, type: !6, isLocal: false, isDefinition: true)
!21 = !DIGlobalVariableExpression(var: !22)
!22 = distinct !DIGlobalVariable(name: "epilog", scope: !2, file: !13, line: 20, type: !6, isLocal: false, isDefinition: true)
!23 = !DIGlobalVariableExpression(var: !24)
!24 = distinct !DIGlobalVariable(name: "kernel", scope: !2, file: !13, line: 20, type: !6, isLocal: false, isDefinition: true)
!25 = !DIGlobalVariableExpression(var: !26)
!26 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !2, file: !13, line: 21, type: !6, isLocal: false, isDefinition: true)
!27 = !DIGlobalVariableExpression(var: !28)
!28 = distinct !DIGlobalVariable(name: "initCGRA", scope: !2, file: !13, line: 23, type: !29, isLocal: false, isDefinition: true)
!29 = !DICompositeType(tag: DW_TAG_array_type, baseType: !18, size: 224, elements: !30)
!30 = !{!31}
!31 = !DISubrange(count: 7)
!32 = !DIGlobalVariableExpression(var: !33)
!33 = distinct !DIGlobalVariable(name: "pth", scope: !2, file: !13, line: 25, type: !34, isLocal: false, isDefinition: true)
!34 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !35, line: 37, baseType: !36)
!35 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!36 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!37 = !DIGlobalVariableExpression(var: !38)
!38 = distinct !DIGlobalVariable(name: "text", scope: !39, file: !40, line: 38, type: !75, isLocal: true, isDefinition: true)
!39 = distinct !DISubprogram(name: "main", scope: !40, file: !40, line: 21, type: !41, isLocal: false, isDefinition: true, scopeLine: 22, flags: DIFlagPrototyped, isOptimized: true, unit: !46, variables: !56)
!40 = !DIFile(filename: "bitcnts.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!41 = !DISubroutineType(types: !42)
!42 = !{!18, !18, !43}
!43 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !44, size: 32)
!44 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !45, size: 32)
!45 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!46 = distinct !DICompileUnit(language: DW_LANG_C99, file: !40, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !47, globals: !49)
!47 = !{!48, !18, !43, !9}
!48 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!49 = !{!50, !37}
!50 = !DIGlobalVariableExpression(var: !51)
!51 = distinct !DIGlobalVariable(name: "pBitCntFunc", scope: !39, file: !40, line: 28, type: !52, isLocal: true, isDefinition: true)
!52 = !DICompositeType(tag: DW_TAG_array_type, baseType: !53, size: 224, elements: !30)
!53 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !54, size: 32)
!54 = !DISubroutineType(types: !55)
!55 = !{!18, !8}
!56 = !{!57, !58, !59, !64, !65, !66, !67, !68, !69, !70, !71, !72, !73, !74}
!57 = !DILocalVariable(name: "argc", arg: 1, scope: !39, file: !40, line: 21, type: !18)
!58 = !DILocalVariable(name: "argv", arg: 2, scope: !39, file: !40, line: 21, type: !43)
!59 = !DILocalVariable(name: "start", scope: !39, file: !40, line: 23, type: !60)
!60 = !DIDerivedType(tag: DW_TAG_typedef, name: "clock_t", file: !61, line: 59, baseType: !62)
!61 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/time.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!62 = !DIDerivedType(tag: DW_TAG_typedef, name: "__clock_t", file: !63, line: 135, baseType: !8)
!63 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/types.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!64 = !DILocalVariable(name: "stop", scope: !39, file: !40, line: 23, type: !60)
!65 = !DILocalVariable(name: "ct", scope: !39, file: !40, line: 24, type: !48)
!66 = !DILocalVariable(name: "cmin", scope: !39, file: !40, line: 24, type: !48)
!67 = !DILocalVariable(name: "cmax", scope: !39, file: !40, line: 24, type: !48)
!68 = !DILocalVariable(name: "i", scope: !39, file: !40, line: 25, type: !18)
!69 = !DILocalVariable(name: "cminix", scope: !39, file: !40, line: 25, type: !18)
!70 = !DILocalVariable(name: "cmaxix", scope: !39, file: !40, line: 25, type: !18)
!71 = !DILocalVariable(name: "j", scope: !39, file: !40, line: 26, type: !8)
!72 = !DILocalVariable(name: "n", scope: !39, file: !40, line: 26, type: !8)
!73 = !DILocalVariable(name: "seed", scope: !39, file: !40, line: 26, type: !8)
!74 = !DILocalVariable(name: "iterations", scope: !39, file: !40, line: 27, type: !18)
!75 = !DICompositeType(tag: DW_TAG_array_type, baseType: !44, size: 224, elements: !30)
!76 = !DIGlobalVariableExpression(var: !77)
!77 = distinct !DIGlobalVariable(name: "bits", scope: !78, file: !79, line: 11, type: !82, isLocal: true, isDefinition: true)
!78 = distinct !DICompileUnit(language: DW_LANG_C99, file: !79, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !80, globals: !81)
!79 = !DIFile(filename: "bitcnt_4.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!80 = !{!18, !44}
!81 = !{!76}
!82 = !DICompositeType(tag: DW_TAG_array_type, baseType: !45, size: 2048, elements: !83)
!83 = !{!84}
!84 = !DISubrange(count: 256)
!85 = distinct !DICompileUnit(language: DW_LANG_C99, file: !86, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4)
!86 = !DIFile(filename: "bstr_i.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!87 = distinct !DICompileUnit(language: DW_LANG_C99, file: !88, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4)
!88 = !DIFile(filename: "bitstrng.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!89 = distinct !DICompileUnit(language: DW_LANG_C99, file: !90, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !91)
!90 = !DIFile(filename: "bitfiles.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!91 = !{!9, !45}
!92 = distinct !DICompileUnit(language: DW_LANG_C99, file: !93, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !94, globals: !97)
!93 = !DIFile(filename: "bitcnt_3.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!94 = !{!18, !95}
!95 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !96, size: 32)
!96 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!97 = !{!98}
!98 = !DIGlobalVariableExpression(var: !99)
!99 = distinct !DIGlobalVariable(name: "bits", scope: !92, file: !93, line: 15, type: !82, isLocal: true, isDefinition: true)
!100 = distinct !DICompileUnit(language: DW_LANG_C99, file: !101, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !102)
!101 = !DIFile(filename: "bitcnt_2.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!102 = !{!18}
!103 = distinct !DICompileUnit(language: DW_LANG_C99, file: !104, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4)
!104 = !DIFile(filename: "bitcnt_1.c", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!105 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!106 = !{i32 2, !"Dwarf Version", i32 4}
!107 = !{i32 2, !"Debug Info Version", i32 3}
!108 = !{i32 1, !"wchar_size", i32 4}
!109 = !{i32 1, !"min_enum_size", i32 4}
!110 = distinct !DISubprogram(name: "bstr_i", scope: !86, file: !86, line: 12, type: !111, isLocal: false, isDefinition: true, scopeLine: 13, flags: DIFlagPrototyped, isOptimized: true, unit: !85, variables: !113)
!111 = !DISubroutineType(types: !112)
!112 = !{!7, !44}
!113 = !{!114, !115, !116}
!114 = !DILocalVariable(name: "cptr", arg: 1, scope: !110, file: !86, line: 12, type: !44)
!115 = !DILocalVariable(name: "i", scope: !110, file: !86, line: 14, type: !7)
!116 = !DILocalVariable(name: "j", scope: !110, file: !86, line: 14, type: !7)
!117 = !DIExpression()
!118 = !DILocation(line: 12, column: 27, scope: !110)
!119 = !DILocation(line: 14, column: 23, scope: !110)
!120 = !DILocation(line: 16, column: 14, scope: !110)
!121 = !DILocation(line: 16, column: 19, scope: !110)
!122 = !DILocation(line: 16, column: 22, scope: !110)
!123 = !{!124, !124, i64 0}
!124 = !{!"omnipotent char", !125, i64 0}
!125 = !{!"Simple C/C++ TBAA"}
!126 = !DILocation(line: 16, column: 28, scope: !110)
!127 = !DILocation(line: 16, column: 31, scope: !110)
!128 = !DILocation(line: 16, column: 7, scope: !110)
!129 = !DILocation(line: 18, column: 22, scope: !130)
!130 = distinct !DILexicalBlock(scope: !110, file: !86, line: 17, column: 7)
!131 = !DILocation(line: 19, column: 15, scope: !130)
!132 = !DILocation(line: 20, column: 21, scope: !130)
!133 = !DILocation(line: 20, column: 15, scope: !130)
!134 = !DILocation(line: 22, column: 7, scope: !110)
!135 = distinct !DISubprogram(name: "bitstring", scope: !88, file: !88, line: 31, type: !136, isLocal: false, isDefinition: true, scopeLine: 32, flags: DIFlagPrototyped, isOptimized: true, unit: !87, variables: !138)
!136 = !DISubroutineType(types: !137)
!137 = !{null, !44, !8, !18, !18}
!138 = !{!139, !140, !141, !142, !143, !144}
!139 = !DILocalVariable(name: "str", arg: 1, scope: !135, file: !88, line: 31, type: !44)
!140 = !DILocalVariable(name: "byze", arg: 2, scope: !135, file: !88, line: 31, type: !8)
!141 = !DILocalVariable(name: "biz", arg: 3, scope: !135, file: !88, line: 31, type: !18)
!142 = !DILocalVariable(name: "strwid", arg: 4, scope: !135, file: !88, line: 31, type: !18)
!143 = !DILocalVariable(name: "i", scope: !135, file: !88, line: 33, type: !18)
!144 = !DILocalVariable(name: "j", scope: !135, file: !88, line: 33, type: !18)
!145 = !DILocation(line: 31, column: 22, scope: !135)
!146 = !DILocation(line: 31, column: 32, scope: !135)
!147 = !DILocation(line: 31, column: 42, scope: !135)
!148 = !DILocation(line: 31, column: 51, scope: !135)
!149 = !DILocation(line: 35, column: 32, scope: !135)
!150 = !DILocation(line: 35, column: 40, scope: !135)
!151 = !DILocation(line: 35, column: 25, scope: !135)
!152 = !DILocation(line: 35, column: 18, scope: !135)
!153 = !DILocation(line: 33, column: 14, scope: !135)
!154 = !DILocation(line: 33, column: 11, scope: !135)
!155 = !DILocation(line: 36, column: 21, scope: !156)
!156 = distinct !DILexicalBlock(scope: !157, file: !88, line: 36, column: 7)
!157 = distinct !DILexicalBlock(scope: !135, file: !88, line: 36, column: 7)
!158 = !DILocation(line: 36, column: 7, scope: !157)
!159 = !DILocation(line: 37, column: 20, scope: !156)
!160 = !DILocation(line: 37, column: 17, scope: !156)
!161 = !DILocation(line: 38, column: 20, scope: !135)
!162 = !DILocation(line: 38, column: 7, scope: !135)
!163 = !DILocation(line: 38, column: 14, scope: !135)
!164 = !DILocation(line: 40, column: 29, scope: !165)
!165 = distinct !DILexicalBlock(scope: !135, file: !88, line: 39, column: 7)
!166 = !DILocation(line: 40, column: 37, scope: !165)
!167 = !DILocation(line: 40, column: 42, scope: !165)
!168 = !DILocation(line: 40, column: 22, scope: !165)
!169 = !DILocation(line: 40, column: 17, scope: !165)
!170 = !DILocation(line: 40, column: 20, scope: !165)
!171 = !DILocation(line: 41, column: 23, scope: !172)
!172 = distinct !DILexicalBlock(scope: !165, file: !88, line: 41, column: 17)
!173 = !DILocation(line: 41, column: 31, scope: !172)
!174 = !DILocation(line: 41, column: 28, scope: !172)
!175 = distinct !{!175, !162, !176}
!176 = !DILocation(line: 43, column: 7, scope: !135)
!177 = !DILocation(line: 42, column: 23, scope: !172)
!178 = !DILocation(line: 42, column: 26, scope: !172)
!179 = !DILocation(line: 42, column: 19, scope: !172)
!180 = !DILocation(line: 44, column: 12, scope: !135)
!181 = !DILocation(line: 45, column: 1, scope: !135)
!182 = distinct !DISubprogram(name: "bfopen", scope: !90, file: !90, line: 12, type: !183, isLocal: false, isDefinition: true, scopeLine: 13, flags: DIFlagPrototyped, isOptimized: true, unit: !89, variables: !253)
!183 = !DISubroutineType(types: !184)
!184 = !{!185, !44, !44}
!185 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !186, size: 32)
!186 = !DIDerivedType(tag: DW_TAG_typedef, name: "bfile", file: !187, line: 66, baseType: !188)
!187 = !DIFile(filename: "./bitops.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!188 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !187, line: 60, size: 64, elements: !189)
!189 = !{!190, !249, !250, !251, !252}
!190 = !DIDerivedType(tag: DW_TAG_member, name: "file", scope: !188, file: !187, line: 61, baseType: !191, size: 32)
!191 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !192, size: 32)
!192 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !193, line: 48, baseType: !194)
!193 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdio.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!194 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !195, line: 245, size: 1216, elements: !196)
!195 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/libio.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!196 = !{!197, !198, !199, !200, !201, !202, !203, !204, !205, !206, !207, !208, !209, !217, !218, !219, !220, !222, !224, !226, !230, !233, !237, !238, !239, !240, !241, !244, !245}
!197 = !DIDerivedType(tag: DW_TAG_member, name: "_flags", scope: !194, file: !195, line: 246, baseType: !18, size: 32)
!198 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_ptr", scope: !194, file: !195, line: 251, baseType: !44, size: 32, offset: 32)
!199 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_end", scope: !194, file: !195, line: 252, baseType: !44, size: 32, offset: 64)
!200 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_base", scope: !194, file: !195, line: 253, baseType: !44, size: 32, offset: 96)
!201 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_base", scope: !194, file: !195, line: 254, baseType: !44, size: 32, offset: 128)
!202 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_ptr", scope: !194, file: !195, line: 255, baseType: !44, size: 32, offset: 160)
!203 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_end", scope: !194, file: !195, line: 256, baseType: !44, size: 32, offset: 192)
!204 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_base", scope: !194, file: !195, line: 257, baseType: !44, size: 32, offset: 224)
!205 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_end", scope: !194, file: !195, line: 258, baseType: !44, size: 32, offset: 256)
!206 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_base", scope: !194, file: !195, line: 260, baseType: !44, size: 32, offset: 288)
!207 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_backup_base", scope: !194, file: !195, line: 261, baseType: !44, size: 32, offset: 320)
!208 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_end", scope: !194, file: !195, line: 262, baseType: !44, size: 32, offset: 352)
!209 = !DIDerivedType(tag: DW_TAG_member, name: "_markers", scope: !194, file: !195, line: 264, baseType: !210, size: 32, offset: 384)
!210 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !211, size: 32)
!211 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_marker", file: !195, line: 160, size: 96, elements: !212)
!212 = !{!213, !214, !216}
!213 = !DIDerivedType(tag: DW_TAG_member, name: "_next", scope: !211, file: !195, line: 161, baseType: !210, size: 32)
!214 = !DIDerivedType(tag: DW_TAG_member, name: "_sbuf", scope: !211, file: !195, line: 162, baseType: !215, size: 32, offset: 32)
!215 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !194, size: 32)
!216 = !DIDerivedType(tag: DW_TAG_member, name: "_pos", scope: !211, file: !195, line: 166, baseType: !18, size: 32, offset: 64)
!217 = !DIDerivedType(tag: DW_TAG_member, name: "_chain", scope: !194, file: !195, line: 266, baseType: !215, size: 32, offset: 416)
!218 = !DIDerivedType(tag: DW_TAG_member, name: "_fileno", scope: !194, file: !195, line: 268, baseType: !18, size: 32, offset: 448)
!219 = !DIDerivedType(tag: DW_TAG_member, name: "_flags2", scope: !194, file: !195, line: 272, baseType: !18, size: 32, offset: 480)
!220 = !DIDerivedType(tag: DW_TAG_member, name: "_old_offset", scope: !194, file: !195, line: 274, baseType: !221, size: 32, offset: 512)
!221 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off_t", file: !63, line: 131, baseType: !8)
!222 = !DIDerivedType(tag: DW_TAG_member, name: "_cur_column", scope: !194, file: !195, line: 278, baseType: !223, size: 16, offset: 544)
!223 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!224 = !DIDerivedType(tag: DW_TAG_member, name: "_vtable_offset", scope: !194, file: !195, line: 279, baseType: !225, size: 8, offset: 560)
!225 = !DIBasicType(name: "signed char", size: 8, encoding: DW_ATE_signed_char)
!226 = !DIDerivedType(tag: DW_TAG_member, name: "_shortbuf", scope: !194, file: !195, line: 280, baseType: !227, size: 8, offset: 568)
!227 = !DICompositeType(tag: DW_TAG_array_type, baseType: !45, size: 8, elements: !228)
!228 = !{!229}
!229 = !DISubrange(count: 1)
!230 = !DIDerivedType(tag: DW_TAG_member, name: "_lock", scope: !194, file: !195, line: 284, baseType: !231, size: 32, offset: 576)
!231 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !232, size: 32)
!232 = !DIDerivedType(tag: DW_TAG_typedef, name: "_IO_lock_t", file: !195, line: 154, baseType: null)
!233 = !DIDerivedType(tag: DW_TAG_member, name: "_offset", scope: !194, file: !195, line: 293, baseType: !234, size: 64, offset: 640)
!234 = !DIDerivedType(tag: DW_TAG_typedef, name: "__off64_t", file: !63, line: 132, baseType: !235)
!235 = !DIDerivedType(tag: DW_TAG_typedef, name: "__quad_t", file: !63, line: 55, baseType: !236)
!236 = !DIBasicType(name: "long long int", size: 64, encoding: DW_ATE_signed)
!237 = !DIDerivedType(tag: DW_TAG_member, name: "__pad1", scope: !194, file: !195, line: 302, baseType: !9, size: 32, offset: 704)
!238 = !DIDerivedType(tag: DW_TAG_member, name: "__pad2", scope: !194, file: !195, line: 303, baseType: !9, size: 32, offset: 736)
!239 = !DIDerivedType(tag: DW_TAG_member, name: "__pad3", scope: !194, file: !195, line: 304, baseType: !9, size: 32, offset: 768)
!240 = !DIDerivedType(tag: DW_TAG_member, name: "__pad4", scope: !194, file: !195, line: 305, baseType: !9, size: 32, offset: 800)
!241 = !DIDerivedType(tag: DW_TAG_member, name: "__pad5", scope: !194, file: !195, line: 306, baseType: !242, size: 32, offset: 832)
!242 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !243, line: 62, baseType: !7)
!243 = !DIFile(filename: "/home/shail/llvm/build/lib/clang/5.0.0/include/stddef.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!244 = !DIDerivedType(tag: DW_TAG_member, name: "_mode", scope: !194, file: !195, line: 308, baseType: !18, size: 32, offset: 864)
!245 = !DIDerivedType(tag: DW_TAG_member, name: "_unused2", scope: !194, file: !195, line: 310, baseType: !246, size: 320, offset: 896)
!246 = !DICompositeType(tag: DW_TAG_array_type, baseType: !45, size: 320, elements: !247)
!247 = !{!248}
!248 = !DISubrange(count: 40)
!249 = !DIDerivedType(tag: DW_TAG_member, name: "rbuf", scope: !188, file: !187, line: 62, baseType: !45, size: 8, offset: 32)
!250 = !DIDerivedType(tag: DW_TAG_member, name: "rcnt", scope: !188, file: !187, line: 63, baseType: !45, size: 8, offset: 40)
!251 = !DIDerivedType(tag: DW_TAG_member, name: "wbuf", scope: !188, file: !187, line: 64, baseType: !45, size: 8, offset: 48)
!252 = !DIDerivedType(tag: DW_TAG_member, name: "wcnt", scope: !188, file: !187, line: 65, baseType: !45, size: 8, offset: 56)
!253 = !{!254, !255, !256}
!254 = !DILocalVariable(name: "name", arg: 1, scope: !182, file: !90, line: 12, type: !44)
!255 = !DILocalVariable(name: "mode", arg: 2, scope: !182, file: !90, line: 12, type: !44)
!256 = !DILocalVariable(name: "bf", scope: !182, file: !90, line: 14, type: !185)
!257 = !DILocation(line: 12, column: 21, scope: !182)
!258 = !DILocation(line: 12, column: 33, scope: !182)
!259 = !DILocation(line: 16, column: 12, scope: !182)
!260 = !DILocation(line: 14, column: 15, scope: !182)
!261 = !DILocation(line: 17, column: 16, scope: !262)
!262 = distinct !DILexicalBlock(scope: !182, file: !90, line: 17, column: 11)
!263 = !DILocation(line: 17, column: 11, scope: !182)
!264 = !DILocation(line: 19, column: 18, scope: !182)
!265 = !DILocation(line: 19, column: 11, scope: !182)
!266 = !DILocation(line: 19, column: 16, scope: !182)
!267 = !{!268, !269, i64 0}
!268 = !{!"", !269, i64 0, !124, i64 4, !124, i64 5, !124, i64 6, !124, i64 7}
!269 = !{!"any pointer", !124, i64 0}
!270 = !DILocation(line: 20, column: 16, scope: !271)
!271 = distinct !DILexicalBlock(scope: !182, file: !90, line: 20, column: 11)
!272 = !DILocation(line: 20, column: 11, scope: !182)
!273 = !DILocation(line: 22, column: 13, scope: !274)
!274 = distinct !DILexicalBlock(scope: !271, file: !90, line: 21, column: 7)
!275 = !DILocation(line: 23, column: 13, scope: !274)
!276 = !DILocation(line: 25, column: 11, scope: !182)
!277 = !DILocation(line: 25, column: 16, scope: !182)
!278 = !{!268, !124, i64 5}
!279 = !DILocation(line: 26, column: 11, scope: !182)
!280 = !DILocation(line: 26, column: 16, scope: !182)
!281 = !{!268, !124, i64 7}
!282 = !DILocation(line: 27, column: 7, scope: !182)
!283 = !DILocation(line: 28, column: 1, scope: !182)
!284 = distinct !DISubprogram(name: "bfread", scope: !90, file: !90, line: 30, type: !285, isLocal: false, isDefinition: true, scopeLine: 31, flags: DIFlagPrototyped, isOptimized: true, unit: !89, variables: !287)
!285 = !DISubroutineType(types: !286)
!286 = !{!18, !185}
!287 = !{!288}
!288 = !DILocalVariable(name: "bf", arg: 1, scope: !284, file: !90, line: 30, type: !185)
!289 = !DILocation(line: 30, column: 19, scope: !284)
!290 = !DILocation(line: 32, column: 20, scope: !291)
!291 = distinct !DILexicalBlock(scope: !284, file: !90, line: 32, column: 11)
!292 = !DILocation(line: 32, column: 13, scope: !291)
!293 = !DILocation(line: 32, column: 11, scope: !284)
!294 = !{!268, !124, i64 4}
!295 = !DILocation(line: 34, column: 40, scope: !296)
!296 = distinct !DILexicalBlock(scope: !291, file: !90, line: 33, column: 7)
!297 = !DILocation(line: 34, column: 30, scope: !296)
!298 = !DILocation(line: 34, column: 24, scope: !296)
!299 = !DILocation(line: 34, column: 17, scope: !296)
!300 = !DILocation(line: 34, column: 22, scope: !296)
!301 = !DILocation(line: 35, column: 22, scope: !296)
!302 = !DILocation(line: 36, column: 7, scope: !296)
!303 = !DILocation(line: 38, column: 19, scope: !284)
!304 = !DILocation(line: 37, column: 15, scope: !284)
!305 = !DILocation(line: 38, column: 15, scope: !284)
!306 = !DILocation(line: 38, column: 32, scope: !284)
!307 = !DILocation(line: 38, column: 29, scope: !284)
!308 = !DILocation(line: 38, column: 24, scope: !284)
!309 = !DILocation(line: 38, column: 43, scope: !284)
!310 = !DILocation(line: 38, column: 7, scope: !284)
!311 = distinct !DISubprogram(name: "bfwrite", scope: !90, file: !90, line: 41, type: !312, isLocal: false, isDefinition: true, scopeLine: 42, flags: DIFlagPrototyped, isOptimized: true, unit: !89, variables: !314)
!312 = !DISubroutineType(types: !313)
!313 = !{null, !18, !185}
!314 = !{!315, !316}
!315 = !DILocalVariable(name: "bit", arg: 1, scope: !311, file: !90, line: 41, type: !18)
!316 = !DILocalVariable(name: "bf", arg: 2, scope: !311, file: !90, line: 41, type: !185)
!317 = !DILocation(line: 41, column: 18, scope: !311)
!318 = !DILocation(line: 41, column: 30, scope: !311)
!319 = !DILocation(line: 43, column: 20, scope: !320)
!320 = distinct !DILexicalBlock(scope: !311, file: !90, line: 43, column: 11)
!321 = !DILocation(line: 43, column: 13, scope: !320)
!322 = !DILocation(line: 43, column: 11, scope: !311)
!323 = !DILocation(line: 45, column: 23, scope: !324)
!324 = distinct !DILexicalBlock(scope: !320, file: !90, line: 44, column: 7)
!325 = !{!268, !124, i64 6}
!326 = !DILocation(line: 45, column: 19, scope: !324)
!327 = !DILocation(line: 45, column: 33, scope: !324)
!328 = !DILocation(line: 45, column: 13, scope: !324)
!329 = !DILocation(line: 46, column: 22, scope: !324)
!330 = !DILocation(line: 47, column: 7, scope: !324)
!331 = !DILocation(line: 48, column: 15, scope: !311)
!332 = !DILocation(line: 49, column: 16, scope: !311)
!333 = !DILocation(line: 50, column: 23, scope: !311)
!334 = !DILocation(line: 50, column: 16, scope: !311)
!335 = !DILocation(line: 51, column: 1, scope: !311)
!336 = distinct !DISubprogram(name: "bfclose", scope: !90, file: !90, line: 53, type: !337, isLocal: false, isDefinition: true, scopeLine: 54, flags: DIFlagPrototyped, isOptimized: true, unit: !89, variables: !339)
!337 = !DISubroutineType(types: !338)
!338 = !{null, !185}
!339 = !{!340}
!340 = !DILocalVariable(name: "bf", arg: 1, scope: !336, file: !90, line: 53, type: !185)
!341 = !DILocation(line: 53, column: 21, scope: !336)
!342 = !DILocation(line: 55, column: 18, scope: !336)
!343 = !DILocation(line: 55, column: 7, scope: !336)
!344 = !DILocation(line: 56, column: 12, scope: !336)
!345 = !DILocation(line: 56, column: 7, scope: !336)
!346 = !DILocation(line: 57, column: 1, scope: !336)
!347 = !DILocation(line: 21, column: 14, scope: !39)
!348 = !DILocation(line: 21, column: 26, scope: !39)
!349 = !DILocation(line: 24, column: 14, scope: !39)
!350 = !DILocation(line: 24, column: 30, scope: !39)
!351 = !DILocation(line: 48, column: 11, scope: !352)
!352 = distinct !DILexicalBlock(scope: !39, file: !40, line: 48, column: 7)
!353 = !DILocation(line: 48, column: 7, scope: !39)
!354 = !DILocation(line: 49, column: 13, scope: !355)
!355 = distinct !DILexicalBlock(scope: !352, file: !40, line: 48, column: 15)
!356 = !{!269, !269, i64 0}
!357 = !DILocation(line: 49, column: 5, scope: !355)
!358 = !DILocation(line: 50, column: 5, scope: !355)
!359 = !DILocation(line: 52, column: 19, scope: !39)
!360 = !DILocalVariable(name: "__nptr", arg: 1, scope: !361, file: !362, line: 278, type: !365)
!361 = distinct !DISubprogram(name: "atoi", scope: !362, file: !362, line: 278, type: !363, isLocal: false, isDefinition: true, scopeLine: 279, flags: DIFlagPrototyped, isOptimized: true, unit: !46, variables: !367)
!362 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/stdlib.h", directory: "/home/shail/Results-extra/MiBench/bitcount-ccf/bitcount2")
!363 = !DISubroutineType(types: !364)
!364 = !{!18, !365}
!365 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !366, size: 32)
!366 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !45)
!367 = !{!360}
!368 = !DILocation(line: 278, column: 1, scope: !361, inlinedAt: !369)
!369 = distinct !DILocation(line: 52, column: 14, scope: !39)
!370 = !DILocation(line: 280, column: 16, scope: !361, inlinedAt: !369)
!371 = !DILocation(line: 27, column: 7, scope: !39)
!372 = !DILocation(line: 54, column: 3, scope: !39)
!373 = !DILocation(line: 25, column: 7, scope: !39)
!374 = !DILocation(line: 25, column: 18, scope: !39)
!375 = !DILocation(line: 25, column: 10, scope: !39)
!376 = !DILocation(line: 57, column: 13, scope: !377)
!377 = distinct !DILexicalBlock(scope: !378, file: !40, line: 56, column: 31)
!378 = distinct !DILexicalBlock(scope: !379, file: !40, line: 56, column: 3)
!379 = distinct !DILexicalBlock(scope: !39, file: !40, line: 56, column: 3)
!380 = !DILocation(line: 23, column: 11, scope: !39)
!381 = !DILocation(line: 26, column: 11, scope: !39)
!382 = !DILocation(line: 26, column: 8, scope: !39)
!383 = !DILocation(line: 59, column: 28, scope: !384)
!384 = distinct !DILexicalBlock(scope: !377, file: !40, line: 59, column: 5)
!385 = !DILocation(line: 56, column: 3, scope: !379)
!386 = !DILocation(line: 16, column: 13, scope: !387, inlinedAt: !392)
!387 = distinct !DILexicalBlock(scope: !388, file: !104, line: 16, column: 13)
!388 = distinct !DISubprogram(name: "bit_count", scope: !104, file: !104, line: 9, type: !54, isLocal: false, isDefinition: true, scopeLine: 10, flags: DIFlagPrototyped, isOptimized: true, unit: !103, variables: !389)
!389 = !{!390, !391}
!390 = !DILocalVariable(name: "x", arg: 1, scope: !388, file: !104, line: 9, type: !8)
!391 = !DILocalVariable(name: "n", scope: !388, file: !104, line: 11, type: !18)
!392 = distinct !DILocation(line: 60, column: 8, scope: !393)
!393 = distinct !DILexicalBlock(scope: !384, file: !40, line: 59, column: 5)
!394 = !DILocation(line: 62, column: 12, scope: !377)
!395 = !DILocation(line: 23, column: 18, scope: !39)
!396 = !DILocation(line: 63, column: 16, scope: !377)
!397 = !DILocation(line: 63, column: 10, scope: !377)
!398 = !DILocation(line: 63, column: 25, scope: !377)
!399 = !DILocation(line: 24, column: 10, scope: !39)
!400 = !DILocation(line: 64, column: 12, scope: !401)
!401 = distinct !DILexicalBlock(scope: !377, file: !40, line: 64, column: 9)
!402 = !DILocation(line: 64, column: 9, scope: !377)
!403 = !DILocation(line: 68, column: 12, scope: !404)
!404 = distinct !DILexicalBlock(scope: !377, file: !40, line: 68, column: 9)
!405 = !DILocation(line: 68, column: 9, scope: !377)
!406 = !DILocation(line: 73, column: 5, scope: !377)
!407 = !DILocation(line: 75, column: 28, scope: !39)
!408 = !DILocation(line: 9, column: 26, scope: !388, inlinedAt: !392)
!409 = !DILocation(line: 11, column: 13, scope: !388, inlinedAt: !392)
!410 = !DILocation(line: 16, column: 13, scope: !388, inlinedAt: !392)
!411 = distinct !{!411, !412, !413}
!412 = !DILocation(line: 16, column: 16, scope: !387)
!413 = !DILocation(line: 18, column: 34, scope: !387)
!414 = !DILocation(line: 17, column: 16, scope: !387, inlinedAt: !392)
!415 = !DILocation(line: 18, column: 30, scope: !387, inlinedAt: !392)
!416 = !DILocation(line: 18, column: 27, scope: !387, inlinedAt: !392)
!417 = !DILocation(line: 18, column: 18, scope: !387, inlinedAt: !392)
!418 = !DILocation(line: 17, column: 15, scope: !387, inlinedAt: !392)
!419 = !DILocation(line: 60, column: 5, scope: !393)
!420 = !DILocation(line: 59, column: 53, scope: !393)
!421 = !DILocation(line: 59, column: 62, scope: !393)
!422 = !DILocation(line: 26, column: 14, scope: !39)
!423 = !DILocation(line: 59, column: 38, scope: !393)
!424 = !DILocation(line: 59, column: 5, scope: !384)
!425 = distinct !{!425, !424, !426}
!426 = !DILocation(line: 60, column: 27, scope: !384)
!427 = !DILocation(line: 11, column: 30, scope: !428, inlinedAt: !431)
!428 = distinct !DISubprogram(name: "bitcount", scope: !101, file: !101, line: 9, type: !54, isLocal: false, isDefinition: true, scopeLine: 10, flags: DIFlagPrototyped, isOptimized: true, unit: !100, variables: !429)
!429 = !{!430}
!430 = !DILocalVariable(name: "i", arg: 1, scope: !428, file: !101, line: 9, type: !8)
!431 = distinct !DILocation(line: 60, column: 8, scope: !393)
!432 = !DILocation(line: 11, column: 42, scope: !428, inlinedAt: !431)
!433 = !DILocation(line: 11, column: 37, scope: !428, inlinedAt: !431)
!434 = !DILocation(line: 12, column: 30, scope: !428, inlinedAt: !431)
!435 = !DILocation(line: 12, column: 42, scope: !428, inlinedAt: !431)
!436 = !DILocation(line: 12, column: 37, scope: !428, inlinedAt: !431)
!437 = !DILocation(line: 13, column: 30, scope: !428, inlinedAt: !431)
!438 = !DILocation(line: 13, column: 42, scope: !428, inlinedAt: !431)
!439 = !DILocation(line: 13, column: 37, scope: !428, inlinedAt: !431)
!440 = !DILocation(line: 14, column: 30, scope: !428, inlinedAt: !431)
!441 = !DILocation(line: 14, column: 42, scope: !428, inlinedAt: !431)
!442 = !DILocation(line: 14, column: 37, scope: !428, inlinedAt: !431)
!443 = !DILocation(line: 15, column: 30, scope: !428, inlinedAt: !431)
!444 = !DILocation(line: 15, column: 42, scope: !428, inlinedAt: !431)
!445 = !DILocation(line: 15, column: 37, scope: !428, inlinedAt: !431)
!446 = distinct !{!446, !424, !426, !447, !448}
!447 = !{!"llvm.loop.vectorize.width", i32 1}
!448 = !{!"llvm.loop.interleave.count", i32 1}
!449 = !DILocation(line: 75, column: 3, scope: !39)
!450 = !DILocation(line: 76, column: 26, scope: !39)
!451 = !DILocation(line: 76, column: 3, scope: !39)
!452 = !DILocation(line: 77, column: 3, scope: !39)
!453 = !DILocation(line: 9, column: 25, scope: !428, inlinedAt: !431)
!454 = distinct !{!454, !424, !426, !455, !447, !448}
!455 = !{!"llvm.loop.unroll.runtime.disable"}
!456 = !DILocation(line: 60, column: 8, scope: !393)
!457 = !DILocalVariable(name: "x", arg: 1, scope: !458, file: !93, line: 42, type: !8)
!458 = distinct !DISubprogram(name: "ntbl_bitcount", scope: !93, file: !93, line: 42, type: !54, isLocal: false, isDefinition: true, scopeLine: 43, flags: DIFlagPrototyped, isOptimized: true, unit: !92, variables: !459)
!459 = !{!457}
!460 = !DILocation(line: 42, column: 34, scope: !458, inlinedAt: !461)
!461 = distinct !DILocation(line: 60, column: 8, scope: !393)
!462 = !DILocation(line: 45, column: 28, scope: !458, inlinedAt: !461)
!463 = !DILocation(line: 45, column: 13, scope: !458, inlinedAt: !461)
!464 = !DILocation(line: 46, column: 44, scope: !458, inlinedAt: !461)
!465 = !DILocation(line: 46, column: 13, scope: !458, inlinedAt: !461)
!466 = !DILocation(line: 47, column: 44, scope: !458, inlinedAt: !461)
!467 = !DILocation(line: 47, column: 13, scope: !458, inlinedAt: !461)
!468 = !DILocation(line: 48, column: 44, scope: !458, inlinedAt: !461)
!469 = !DILocation(line: 48, column: 13, scope: !458, inlinedAt: !461)
!470 = !DILocation(line: 49, column: 44, scope: !458, inlinedAt: !461)
!471 = !DILocation(line: 49, column: 13, scope: !458, inlinedAt: !461)
!472 = !DILocation(line: 50, column: 44, scope: !458, inlinedAt: !461)
!473 = !DILocation(line: 50, column: 13, scope: !458, inlinedAt: !461)
!474 = !DILocation(line: 51, column: 44, scope: !458, inlinedAt: !461)
!475 = !DILocation(line: 51, column: 13, scope: !458, inlinedAt: !461)
!476 = !DILocation(line: 52, column: 44, scope: !458, inlinedAt: !461)
!477 = !DILocation(line: 52, column: 13, scope: !458, inlinedAt: !461)
!478 = !DILocation(line: 45, column: 52, scope: !458, inlinedAt: !461)
!479 = !DILocation(line: 46, column: 52, scope: !458, inlinedAt: !461)
!480 = !DILocation(line: 47, column: 52, scope: !458, inlinedAt: !461)
!481 = !DILocation(line: 48, column: 52, scope: !458, inlinedAt: !461)
!482 = !DILocation(line: 49, column: 52, scope: !458, inlinedAt: !461)
!483 = !DILocation(line: 50, column: 52, scope: !458, inlinedAt: !461)
!484 = !DILocation(line: 51, column: 52, scope: !458, inlinedAt: !461)
!485 = !DILocalVariable(name: "x", arg: 1, scope: !486, file: !93, line: 61, type: !8)
!486 = distinct !DISubprogram(name: "BW_btbl_bitcount", scope: !93, file: !93, line: 61, type: !54, isLocal: false, isDefinition: true, scopeLine: 62, flags: DIFlagPrototyped, isOptimized: true, unit: !92, variables: !487)
!487 = !{!485, !488}
!488 = !DILocalVariable(name: "U", scope: !486, file: !93, line: 67, type: !489)
!489 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !486, file: !93, line: 63, size: 32, elements: !490)
!490 = !{!491, !495}
!491 = !DIDerivedType(tag: DW_TAG_member, name: "ch", scope: !489, file: !93, line: 65, baseType: !492, size: 32)
!492 = !DICompositeType(tag: DW_TAG_array_type, baseType: !96, size: 32, elements: !493)
!493 = !{!494}
!494 = !DISubrange(count: 4)
!495 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !489, file: !93, line: 66, baseType: !8, size: 32)
!496 = !DILocation(line: 61, column: 37, scope: !486, inlinedAt: !497)
!497 = distinct !DILocation(line: 60, column: 8, scope: !393)
!498 = !DILocation(line: 67, column: 9, scope: !486, inlinedAt: !497)
!499 = !DILocation(line: 69, column: 11, scope: !486, inlinedAt: !497)
!500 = !DILocation(line: 71, column: 14, scope: !486, inlinedAt: !497)
!501 = !DILocation(line: 71, column: 32, scope: !486, inlinedAt: !497)
!502 = !DILocation(line: 72, column: 14, scope: !486, inlinedAt: !497)
!503 = !DILocation(line: 72, column: 32, scope: !486, inlinedAt: !497)
!504 = !DILocation(line: 71, column: 30, scope: !486, inlinedAt: !497)
!505 = !DILocation(line: 71, column: 48, scope: !486, inlinedAt: !497)
!506 = !DILocation(line: 72, column: 30, scope: !486, inlinedAt: !497)
!507 = !DILocalVariable(name: "x", arg: 1, scope: !508, file: !93, line: 81, type: !8)
!508 = distinct !DISubprogram(name: "AR_btbl_bitcount", scope: !93, file: !93, line: 81, type: !54, isLocal: false, isDefinition: true, scopeLine: 82, flags: DIFlagPrototyped, isOptimized: true, unit: !92, variables: !509)
!509 = !{!507, !510, !511}
!510 = !DILocalVariable(name: "Ptr", scope: !508, file: !93, line: 83, type: !95)
!511 = !DILocalVariable(name: "Accu", scope: !508, file: !93, line: 84, type: !18)
!512 = !DILocation(line: 81, column: 37, scope: !508, inlinedAt: !513)
!513 = distinct !DILocation(line: 60, column: 8, scope: !393)
!514 = !DILocation(line: 83, column: 23, scope: !508, inlinedAt: !513)
!515 = !DILocation(line: 86, column: 15, scope: !508, inlinedAt: !513)
!516 = !DILocation(line: 84, column: 11, scope: !508, inlinedAt: !513)
!517 = !DILocation(line: 87, column: 15, scope: !508, inlinedAt: !513)
!518 = !DILocation(line: 88, column: 15, scope: !508, inlinedAt: !513)
!519 = !DILocation(line: 89, column: 15, scope: !508, inlinedAt: !513)
!520 = !DILocation(line: 87, column: 12, scope: !508, inlinedAt: !513)
!521 = !DILocation(line: 88, column: 12, scope: !508, inlinedAt: !513)
!522 = !DILocation(line: 89, column: 12, scope: !508, inlinedAt: !513)
!523 = !DILocalVariable(name: "x", arg: 1, scope: !524, file: !40, line: 80, type: !8)
!524 = distinct !DISubprogram(name: "bit_shifter", scope: !40, file: !40, line: 80, type: !54, isLocal: true, isDefinition: true, scopeLine: 81, flags: DIFlagPrototyped, isOptimized: true, unit: !46, variables: !525)
!525 = !{!523, !526, !527}
!526 = !DILocalVariable(name: "i", scope: !524, file: !40, line: 82, type: !18)
!527 = !DILocalVariable(name: "n", scope: !524, file: !40, line: 82, type: !18)
!528 = !DILocation(line: 80, column: 39, scope: !524, inlinedAt: !529)
!529 = distinct !DILocation(line: 60, column: 8, scope: !393)
!530 = !DILocation(line: 82, column: 10, scope: !524, inlinedAt: !529)
!531 = !DILocation(line: 82, column: 7, scope: !524, inlinedAt: !529)
!532 = !DILocation(line: 84, column: 19, scope: !533, inlinedAt: !529)
!533 = distinct !DILexicalBlock(scope: !534, file: !40, line: 84, column: 3)
!534 = distinct !DILexicalBlock(scope: !524, file: !40, line: 84, column: 3)
!535 = !DILocation(line: 84, column: 3, scope: !534, inlinedAt: !529)
!536 = !DILocation(line: 85, column: 18, scope: !533, inlinedAt: !529)
!537 = !DILocation(line: 85, column: 7, scope: !533, inlinedAt: !529)
!538 = !DILocation(line: 84, column: 57, scope: !533, inlinedAt: !529)
!539 = !DILocation(line: 84, column: 64, scope: !533, inlinedAt: !529)
!540 = !DILocation(line: 84, column: 27, scope: !533, inlinedAt: !529)
!541 = !DILocation(line: 84, column: 21, scope: !533, inlinedAt: !529)
!542 = distinct !{!542, !543, !544}
!543 = !DILocation(line: 84, column: 3, scope: !534)
!544 = !DILocation(line: 85, column: 22, scope: !534)
!545 = !DILocation(line: 9, column: 26, scope: !388)
!546 = !DILocation(line: 11, column: 13, scope: !388)
!547 = !DILocation(line: 16, column: 13, scope: !387)
!548 = !DILocation(line: 16, column: 13, scope: !388)
!549 = !DILocation(line: 17, column: 16, scope: !387)
!550 = !DILocation(line: 18, column: 30, scope: !387)
!551 = !DILocation(line: 18, column: 27, scope: !387)
!552 = !DILocation(line: 18, column: 18, scope: !387)
!553 = !DILocation(line: 17, column: 15, scope: !387)
!554 = !DILocation(line: 19, column: 9, scope: !388)
!555 = !DILocation(line: 9, column: 25, scope: !428)
!556 = !DILocation(line: 11, column: 30, scope: !428)
!557 = !DILocation(line: 11, column: 42, scope: !428)
!558 = !DILocation(line: 11, column: 37, scope: !428)
!559 = !DILocation(line: 12, column: 30, scope: !428)
!560 = !DILocation(line: 12, column: 42, scope: !428)
!561 = !DILocation(line: 12, column: 37, scope: !428)
!562 = !DILocation(line: 13, column: 30, scope: !428)
!563 = !DILocation(line: 13, column: 42, scope: !428)
!564 = !DILocation(line: 13, column: 37, scope: !428)
!565 = !DILocation(line: 14, column: 30, scope: !428)
!566 = !DILocation(line: 14, column: 42, scope: !428)
!567 = !DILocation(line: 14, column: 37, scope: !428)
!568 = !DILocation(line: 15, column: 30, scope: !428)
!569 = !DILocation(line: 15, column: 42, scope: !428)
!570 = !DILocation(line: 15, column: 37, scope: !428)
!571 = !DILocation(line: 16, column: 7, scope: !428)
!572 = distinct !DISubprogram(name: "ntbl_bitcnt", scope: !79, file: !79, line: 38, type: !54, isLocal: false, isDefinition: true, scopeLine: 39, flags: DIFlagPrototyped, isOptimized: true, unit: !78, variables: !573)
!573 = !{!574, !575}
!574 = !DILocalVariable(name: "x", arg: 1, scope: !572, file: !79, line: 38, type: !8)
!575 = !DILocalVariable(name: "cnt", scope: !572, file: !79, line: 40, type: !18)
!576 = !DILocation(line: 38, column: 28, scope: !572)
!577 = !DILocation(line: 40, column: 30, scope: !572)
!578 = !DILocation(line: 40, column: 17, scope: !572)
!579 = !DILocation(line: 40, column: 11, scope: !572)
!580 = !DILocation(line: 42, column: 20, scope: !581)
!581 = distinct !DILexicalBlock(scope: !572, file: !79, line: 42, column: 11)
!582 = !DILocation(line: 42, column: 14, scope: !581)
!583 = !DILocation(line: 42, column: 11, scope: !572)
!584 = !DILocation(line: 43, column: 20, scope: !581)
!585 = !DILocation(line: 43, column: 17, scope: !581)
!586 = !DILocation(line: 45, column: 7, scope: !572)
!587 = !DILocation(line: 42, column: 34, scope: !458)
!588 = !DILocation(line: 45, column: 28, scope: !458)
!589 = !DILocation(line: 45, column: 13, scope: !458)
!590 = !DILocation(line: 46, column: 44, scope: !458)
!591 = !DILocation(line: 46, column: 13, scope: !458)
!592 = !DILocation(line: 45, column: 52, scope: !458)
!593 = !DILocation(line: 47, column: 44, scope: !458)
!594 = !DILocation(line: 47, column: 13, scope: !458)
!595 = !DILocation(line: 46, column: 52, scope: !458)
!596 = !DILocation(line: 48, column: 44, scope: !458)
!597 = !DILocation(line: 48, column: 13, scope: !458)
!598 = !DILocation(line: 47, column: 52, scope: !458)
!599 = !DILocation(line: 49, column: 44, scope: !458)
!600 = !DILocation(line: 49, column: 13, scope: !458)
!601 = !DILocation(line: 48, column: 52, scope: !458)
!602 = !DILocation(line: 50, column: 44, scope: !458)
!603 = !DILocation(line: 50, column: 13, scope: !458)
!604 = !DILocation(line: 49, column: 52, scope: !458)
!605 = !DILocation(line: 51, column: 44, scope: !458)
!606 = !DILocation(line: 51, column: 13, scope: !458)
!607 = !DILocation(line: 50, column: 52, scope: !458)
!608 = !DILocation(line: 52, column: 44, scope: !458)
!609 = !DILocation(line: 52, column: 13, scope: !458)
!610 = !DILocation(line: 51, column: 52, scope: !458)
!611 = !DILocation(line: 44, column: 7, scope: !458)
!612 = !DILocation(line: 61, column: 37, scope: !486)
!613 = !DILocation(line: 67, column: 9, scope: !486)
!614 = !DILocation(line: 69, column: 11, scope: !486)
!615 = !DILocation(line: 71, column: 14, scope: !486)
!616 = !DILocation(line: 71, column: 32, scope: !486)
!617 = !DILocation(line: 71, column: 30, scope: !486)
!618 = !DILocation(line: 72, column: 14, scope: !486)
!619 = !DILocation(line: 71, column: 48, scope: !486)
!620 = !DILocation(line: 72, column: 32, scope: !486)
!621 = !DILocation(line: 72, column: 30, scope: !486)
!622 = !DILocation(line: 71, column: 7, scope: !486)
!623 = !DILocation(line: 81, column: 37, scope: !508)
!624 = !DILocation(line: 83, column: 23, scope: !508)
!625 = !DILocation(line: 86, column: 15, scope: !508)
!626 = !DILocation(line: 84, column: 11, scope: !508)
!627 = !DILocation(line: 87, column: 15, scope: !508)
!628 = !DILocation(line: 87, column: 12, scope: !508)
!629 = !DILocation(line: 88, column: 15, scope: !508)
!630 = !DILocation(line: 88, column: 12, scope: !508)
!631 = !DILocation(line: 89, column: 15, scope: !508)
!632 = !DILocation(line: 89, column: 12, scope: !508)
!633 = !DILocation(line: 90, column: 7, scope: !508)
!634 = distinct !DISubprogram(name: "btbl_bitcnt", scope: !79, file: !79, line: 52, type: !54, isLocal: false, isDefinition: true, scopeLine: 53, flags: DIFlagPrototyped, isOptimized: true, unit: !78, variables: !635)
!635 = !{!636, !637}
!636 = !DILocalVariable(name: "x", arg: 1, scope: !634, file: !79, line: 52, type: !8)
!637 = !DILocalVariable(name: "cnt", scope: !634, file: !79, line: 54, type: !18)
!638 = !DIExpression(DW_OP_LLVM_fragment, 8, 24)
!639 = !DILocation(line: 52, column: 28, scope: !634)
!640 = !DILocation(line: 54, column: 39, scope: !634)
!641 = !DILocation(line: 54, column: 17, scope: !634)
!642 = !DILocation(line: 54, column: 11, scope: !634)
!643 = !DILocation(line: 56, column: 20, scope: !644)
!644 = distinct !DILexicalBlock(scope: !634, file: !79, line: 56, column: 11)
!645 = !DILocation(line: 56, column: 14, scope: !644)
!646 = !DILocation(line: 56, column: 11, scope: !634)
!647 = !DILocation(line: 57, column: 20, scope: !644)
!648 = !DILocation(line: 57, column: 17, scope: !644)
!649 = !DILocation(line: 58, column: 7, scope: !634)
!650 = distinct !DISubprogram(name: "initializeParameters", scope: !3, file: !3, line: 24, type: !651, isLocal: false, isDefinition: true, scopeLine: 25, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !653)
!651 = !DISubroutineType(types: !652)
!652 = !{!18, !7}
!653 = !{!654, !655, !656, !697, !698, !699, !700, !704, !708, !709, !710, !711, !712, !713, !714, !715, !716, !717, !718, !719, !720, !721, !724}
!654 = !DILocalVariable(name: "loopID", arg: 1, scope: !650, file: !3, line: 24, type: !7)
!655 = !DILocalVariable(name: "i", scope: !650, file: !3, line: 26, type: !7)
!656 = !DILocalVariable(name: "epi", scope: !650, file: !3, line: 36, type: !657)
!657 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !658, size: 32)
!658 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !193, line: 48, baseType: !659)
!659 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !195, line: 245, size: 1216, elements: !660)
!660 = !{!661, !662, !663, !664, !665, !666, !667, !668, !669, !670, !671, !672, !673, !681, !682, !683, !684, !685, !686, !687, !688, !689, !690, !691, !692, !693, !694, !695, !696}
!661 = !DIDerivedType(tag: DW_TAG_member, name: "_flags", scope: !659, file: !195, line: 246, baseType: !18, size: 32)
!662 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_ptr", scope: !659, file: !195, line: 251, baseType: !44, size: 32, offset: 32)
!663 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_end", scope: !659, file: !195, line: 252, baseType: !44, size: 32, offset: 64)
!664 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_read_base", scope: !659, file: !195, line: 253, baseType: !44, size: 32, offset: 96)
!665 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_base", scope: !659, file: !195, line: 254, baseType: !44, size: 32, offset: 128)
!666 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_ptr", scope: !659, file: !195, line: 255, baseType: !44, size: 32, offset: 160)
!667 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_write_end", scope: !659, file: !195, line: 256, baseType: !44, size: 32, offset: 192)
!668 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_base", scope: !659, file: !195, line: 257, baseType: !44, size: 32, offset: 224)
!669 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_buf_end", scope: !659, file: !195, line: 258, baseType: !44, size: 32, offset: 256)
!670 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_base", scope: !659, file: !195, line: 260, baseType: !44, size: 32, offset: 288)
!671 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_backup_base", scope: !659, file: !195, line: 261, baseType: !44, size: 32, offset: 320)
!672 = !DIDerivedType(tag: DW_TAG_member, name: "_IO_save_end", scope: !659, file: !195, line: 262, baseType: !44, size: 32, offset: 352)
!673 = !DIDerivedType(tag: DW_TAG_member, name: "_markers", scope: !659, file: !195, line: 264, baseType: !674, size: 32, offset: 384)
!674 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !675, size: 32)
!675 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_marker", file: !195, line: 160, size: 96, elements: !676)
!676 = !{!677, !678, !680}
!677 = !DIDerivedType(tag: DW_TAG_member, name: "_next", scope: !675, file: !195, line: 161, baseType: !674, size: 32)
!678 = !DIDerivedType(tag: DW_TAG_member, name: "_sbuf", scope: !675, file: !195, line: 162, baseType: !679, size: 32, offset: 32)
!679 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !659, size: 32)
!680 = !DIDerivedType(tag: DW_TAG_member, name: "_pos", scope: !675, file: !195, line: 166, baseType: !18, size: 32, offset: 64)
!681 = !DIDerivedType(tag: DW_TAG_member, name: "_chain", scope: !659, file: !195, line: 266, baseType: !679, size: 32, offset: 416)
!682 = !DIDerivedType(tag: DW_TAG_member, name: "_fileno", scope: !659, file: !195, line: 268, baseType: !18, size: 32, offset: 448)
!683 = !DIDerivedType(tag: DW_TAG_member, name: "_flags2", scope: !659, file: !195, line: 272, baseType: !18, size: 32, offset: 480)
!684 = !DIDerivedType(tag: DW_TAG_member, name: "_old_offset", scope: !659, file: !195, line: 274, baseType: !221, size: 32, offset: 512)
!685 = !DIDerivedType(tag: DW_TAG_member, name: "_cur_column", scope: !659, file: !195, line: 278, baseType: !223, size: 16, offset: 544)
!686 = !DIDerivedType(tag: DW_TAG_member, name: "_vtable_offset", scope: !659, file: !195, line: 279, baseType: !225, size: 8, offset: 560)
!687 = !DIDerivedType(tag: DW_TAG_member, name: "_shortbuf", scope: !659, file: !195, line: 280, baseType: !227, size: 8, offset: 568)
!688 = !DIDerivedType(tag: DW_TAG_member, name: "_lock", scope: !659, file: !195, line: 284, baseType: !231, size: 32, offset: 576)
!689 = !DIDerivedType(tag: DW_TAG_member, name: "_offset", scope: !659, file: !195, line: 293, baseType: !234, size: 64, offset: 640)
!690 = !DIDerivedType(tag: DW_TAG_member, name: "__pad1", scope: !659, file: !195, line: 302, baseType: !9, size: 32, offset: 704)
!691 = !DIDerivedType(tag: DW_TAG_member, name: "__pad2", scope: !659, file: !195, line: 303, baseType: !9, size: 32, offset: 736)
!692 = !DIDerivedType(tag: DW_TAG_member, name: "__pad3", scope: !659, file: !195, line: 304, baseType: !9, size: 32, offset: 768)
!693 = !DIDerivedType(tag: DW_TAG_member, name: "__pad4", scope: !659, file: !195, line: 305, baseType: !9, size: 32, offset: 800)
!694 = !DIDerivedType(tag: DW_TAG_member, name: "__pad5", scope: !659, file: !195, line: 306, baseType: !242, size: 32, offset: 832)
!695 = !DIDerivedType(tag: DW_TAG_member, name: "_mode", scope: !659, file: !195, line: 308, baseType: !18, size: 32, offset: 864)
!696 = !DIDerivedType(tag: DW_TAG_member, name: "_unused2", scope: !659, file: !195, line: 310, baseType: !246, size: 320, offset: 896)
!697 = !DILocalVariable(name: "pro", scope: !650, file: !3, line: 36, type: !657)
!698 = !DILocalVariable(name: "kern", scope: !650, file: !3, line: 36, type: !657)
!699 = !DILocalVariable(name: "count", scope: !650, file: !3, line: 36, type: !657)
!700 = !DILocalVariable(name: "loopno", scope: !650, file: !3, line: 37, type: !701)
!701 = !DICompositeType(tag: DW_TAG_array_type, baseType: !45, size: 200, elements: !702)
!702 = !{!703}
!703 = !DISubrange(count: 25)
!704 = !DILocalVariable(name: "directoryPath", scope: !650, file: !3, line: 38, type: !705)
!705 = !DICompositeType(tag: DW_TAG_array_type, baseType: !45, size: 160, elements: !706)
!706 = !{!707}
!707 = !DISubrange(count: 20)
!708 = !DILocalVariable(name: "epifile", scope: !650, file: !3, line: 43, type: !246)
!709 = !DILocalVariable(name: "profile", scope: !650, file: !3, line: 44, type: !246)
!710 = !DILocalVariable(name: "kernfile", scope: !650, file: !3, line: 45, type: !246)
!711 = !DILocalVariable(name: "liveOutfile", scope: !650, file: !3, line: 46, type: !246)
!712 = !DILocalVariable(name: "episize", scope: !650, file: !3, line: 62, type: !18)
!713 = !DILocalVariable(name: "prosize", scope: !650, file: !3, line: 62, type: !18)
!714 = !DILocalVariable(name: "kernelsize", scope: !650, file: !3, line: 62, type: !18)
!715 = !DILocalVariable(name: "livevar_st_size", scope: !650, file: !3, line: 62, type: !18)
!716 = !DILocalVariable(name: "II", scope: !650, file: !3, line: 86, type: !18)
!717 = !DILocalVariable(name: "epiLength", scope: !650, file: !3, line: 87, type: !18)
!718 = !DILocalVariable(name: "prolength", scope: !650, file: !3, line: 88, type: !18)
!719 = !DILocalVariable(name: "loopitfile", scope: !650, file: !3, line: 99, type: !246)
!720 = !DILocalVariable(name: "kernelCount", scope: !650, file: !3, line: 103, type: !18)
!721 = !DILocalVariable(name: "newTC", scope: !722, file: !3, line: 112, type: !18)
!722 = distinct !DILexicalBlock(scope: !723, file: !3, line: 111, column: 3)
!723 = distinct !DILexicalBlock(scope: !650, file: !3, line: 108, column: 6)
!724 = !DILocalVariable(name: "initCGRAfile", scope: !650, file: !3, line: 129, type: !246)
!725 = !DILocation(line: 24, column: 39, scope: !650)
!726 = !DILocation(line: 26, column: 16, scope: !650)
!727 = !DILocation(line: 28, column: 15, scope: !650)
!728 = !{!729, !729, i64 0}
!729 = !{!"int", !124, i64 0}
!730 = !DILocation(line: 29, column: 15, scope: !650)
!731 = !DILocation(line: 37, column: 3, scope: !650)
!732 = !DILocation(line: 31, column: 15, scope: !650)
!733 = !DILocation(line: 37, column: 8, scope: !650)
!734 = !DILocation(line: 38, column: 3, scope: !650)
!735 = !DILocation(line: 38, column: 8, scope: !650)
!736 = !DILocation(line: 40, column: 3, scope: !650)
!737 = !DILocation(line: 41, column: 3, scope: !650)
!738 = !DILocation(line: 43, column: 3, scope: !650)
!739 = !DILocation(line: 43, column: 8, scope: !650)
!740 = !DILocation(line: 44, column: 3, scope: !650)
!741 = !DILocation(line: 44, column: 8, scope: !650)
!742 = !DILocation(line: 45, column: 3, scope: !650)
!743 = !DILocation(line: 45, column: 8, scope: !650)
!744 = !DILocation(line: 46, column: 3, scope: !650)
!745 = !DILocation(line: 46, column: 8, scope: !650)
!746 = !DILocation(line: 48, column: 3, scope: !650)
!747 = !DILocation(line: 49, column: 3, scope: !650)
!748 = !DILocation(line: 51, column: 3, scope: !650)
!749 = !DILocation(line: 52, column: 3, scope: !650)
!750 = !DILocation(line: 54, column: 3, scope: !650)
!751 = !DILocation(line: 55, column: 3, scope: !650)
!752 = !DILocation(line: 58, column: 7, scope: !650)
!753 = !DILocation(line: 36, column: 9, scope: !650)
!754 = !DILocation(line: 59, column: 7, scope: !650)
!755 = !DILocation(line: 36, column: 14, scope: !650)
!756 = !DILocation(line: 60, column: 8, scope: !650)
!757 = !DILocation(line: 36, column: 19, scope: !650)
!758 = !DILocation(line: 62, column: 3, scope: !650)
!759 = !DILocation(line: 64, column: 3, scope: !650)
!760 = !DILocation(line: 65, column: 3, scope: !650)
!761 = !DILocation(line: 66, column: 3, scope: !650)
!762 = !DILocation(line: 68, column: 46, scope: !650)
!763 = !DILocation(line: 62, column: 7, scope: !650)
!764 = !DILocation(line: 68, column: 3, scope: !650)
!765 = !DILocation(line: 69, column: 46, scope: !650)
!766 = !DILocation(line: 62, column: 15, scope: !650)
!767 = !DILocation(line: 69, column: 3, scope: !650)
!768 = !DILocation(line: 70, column: 47, scope: !650)
!769 = !DILocation(line: 62, column: 23, scope: !650)
!770 = !DILocation(line: 70, column: 3, scope: !650)
!771 = !DILocation(line: 71, column: 3, scope: !650)
!772 = !DILocation(line: 73, column: 39, scope: !650)
!773 = !DILocation(line: 73, column: 25, scope: !650)
!774 = !DILocation(line: 73, column: 9, scope: !650)
!775 = !DILocation(line: 74, column: 39, scope: !650)
!776 = !DILocation(line: 74, column: 25, scope: !650)
!777 = !DILocation(line: 74, column: 9, scope: !650)
!778 = !DILocation(line: 75, column: 42, scope: !650)
!779 = !DILocation(line: 75, column: 25, scope: !650)
!780 = !DILocation(line: 75, column: 9, scope: !650)
!781 = !DILocation(line: 77, column: 3, scope: !650)
!782 = !DILocation(line: 78, column: 9, scope: !650)
!783 = !DILocation(line: 78, column: 3, scope: !650)
!784 = !DILocation(line: 79, column: 9, scope: !650)
!785 = !DILocation(line: 79, column: 3, scope: !650)
!786 = !DILocation(line: 81, column: 3, scope: !650)
!787 = !DILocation(line: 82, column: 3, scope: !650)
!788 = !DILocation(line: 83, column: 9, scope: !650)
!789 = !DILocation(line: 36, column: 26, scope: !650)
!790 = !DIExpression(DW_OP_deref)
!791 = !DILocation(line: 62, column: 35, scope: !650)
!792 = !DILocation(line: 84, column: 3, scope: !650)
!793 = !DILocation(line: 86, column: 22, scope: !650)
!794 = !DILocation(line: 86, column: 7, scope: !650)
!795 = !DILocation(line: 87, column: 26, scope: !650)
!796 = !DILocation(line: 87, column: 7, scope: !650)
!797 = !DILocation(line: 88, column: 26, scope: !650)
!798 = !DILocation(line: 88, column: 7, scope: !650)
!799 = !DILocation(line: 89, column: 15, scope: !650)
!800 = !DILocation(line: 90, column: 15, scope: !650)
!801 = !DILocation(line: 91, column: 15, scope: !650)
!802 = !DILocation(line: 92, column: 17, scope: !650)
!803 = !DILocation(line: 92, column: 32, scope: !650)
!804 = !DILocation(line: 92, column: 15, scope: !650)
!805 = !DILocation(line: 94, column: 3, scope: !650)
!806 = !DILocation(line: 95, column: 3, scope: !650)
!807 = !DILocation(line: 96, column: 3, scope: !650)
!808 = !DILocation(line: 97, column: 3, scope: !650)
!809 = !DILocation(line: 99, column: 3, scope: !650)
!810 = !DILocation(line: 99, column: 8, scope: !650)
!811 = !DILocation(line: 100, column: 3, scope: !650)
!812 = !DILocation(line: 101, column: 3, scope: !650)
!813 = !DILocation(line: 103, column: 3, scope: !650)
!814 = !DILocation(line: 103, column: 7, scope: !650)
!815 = !DILocation(line: 104, column: 9, scope: !650)
!816 = !DILocation(line: 105, column: 3, scope: !650)
!817 = !DILocation(line: 106, column: 3, scope: !650)
!818 = !DILocation(line: 107, column: 29, scope: !650)
!819 = !DILocation(line: 107, column: 3, scope: !650)
!820 = !DILocation(line: 108, column: 6, scope: !723)
!821 = !DILocation(line: 108, column: 18, scope: !723)
!822 = !DILocation(line: 112, column: 31, scope: !722)
!823 = !DILocation(line: 108, column: 6, scope: !650)
!824 = !DILocation(line: 129, column: 3, scope: !650)
!825 = !DILocation(line: 129, column: 8, scope: !650)
!826 = !DILocation(line: 130, column: 3, scope: !650)
!827 = !DILocation(line: 131, column: 11, scope: !650)
!828 = !DILocation(line: 133, column: 28, scope: !829)
!829 = distinct !DILexicalBlock(scope: !830, file: !3, line: 132, column: 3)
!830 = distinct !DILexicalBlock(scope: !650, file: !3, line: 132, column: 3)
!831 = !DILocation(line: 133, column: 5, scope: !829)
!832 = !DILocation(line: 134, column: 34, scope: !650)
!833 = !DILocation(line: 134, column: 3, scope: !650)
!834 = !DILocation(line: 135, column: 34, scope: !650)
!835 = !DILocation(line: 135, column: 3, scope: !650)
!836 = !DILocation(line: 136, column: 34, scope: !650)
!837 = !DILocation(line: 136, column: 3, scope: !650)
!838 = !DILocation(line: 137, column: 3, scope: !650)
!839 = !DILocation(line: 140, column: 1, scope: !650)
!840 = !DILocation(line: 139, column: 3, scope: !650)
!841 = distinct !DISubprogram(name: "updateLoopTC", scope: !3, file: !3, line: 143, type: !842, isLocal: false, isDefinition: true, scopeLine: 144, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !844)
!842 = !DISubroutineType(types: !843)
!843 = !{null, !7}
!844 = !{!845, !846, !847}
!845 = !DILocalVariable(name: "loopID", arg: 1, scope: !841, file: !3, line: 143, type: !7)
!846 = !DILocalVariable(name: "loopno", scope: !841, file: !3, line: 149, type: !701)
!847 = !DILocalVariable(name: "directoryPath", scope: !841, file: !3, line: 150, type: !705)
!848 = !DILocation(line: 143, column: 32, scope: !841)
!849 = !DILocation(line: 146, column: 6, scope: !850)
!850 = distinct !DILexicalBlock(scope: !841, file: !3, line: 146, column: 6)
!851 = !DILocation(line: 146, column: 18, scope: !850)
!852 = !DILocation(line: 146, column: 6, scope: !841)
!853 = !DILocation(line: 152, column: 17, scope: !841)
!854 = !DILocation(line: 152, column: 15, scope: !841)
!855 = !DILocation(line: 163, column: 1, scope: !841)
!856 = distinct !DISubprogram(name: "runOnCGRA", scope: !3, file: !3, line: 165, type: !857, isLocal: false, isDefinition: true, scopeLine: 166, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !859)
!857 = !DISubroutineType(types: !858)
!858 = !{!9, !9}
!859 = !{!860}
!860 = !DILocalVariable(name: "arg", arg: 1, scope: !856, file: !3, line: 165, type: !9)
!861 = !DILocation(line: 165, column: 23, scope: !856)
!862 = !DILocation(line: 168, column: 5, scope: !856)
!863 = !DILocation(line: 169, column: 2, scope: !864)
!864 = distinct !DILexicalBlock(scope: !856, file: !3, line: 168, column: 14)
!865 = !DILocation(line: 171, column: 15, scope: !864)
!866 = !DILocation(line: 171, column: 32, scope: !864)
!867 = !DILocation(line: 171, column: 9, scope: !864)
!868 = !DILocation(line: 171, column: 38, scope: !864)
!869 = distinct !{!869, !867, !870}
!870 = !DILocation(line: 171, column: 46, scope: !864)
!871 = !DILocation(line: 174, column: 12, scope: !872)
!872 = distinct !DILexicalBlock(scope: !864, file: !3, line: 174, column: 12)
!873 = !DILocation(line: 174, column: 24, scope: !872)
!874 = !DILocation(line: 174, column: 12, scope: !864)
!875 = !DILocation(line: 177, column: 7, scope: !876)
!876 = distinct !DILexicalBlock(scope: !872, file: !3, line: 174, column: 30)
!877 = !{i32 4492}
!878 = !DILocation(line: 205, column: 3, scope: !856)
!879 = !DILocation(line: 182, column: 19, scope: !864)
!880 = !DILocation(line: 185, column: 2, scope: !864)
!881 = !DILocation(line: 196, column: 2, scope: !864)
!882 = !{i32 5516}
!883 = !DILocation(line: 198, column: 2, scope: !864)
!884 = !DILocation(line: 199, column: 9, scope: !864)
!885 = !DILocation(line: 200, column: 18, scope: !864)
!886 = distinct !{!886, !862, !887}
!887 = !DILocation(line: 204, column: 5, scope: !856)
!888 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !3, file: !3, line: 208, type: !842, isLocal: false, isDefinition: true, scopeLine: 209, flags: DIFlagPrototyped, isOptimized: true, unit: !2, variables: !889)
!889 = !{!890, !891, !892}
!890 = !DILocalVariable(name: "loopNo", arg: 1, scope: !888, file: !3, line: 208, type: !7)
!891 = !DILocalVariable(name: "result", scope: !888, file: !3, line: 210, type: !18)
!892 = !DILocalVariable(name: "initialize", scope: !888, file: !3, line: 213, type: !18)
!893 = !DILocation(line: 208, column: 36, scope: !888)
!894 = !DILocation(line: 210, column: 9, scope: !888)
!895 = !DILocation(line: 213, column: 22, scope: !888)
!896 = !DILocation(line: 213, column: 9, scope: !888)
!897 = !DILocation(line: 221, column: 21, scope: !888)
!898 = !DILocation(line: 222, column: 22, scope: !888)
!899 = !DILocation(line: 223, column: 5, scope: !888)
!900 = !DILocation(line: 224, column: 11, scope: !888)
!901 = !DILocation(line: 224, column: 27, scope: !888)
!902 = !DILocation(line: 224, column: 5, scope: !888)
!903 = !DILocation(line: 224, column: 33, scope: !888)
!904 = distinct !{!904, !902, !905}
!905 = !DILocation(line: 224, column: 41, scope: !888)
!906 = !DILocation(line: 225, column: 5, scope: !888)
!907 = !DILocation(line: 226, column: 1, scope: !888)
!908 = distinct !DISubprogram(name: "deleteCGRA", scope: !3, file: !3, line: 228, type: !909, isLocal: false, isDefinition: true, scopeLine: 229, isOptimized: true, unit: !2, variables: !4)
!909 = !DISubroutineType(types: !910)
!910 = !{null}
!911 = !DILocation(line: 230, column: 3, scope: !908)
!912 = !DILocation(line: 232, column: 15, scope: !908)
!913 = !DILocation(line: 233, column: 20, scope: !908)
!914 = !DILocation(line: 235, column: 3, scope: !908)
!915 = !DILocation(line: 236, column: 16, scope: !908)
!916 = !{!917, !917, i64 0}
!917 = !{!"long", !124, i64 0}
!918 = !DILocation(line: 236, column: 3, scope: !908)
!919 = !DILocation(line: 237, column: 1, scope: !908)
!920 = distinct !DISubprogram(name: "createCGRA", scope: !3, file: !3, line: 240, type: !909, isLocal: false, isDefinition: true, scopeLine: 241, isOptimized: true, unit: !2, variables: !921)
!921 = !{!922}
!922 = !DILocalVariable(name: "result", scope: !920, file: !3, line: 250, type: !18)
!923 = !DILocation(line: 242, column: 15, scope: !920)
!924 = !DILocation(line: 243, column: 15, scope: !920)
!925 = !DILocation(line: 250, column: 7, scope: !920)
!926 = !DILocation(line: 245, column: 15, scope: !920)
!927 = !DILocation(line: 259, column: 3, scope: !920)
!928 = !DILocation(line: 260, column: 12, scope: !920)
!929 = !DILocation(line: 261, column: 1, scope: !920)
