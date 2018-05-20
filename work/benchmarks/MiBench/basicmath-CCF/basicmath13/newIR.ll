; ModuleID = 'combinedIR.bc'
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
@gVar1 = common local_unnamed_addr global i32 0
@gVar2 = common local_unnamed_addr global i32 0
@gVar3 = common local_unnamed_addr global i32 0
@gVar4 = common local_unnamed_addr global i32 0

; Function Attrs: norecurse nounwind
define void @usqrt(i32, %struct.int_sqrt* nocapture) local_unnamed_addr #0 {
  store i32 %0, i32* @gVar1
  br label %3

; <label>:3:                                      ; preds = %2
  call void @accelerateOnCGRA(i32 1)
  br label %4

; <label>:4:                                      ; preds = %3
  %5 = getelementptr inbounds %struct.int_sqrt, %struct.int_sqrt* %1, i32 0, i32 0
  %gVar2 = load i32, i32* @gVar2
  store i32 %gVar2, i32* %5, align 4
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
  store i32 3, i32* %4, align 4, !tbaa !5
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
  store double %38, double* %5, align 8, !tbaa !9
  %39 = tail call double @sqrt(double %13) #6
  %40 = fmul double %39, -2.000000e+00
  %41 = tail call double @atan(double 1.000000e+00) #6
  %42 = fadd double %31, 0x401921FB54442D18
  %43 = fdiv double %42, 3.000000e+00
  %44 = tail call double @cos(double %43) #6
  %45 = fmul double %40, %44
  %46 = fsub double %45, %37
  %47 = getelementptr inbounds double, double* %5, i32 1
  store double %46, double* %47, align 8, !tbaa !9
  %48 = tail call double @sqrt(double %13) #6
  %49 = fmul double %48, -2.000000e+00
  %50 = tail call double @atan(double 1.000000e+00) #6
  %51 = fadd double %31, 0x402921FB54442D18
  %52 = fdiv double %51, 3.000000e+00
  %53 = tail call double @cos(double %52) #6
  %54 = fmul double %49, %53
  %55 = fsub double %54, %37
  %56 = getelementptr inbounds double, double* %5, i32 2
  store double %55, double* %56, align 8, !tbaa !9
  br label %69

; <label>:57:                                     ; preds = %6
  store i32 1, i32* %4, align 4, !tbaa !5
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
  store double %68, double* %5, align 8, !tbaa !9
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
  call void @createCGRA()
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
  %8 = load i32, i32* %2, align 4, !tbaa !5
  %9 = icmp sgt i32 %8, 0
  br i1 %9, label %.preheader30.preheader, label %.loopexit31

.preheader30.preheader:                           ; preds = %0
  br label %.preheader30

.preheader30:                                     ; preds = %.preheader30, %.preheader30.preheader
  %10 = phi i32 [ %14, %.preheader30 ], [ 0, %.preheader30.preheader ]
  %11 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %10
  %12 = load double, double* %11, align 8, !tbaa !9
  %13 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %12)
  %14 = add nuw nsw i32 %10, 1
  %15 = icmp slt i32 %14, %8
  br i1 %15, label %.preheader30, label %.loopexit31.loopexit, !llvm.loop !11

.loopexit31.loopexit:                             ; preds = %.preheader30
  br label %.loopexit31

.loopexit31:                                      ; preds = %.loopexit31.loopexit, %0
  %16 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double 1.000000e+00, double -4.500000e+00, double 1.700000e+01, double -3.000000e+01, i32* nonnull %2, double* nonnull %6) #6
  %17 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %18 = load i32, i32* %2, align 4, !tbaa !5
  %19 = icmp sgt i32 %18, 0
  br i1 %19, label %.preheader28.preheader, label %.loopexit29

.preheader28.preheader:                           ; preds = %.loopexit31
  br label %.preheader28

.preheader28:                                     ; preds = %.preheader28, %.preheader28.preheader
  %20 = phi i32 [ %24, %.preheader28 ], [ 0, %.preheader28.preheader ]
  %21 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %20
  %22 = load double, double* %21, align 8, !tbaa !9
  %23 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %22)
  %24 = add nuw nsw i32 %20, 1
  %25 = icmp slt i32 %24, %18
  br i1 %25, label %.preheader28, label %.loopexit29.loopexit, !llvm.loop !57

.loopexit29.loopexit:                             ; preds = %.preheader28
  br label %.loopexit29

.loopexit29:                                      ; preds = %.loopexit29.loopexit, %.loopexit31
  %26 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double 1.000000e+00, double -3.500000e+00, double 2.200000e+01, double -3.100000e+01, i32* nonnull %2, double* nonnull %6) #6
  %27 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %28 = load i32, i32* %2, align 4, !tbaa !5
  %29 = icmp sgt i32 %28, 0
  br i1 %29, label %.preheader26.preheader, label %.loopexit27

.preheader26.preheader:                           ; preds = %.loopexit29
  br label %.preheader26

.preheader26:                                     ; preds = %.preheader26, %.preheader26.preheader
  %30 = phi i32 [ %34, %.preheader26 ], [ 0, %.preheader26.preheader ]
  %31 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %30
  %32 = load double, double* %31, align 8, !tbaa !9
  %33 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %32)
  %34 = add nuw nsw i32 %30, 1
  %35 = icmp slt i32 %34, %28
  br i1 %35, label %.preheader26, label %.loopexit27.loopexit, !llvm.loop !60

.loopexit27.loopexit:                             ; preds = %.preheader26
  br label %.loopexit27

.loopexit27:                                      ; preds = %.loopexit27.loopexit, %.loopexit29
  %36 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double 1.000000e+00, double -1.370000e+01, double 1.000000e+00, double -3.500000e+01, i32* nonnull %2, double* nonnull %6) #6
  %37 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %38 = load i32, i32* %2, align 4, !tbaa !5
  %39 = icmp sgt i32 %38, 0
  br i1 %39, label %.preheader24.preheader, label %.loopexit25

.preheader24.preheader:                           ; preds = %.loopexit27
  br label %.preheader24

.preheader24:                                     ; preds = %.preheader24, %.preheader24.preheader
  %40 = phi i32 [ %44, %.preheader24 ], [ 0, %.preheader24.preheader ]
  %41 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %40
  %42 = load double, double* %41, align 8, !tbaa !9
  %43 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %42)
  %44 = add nuw nsw i32 %40, 1
  %45 = icmp slt i32 %44, %38
  br i1 %45, label %.preheader24, label %.loopexit25.loopexit, !llvm.loop !63

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
  %54 = phi double [ 5.000000e+00, %50 ], [ %194, %.loopexit ]
  call void @SolveCubic(double %49, double %52, double %54, double -1.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %55 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %56 = load i32, i32* %2, align 4, !tbaa !5
  %57 = icmp sgt i32 %56, 0
  br i1 %57, label %.preheader22.preheader, label %.loopexit23

.preheader22.preheader:                           ; preds = %53
  br label %.preheader22

.preheader22:                                     ; preds = %.preheader22, %.preheader22.preheader
  %58 = phi i32 [ %62, %.preheader22 ], [ 0, %.preheader22.preheader ]
  %59 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %58
  %60 = load double, double* %59, align 8, !tbaa !9
  %61 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %60)
  %62 = add nuw nsw i32 %58, 1
  %63 = icmp slt i32 %62, %56
  br i1 %63, label %.preheader22, label %.loopexit23.loopexit, !llvm.loop !66

.loopexit23.loopexit:                             ; preds = %.preheader22
  br label %.loopexit23

.loopexit23:                                      ; preds = %.loopexit23.loopexit, %53
  %64 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -2.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %65 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %66 = load i32, i32* %2, align 4, !tbaa !5
  %67 = icmp sgt i32 %66, 0
  br i1 %67, label %.preheader20.preheader, label %.loopexit21

.preheader20.preheader:                           ; preds = %.loopexit23
  br label %.preheader20

; <label>:68:                                     ; preds = %.loopexit
  %69 = add nsw i32 %51, -1
  %70 = icmp sgt i32 %51, 1
  br i1 %70, label %50, label %71, !llvm.loop !81

; <label>:71:                                     ; preds = %68
  %72 = add nuw nsw i32 %48, 1
  %73 = icmp eq i32 %72, 10
  br i1 %73, label %74, label %47, !llvm.loop !83

; <label>:74:                                     ; preds = %71
  %75 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.11, i32 0, i32 0)) #6
  br label %76

; <label>:76:                                     ; preds = %usqrt.exit, %74
  %77 = phi i32 [ 0, %74 ], [ %80, %usqrt.exit ]
  store i32 %77, i32* @gVar3
  br label %78

; <label>:78:                                     ; preds = %76
  call void @accelerateOnCGRA(i32 2)
  br label %usqrt.exit

usqrt.exit:                                       ; preds = %78
  %gVar4 = load i32, i32* @gVar4
  %79 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.5, i32 0, i32 0), i32 %77, i32 %gVar4)
  %80 = add nuw nsw i32 %77, 1
  %81 = icmp eq i32 %80, 1001
  br i1 %81, label %.preheader.preheader, label %76, !llvm.loop !85

.preheader.preheader:                             ; preds = %usqrt.exit
  %82 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.6, i32 0, i32 0), i32 1072497001, i32 2146238464)
  %83 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([39 x i8], [39 x i8]* @str.12, i32 0, i32 0)) #6
  br label %84

; <label>:84:                                     ; preds = %84, %.preheader.preheader
  %85 = phi i32 [ 0, %.preheader.preheader ], [ %91, %84 ]
  %86 = sitofp i32 %85 to double
  %87 = tail call double @atan(double 1.000000e+00) #6
  %88 = fmul double %86, 0x400921FB54442D18
  %89 = fdiv double %88, 1.800000e+02
  %90 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.8, i32 0, i32 0), double %86, double %89)
  %91 = add nuw nsw i32 %85, 1
  %92 = icmp eq i32 %91, 361
  br i1 %92, label %93, label %84, !llvm.loop !88

; <label>:93:                                     ; preds = %84
  %94 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.9, i32 0, i32 0)) #6
  %95 = tail call double @atan(double 1.000000e+00) #6
  br label %96

; <label>:96:                                     ; preds = %96, %93
  %97 = phi double [ 0.000000e+00, %93 ], [ %103, %96 ]
  %98 = fmul double %97, 1.800000e+02
  %99 = tail call double @atan(double 1.000000e+00) #6
  %100 = fdiv double %98, 0x400921FB54442D18
  %101 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.10, i32 0, i32 0), double %97, double %100)
  %102 = tail call double @atan(double 1.000000e+00) #6
  %103 = fadd double %97, 0x3F91DF46A2529D39
  %104 = tail call double @atan(double 1.000000e+00) #6
  %105 = fcmp ugt double %103, 0x401921FB97600B9B
  br i1 %105, label %106, label %96, !llvm.loop !91

; <label>:106:                                    ; preds = %96
  call void @llvm.lifetime.end(i64 4, i8* nonnull %4) #6
  call void @llvm.lifetime.end(i64 24, i8* nonnull %3) #6
  ret i32 0

.preheader20:                                     ; preds = %.preheader20, %.preheader20.preheader
  %107 = phi i32 [ %111, %.preheader20 ], [ 0, %.preheader20.preheader ]
  %108 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %107
  %109 = load double, double* %108, align 8, !tbaa !9
  %110 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %109)
  %111 = add nuw nsw i32 %107, 1
  %112 = icmp slt i32 %111, %66
  br i1 %112, label %.preheader20, label %.loopexit21.loopexit, !llvm.loop !66

.loopexit21.loopexit:                             ; preds = %.preheader20
  br label %.loopexit21

.loopexit21:                                      ; preds = %.loopexit21.loopexit, %.loopexit23
  %113 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -3.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %114 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %115 = load i32, i32* %2, align 4, !tbaa !5
  %116 = icmp sgt i32 %115, 0
  br i1 %116, label %.preheader18.preheader, label %.loopexit19

.preheader18.preheader:                           ; preds = %.loopexit21
  br label %.preheader18

.preheader18:                                     ; preds = %.preheader18, %.preheader18.preheader
  %117 = phi i32 [ %121, %.preheader18 ], [ 0, %.preheader18.preheader ]
  %118 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %117
  %119 = load double, double* %118, align 8, !tbaa !9
  %120 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %119)
  %121 = add nuw nsw i32 %117, 1
  %122 = icmp slt i32 %121, %115
  br i1 %122, label %.preheader18, label %.loopexit19.loopexit, !llvm.loop !66

.loopexit19.loopexit:                             ; preds = %.preheader18
  br label %.loopexit19

.loopexit19:                                      ; preds = %.loopexit19.loopexit, %.loopexit21
  %123 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -4.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %124 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %125 = load i32, i32* %2, align 4, !tbaa !5
  %126 = icmp sgt i32 %125, 0
  br i1 %126, label %.preheader16.preheader, label %.loopexit17

.preheader16.preheader:                           ; preds = %.loopexit19
  br label %.preheader16

.preheader16:                                     ; preds = %.preheader16, %.preheader16.preheader
  %127 = phi i32 [ %131, %.preheader16 ], [ 0, %.preheader16.preheader ]
  %128 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %127
  %129 = load double, double* %128, align 8, !tbaa !9
  %130 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %129)
  %131 = add nuw nsw i32 %127, 1
  %132 = icmp slt i32 %131, %125
  br i1 %132, label %.preheader16, label %.loopexit17.loopexit, !llvm.loop !66

.loopexit17.loopexit:                             ; preds = %.preheader16
  br label %.loopexit17

.loopexit17:                                      ; preds = %.loopexit17.loopexit, %.loopexit19
  %133 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -5.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %134 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %135 = load i32, i32* %2, align 4, !tbaa !5
  %136 = icmp sgt i32 %135, 0
  br i1 %136, label %.preheader14.preheader, label %.loopexit15

.preheader14.preheader:                           ; preds = %.loopexit17
  br label %.preheader14

.preheader14:                                     ; preds = %.preheader14, %.preheader14.preheader
  %137 = phi i32 [ %141, %.preheader14 ], [ 0, %.preheader14.preheader ]
  %138 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %137
  %139 = load double, double* %138, align 8, !tbaa !9
  %140 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %139)
  %141 = add nuw nsw i32 %137, 1
  %142 = icmp slt i32 %141, %135
  br i1 %142, label %.preheader14, label %.loopexit15.loopexit, !llvm.loop !66

.loopexit15.loopexit:                             ; preds = %.preheader14
  br label %.loopexit15

.loopexit15:                                      ; preds = %.loopexit15.loopexit, %.loopexit17
  %143 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -6.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %144 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %145 = load i32, i32* %2, align 4, !tbaa !5
  %146 = icmp sgt i32 %145, 0
  br i1 %146, label %.preheader12.preheader, label %.loopexit13

.preheader12.preheader:                           ; preds = %.loopexit15
  br label %.preheader12

.preheader12:                                     ; preds = %.preheader12, %.preheader12.preheader
  %147 = phi i32 [ %151, %.preheader12 ], [ 0, %.preheader12.preheader ]
  %148 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %147
  %149 = load double, double* %148, align 8, !tbaa !9
  %150 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %149)
  %151 = add nuw nsw i32 %147, 1
  %152 = icmp slt i32 %151, %145
  br i1 %152, label %.preheader12, label %.loopexit13.loopexit, !llvm.loop !66

.loopexit13.loopexit:                             ; preds = %.preheader12
  br label %.loopexit13

.loopexit13:                                      ; preds = %.loopexit13.loopexit, %.loopexit15
  %153 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -7.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %154 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %155 = load i32, i32* %2, align 4, !tbaa !5
  %156 = icmp sgt i32 %155, 0
  br i1 %156, label %.preheader10.preheader, label %.loopexit11

.preheader10.preheader:                           ; preds = %.loopexit13
  br label %.preheader10

.preheader10:                                     ; preds = %.preheader10, %.preheader10.preheader
  %157 = phi i32 [ %161, %.preheader10 ], [ 0, %.preheader10.preheader ]
  %158 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %157
  %159 = load double, double* %158, align 8, !tbaa !9
  %160 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %159)
  %161 = add nuw nsw i32 %157, 1
  %162 = icmp slt i32 %161, %155
  br i1 %162, label %.preheader10, label %.loopexit11.loopexit, !llvm.loop !66

.loopexit11.loopexit:                             ; preds = %.preheader10
  br label %.loopexit11

.loopexit11:                                      ; preds = %.loopexit11.loopexit, %.loopexit13
  %163 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -8.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %164 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %165 = load i32, i32* %2, align 4, !tbaa !5
  %166 = icmp sgt i32 %165, 0
  br i1 %166, label %.preheader8.preheader, label %.loopexit9

.preheader8.preheader:                            ; preds = %.loopexit11
  br label %.preheader8

.preheader8:                                      ; preds = %.preheader8, %.preheader8.preheader
  %167 = phi i32 [ %171, %.preheader8 ], [ 0, %.preheader8.preheader ]
  %168 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %167
  %169 = load double, double* %168, align 8, !tbaa !9
  %170 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %169)
  %171 = add nuw nsw i32 %167, 1
  %172 = icmp slt i32 %171, %165
  br i1 %172, label %.preheader8, label %.loopexit9.loopexit, !llvm.loop !66

.loopexit9.loopexit:                              ; preds = %.preheader8
  br label %.loopexit9

.loopexit9:                                       ; preds = %.loopexit9.loopexit, %.loopexit11
  %173 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -9.000000e+00, i32* nonnull %2, double* nonnull %6) #6
  %174 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %175 = load i32, i32* %2, align 4, !tbaa !5
  %176 = icmp sgt i32 %175, 0
  br i1 %176, label %.preheader6.preheader, label %.loopexit7

.preheader6.preheader:                            ; preds = %.loopexit9
  br label %.preheader6

.preheader6:                                      ; preds = %.preheader6, %.preheader6.preheader
  %177 = phi i32 [ %181, %.preheader6 ], [ 0, %.preheader6.preheader ]
  %178 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %177
  %179 = load double, double* %178, align 8, !tbaa !9
  %180 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %179)
  %181 = add nuw nsw i32 %177, 1
  %182 = icmp slt i32 %181, %175
  br i1 %182, label %.preheader6, label %.loopexit7.loopexit, !llvm.loop !66

.loopexit7.loopexit:                              ; preds = %.preheader6
  br label %.loopexit7

.loopexit7:                                       ; preds = %.loopexit7.loopexit, %.loopexit9
  %183 = tail call i32 @putchar(i32 10) #6
  call void @SolveCubic(double %49, double %52, double %54, double -1.000000e+01, i32* nonnull %2, double* nonnull %6) #6
  %184 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0))
  %185 = load i32, i32* %2, align 4, !tbaa !5
  %186 = icmp sgt i32 %185, 0
  br i1 %186, label %.preheader5.preheader, label %.loopexit

.preheader5.preheader:                            ; preds = %.loopexit7
  br label %.preheader5

.preheader5:                                      ; preds = %.preheader5, %.preheader5.preheader
  %187 = phi i32 [ %191, %.preheader5 ], [ 0, %.preheader5.preheader ]
  %188 = getelementptr inbounds [3 x double], [3 x double]* %1, i32 0, i32 %187
  %189 = load double, double* %188, align 8, !tbaa !9
  %190 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double %189)
  %191 = add nuw nsw i32 %187, 1
  %192 = icmp slt i32 %191, %185
  br i1 %192, label %.preheader5, label %.loopexit.loopexit, !llvm.loop !66

.loopexit.loopexit:                               ; preds = %.preheader5
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %.loopexit7
  %193 = tail call i32 @putchar(i32 10) #6
  %194 = fadd double %54, 5.000000e-01
  %195 = fcmp olt double %194, 1.500000e+01
  br i1 %195, label %53, label %68, !llvm.loop !94
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
  %14 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %15 = add i32 %0, -1
  %16 = mul i32 %15, 7
  %17 = getelementptr inbounds i32, i32* %14, i32 %16
  store i32 2011168768, i32* %17, align 4, !tbaa !5
  %18 = getelementptr inbounds i32, i32* %17, i32 1
  store i32 2011168768, i32* %18, align 4, !tbaa !5
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
  %48 = load i32, i32* %8, align 4, !tbaa !5
  %49 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.5.3, i32 0, i32 0), i32 %48)
  %50 = load i32, i32* %9, align 4, !tbaa !5
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6.4, i32 0, i32 0), i32 %50)
  %52 = load i32, i32* %10, align 4, !tbaa !5
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7, i32 0, i32 0), i32 %52)
  %54 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.8.5, i32 0, i32 0), i32 4)
  %55 = shl i32 %48, 2
  %56 = call noalias i8* @malloc(i32 %55) #6
  store i8* %56, i8** bitcast (i32** @epilog to i8**), align 4, !tbaa !96
  %57 = shl i32 %50, 2
  %58 = call noalias i8* @malloc(i32 %57) #6
  store i8* %58, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !96
  %59 = shl i32 %52, 2
  %60 = call noalias i8* @malloc(i32 %59) #6
  store i8* %60, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !96
  %61 = call i32 @fread(i8* %56, i32 4, i32 %48, %struct._IO_FILE* %38)
  %62 = load i8*, i8** bitcast (i32** @prolog to i8**), align 4, !tbaa !96
  %63 = call i32 @fread(i8* %62, i32 4, i32 %50, %struct._IO_FILE* %39)
  %64 = load i8*, i8** bitcast (i32** @kernel to i8**), align 4, !tbaa !96
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
  %74 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %75 = getelementptr inbounds i32, i32* %74, i32 %16
  %76 = getelementptr inbounds i32, i32* %75, i32 2
  store i32 %71, i32* %76, align 4, !tbaa !5
  %77 = getelementptr inbounds i32, i32* %75, i32 3
  store i32 %72, i32* %77, align 4, !tbaa !5
  %78 = getelementptr inbounds i32, i32* %75, i32 4
  store i32 %73, i32* %78, align 4, !tbaa !5
  %79 = load i32, i32* %11, align 4, !tbaa !5
  %80 = sdiv i32 %79, 16
  %81 = getelementptr inbounds i32, i32* %75, i32 6
  store i32 %80, i32* %81, align 4, !tbaa !5
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
  store i32 0, i32* %13, align 4, !tbaa !5
  %91 = call %struct._IO_FILE* @fopen(i8* nonnull %86, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.10.7, i32 0, i32 0))
  %92 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %91, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* nonnull %13) #6
  %93 = call i32 @fclose(%struct._IO_FILE* %91)
  %94 = load i32, i32* %13, align 4, !tbaa !5
  %95 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.12, i32 0, i32 0), i32 %94)
  %96 = load i32, i32* %13, align 4, !tbaa !5
  %97 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %98 = getelementptr inbounds i32, i32* %97, i32 %16
  %99 = getelementptr inbounds i32, i32* %98, i32 5
  store i32 %96, i32* %99, align 4, !tbaa !5
  %100 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !96
  %101 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !96
  %102 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !96
  %103 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([52 x i8], [52 x i8]* @.str.13, i32 0, i32 0), i32 %100, i32 %101, i32 %102)
  %104 = load i32, i32* bitcast (i32** @prolog to i32*), align 4, !tbaa !96
  %105 = load i32*, i32** @prologPtr, align 4, !tbaa !96
  %106 = getelementptr inbounds i32, i32* %105, i32 %15
  store i32 %104, i32* %106, align 4, !tbaa !98
  %107 = load i32, i32* bitcast (i32** @epilog to i32*), align 4, !tbaa !96
  %108 = load i32*, i32** @epilogPtr, align 4, !tbaa !96
  %109 = getelementptr inbounds i32, i32* %108, i32 %15
  store i32 %107, i32* %109, align 4, !tbaa !98
  %110 = load i32, i32* bitcast (i32** @kernel to i32*), align 4, !tbaa !96
  %111 = load i32*, i32** @kernelPtr, align 4, !tbaa !96
  %112 = getelementptr inbounds i32, i32* %111, i32 %15
  store i32 %110, i32* %112, align 4, !tbaa !98
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
  %5 = load i32*, i32** @initCGRA, align 4, !tbaa !96
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
  %13 = call i32 (i8*, i8*, ...) @sprintf(i8* nonnull %11, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %0) #6
  %14 = call i8* @strcat(i8* nonnull %12, i8* nonnull %11) #6
  %15 = icmp slt i32 %10, 1
  br i1 %15, label %16, label %22

; <label>:16:                                     ; preds = %1
  %17 = load i32, i32* @dynamicTCVal, align 4, !tbaa !5
  %18 = add nsw i32 %17, %10
  %19 = load i32*, i32** @initCGRA, align 4, !tbaa !96
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
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %25, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.14, i32 0, i32 0), i32 14, i32 1, i1 false)
  %26 = call %struct._IO_FILE* @fopen(i8* nonnull %23, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.15, i32 0, i32 0))
  %27 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %28 = getelementptr inbounds i32, i32* %27, i32 %7
  %29 = load i32, i32* %28, align 4, !tbaa !5
  %30 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %29)
  %31 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %32 = getelementptr inbounds i32, i32* %31, i32 %7
  %33 = getelementptr inbounds i32, i32* %32, i32 1
  %34 = load i32, i32* %33, align 4, !tbaa !5
  %35 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %34)
  %36 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %37 = getelementptr inbounds i32, i32* %36, i32 %7
  %38 = getelementptr inbounds i32, i32* %37, i32 2
  %39 = load i32, i32* %38, align 4, !tbaa !5
  %40 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %39)
  %41 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %42 = getelementptr inbounds i32, i32* %41, i32 %7
  %43 = getelementptr inbounds i32, i32* %42, i32 3
  %44 = load i32, i32* %43, align 4, !tbaa !5
  %45 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %44)
  %46 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %47 = getelementptr inbounds i32, i32* %46, i32 %7
  %48 = getelementptr inbounds i32, i32* %47, i32 4
  %49 = load i32, i32* %48, align 4, !tbaa !5
  %50 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %49)
  %51 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %52 = getelementptr inbounds i32, i32* %51, i32 %7
  %53 = getelementptr inbounds i32, i32* %52, i32 5
  %54 = load i32, i32* %53, align 4, !tbaa !5
  %55 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %54)
  %56 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %57 = getelementptr inbounds i32, i32* %56, i32 %7
  %58 = getelementptr inbounds i32, i32* %57, i32 6
  %59 = load i32, i32* %58, align 4, !tbaa !5
  %60 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.16, i32 0, i32 0), i32 %59)
  %61 = load i32*, i32** @epilogPtr, align 4, !tbaa !96
  %62 = getelementptr inbounds i32, i32* %61, i32 %6
  %63 = load i32, i32* %62, align 4, !tbaa !98
  %64 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %63)
  %65 = load i32*, i32** @prologPtr, align 4, !tbaa !96
  %66 = getelementptr inbounds i32, i32* %65, i32 %6
  %67 = load i32, i32* %66, align 4, !tbaa !98
  %68 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %26, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.17, i32 0, i32 0), i32 %67)
  %69 = load i32*, i32** @kernelPtr, align 4, !tbaa !96
  %70 = getelementptr inbounds i32, i32* %69, i32 %6
  %71 = load i32, i32* %70, align 4, !tbaa !98
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
  %2 = load i32*, i32** @initCGRA, align 4, !tbaa !96
  %3 = getelementptr inbounds i32, i32* %2, i32 5
  %4 = load i32, i32* %3, align 4, !tbaa !5
  %5 = icmp sgt i32 %4, 0
  br i1 %5, label %8, label %6

; <label>:6:                                      ; preds = %1
  %7 = load i32, i32* @dynamicTCVal, align 4, !tbaa !5
  store i32 %7, i32* %3, align 4, !tbaa !5
  br label %8

; <label>:8:                                      ; preds = %6, %1
  ret void
}

; Function Attrs: nounwind
define noalias i8* @runOnCGRA(i8* nocapture readnone) #1 {
  br label %2

; <label>:2:                                      ; preds = %11, %1
  %3 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !5
  %4 = icmp eq i32 %3, 1
  br i1 %4, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %2
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %5 = tail call i32 @usleep(i32 1) #6
  %6 = load i32, i32* @thread_cond_cgra, align 4, !tbaa !5
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %.loopexit.loopexit, label %.preheader, !llvm.loop !100

.loopexit.loopexit:                               ; preds = %.preheader
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %2
  %8 = load i32, i32* @thread_exit, align 4, !tbaa !5
  %9 = icmp eq i32 %8, 1
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %.loopexit
  tail call void asm sideeffect "mov r11,$0", "r"(i32 17) #6, !srcloc !147
  ret i8* null

; <label>:11:                                     ; preds = %.loopexit
  store i32 0, i32* @thread_cond_cgra, align 4, !tbaa !5
  tail call void asm sideeffect "mov r11,$0", "r"(i32 15) #6, !srcloc !148
  %12 = tail call i32 @usleep(i32 1) #6
  store i32 1, i32* @thread_cond_cpu, align 4, !tbaa !5
  br label %2, !llvm.loop !149
}

declare i32 @usleep(i32) local_unnamed_addr #5

; Function Attrs: nounwind
define void @accelerateOnCGRA(i32) local_unnamed_addr #1 {
  %2 = tail call i32 @configureCGRA(i32 %0)
  %3 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.20, i32 0, i32 0), i32 %0)
  store i32 0, i32* @thread_cond_cpu, align 4, !tbaa !5
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !5
  br label %4

; <label>:4:                                      ; preds = %4, %1
  %5 = tail call i32 @usleep(i32 1) #6
  %6 = load i32, i32* @thread_cond_cpu, align 4, !tbaa !5
  %7 = icmp eq i32 %6, 1
  br i1 %7, label %8, label %4, !llvm.loop !151

; <label>:8:                                      ; preds = %4
  ret void
}

; Function Attrs: nounwind
define void @deleteCGRA() local_unnamed_addr #1 {
  %1 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.8, i32 0, i32 0)) #6
  store i32 1, i32* @thread_exit, align 4, !tbaa !5
  store i32 1, i32* @thread_cond_cgra, align 4, !tbaa !5
  %2 = tail call i32 bitcast (i32 (...)* @puts to i32 (i8*)*)(i8* getelementptr inbounds ([40 x i8], [40 x i8]* @str.24, i32 0, i32 0)) #6
  %3 = load i32, i32* @pth, align 4, !tbaa !98
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
  %6 = load i32, i32* @totalLoops, align 4, !tbaa !5
  %7 = mul i32 %6, 28
  %8 = tail call noalias i8* @malloc(i32 %7) #6
  store i8* %8, i8** bitcast (i32** @initCGRA to i8**), align 4, !tbaa !96
  %9 = shl i32 %6, 2
  %10 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %10, i8** bitcast (i32** @prologPtr to i8**), align 4, !tbaa !96
  %11 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %11, i8** bitcast (i32** @kernelPtr to i8**), align 4, !tbaa !96
  %12 = tail call noalias i8* @malloc(i32 %9) #6
  store i8* %12, i8** bitcast (i32** @epilogPtr to i8**), align 4, !tbaa !96
  %13 = icmp eq i32 %6, 0
  br i1 %13, label %.loopexit, label %.preheader.preheader

.preheader.preheader:                             ; preds = %0
  br label %.preheader

.preheader:                                       ; preds = %.preheader, %.preheader.preheader
  %14 = phi i32 [ %16, %.preheader ], [ 1, %.preheader.preheader ]
  %15 = tail call i32 @initializeParameters(i32 %14)
  %16 = add i32 %14, 1
  %17 = load i32, i32* @totalLoops, align 4, !tbaa !5
  %18 = icmp ugt i32 %16, %17
  br i1 %18, label %.loopexit.loopexit, label %.preheader, !llvm.loop !160

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
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{!10, !10, i64 0}
!10 = !{!"double", !7, i64 0}
!11 = distinct !{!11, !12}
!12 = !DILocation(line: 26, column: 22, scope: !13)
!13 = distinct !DILexicalBlock(scope: !15, file: !14, line: 25, column: 3)
!14 = !DIFile(filename: "basicmath_small.c", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!15 = distinct !DISubprogram(name: "main", scope: !14, file: !14, line: 6, type: !16, isLocal: false, isDefinition: true, scopeLine: 7, flags: DIFlagPrototyped, isOptimized: true, unit: !19, variables: !21)
!16 = !DISubroutineType(types: !17)
!17 = !{!18}
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = distinct !DICompileUnit(language: DW_LANG_C99, file: !14, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !20)
!20 = !{}
!21 = !{!22, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33, !34, !35, !36, !37, !38, !39, !43, !44, !45, !46, !48, !55}
!22 = !DILocalVariable(name: "a1", scope: !15, file: !14, line: 8, type: !23)
!23 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!24 = !DILocalVariable(name: "b1", scope: !15, file: !14, line: 8, type: !23)
!25 = !DILocalVariable(name: "c1", scope: !15, file: !14, line: 8, type: !23)
!26 = !DILocalVariable(name: "d1", scope: !15, file: !14, line: 8, type: !23)
!27 = !DILocalVariable(name: "a2", scope: !15, file: !14, line: 9, type: !23)
!28 = !DILocalVariable(name: "b2", scope: !15, file: !14, line: 9, type: !23)
!29 = !DILocalVariable(name: "c2", scope: !15, file: !14, line: 9, type: !23)
!30 = !DILocalVariable(name: "d2", scope: !15, file: !14, line: 9, type: !23)
!31 = !DILocalVariable(name: "a3", scope: !15, file: !14, line: 10, type: !23)
!32 = !DILocalVariable(name: "b3", scope: !15, file: !14, line: 10, type: !23)
!33 = !DILocalVariable(name: "c3", scope: !15, file: !14, line: 10, type: !23)
!34 = !DILocalVariable(name: "d3", scope: !15, file: !14, line: 10, type: !23)
!35 = !DILocalVariable(name: "a4", scope: !15, file: !14, line: 11, type: !23)
!36 = !DILocalVariable(name: "b4", scope: !15, file: !14, line: 11, type: !23)
!37 = !DILocalVariable(name: "c4", scope: !15, file: !14, line: 11, type: !23)
!38 = !DILocalVariable(name: "d4", scope: !15, file: !14, line: 11, type: !23)
!39 = !DILocalVariable(name: "x", scope: !15, file: !14, line: 12, type: !40)
!40 = !DICompositeType(tag: DW_TAG_array_type, baseType: !23, size: 192, elements: !41)
!41 = !{!42}
!42 = !DISubrange(count: 3)
!43 = !DILocalVariable(name: "X", scope: !15, file: !14, line: 13, type: !23)
!44 = !DILocalVariable(name: "solutions", scope: !15, file: !14, line: 14, type: !18)
!45 = !DILocalVariable(name: "i", scope: !15, file: !14, line: 15, type: !18)
!46 = !DILocalVariable(name: "l", scope: !15, file: !14, line: 16, type: !47)
!47 = !DIBasicType(name: "long unsigned int", size: 32, encoding: DW_ATE_unsigned)
!48 = !DILocalVariable(name: "q", scope: !15, file: !14, line: 17, type: !49)
!49 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "int_sqrt", file: !50, line: 67, size: 64, elements: !51)
!50 = !DIFile(filename: "./snipmath.h", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!51 = !{!52, !54}
!52 = !DIDerivedType(tag: DW_TAG_member, name: "sqrt", scope: !49, file: !50, line: 68, baseType: !53, size: 32)
!53 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!54 = !DIDerivedType(tag: DW_TAG_member, name: "frac", scope: !49, file: !50, line: 69, baseType: !53, size: 32, offset: 32)
!55 = !DILocalVariable(name: "n", scope: !15, file: !14, line: 18, type: !56)
!56 = !DIBasicType(name: "long int", size: 32, encoding: DW_ATE_signed)
!57 = distinct !{!57, !58}
!58 = !DILocation(line: 32, column: 22, scope: !59)
!59 = distinct !DILexicalBlock(scope: !15, file: !14, line: 31, column: 3)
!60 = distinct !{!60, !61}
!61 = !DILocation(line: 37, column: 22, scope: !62)
!62 = distinct !DILexicalBlock(scope: !15, file: !14, line: 36, column: 3)
!63 = distinct !{!63, !64}
!64 = !DILocation(line: 42, column: 22, scope: !65)
!65 = distinct !DILexicalBlock(scope: !15, file: !14, line: 41, column: 3)
!66 = distinct !{!66, !67}
!67 = !DILocation(line: 52, column: 28, scope: !68)
!68 = distinct !DILexicalBlock(scope: !69, file: !14, line: 51, column: 11)
!69 = distinct !DILexicalBlock(scope: !70, file: !14, line: 48, column: 32)
!70 = distinct !DILexicalBlock(scope: !71, file: !14, line: 48, column: 9)
!71 = distinct !DILexicalBlock(scope: !72, file: !14, line: 48, column: 9)
!72 = distinct !DILexicalBlock(scope: !73, file: !14, line: 47, column: 31)
!73 = distinct !DILexicalBlock(scope: !74, file: !14, line: 47, column: 7)
!74 = distinct !DILexicalBlock(scope: !75, file: !14, line: 47, column: 7)
!75 = distinct !DILexicalBlock(scope: !76, file: !14, line: 46, column: 26)
!76 = distinct !DILexicalBlock(scope: !77, file: !14, line: 46, column: 5)
!77 = distinct !DILexicalBlock(scope: !78, file: !14, line: 46, column: 5)
!78 = distinct !DILexicalBlock(scope: !79, file: !14, line: 45, column: 24)
!79 = distinct !DILexicalBlock(scope: !80, file: !14, line: 45, column: 3)
!80 = distinct !DILexicalBlock(scope: !15, file: !14, line: 45, column: 3)
!81 = distinct !{!81, !82}
!82 = !DILocation(line: 56, column: 5, scope: !77)
!83 = distinct !{!83, !84}
!84 = !DILocation(line: 57, column: 3, scope: !80)
!85 = distinct !{!85, !86}
!86 = !DILocation(line: 69, column: 5, scope: !87)
!87 = distinct !DILexicalBlock(scope: !15, file: !14, line: 61, column: 3)
!88 = distinct !{!88, !89}
!89 = !DILocation(line: 78, column: 60, scope: !90)
!90 = distinct !DILexicalBlock(scope: !15, file: !14, line: 77, column: 3)
!91 = distinct !{!91, !92}
!92 = !DILocation(line: 81, column: 60, scope: !93)
!93 = distinct !DILexicalBlock(scope: !15, file: !14, line: 80, column: 3)
!94 = distinct !{!94, !95}
!95 = !DILocation(line: 55, column: 7, scope: !74)
!96 = !{!97, !97, i64 0}
!97 = !{!"any pointer", !7, i64 0}
!98 = !{!99, !99, i64 0}
!99 = !{!"long", !7, i64 0}
!100 = distinct !{!100, !101}
!101 = !DILocation(line: 223, column: 46, scope: !102)
!102 = distinct !DILexicalBlock(scope: !104, file: !103, line: 220, column: 14)
!103 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.c", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!104 = distinct !DISubprogram(name: "runOnCGRA", scope: !103, file: !103, line: 217, type: !105, isLocal: false, isDefinition: true, scopeLine: 218, flags: DIFlagPrototyped, isOptimized: true, unit: !108, variables: !145)
!105 = !DISubroutineType(types: !106)
!106 = !{!107, !107}
!107 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!108 = distinct !DICompileUnit(language: DW_LANG_C99, file: !103, producer: "clang version 5.0.0 (trunk 296996) (llvm/trunk 296995)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !20, retainedTypes: !109, globals: !112)
!109 = !{!110, !53, !56, !107, !111}
!110 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !53, size: 32)
!111 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 32)
!112 = !{!113, !116, !118, !120, !122, !124, !126, !128, !130, !132, !135, !137, !139, !141}
!113 = !DIGlobalVariableExpression(var: !114)
!114 = distinct !DIGlobalVariable(name: "totalLoops", scope: !108, file: !115, line: 27, type: !53, isLocal: false, isDefinition: true)
!115 = !DIFile(filename: "/home/shail/CGRA/scripts/CGRALib/cgra.h", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!116 = !DIGlobalVariableExpression(var: !117)
!117 = distinct !DIGlobalVariable(name: "thread_cond_cpu", scope: !108, file: !115, line: 30, type: !53, isLocal: false, isDefinition: true)
!118 = !DIGlobalVariableExpression(var: !119)
!119 = distinct !DIGlobalVariable(name: "thread_cond_cgra", scope: !108, file: !115, line: 31, type: !53, isLocal: false, isDefinition: true)
!120 = !DIGlobalVariableExpression(var: !121)
!121 = distinct !DIGlobalVariable(name: "thread_exit", scope: !108, file: !115, line: 32, type: !53, isLocal: false, isDefinition: true)
!122 = !DIGlobalVariableExpression(var: !123)
!123 = distinct !DIGlobalVariable(name: "dynamicTCVal", scope: !108, file: !115, line: 34, type: !18, isLocal: false, isDefinition: true)
!124 = !DIGlobalVariableExpression(var: !125)
!125 = distinct !DIGlobalVariable(name: "prolog", scope: !108, file: !115, line: 21, type: !110, isLocal: false, isDefinition: true)
!126 = !DIGlobalVariableExpression(var: !127)
!127 = distinct !DIGlobalVariable(name: "epilog", scope: !108, file: !115, line: 21, type: !110, isLocal: false, isDefinition: true)
!128 = !DIGlobalVariableExpression(var: !129)
!129 = distinct !DIGlobalVariable(name: "kernel", scope: !108, file: !115, line: 21, type: !110, isLocal: false, isDefinition: true)
!130 = !DIGlobalVariableExpression(var: !131)
!131 = distinct !DIGlobalVariable(name: "ArrPtr", scope: !108, file: !115, line: 22, type: !110, isLocal: false, isDefinition: true)
!132 = !DIGlobalVariableExpression(var: !133)
!133 = distinct !DIGlobalVariable(name: "prologPtr", scope: !108, file: !115, line: 24, type: !134, isLocal: false, isDefinition: true)
!134 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !56, size: 32)
!135 = !DIGlobalVariableExpression(var: !136)
!136 = distinct !DIGlobalVariable(name: "kernelPtr", scope: !108, file: !115, line: 24, type: !134, isLocal: false, isDefinition: true)
!137 = !DIGlobalVariableExpression(var: !138)
!138 = distinct !DIGlobalVariable(name: "epilogPtr", scope: !108, file: !115, line: 24, type: !134, isLocal: false, isDefinition: true)
!139 = !DIGlobalVariableExpression(var: !140)
!140 = distinct !DIGlobalVariable(name: "initCGRA", scope: !108, file: !115, line: 26, type: !111, isLocal: false, isDefinition: true)
!141 = !DIGlobalVariableExpression(var: !142)
!142 = distinct !DIGlobalVariable(name: "pth", scope: !108, file: !115, line: 29, type: !143, isLocal: false, isDefinition: true)
!143 = !DIDerivedType(tag: DW_TAG_typedef, name: "pthread_t", file: !144, line: 37, baseType: !47)
!144 = !DIFile(filename: "/usr/arm-linux-gnueabi/include/bits/pthreadtypes.h", directory: "/home/shail/Results-extra/MiBench/basicmath-CCF/basicmath13")
!145 = !{!146}
!146 = !DILocalVariable(name: "arg", arg: 1, scope: !104, file: !103, line: 217, type: !107)
!147 = !{i32 6148}
!148 = !{i32 7183}
!149 = distinct !{!149, !150}
!150 = !DILocation(line: 257, column: 5, scope: !104)
!151 = distinct !{!151, !152}
!152 = !DILocation(line: 277, column: 41, scope: !153)
!153 = distinct !DISubprogram(name: "accelerateOnCGRA", scope: !103, file: !103, line: 261, type: !154, isLocal: false, isDefinition: true, scopeLine: 262, flags: DIFlagPrototyped, isOptimized: true, unit: !108, variables: !156)
!154 = !DISubroutineType(types: !155)
!155 = !{null, !53}
!156 = !{!157, !158, !159}
!157 = !DILocalVariable(name: "loopNo", arg: 1, scope: !153, file: !103, line: 261, type: !53)
!158 = !DILocalVariable(name: "result", scope: !153, file: !103, line: 263, type: !18)
!159 = !DILocalVariable(name: "configure", scope: !153, file: !103, line: 266, type: !18)
!160 = distinct !{!160, !161}
!161 = !DILocation(line: 314, column: 29, scope: !162)
!162 = distinct !DILexicalBlock(scope: !163, file: !103, line: 313, column: 3)
!163 = distinct !DISubprogram(name: "createCGRA", scope: !103, file: !103, line: 293, type: !164, isLocal: false, isDefinition: true, scopeLine: 294, isOptimized: true, unit: !108, variables: !166)
!164 = !DISubroutineType(types: !165)
!165 = !{null}
!166 = !{!167, !168}
!167 = !DILocalVariable(name: "result", scope: !163, file: !103, line: 303, type: !18)
!168 = !DILocalVariable(name: "i", scope: !163, file: !103, line: 304, type: !53)
