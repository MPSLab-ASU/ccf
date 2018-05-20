; ModuleID = 'temp.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p:32:32-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7--linux-eabi"

%struct.int_sqrt = type { i32, i32 }
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i32, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i32, i32, [40 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%union.pthread_attr_t = type { i32, [32 x i8] }

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
@str = private unnamed_addr constant [38 x i8] c"********* CUBIC FUNCTIONS ***********\00"
@.str.1 = private unnamed_addr constant [11 x i8] c"Solutions:\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c" %f\00", align 1
@str.11 = private unnamed_addr constant [40 x i8] c"********* INTEGER SQR ROOTS ***********\00"
@.str.5 = private unnamed_addr constant [17 x i8] c"sqrt(%3d) = %2d\0A\00", align 1
@.str.6 = private unnamed_addr constant [17 x i8] c"\0Asqrt(%lX) = %X\0A\00", align 1
@str.12 = private unnamed_addr constant [39 x i8] c"********* ANGLE CONVERSION ***********\00"
@.str.8 = private unnamed_addr constant [31 x i8] c"%3.0f degrees = %.12f radians\0A\00", align 1
@.str.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.10 = private unnamed_addr constant [31 x i8] c"%.12f radians = %3.0f degrees\0A\00", align 1
@configureCGRA.directoryPath = private unnamed_addr constant [20 x i8] c"./CGRAExec/L\00\00\00\00\00\00\00\00", align 1
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1.1 = private unnamed_addr constant [16 x i8] c"/epilog_ins.bin\00", align 1
@.str.2.2 = private unnamed_addr constant [16 x i8] c"/prolog_ins.bin\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"/kernel_ins.bin\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.5.3 = private unnamed_addr constant [32 x i8] c"\0A**********EPISIZE %d*********\0A\00", align 1
@.str.6.4 = private unnamed_addr constant [32 x i8] c"\0A**********PROSIZE %d*********\0A\00", align 1
@.str.7 = private unnamed_addr constant [33 x i8] c"\0A**********KERNSIZE %d*********\0A\00", align 1
@.str.8.5 = private unnamed_addr constant [37 x i8] c"\0A******SIZE OF UNISGNED INT %d*****\0A\00", align 1
@.str.9.6 = private unnamed_addr constant [26 x i8] c"/livevar_st_ins_count.txt\00", align 1
@.str.10.7 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.11 = private unnamed_addr constant [18 x i8] c"/kernel_count.txt\00", align 1
@.str.12 = private unnamed_addr constant [16 x i8] c"Loop Count: %d\0A\00", align 1
@.str.13 = private unnamed_addr constant [52 x i8] c"From FILE: PROLOGPC= %x, EPILOGPC=%x,  KernelPC=%x\0A\00", align 1
@configureCGRA.initCGRAfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/L1\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.14 = private unnamed_addr constant [14 x i8] c"/initCGRA.txt\00", align 1
@.str.15 = private unnamed_addr constant [3 x i8] c"wb\00", align 1
@.str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.17 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@checkTotalLoops.myfile = private unnamed_addr constant [40 x i8] c"./CGRAExec/total_loops.txt\00\00\00\00\00\00\00\00\00\00\00\00\00\00", align 1
@.str.18 = private unnamed_addr constant [3 x i8] c"%u\00", align 1
@.str.20 = private unnamed_addr constant [35 x i8] c"Core will execute loop %u on CGRA\0A\00", align 1
@str.8 = private unnamed_addr constant [14 x i8] c"deleting cgra\00"
@str.24 = private unnamed_addr constant [40 x i8] c"Main thread calling join w/ CGRA thread\00"
@str.25 = private unnamed_addr constant [35 x i8] c"Main thread calling CGRA thread...\00"

; Function Attrs: norecurse nounwind
define void @usqrt(i32, %struct.int_sqrt* nocapture) local_unnamed_addr #0 {
  br label %3

; <label>:3:                                      ; preds = %3, %2
  %4 = phi i32 [ 0, %2 ], [ %21, %3 ]
  %5 = phi i32 [ 0, %2 ], [ %20, %3 ]
  %6 = phi i32 [ 0, %2 ], [ %18, %3 ]
  %7 = phi i32 [ %0, %2 ], [ %11, %3 ]
  %8 = shl i32 %5, 2
  %9 = lshr i32 %7, 30
  %10 = or i32 %9, %8
  %11 = shl i32 %7, 2
  %12 = shl i32 %6, 1
  %13 = shl i32 %6, 2
  %14 = or i32 %13, 1
  %15 = icmp ult i32 %10, %14
  %16 = xor i1 %15, true
  %17 = zext i1 %16 to i32
  %18 = or i32 %12, %17
  %19 = select i1 %15, i32 0, i32 %14
  %20 = sub i32 %10, %19
  %21 = add nuw nsw i32 %4, 1
  %22 = icmp eq i32 %21, 32
  br i1 %22, label %23, label %3, !llvm.loop !5

; <label>:23:                                     ; preds = %3
  %24 = getelementptr inbounds %struct.int_sqrt, %struct.int_sqrt* %1, i32 0, i32 0
  store i32 %18, i32* %24, align 4
  ret void
}

; Function Attrs: nounwind
define void @SolveCubic(double, double, double, double, i32* nocapture, double* nocapture) local_unnamed_addr #1 {
  %7 = fdiv double %1, %0
  %8 = fdiv double %2, %0
  %9 = fdiv double %3, %0
  %10 = fmul double %7, %7
  %11 = fmul double %8, 3.000000e+00
  %12 = fsub double %10, %11
  %13 = fdiv double %12, 9.000000e+00
  %14 = fmul double %7, 2.000000e+00
  %15 = fmul double %7, %14
  %16 = fmul double %7, %15
  %17 = fmul double %7, 9.000000e+00
  %18 = fmul double %17, %8
  %19 = fsub double %16, %18
  %20 = fmul double %9, 2.700000e+01
  %21 = fadd double %20, %19
  %22 = fdiv double %21, 5.400000e+01
  %23 = fmul double %22, %22
  %24 = fmul double %13, %13
  %25 = fmul double %13, %24
  %26 = fsub double %23, %25
  %27 = fcmp ugt double %26, 0.000000e+00
  br i1 %27, label %57, label %28

; <label>:28:                                     ; preds = %6
  store i32 3, i32* %4, align 4, !tbaa !31
  %29 = tail call double @sqrt(double %25) #6
  %30 = fdiv double %22, %29
  %31 = tail call double @acos(double %30) #6
  %32 = tail call double @sqrt(double %13) #6
  %33 = fmul double %32, -2.000000e+00
  %34 = fdiv double %31, 3.000000e+00
  %35 = tail call double @cos(double %34) #6
  %36 = fmul double %33, %35
  %37 = fdiv double %7, 3.000000e+00
  %38 = fsub double %36, %37
  store double %38, double* %5, align 8, !tbaa !35
  %39 = tail call double @sqrt(double %13) #6
  %40 = fmul double %39, -2.000000e+00
  %41 = tail call double @atan(double 1.000000e+00) #6
  %42 = fadd double %31, 0x401921FB54442D18
  %43 = fdiv double %42, 3.000000e+00
  %44 = tail call double @cos(double %43) #6
  %45 = fmul double %40, %44
  %46 = fsub double %45, %37
  %47 = getelementptr inbounds double, double* %5, i32 1
  store double %46, double* %47, align 8, !tbaa !35
  %48 = tail call double @sqrt(double %13) #6
  %49 = fmul double %48, -2.000000e+00
  %50 = tail call double @atan(double 1.000000e+00) #6
  %51 = fadd double %31, 0x402921FB54442D18
  %52 = fdiv double %51, 3.000000e+00
  %53 = tail call double @cos(double %52) #6
  %54 = fmul double %49, %53
  %55 = fsub double %54, %37
  %56 = getelementptr inbounds double, double* %5, i32 2
  store double %55, double* %56, align 8, !tbaa !35
  br label %69

; <label>:57:                                     ; preds = %6
  store i32 1, i32* %4, align 4, !tbaa !31
  %58 = tail call double @sqrt(double %26) #6
  %59 = tail call double @llvm.fabs.f64(double %22)
  %60 = fadd double %59, %58
  %61 = tail call double @pow(double %60, double 0x3FD5555555555555) #6
  %62 = fdiv double %13, %61
  %63 = fadd double %61, %62
  %64 = fcmp olt double %22, 0.000000e+00
  %65 = select i1 %64, double 1.000000e+00, double -1.000000e+00
  %66 = fmul double %65, %63
  %67 = fdiv double %7, 3.000000e+00
  %68 = fsub double %66, %67
  store double %68, double* %5, align 8, !tbaa !35
  br label %69

; <label>:69:                                     ; preds = %57, %28
  ret void
}

; Function Attrs: nounwind
declare double @sqrt(double) local_unnamed_addr #2

; Function Attrs: nounwind
declare double @acos(double) local_unnamed_addr #2

; Function Attrs: nounwind
declare double @cos(double) local_unnamed_addr #2

; Function Attrs: nounwind
declare double @atan(double) local_unnamed_addr #2

; Function Attrs: nounwind readnone
declare double @llvm.fabs.f64(double) #3

; Function Attrs: nounwind
declare double @pow(double, double) local_unnamed_addr #2

; Function Attrs: nounwind
define double @rad2deg(double) local_unnamed_addr #1 {
  %2 = fmul double %0, 1.800000e+02
  %3 = tail call double @atan(double 1.000000e+00) #6
  %4 = fdiv double %2, 0x400921FB54442D18
  ret double %4
}

; Function Attrs: nounwind
define double @deg2rad(double) local_unnamed_addr #1 {
  %2 = tail call double @atan(double 1.000000e+00) #6
  %3 = fmul double %0, 0x400921FB54442D18
  %4 = fdiv double %3, 1.800000e+02
  ret double %4
}

; Function Attrs: nounwind
define i32 @main() local_unnamed_addr #1 {
  %1 = alloca [3 x double], align 8
  %2 = alloca i32, align 4
  %3 = bitcast [3 x double]* %1 to i8*
  call void @llvm.lifetime.start(i64 24, i8* nonnull %3) #6
  %4 = bitcast i32* %2 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %4) #6
  %5 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([38 x i8], [38 x i8]* @str, i32 0, i32 0)) #6
  %6 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 0
  call void @SolveCubic(double 1.000000e+00, double -1.050000e+01, double 3.200000e+01, double -3.000000e+01, i32* nonnull %2, double* nonnull %6) #6
  %7 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %8 = load i32, i32* %2, align 4, !tbaa !31
  %9 = icmp sgt i32 %8, 0
  br i1 %9, label %.preheader30.preheader, label %.loopexit31

.preheader30.preheader:                           ; preds = %0
  br label %.preheader30

.preheader30:                                     ; preds = %.preheader30.preheader, %.preheader30
  %10 = phi i32 [ %14, %.preheader30 ], [ 0, %.preheader30.preheader ]
  %11 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %10
  %12 = load double, double* %11, align 8, !tbaa !35
  %13 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %12)
  %14 = add nuw nsw i32 %10, 1
  %15 = icmp slt i32 %14, %8
  br i1 %15, label %.preheader30, label %.loopexit31.loopexit, !llvm.loop !37

.loopexit31.loopexit:                             ; preds = %.preheader30
  br label %.loopexit31

.loopexit31:                                      ; preds = %.loopexit31.loopexit, %0
  %16 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double 1.000000e+00, double -4.500000e+00, double 1.700000e+01, double -3.000000e+01, i32* nonnull %2, double* nonnull %6) #6
  %17 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %18 = load i32, i32* %2, align 4, !tbaa !31
  %19 = icmp sgt i32 %18, 0
  br i1 %19, label %.preheader28.preheader, label %.loopexit29

.preheader28.preheader:                           ; preds = %.loopexit31
  br label %.preheader28

.preheader28:                                     ; preds = %.preheader28.preheader, %.preheader28
  %20 = phi i32 [ %24, %.preheader28 ], [ 0, %.preheader28.preheader ]
  %21 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %20
  %22 = load double, double* %21, align 8, !tbaa !35
  %23 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %22)
  %24 = add nuw nsw i32 %20, 1
  %25 = icmp slt i32 %24, %18
  br i1 %25, label %.preheader28, label %.loopexit29.loopexit, !llvm.loop !78

.loopexit29.loopexit:                             ; preds = %.preheader28
  br label %.loopexit29

.loopexit29:                                      ; preds = %.loopexit29.loopexit, %.loopexit31
  %26 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double 1.000000e+00, double -3.500000e+00, double 2.200000e+01, double -3.100000e+01, i32* nonnull %2, double* nonnull %6) #6
  %27 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %28 = load i32, i32* %2, align 4, !tbaa !31
  %29 = icmp sgt i32 %28, 0
  br i1 %29, label %.preheader26.preheader, label %.loopexit27

.preheader26.preheader:                           ; preds = %.loopexit29
  br label %.preheader26

.preheader26:                                     ; preds = %.preheader26.preheader, %.preheader26
  %30 = phi i32 [ %34, %.preheader26 ], [ 0, %.preheader26.preheader ]
  %31 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %30
  %32 = load double, double* %31, align 8, !tbaa !35
  %33 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %32)
  %34 = add nuw nsw i32 %30, 1
  %35 = icmp slt i32 %34, %28
  br i1 %35, label %.preheader26, label %.loopexit27.loopexit, !llvm.loop !81

.loopexit27.loopexit:                             ; preds = %.preheader26
  br label %.loopexit27

.loopexit27:                                      ; preds = %.loopexit27.loopexit, %.loopexit29
  %36 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double 1.000000e+00, double -1.370000e+01, double 1.000000e+00, double -3.500000e+01, i32* nonnull %2, double* nonnull %6) #6
  %37 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %38 = load i32, i32* %2, align 4, !tbaa !31
  %39 = icmp sgt i32 %38, 0
  br i1 %39, label %.preheader24.preheader, label %.loopexit25

.preheader24.preheader:                           ; preds = %.loopexit27
  br label %.preheader24

.preheader24:                                     ; preds = %.preheader24.preheader, %.preheader24
  %40 = phi i32 [ %44, %.preheader24 ], [ 0, %.preheader24.preheader ]
  %41 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %40
  %42 = load double, double* %41, align 8, !tbaa !35
  %43 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %42)
  %44 = add nuw nsw i32 %40, 1
  %45 = icmp slt i32 %44, %38
  br i1 %45, label %.preheader24, label %.loopexit25.loopexit, !llvm.loop !84

.loopexit25.loopexit:                             ; preds = %.preheader24
  br label %.loopexit25

.loopexit25:                                      ; preds = %.loopexit25.loopexit, %.loopexit27
  %46 = tail call i32 @putchar(i32 10) #6
  br label %47

; <label>:47:                                     ; preds = %71, %.loopexit25
  %48 = phi i32 [ 1, %.loopexit25 ], [ %72, %71 ]
  %49 = sitofp i32 %48 to double
  br label %50

; <label>:50:                                     ; preds = %68, %47
  %51 = phi i32 [ 10, %47 ], [ %69, %68 ]
  %52 = sitofp i32 %51 to double
  br label %53

; <label>:53:                                     ; preds = %.loopexit, %50
  %54 = phi double [ 5.000000e+00, %50 ], [ %213, %.loopexit ]
  call void @SolveCubic(double %49, double %52, double %54, double -1.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %55 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %56 = load i32, i32* %2, align 4, !tbaa !31
  %57 = icmp sgt i32 %56, 0
  br i1 %57, label %.preheader22.preheader, label %.loopexit23

.preheader22.preheader:                           ; preds = %53
  br label %.preheader22

.preheader22:                                     ; preds = %.preheader22.preheader, %.preheader22
  %58 = phi i32 [ %62, %.preheader22 ], [ 0, %.preheader22.preheader ]
  %59 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %58
  %60 = load double, double* %59, align 8, !tbaa !35
  %61 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %60)
  %62 = add nuw nsw i32 %58, 1
  %63 = icmp slt i32 %62, %56
  br i1 %63, label %.preheader22, label %.loopexit23.loopexit, !llvm.loop !87

.loopexit23.loopexit:                             ; preds = %.preheader22
  br label %.loopexit23

.loopexit23:                                      ; preds = %.loopexit23.loopexit, %53
  %64 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -2.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %65 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %66 = load i32, i32* %2, align 4, !tbaa !31
  %67 = icmp sgt i32 %66, 0
  br i1 %67, label %.preheader20.preheader, label %.loopexit21

.preheader20.preheader:                           ; preds = %.loopexit23
  br label %.preheader20

; <label>:68:                                     ; preds = %.loopexit
  %69 = add nsw i32 %51, -1
  %70 = icmp sgt i32 %51, 1
  br i1 %70, label %50, label %71, !llvm.loop !102

; <label>:71:                                     ; preds = %68
  %72 = add nuw nsw i32 %48, 1
  %73 = icmp eq i32 %72, 10
  br i1 %73, label %74, label %47, !llvm.loop !104

; <label>:74:                                     ; preds = %71
  %75 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.11, i32 0, i32 0)) #6
  br label %76

; <label>:76:                                     ; preds = %usqrt.exit, %74
  %77 = phi i32 [ 0, %74 ], [ %99, %usqrt.exit ]
  br label %78

; <label>:78:                                     ; preds = %78, %76
  %79 = phi i32 [ 0, %76 ], [ %96, %78 ]
  %80 = phi i32 [ 0, %76 ], [ %95, %78 ]
  %81 = phi i32 [ 0, %76 ], [ %93, %78 ]
  %82 = phi i32 [ %77, %76 ], [ %86, %78 ]
  %83 = shl i32 %80, 2
  %84 = lshr i32 %82, 30
  %85 = or i32 %84, %83
  %86 = shl i32 %82, 2
  %87 = shl i32 %81, 1
  %88 = shl i32 %81, 2
  %89 = or i32 %88, 1
  %90 = icmp ult i32 %85, %89
  %91 = xor i1 %90, true
  %92 = zext i1 %91 to i32
  %93 = or i32 %87, %92
  %94 = select i1 %90, i32 0, i32 %89
  %95 = sub i32 %85, %94
  %96 = add nuw nsw i32 %79, 1
  %97 = icmp eq i32 %96, 32
  br i1 %97, label %usqrt.exit, label %78, !llvm.loop !5

usqrt.exit:                                       ; preds = %78
  %98 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.5, i32 0, i32 0), i32 %77, i32 %93)
  %99 = add nuw nsw i32 %77, 1
  %100 = icmp eq i32 %99, 1001
  br i1 %100, label %.preheader.preheader, label %76, !llvm.loop !106

.preheader.preheader:                             ; preds = %usqrt.exit
  %101 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.6, i32 0, i32 0), i32 1072497001, i32 2146238464)
  %102 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([39 x i8], [39 x i8]* @str.12, i32 0, i32 0)) #6
  br label %103

; <label>:103:                                    ; preds = %103, %.preheader.preheader
  %104 = phi i32 [ 0, %.preheader.preheader ], [ %110, %103 ]
  %105 = sitofp i32 %104 to double
  %106 = tail call double @atan(double 1.000000e+00) #6
  %107 = fmul double %105, 0x400921FB54442D18
  %108 = fdiv double %107, 1.800000e+02
  %109 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.8, i32 0, i32 0), double %105, double %108)
  %110 = add nuw nsw i32 %104, 1
  %111 = icmp eq i32 %110, 361
  br i1 %111, label %112, label %103, !llvm.loop !109

; <label>:112:                                    ; preds = %103
  %113 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.9, i32 0, i32 0)) #6
  %114 = tail call double @atan(double 1.000000e+00) #6
  br label %115

; <label>:115:                                    ; preds = %115, %112
  %116 = phi double [ 0.000000e+00, %112 ], [ %122, %115 ]
  %117 = fmul double %116, 1.800000e+02
  %118 = tail call double @atan(double 1.000000e+00) #6
  %119 = fdiv double %117, 0x400921FB54442D18
  %120 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.10, i32 0, i32 0), double %116, double %119)
  %121 = tail call double @atan(double 1.000000e+00) #6
  %122 = fadd double %116, 0x3F91DF46A2529D39
  %123 = tail call double @atan(double 1.000000e+00) #6
  %124 = fcmp ugt double %122, 0x401921FB97600B9B
  br i1 %124, label %125, label %115, !llvm.loop !112

; <label>:125:                                    ; preds = %115
  call void @llvm.lifetime.end(i64 4, i8* nonnull %4) #6
  call void @llvm.lifetime.end(i64 24, i8* nonnull %3) #6
  ret i32 0

.preheader20:                                     ; preds = %.preheader20.preheader, %.preheader20
  %126 = phi i32 [ %130, %.preheader20 ], [ 0, %.preheader20.preheader ]
  %127 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %126
  %128 = load double, double* %127, align 8, !tbaa !35
  %129 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %128)
  %130 = add nuw nsw i32 %126, 1
  %131 = icmp slt i32 %130, %66
  br i1 %131, label %.preheader20, label %.loopexit21.loopexit, !llvm.loop !87

.loopexit21.loopexit:                             ; preds = %.preheader20
  br label %.loopexit21

.loopexit21:                                      ; preds = %.loopexit21.loopexit, %.loopexit23
  %132 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -3.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %133 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %134 = load i32, i32* %2, align 4, !tbaa !31
  %135 = icmp sgt i32 %134, 0
  br i1 %135, label %.preheader18.preheader, label %.loopexit19

.preheader18.preheader:                           ; preds = %.loopexit21
  br label %.preheader18

.preheader18:                                     ; preds = %.preheader18.preheader, %.preheader18
  %136 = phi i32 [ %140, %.preheader18 ], [ 0, %.preheader18.preheader ]
  %137 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %136
  %138 = load double, double* %137, align 8, !tbaa !35
  %139 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %138)
  %140 = add nuw nsw i32 %136, 1
  %141 = icmp slt i32 %140, %134
  br i1 %141, label %.preheader18, label %.loopexit19.loopexit, !llvm.loop !87

.loopexit19.loopexit:                             ; preds = %.preheader18
  br label %.loopexit19

.loopexit19:                                      ; preds = %.loopexit19.loopexit, %.loopexit21
  %142 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -4.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %143 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %144 = load i32, i32* %2, align 4, !tbaa !31
  %145 = icmp sgt i32 %144, 0
  br i1 %145, label %.preheader16.preheader, label %.loopexit17

.preheader16.preheader:                           ; preds = %.loopexit19
  br label %.preheader16

.preheader16:                                     ; preds = %.preheader16.preheader, %.preheader16
  %146 = phi i32 [ %150, %.preheader16 ], [ 0, %.preheader16.preheader ]
  %147 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %146
  %148 = load double, double* %147, align 8, !tbaa !35
  %149 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %148)
  %150 = add nuw nsw i32 %146, 1
  %151 = icmp slt i32 %150, %144
  br i1 %151, label %.preheader16, label %.loopexit17.loopexit, !llvm.loop !87

.loopexit17.loopexit:                             ; preds = %.preheader16
  br label %.loopexit17

.loopexit17:                                      ; preds = %.loopexit17.loopexit, %.loopexit19
  %152 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -5.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %153 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %154 = load i32, i32* %2, align 4, !tbaa !31
  %155 = icmp sgt i32 %154, 0
  br i1 %155, label %.preheader14.preheader, label %.loopexit15

.preheader14.preheader:                           ; preds = %.loopexit17
  br label %.preheader14

.preheader14:                                     ; preds = %.preheader14.preheader, %.preheader14
  %156 = phi i32 [ %160, %.preheader14 ], [ 0, %.preheader14.preheader ]
  %157 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %156
  %158 = load double, double* %157, align 8, !tbaa !35
  %159 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %158)
  %160 = add nuw nsw i32 %156, 1
  %161 = icmp slt i32 %160, %154
  br i1 %161, label %.preheader14, label %.loopexit15.loopexit, !llvm.loop !87

.loopexit15.loopexit:                             ; preds = %.preheader14
  br label %.loopexit15

.loopexit15:                                      ; preds = %.loopexit15.loopexit, %.loopexit17
  %162 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -6.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %163 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %164 = load i32, i32* %2, align 4, !tbaa !31
  %165 = icmp sgt i32 %164, 0
  br i1 %165, label %.preheader12.preheader, label %.loopexit13

.preheader12.preheader:                           ; preds = %.loopexit15
  br label %.preheader12

.preheader12:                                     ; preds = %.preheader12.preheader, %.preheader12
  %166 = phi i32 [ %170, %.preheader12 ], [ 0, %.preheader12.preheader ]
  %167 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %166
  %168 = load double, double* %167, align 8, !tbaa !35
  %169 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %168)
  %170 = add nuw nsw i32 %166, 1
  %171 = icmp slt i32 %170, %164
  br i1 %171, label %.preheader12, label %.loopexit13.loopexit, !llvm.loop !87

.loopexit13.loopexit:                             ; preds = %.preheader12
  br label %.loopexit13

.loopexit13:                                      ; preds = %.loopexit13.loopexit, %.loopexit15
  %172 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -7.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %173 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %174 = load i32, i32* %2, align 4, !tbaa !31
  %175 = icmp sgt i32 %174, 0
  br i1 %175, label %.preheader10.preheader, label %.loopexit11

.preheader10.preheader:                           ; preds = %.loopexit13
  br label %.preheader10

.preheader10:                                     ; preds = %.preheader10.preheader, %.preheader10
  %176 = phi i32 [ %180, %.preheader10 ], [ 0, %.preheader10.preheader ]
  %177 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %176
  %178 = load double, double* %177, align 8, !tbaa !35
  %179 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %178)
  %180 = add nuw nsw i32 %176, 1
  %181 = icmp slt i32 %180, %174
  br i1 %181, label %.preheader10, label %.loopexit11.loopexit, !llvm.loop !87

.loopexit11.loopexit:                             ; preds = %.preheader10
  br label %.loopexit11

.loopexit11:                                      ; preds = %.loopexit11.loopexit, %.loopexit13
  %182 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -8.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %183 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %184 = load i32, i32* %2, align 4, !tbaa !31
  %185 = icmp sgt i32 %184, 0
  br i1 %185, label %.preheader8.preheader, label %.loopexit9

.preheader8.preheader:                            ; preds = %.loopexit11
  br label %.preheader8

.preheader8:                                      ; preds = %.preheader8.preheader, %.preheader8
  %186 = phi i32 [ %190, %.preheader8 ], [ 0, %.preheader8.preheader ]
  %187 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %186
  %188 = load double, double* %187, align 8, !tbaa !35
  %189 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %188)
  %190 = add nuw nsw i32 %186, 1
  %191 = icmp slt i32 %190, %184
  br i1 %191, label %.preheader8, label %.loopexit9.loopexit, !llvm.loop !87

.loopexit9.loopexit:                              ; preds = %.preheader8
  br label %.loopexit9

.loopexit9:                                       ; preds = %.loopexit9.loopexit, %.loopexit11
  %192 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -9.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %193 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %194 = load i32, i32* %2, align 4, !tbaa !31
  %195 = icmp sgt i32 %194, 0
  br i1 %195, label %.preheader6.preheader, label %.loopexit7

.preheader6.preheader:                            ; preds = %.loopexit9
  br label %.preheader6

.preheader6:                                      ; preds = %.preheader6.preheader, %.preheader6
  %196 = phi i32 [ %200, %.preheader6 ], [ 0, %.preheader6.preheader ]
  %197 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %196
  %198 = load double, double* %197, align 8, !tbaa !35
  %199 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %198)
  %200 = add nuw nsw i32 %196, 1
  %201 = icmp slt i32 %200, %194
  br i1 %201, label %.preheader6, label %.loopexit7.loopexit, !llvm.loop !87

.loopexit7.loopexit:                              ; preds = %.preheader6
  br label %.loopexit7

.loopexit7:                                       ; preds = %.loopexit7.loopexit, %.loopexit9
  %202 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -1.000000e+01, i32* nonnull %2, double* nonnull %6) #6
  %203 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %204 = load i32, i32* %2, align 4, !tbaa !31
  %205 = icmp sgt i32 %204, 0
  br i1 %205, label %.preheader5.preheader, label %.loopexit

.preheader5.preheader:                            ; preds = %.loopexit7
  br label %.preheader5

.preheader5:                                      ; preds = %.preheader5.preheader, %.preheader5
  %206 = phi i32 [ %210, %.preheader5 ], [ 0, %.preheader5.preheader ]
  %207 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %206
  %208 = load double, double* %207, align 8, !tbaa !35
  %209 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %208)
  %210 = add nuw nsw i32 %206, 1
  %211 = icmp slt i32 %210, %204
  br i1 %211, label %.preheader5, label %.loopexit.loopexit, !llvm.loop !87

.loopexit.loopexit:                               ; preds = %.preheader5
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %.loopexit7
  %212 = tail call i32 @putchar(i32 10) #6
  %213 = fadd double %54, 5.000000e-01
  %214 = fcmp olt double %213, 1.500000e+01
  br i1 %214, label %53, label %68, !llvm.loop !115
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #4

declare i32 @puts(...) local_unnamed_addr #5

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
declare i32 @putchar(i32) local_unnamed_addr #6

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #4

; Function Attrs: nounwind
define i32 @initializeParameters(i32) local_unnamed_addr #1 {
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
  %14 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %15 = add i32 %0, -1
  %16 = mul i32 %15, 7
  %17 = getelementptr inbounds i32, i32* %14, i32 %16
  store i32 2011168768, i32* %17, align 4, !tbaa !31
  %18 = getelementptr inbounds i32, i32* %17, i32 1
  store i32 2011168768, i32* %18, align 4, !tbaa !31
  %19 = getelementptr inbounds i32, i32* %17, i32 2
  %20 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  %21 = bitcast i32* %19 to i8*
  tail call void @llvm.memset.p0i8.i64(i8* %21, i8 0, i64 20, i32 4, i1 false)
  call void @llvm.lifetime.start(i64 25, i8* nonnull %20) #6
  %22 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %22) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %22, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %23 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %20, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %0) #6
  %24 = call i8* @strcat(i8* nonnull %22, i8* nonnull %20) #6
  %25 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %25) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %25, i8 0, i32 40, i32 1, i1 false)
  %26 = getelementptr inbounds [40 x i8], [40 x i8]* %5, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %26) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %26, i8 0, i32 40, i32 1, i1 false)
  %27 = getelementptr inbounds [40 x i8], [40 x i8]* %6, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %27) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %27, i8 0, i32 40, i32 1, i1 false)
  %28 = getelementptr inbounds [40 x i8], [40 x i8]* %7, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %28) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %28, i8 0, i32 40, i32 1, i1 false)
  %29 = call i8* @strcat(i8* nonnull %25, i8* nonnull %22) #6
  %30 = call i32 @strlen(i8* nonnull %25)
  %31 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %30
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %31, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1.1, i32 0, i32 0), i32 16, i32 1, i1 false)
  %32 = call i8* @strcat(i8* nonnull %26, i8* nonnull %22) #6
  %33 = call i32 @strlen(i8* nonnull %26)
  %34 = getelementptr [40 x i8], [40 x i8]* %5, i32 0, i32 %33
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %34, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2.2, i32 0, i32 0), i32 16, i32 1, i1 false)
  %35 = call i8* @strcat(i8* nonnull %27, i8* nonnull %22) #6
  %36 = call i32 @strlen(i8* nonnull %27)
  %37 = getelementptr [40 x i8], [40 x i8]* %6, i32 0, i32 %36
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %37, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 16, i32 1, i1 false)
  %38 = call %struct._IO_FILE* @fopen(i8* nonnull %25, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i32 0, i32 0))
  %39 = call %struct._IO_FILE* @fopen(i8* nonnull %26, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i32 0, i32 0))
  %40 = call %struct._IO_FILE* @fopen(i8* nonnull %27, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i32 0, i32 0))
  %41 = bitcast i32* %8 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %41) #6
  %42 = bitcast i32* %9 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %42) #6
  %43 = bitcast i32* %10 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %43) #6
  %44 = bitcast i32* %11 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %44) #6
  %45 = call i32 @fread(i8* nonnull %41, i32 4, i32 1, %struct._IO_FILE* %38)
  %46 = call i32 @fread(i8* nonnull %42, i32 4, i32 1, %struct._IO_FILE* %39)
  %47 = call i32 @fread(i8* nonnull %43, i32 4, i32 1, %struct._IO_FILE* %40)
  %48 = load i32, i32* %8, align 4, !tbaa !31
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5.3, i32 0, i32 0), i32 %48)
  %50 = load i32, i32* %9, align 4, !tbaa !31
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6.4, i32 0, i32 0), i32 %50)
  %52 = load i32, i32* %10, align 4, !tbaa !31
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7, i32 0, i32 0), i32 %52)
  %54 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8.5, i32 0, i32 0), i32 4)
  %55 = shl i32 %48, 2
  %56 = call noalias i8* @malloc(i32 %55) #6
  store i8* %56, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !117
  %57 = shl i32 %50, 2
  %58 = call noalias i8* @malloc(i32 %57) #6
  store i8* %58, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !117
  %59 = shl i32 %52, 2
  %60 = call noalias i8* @malloc(i32 %59) #6
  store i8* %60, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !117
  %61 = call i32 @fread(i8* %56, i32 4, i32 %48, %struct._IO_FILE* %38)
  %62 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !117
  %63 = call i32 @fread(i8* %62, i32 4, i32 %50, %struct._IO_FILE* %39)
  %64 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !117
  %65 = call i32 @fread(i8* %64, i32 4, i32 %52, %struct._IO_FILE* %40)
  %66 = call i8* @strcat(i8* nonnull %28, i8* nonnull %22) #6
  %67 = call i32 @strlen(i8* nonnull %28)
  %68 = getelementptr [40 x i8], [40 x i8]* %7, i32 0, i32 %67
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %68, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.9.6, i32 0, i32 0), i32 26, i32 1, i1 false)
  %69 = call %struct._IO_FILE* @fopen(i8* nonnull %28, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.7, i32 0, i32 0))
  %70 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %69, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* nonnull %11) #6
  %71 = sdiv i32 %52, 16
  %72 = sdiv i32 %48, 16
  %73 = sdiv i32 %50, 16
  %74 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %75 = getelementptr inbounds i32, i32* %74, i32 %16
  %76 = getelementptr inbounds i32, i32* %75, i32 2
  store i32 %71, i32* %76, align 4, !tbaa !31
  %77 = getelementptr inbounds i32, i32* %75, i32 3
  store i32 %72, i32* %77, align 4, !tbaa !31
  %78 = getelementptr inbounds i32, i32* %75, i32 4
  store i32 %73, i32* %78, align 4, !tbaa !31
  %79 = load i32, i32* %11, align 4, !tbaa !31
  %80 = sdiv i32 %79, 16
  %81 = getelementptr inbounds i32, i32* %75, i32 6
  store i32 %80, i32* %81, align 4, !tbaa !31
  %82 = call i32 @fclose(%struct._IO_FILE* %38)
  %83 = call i32 @fclose(%struct._IO_FILE* %39)
  %84 = call i32 @fclose(%struct._IO_FILE* %40)
  %85 = call i32 @fclose(%struct._IO_FILE* %69)
  %86 = getelementptr inbounds [40 x i8], [40 x i8]* %12, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %86) #6
  call void @llvm.memset.p0i8.i32(i8* nonnull %86, i8 0, i32 40, i32 1, i1 false)
  %87 = call i8* @strcat(i8* nonnull %86, i8* nonnull %22) #6
  %88 = call i32 @strlen(i8* nonnull %86)
  %89 = getelementptr [40 x i8], [40 x i8]* %12, i32 0, i32 %88
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %89, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.11, i32 0, i32 0), i32 18, i32 1, i1 false)
  %90 = bitcast i32* %13 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %90) #6
  store i32 0, i32* %13, align 4, !tbaa !31
  %91 = call %struct._IO_FILE* @fopen(i8* nonnull %86, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.7, i32 0, i32 0))
  %92 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %91, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* nonnull %13) #6
  %93 = call i32 @fclose(%struct._IO_FILE* %91)
  %94 = load i32, i32* %13, align 4, !tbaa !31
  %95 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %94)
  %96 = load i32, i32* %13, align 4, !tbaa !31
  %97 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %98 = getelementptr inbounds i32, i32* %97, i32 %16
  %99 = getelementptr inbounds i32, i32* %98, i32 5
  store i32 %96, i32* %99, align 4, !tbaa !31
  %100 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !117
  %101 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !117
  %102 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !117
  %103 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.13, i32 0, i32 0), i32 %100, i32 %101, i32 %102)
  %104 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !117
  %105 = load i32*, i32** @prologPtr, align 4, !tbaa !117
  %106 = getelementptr inbounds i32, i32* %105, i32 %15
  store i32 %104, i32* %106, align 4, !tbaa !119
  %107 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !117
  %108 = load i32*, i32** @epilogPtr, align 4, !tbaa !117
  %109 = getelementptr inbounds i32, i32* %108, i32 %15
  store i32 %107, i32* %109, align 4, !tbaa !119
  %110 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !117
  %111 = load i32*, i32** @kernelPtr, align 4, !tbaa !117
  %112 = getelementptr inbounds i32, i32* %111, i32 %15
  store i32 %110, i32* %112, align 4, !tbaa !119
  call void @llvm.lifetime.end(i64 4, i8* nonnull %90) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %86) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %44) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %43) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %42) #6
  call void @llvm.lifetime.end(i64 4, i8* nonnull %41) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %28) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %27) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %26) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %25) #6
  call void @llvm.lifetime.end(i64 20, i8* nonnull %22) #6
  call void @llvm.lifetime.end(i64 25, i8* nonnull %20) #6
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #4

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture writeonly, i8* nocapture readonly, i32, i32, i1) #4

; Function Attrs: nounwind
declare i32 @sprintf(i8* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
declare i8* @strcat(i8*, i8* nocapture readonly) local_unnamed_addr #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i32(i8* nocapture writeonly, i8, i32, i32, i1) #4

; Function Attrs: nounwind readonly
declare i32 @strlen(i8* nocapture) local_unnamed_addr #7

; Function Attrs: nounwind
declare noalias %struct._IO_FILE* @fopen(i8* nocapture readonly, i8* nocapture readonly) local_unnamed_addr #2

; Function Attrs: nounwind
declare i32 @fread(i8* nocapture, i32, i32, %struct._IO_FILE* nocapture) local_unnamed_addr #2

; Function Attrs: nounwind
declare noalias i8* @malloc(i32) local_unnamed_addr #2

declare i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) local_unnamed_addr #5

; Function Attrs: nounwind
declare i32 @fclose(%struct._IO_FILE* nocapture) local_unnamed_addr #2

; Function Attrs: nounwind
define i32 @configureCGRA(i32) local_unnamed_addr #1 {
  %2 = alloca [25 x i8], align 1
  %3 = alloca [20 x i8], align 1
  %4 = alloca [40 x i8], align 1
  %5 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %6 = add i32 %0, -1
  %7 = mul i32 %6, 7
  %8 = getelementptr inbounds i32, i32* %5, i32 %7
  %9 = getelementptr inbounds i32, i32* %8, i32 5
  %10 = load i32, i32* %9, align 4, !tbaa !31
  %11 = getelementptr inbounds [25 x i8], [25 x i8]* %2, i32 0, i32 0
  call void @llvm.lifetime.start(i64 25, i8* nonnull %11) #6
  %12 = getelementptr inbounds [20 x i8], [20 x i8]* %3, i32 0, i32 0
  call void @llvm.lifetime.start(i64 20, i8* nonnull %12) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %12, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @configureCGRA.directoryPath, i32 0, i32 0), i32 20, i32 1, i1 false)
  %13 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %11, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %0) #6
  %14 = call i8* @strcat(i8* nonnull %12, i8* nonnull %11) #6
  %15 = icmp slt i32 %10, 1
  br i1 %15, label %16, label %22

; <label>:16:                                     ; preds = %1
  %17 = load i32, i32* @dynamicTCVal, align 4, !tbaa !31
  %18 = add nsw i32 %17, %10
  %19 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %20 = getelementptr inbounds i32, i32* %19, i32 %7
  %21 = getelementptr inbounds i32, i32* %20, i32 5
  store i32 %18, i32* %21, align 4, !tbaa !31
  br label %22

; <label>:22:                                     ; preds = %16, %1
  %23 = getelementptr inbounds [40 x i8], [40 x i8]* %4, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %23) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %23, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @configureCGRA.initCGRAfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %24 = call i32 @strlen(i8* nonnull %23)
  %25 = getelementptr [40 x i8], [40 x i8]* %4, i32 0, i32 %24
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %25, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.14, i32 0, i32 0), i32 14, i32 1, i1 false)
  %26 = call %struct._IO_FILE* @fopen(i8* nonnull %23, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.15, i32 0, i32 0))
  %27 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %28 = getelementptr inbounds i32, i32* %27, i32 %7
  %29 = load i32, i32* %28, align 4, !tbaa !31
  %30 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %29)
  %31 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %32 = getelementptr inbounds i32, i32* %31, i32 %7
  %33 = getelementptr inbounds i32, i32* %32, i32 1
  %34 = load i32, i32* %33, align 4, !tbaa !31
  %35 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %34)
  %36 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %37 = getelementptr inbounds i32, i32* %36, i32 %7
  %38 = getelementptr inbounds i32, i32* %37, i32 2
  %39 = load i32, i32* %38, align 4, !tbaa !31
  %40 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %39)
  %41 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %42 = getelementptr inbounds i32, i32* %41, i32 %7
  %43 = getelementptr inbounds i32, i32* %42, i32 3
  %44 = load i32, i32* %43, align 4, !tbaa !31
  %45 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %44)
  %46 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %47 = getelementptr inbounds i32, i32* %46, i32 %7
  %48 = getelementptr inbounds i32, i32* %47, i32 4
  %49 = load i32, i32* %48, align 4, !tbaa !31
  %50 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %49)
  %51 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %52 = getelementptr inbounds i32, i32* %51, i32 %7
  %53 = getelementptr inbounds i32, i32* %52, i32 5
  %54 = load i32, i32* %53, align 4, !tbaa !31
  %55 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %54)
  %56 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %57 = getelementptr inbounds i32, i32* %56, i32 %7
  %58 = getelementptr inbounds i32, i32* %57, i32 6
  %59 = load i32, i32* %58, align 4, !tbaa !31
  %60 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %59)
  %61 = load i32*, i32** @epilogPtr, align 4, !tbaa !117
  %62 = getelementptr inbounds i32, i32* %61, i32 %6
  %63 = load i32, i32* %62, align 4, !tbaa !119
  %64 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %63)
  %65 = load i32*, i32** @prologPtr, align 4, !tbaa !117
  %66 = getelementptr inbounds i32, i32* %65, i32 %6
  %67 = load i32, i32* %66, align 4, !tbaa !119
  %68 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %67)
  %69 = load i32*, i32** @kernelPtr, align 4, !tbaa !117
  %70 = getelementptr inbounds i32, i32* %69, i32 %6
  %71 = load i32, i32* %70, align 4, !tbaa !119
  %72 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %71)
  %73 = call i32 @fclose(%struct._IO_FILE* %26)
  call void @llvm.lifetime.end(i64 40, i8* nonnull %23) #6
  call void @llvm.lifetime.end(i64 20, i8* nonnull %12) #6
  call void @llvm.lifetime.end(i64 25, i8* nonnull %11) #6
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind
define void @checkTotalLoops() local_unnamed_addr #1 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false)
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.7, i32 0, i32 0))
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.18, i32 0, i32 0), i32* nonnull @totalLoops) #6
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3)
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #6
  ret void
}

; Function Attrs: norecurse nounwind
define void @updateLoopTC(i32) local_unnamed_addr #0 {
  %2 = load i32*, i32** @initCGRA, align 4, !tbaa !117
  %3 = getelementptr inbounds i32, i32* %2, i32 5
  %4 = load i32, i32* %3, align 4, !tbaa !31
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %8, label %6

; <label>:6:                                      ; preds = %1
  %7 = load i32, i32* @dynamicTCVal, align 4, !tbaa !31
  store i32 %7, i32* %3, align 4, !tbaa !31
  br label %8

; <label>:8:                                      ; preds = %6, %1
  ret void
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #1 {
  br label %2

; <label>:2:                                      ; preds = %11, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !31
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %5 = tail call i32 @usleep(i32 1) #6
  %6 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !31
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %.loopexit.loopexit, label %.preheader, !llvm.loop !121

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %8 = load i32, i32* @thread_exit, align 4, !tbaa !31
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #6, !srcloc !168
  ret i8* null

; <label>:11:                                     ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !31
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #6, !srcloc !169
  %12 = tail call i32 @usleep(i32 1) #6
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !31
  br label %2, !llvm.loop !170
}

declare i32 @usleep(i32) local_unnamed_addr #5

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #1 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.20, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !31
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !31
  br label %4

; <label>:4:                                      ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #6
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !31
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !172

; <label>:8:                                      ; preds = %4
  ret void
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #1 {
  %1 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.8, i32 0, i32 0)) #6
  store i32 1, i32* @thread_exit, align 4, !tbaa !31
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !31
  %2 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.24, i32 0, i32 0)) #6
  %3 = load i32, i32* @pth, align 4, !tbaa !119
  %4 = tail call i32 @pthread_join(i32 %3, i8** null) #6
  ret void
}

declare i32 @pthread_join(i32, i8**) local_unnamed_addr #5

; Function Attrs: nounwind
define void @createCGRA() local_unnamed_addr #1 {
  %1 = alloca [40 x i8], align 1
  %2 = getelementptr inbounds [40 x i8], [40 x i8]* %1, i32 0, i32 0
  call void @llvm.lifetime.start(i64 40, i8* nonnull %2) #6
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* nonnull %2, i8* getelementptr inbounds ([40 x i8], [40 x i8]* @checkTotalLoops.myfile, i32 0, i32 0), i32 40, i32 1, i1 false) #6
  %3 = call %struct._IO_FILE* @fopen(i8* nonnull %2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.7, i32 0, i32 0)) #6
  %4 = tail call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.18, i32 0, i32 0), i32* nonnull @totalLoops) #6
  %5 = tail call i32 @fclose(%struct._IO_FILE* %3) #6
  call void @llvm.lifetime.end(i64 40, i8* nonnull %2) #6
  %6 = load i32, i32* @totalLoops, align 4, !tbaa !31
  %7 = mul i32 %6, 28
  %8 = tail call noalias i8* @malloc(i32 %7) #6
  store i8* %8, i8** bitcast (i32** @initCGRA to i8**), align 4, !tbaa !117
  %9 = shl i32 %6, 2
  %10 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %10, i8** bitcast (i32** @prologPtr to i8**), align 4, !tbaa !117
  %11 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %11, i8** bitcast (i32** @kernelPtr to i8**), align 4, !tbaa !117
  %12 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %12, i8** bitcast (i32** @epilogPtr to i8**), align 4, !tbaa !117
  %13 = icmp eq i32 %6, 0
  br i1 %13, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %0
  br label %.preheader

.preheader:                                       ; preds = %.preheader.preheader, %.preheader
  %14 = phi i32 [ %16, %.preheader ], [ 1, %.preheader.preheader ]
  %15 = tail call i32 @initializeParameters(i32 %14)
  %16 = add i32 %14, 1
  %17 = load i32, i32* @totalLoops, align 4, !tbaa !31
  %18 = icmp ugt i32 %16, %17
  br i1 %18, label %.loopexit.loopexit, label %.preheader, !llvm.loop !181

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %0
  %19 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str.25, i32 0, i32 0)) #6
  %20 = tail call i32 @pthread_create(i32* nonnull @pth, %union.pthread_attr_t* null, i8* (i8*)* nonnull @runOnCGRA, i8* null) #6
  ret void
}

; Function Attrs: nounwind
declare i32 @pthread_create(i32*, %union.pthread_attr_t*, i8* (i8*)*, i8*) local_unnamed_addr #2

attributes #0 = { norecurse nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone }
attributes #4 = { argmemonly nounwind }
attributes #5 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="cortex-a8" "target-features"="+dsp,+neon,+vfp3" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nounwind }
attributes #7 = { nounwind readonly }

!llvm.ident = !{!0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3, !4}

!0 = !{!"clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)"}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{i32 1, !"min_enum_size", i32 4}
!5 = distinct !{!5, !6, !30}
!6 = !DILocation(line: 63, column: 7, scope: !7)
!7 = distinct !DILexicalBlock(scope: !9, file: !8, line: 53, column: 7)
!8 = !DIFile(filename: "isqrt.c", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!9 = distinct !DISubprogram(name: "usqrt", scope: !8, file: !8, line: 45, type: !10, isLocal: false, isDefinition: true, scopeLine: 46, flags: DIFlagPrototyped, isOptimized: true, unit: !20, variables: !22)
!10 = !DISubroutineType(types: !11)
!11 = !{null, !12, !13}
!12 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 32)
!14 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "int_sqrt", file: !15, line: 67, size: 64, elements: !16)
!15 = !DIFile(filename: "./snipmath.h", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!16 = !{!17, !19}
!17 = !DIDerivedType(tag: DW_TAG_member, name: "sqrt", scope: !14, file: !15, line: 68, baseType: !18, size: 32)
!18 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!19 = !DIDerivedType(tag: DW_TAG_member, name: "frac", scope: !14, file: !15, line: 69, baseType: !18, size: 32, offset: 32)
!20 = distinct !DICompileUnit(language: DW_LANG_C99, file: !8, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !21)
!21 = !{}
!22 = !{!23, !24, !25, !26, !27, !28}
!23 = !DILocalVariable(name: "x", arg: 1, scope: !9, file: !8, line: 45, type: !12)
!24 = !DILocalVariable(name: "q", arg: 2, scope: !9, file: !8, line: 45, type: !13)
!25 = !DILocalVariable(name: "a", scope: !9, file: !8, line: 47, type: !12)
!26 = !DILocalVariable(name: "r", scope: !9, file: !8, line: 48, type: !12)
!27 = !DILocalVariable(name: "e", scope: !9, file: !8, line: 49, type: !12)
!28 = !DILocalVariable(name: "i", scope: !9, file: !8, line: 51, type: !29)
!29 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!30 = !{!"llvm.loop.CGRA.enable"}
!31 = !{!32, !32, i64 0}
!32 = !{!"int", !33, i64 0}
!33 = !{!"omnipotent char", !34, i64 0}
!34 = !{!"Simple C/C++ TBAA"}
!35 = !{!36, !36, i64 0}
!36 = !{!"double", !33, i64 0}
!37 = distinct !{!37, !38}
!38 = !DILocation(line: 26, column: 22, scope: !39)
!39 = distinct !DILexicalBlock(scope: !41, file: !40, line: 25, column: 3)
!40 = !DIFile(filename: "basicmath_small.c", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!41 = distinct !DISubprogram(name: "main", scope: !40, file: !40, line: 6, type: !42, isLocal: false, isDefinition: true, scopeLine: 7, flags: DIFlagPrototyped, isOptimized: true, unit: !44, variables: !45)
!42 = !DISubroutineType(types: !43)
!43 = !{!29}
!44 = distinct !DICompileUnit(language: DW_LANG_C99, file: !40, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !21)
!45 = !{!46, !48, !49, !50, !51, !52, !53, !54, !55, !56, !57, !58, !59, !60, !61, !62, !63, !67, !68, !69, !70, !71, !76}
!46 = !DILocalVariable(name: "a1", scope: !41, file: !40, line: 8, type: !47)
!47 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!48 = !DILocalVariable(name: "b1", scope: !41, file: !40, line: 8, type: !47)
!49 = !DILocalVariable(name: "c1", scope: !41, file: !40, line: 8, type: !47)
!50 = !DILocalVariable(name: "d1", scope: !41, file: !40, line: 8, type: !47)
!51 = !DILocalVariable(name: "a2", scope: !41, file: !40, line: 9, type: !47)
!52 = !DILocalVariable(name: "b2", scope: !41, file: !40, line: 9, type: !47)
!53 = !DILocalVariable(name: "c2", scope: !41, file: !40, line: 9, type: !47)
!54 = !DILocalVariable(name: "d2", scope: !41, file: !40, line: 9, type: !47)
!55 = !DILocalVariable(name: "a3", scope: !41, file: !40, line: 10, type: !47)
!56 = !DILocalVariable(name: "b3", scope: !41, file: !40, line: 10, type: !47)
!57 = !DILocalVariable(name: "c3", scope: !41, file: !40, line: 10, type: !47)
!58 = !DILocalVariable(name: "d3", scope: !41, file: !40, line: 10, type: !47)
!59 = !DILocalVariable(name: "a4", scope: !41, file: !40, line: 11, type: !47)
!60 = !DILocalVariable(name: "b4", scope: !41, file: !40, line: 11, type: !47)
!61 = !DILocalVariable(name: "c4", scope: !41, file: !40, line: 11, type: !47)
!62 = !DILocalVariable(name: "d4", scope: !41, file: !40, line: 11, type: !47)
!63 = !DILocalVariable(name: "x", scope: !41, file: !40, line: 12, type: !64)
!64 = !DICompositeType(tag: DW_TAG_array_type, baseType: !47, size: 192, elements: !65)
!65 = !{!66}
!66 = !DISubrange(count: 3)
!67 = !DILocalVariable(name: "X", scope: !41, file: !40, line: 13, type: !47)
!68 = !DILocalVariable(name: "solutions", scope: !41, file: !40, line: 14, type: !29)
!69 = !DILocalVariable(name: "i", scope: !41, file: !40, line: 15, type: !29)
!70 = !DILocalVariable(name: "l", scope: !41, file: !40, line: 16, type: !12)
!71 = !DILocalVariable(name: "q", scope: !41, file: !40, line: 17, type: !72)
!72 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "int_sqrt", file: !15, line: 67, size: 64, elements: !73)
!73 = !{!74, !75}
!74 = !DIDerivedType(tag: DW_TAG_member, name: "sqrt", scope: !72, file: !15, line: 68, baseType: !18, size: 32)
!75 = !DIDerivedType(tag: DW_TAG_member, name: "frac", scope: !72, file: !15, line: 69, baseType: !18, size: 32, offset: 32)
!76 = !DILocalVariable(name: "n", scope: !41, file: !40, line: 18, type: !77)
!77 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!78 = distinct !{!78, !79}
!79 = !DILocation(line: 32, column: 22, scope: !80)
!80 = distinct !DILexicalBlock(scope: !41, file: !40, line: 31, column: 3)
!81 = distinct !{!81, !82}
!82 = !DILocation(line: 37, column: 22, scope: !83)
!83 = distinct !DILexicalBlock(scope: !41, file: !40, line: 36, column: 3)
!84 = distinct !{!84, !85}
!85 = !DILocation(line: 42, column: 22, scope: !86)
!86 = distinct !DILexicalBlock(scope: !41, file: !40, line: 41, column: 3)
!87 = distinct !{!87, !88}
!88 = !DILocation(line: 52, column: 28, scope: !89)
!89 = distinct !DILexicalBlock(scope: !90, file: !40, line: 51, column: 11)
!90 = distinct !DILexicalBlock(scope: !91, file: !40, line: 48, column: 32)
!91 = distinct !DILexicalBlock(scope: !92, file: !40, line: 48, column: 9)
!92 = distinct !DILexicalBlock(scope: !93, file: !40, line: 48, column: 9)
!93 = distinct !DILexicalBlock(scope: !94, file: !40, line: 47, column: 31)
!94 = distinct !DILexicalBlock(scope: !95, file: !40, line: 47, column: 7)
!95 = distinct !DILexicalBlock(scope: !96, file: !40, line: 47, column: 7)
!96 = distinct !DILexicalBlock(scope: !97, file: !40, line: 46, column: 26)
!97 = distinct !DILexicalBlock(scope: !98, file: !40, line: 46, column: 5)
!98 = distinct !DILexicalBlock(scope: !99, file: !40, line: 46, column: 5)
!99 = distinct !DILexicalBlock(scope: !100, file: !40, line: 45, column: 24)
!100 = distinct !DILexicalBlock(scope: !101, file: !40, line: 45, column: 3)
!101 = distinct !DILexicalBlock(scope: !41, file: !40, line: 45, column: 3)
!102 = distinct !{!102, !103}
!103 = !DILocation(line: 56, column: 5, scope: !98)
!104 = distinct !{!104, !105}
!105 = !DILocation(line: 57, column: 3, scope: !101)
!106 = distinct !{!106, !107}
!107 = !DILocation(line: 69, column: 5, scope: !108)
!108 = distinct !DILexicalBlock(scope: !41, file: !40, line: 61, column: 3)
!109 = distinct !{!109, !110}
!110 = !DILocation(line: 78, column: 60, scope: !111)
!111 = distinct !DILexicalBlock(scope: !41, file: !40, line: 77, column: 3)
!112 = distinct !{!112, !113}
!113 = !DILocation(line: 81, column: 60, scope: !114)
!114 = distinct !DILexicalBlock(scope: !41, file: !40, line: 80, column: 3)
!115 = distinct !{!115, !116}
!116 = !DILocation(line: 55, column: 7, scope: !95)
!117 = !{!118, !118, i64 0}
!118 = !{!"any pointer", !33, i64 0}
!119 = !{!120, !120, i64 0}
!120 = !{!"long", !33, i64 0}
!121 = distinct !{!121, !122}
!122 = !DILocation(line: 223, column: 46, scope: !123)
!123 = distinct !DILexicalBlock(scope: !125, file: !124, line: 220, column: 14)
!124 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.c", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!125 = distinct !DISubprogram(name: "runOnCGRA", scope: !124, file: !124, line: 217, type: !126, isLocal: false, isDefinition: true, scopeLine: 218, flags: DIFlagPrototyped, isOptimized: true, unit: !129, variables: !166)
!126 = !DISubroutineType(types: !127)
!127 = !{!128, !128}
!128 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!129 = distinct !DICompileUnit(language: DW_LANG_C99, file: !124, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !21, retainedTypes: !130, globals: !133)
!130 = !{!131, !18, !77, !128, !132}
!131 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 32)
!132 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !29, size: 32)
!133 = !{!134, !137, !139, !141, !143, !145, !147, !149, !151, !153, !156, !158, !160, !162}
!134 = !DIGlobalVariableExpression(var: !135)
!135 = distinct !DIGlobalVariable(name: "totalLoops", scope: !129, file: !136, line: 27, type: !18, isLocal: false, isDefinition: true)
!136 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.h", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!137 = !DIGlobalVariableExpression(var: !138)
!138 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !129, file: !136, line: 30, type: !18, isLocal: false, isDefinition: true)
!139 = !DIGlobalVariableExpression(var: !140)
!140 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !129, file: !136, line: 31, type: !18, isLocal: false, isDefinition: true)
!141 = !DIGlobalVariableExpression(var: !142)
!142 = distinct !DIGlobalVariable(name: "thread_exit", scope: !129, file: !136, line: 32, type: !18, isLocal: false, isDefinition: true)
!143 = !DIGlobalVariableExpression(var: !144)
!144 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !129, file: !136, line: 34, type: !29, isLocal: false, isDefinition: true)
!145 = !DIGlobalVariableExpression(var: !146)
!146 = distinct !DIGlobalVariable(name: "prolog", scope: !129, file: !136, line: 21, type: !131, isLocal: false, isDefinition: true)
!147 = !DIGlobalVariableExpression(var: !148)
!148 = distinct !DIGlobalVariable(name: "epilog", scope: !129, file: !136, line: 21, type: !131, isLocal: false, isDefinition: true)
!149 = !DIGlobalVariableExpression(var: !150)
!150 = distinct !DIGlobalVariable(name: "kernel", scope: !129, file: !136, line: 21, type: !131, isLocal: false, isDefinition: true)
!151 = !DIGlobalVariableExpression(var: !152)
!152 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !129, file: !136, line: 22, type: !131, isLocal: false, isDefinition: true)
!153 = !DIGlobalVariableExpression(var: !154)
!154 = distinct !DIGlobalVariable(name: "prologPtr", scope: !129, file: !136, line: 24, type: !155, isLocal: false, isDefinition: true)
!155 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !77, size: 32)
!156 = !DIGlobalVariableExpression(var: !157)
!157 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !129, file: !136, line: 24, type: !155, isLocal: false, isDefinition: true)
!158 = !DIGlobalVariableExpression(var: !159)
!159 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !129, file: !136, line: 24, type: !155, isLocal: false, isDefinition: true)
!160 = !DIGlobalVariableExpression(var: !161)
!161 = distinct !DIGlobalVariable(name: "initCGRA", scope: !129, file: !136, line: 26, type: !132, isLocal: false, isDefinition: true)
!162 = !DIGlobalVariableExpression(var: !163)
!163 = distinct !DIGlobalVariable(name: "pth", scope: !129, file: !136, line: 29, type: !164, isLocal: false, isDefinition: true)
!164 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !165, line: 37, baseType: !12)
!165 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!166 = !{!167}
!167 = !DILocalVariable(name: "arg", arg: 1, scope: !125, file: !124, line: 217, type: !128)
!168 = !{i32 6148}
!169 = !{i32 7183}
!170 = distinct !{!170, !171}
!171 = !DILocation(line: 257, column: 5, scope: !125)
!172 = distinct !{!172, !173}
!173 = !DILocation(line: 277, column: 41, scope: !174)
!174 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !124, file: !124, line: 261, type: !175, isLocal: false, isDefinition: true, scopeLine: 262, flags: DIFlagPrototyped, isOptimized: true, unit: !129, variables: !177)
!175 = !DISubroutineType(types: !176)
!176 = !{null, !18}
!177 = !{!178, !179, !180}
!178 = !DILocalVariable(name: "loopNo", arg: 1, scope: !174, file: !124, line: 261, type: !18)
!179 = !DILocalVariable(name: "result", scope: !174, file: !124, line: 263, type: !29)
!180 = !DILocalVariable(name: "configure", scope: !174, file: !124, line: 266, type: !29)
!181 = distinct !{!181, !182}
!182 = !DILocation(line: 314, column: 29, scope: !183)
!183 = distinct !DILexicalBlock(scope: !184, file: !124, line: 313, column: 3)
!184 = distinct !DISubprogram(name: "createCGRA", scope: !124, file: !124, line: 293, type: !185, isLocal: false, isDefinition: true, scopeLine: 294, isOptimized: true, unit: !129, variables: !187)
!185 = !DISubroutineType(types: !186)
!186 = !{null}
!187 = !{!188, !189}
!188 = !DILocalVariable(name: "result", scope: !184, file: !124, line: 303, type: !29)
!189 = !DILocalVariable(name: "i", scope: !184, file: !124, line: 304, type: !18)
