; RUN: llc -march=amdgcn -verify-machineinstrs < %s | FileCheck -check-prefix=GCN -check-prefix=SI %s
; RUN: llc -march=amdgcn -mcpu=tonga -mattr=-flat-for-global -verify-machineinstrs < %s | FileCheck -check-prefix=GCN -check-prefix=VI %s
; RUN: llc -march=amdgcn -mcpu=gfx901 -mattr=-flat-for-global -verify-machineinstrs < %s | FileCheck -check-prefix=GCN -check-prefix=VI %s

declare half @llvm.copysign.f16(half, half)
declare float @llvm.copysign.f32(float, float)
declare double @llvm.copysign.f64(double, double)
declare <2 x half> @llvm.copysign.v2f16(<2 x half>, <2 x half>)
declare <3 x half> @llvm.copysign.v3f16(<3 x half>, <3 x half>)
declare <4 x half> @llvm.copysign.v4f16(<4 x half>, <4 x half>)

; GCN-LABEL: {{^}}test_copysign_f16:
; SI: buffer_load_ushort v[[MAG:[0-9]+]]
; SI: buffer_load_ushort v[[SIGN:[0-9]+]]
; SI: s_brev_b32 s[[CONST:[0-9]+]], -2
; SI: v_cvt_f32_f16_e32 v[[MAG_F32:[0-9]+]], v[[MAG]]
; SI: v_cvt_f32_f16_e32 v[[SIGN_F32:[0-9]+]], v[[SIGN]]
; SI: v_bfi_b32 v[[OUT_F32:[0-9]+]], s[[CONST]], v[[MAG_F32]], v[[SIGN_F32]]
; SI: v_cvt_f16_f32_e32 v[[OUT:[0-9]+]], v[[OUT_F32]]
; VI: buffer_load_ushort v[[SIGN:[0-9]+]]
; VI: buffer_load_ushort v[[MAG:[0-9]+]]
; VI: s_movk_i32 s[[CONST:[0-9]+]], 0x7fff
; VI: v_bfi_b32 v[[OUT:[0-9]+]], s[[CONST]], v[[MAG]], v[[SIGN]]
; GCN: buffer_store_short v[[OUT]]
; GCN: s_endpgm
define void @test_copysign_f16(
  half addrspace(1)* %arg_out,
  half addrspace(1)* %arg_mag,
  half addrspace(1)* %arg_sign) {
entry:
  %mag = load half, half addrspace(1)* %arg_mag
  %sign = load half, half addrspace(1)* %arg_sign
  %out = call half @llvm.copysign.f16(half %mag, half %sign)
  store half %out, half addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f32_mag_f16_sign_f32:
; GCN-DAG: buffer_load_ushort v[[MAG:[0-9]+]]
; GCN-DAG: buffer_load_dword v[[SIGN:[0-9]+]]
; GCN-DAG: s_brev_b32 s[[CONST:[0-9]+]], -2
; GCN-DAG: v_cvt_f32_f16_e32 v[[MAG_EXT:[0-9]+]], v[[MAG]]
; GCN: v_bfi_b32 v[[OUT:[0-9]+]], s[[CONST]], v[[MAG_EXT]], v[[SIGN]]
; GCN: buffer_store_dword v[[OUT]]
; GCN: s_endpgm
define void @test_copysign_out_f32_mag_f16_sign_f32(
  float addrspace(1)* %arg_out,
  half addrspace(1)* %arg_mag,
  float addrspace(1)* %arg_sign) {
entry:
  %mag = load half, half addrspace(1)* %arg_mag
  %mag.ext = fpext half %mag to float
  %sign = load float, float addrspace(1)* %arg_sign
  %out = call float @llvm.copysign.f32(float %mag.ext, float %sign)
  store float %out, float addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f64_mag_f16_sign_f64:
; GCN-DAG: buffer_load_ushort v[[MAG:[0-9]+]]
; GCN-DAG: buffer_load_dwordx2 v{{\[}}[[SIGN_LO:[0-9]+]]:[[SIGN_HI:[0-9]+]]{{\]}}
; GCN-DAG: s_brev_b32 s[[CONST:[0-9]+]], -2
; GCN-DAG: v_cvt_f32_f16_e32 v[[MAG_EXT:[0-9]+]], v[[MAG]]
; GCN-DAG: v_cvt_f64_f32_e32 v{{\[}}[[MAG_EXT_LO:[0-9]+]]:[[MAG_EXT_HI:[0-9]+]]{{\]}}, v[[MAG_EXT]]
; GCN: v_bfi_b32 v[[OUT_HI:[0-9]+]], s[[CONST]], v[[MAG_EXT_HI]], v[[SIGN_HI]]
; GCN: buffer_store_dwordx2 v{{\[}}[[MAG_EXT_LO]]:[[OUT_HI]]{{\]}}
; GCN: s_endpgm
define void @test_copysign_out_f64_mag_f16_sign_f64(
  double addrspace(1)* %arg_out,
  half addrspace(1)* %arg_mag,
  double addrspace(1)* %arg_sign) {
entry:
  %mag = load half, half addrspace(1)* %arg_mag
  %mag.ext = fpext half %mag to double
  %sign = load double, double addrspace(1)* %arg_sign
  %out = call double @llvm.copysign.f64(double %mag.ext, double %sign)
  store double %out, double addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f32_mag_f32_sign_f16:
; GCN-DAG: buffer_load_dword v[[MAG:[0-9]+]]
; GCN-DAG: buffer_load_ushort v[[SIGN:[0-9]+]]
; GCN-DAG: s_brev_b32 s[[CONST:[0-9]+]], -2
; SI-DAG: v_cvt_f32_f16_e32 v[[SIGN_F32:[0-9]+]], v[[SIGN]]
; SI: v_bfi_b32 v[[OUT:[0-9]+]], s[[CONST]], v[[MAG]], v[[SIGN_F32]]
; VI-DAG: v_lshlrev_b32_e32 v[[SIGN_SHIFT:[0-9]+]], 16, v[[SIGN]]
; VI: v_bfi_b32 v[[OUT:[0-9]+]], s[[CONST]], v[[MAG]], v[[SIGN_SHIFT]]
; GCN: buffer_store_dword v[[OUT]]
; GCN: s_endpgm
define void @test_copysign_out_f32_mag_f32_sign_f16(
  float addrspace(1)* %arg_out,
  float addrspace(1)* %arg_mag,
  half addrspace(1)* %arg_sign) {
entry:
  %mag = load float, float addrspace(1)* %arg_mag
  %sign = load half, half addrspace(1)* %arg_sign
  %sign.ext = fpext half %sign to float
  %out = call float @llvm.copysign.f32(float %mag, float %sign.ext)
  store float %out, float addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f64_mag_f64_sign_f16:
; GCN-DAG: buffer_load_dwordx2 v{{\[}}[[MAG_LO:[0-9]+]]:[[MAG_HI:[0-9]+]]{{\]}}
; GCN-DAG: buffer_load_ushort v[[SIGN:[0-9]+]]
; GCN-DAG: s_brev_b32 s[[CONST:[0-9]+]], -2
; SI-DAG: v_cvt_f32_f16_e32 v[[SIGN_F32:[0-9]+]], v[[SIGN]]
; SI: v_bfi_b32 v[[OUT_HI:[0-9]+]], s[[CONST]], v[[MAG_HI]], v[[SIGN_F32]]
; VI-DAG: v_lshlrev_b32_e32 v[[SIGN_SHIFT:[0-9]+]], 16, v[[SIGN]]
; VI: v_bfi_b32 v[[OUT_HI:[0-9]+]], s[[CONST]], v[[MAG_HI]], v[[SIGN_SHIFT]]
; GCN: buffer_store_dwordx2 v{{\[}}[[MAG_LO]]:[[OUT_HI]]{{\]}}
; GCN: s_endpgm
define void @test_copysign_out_f64_mag_f64_sign_f16(
  double addrspace(1)* %arg_out,
  double addrspace(1)* %arg_mag,
  half addrspace(1)* %arg_sign) {
entry:
  %mag = load double, double addrspace(1)* %arg_mag
  %sign = load half, half addrspace(1)* %arg_sign
  %sign.ext = fpext half %sign to double
  %out = call double @llvm.copysign.f64(double %mag, double %sign.ext)
  store double %out, double addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f16_mag_f16_sign_f32:
; GCN-DAG: buffer_load_ushort v[[MAG:[0-9]+]]
; GCN-DAG: buffer_load_dword v[[SIGN:[0-9]+]]
; SI-DAG: s_brev_b32 s[[CONST:[0-9]+]], -2
; SI-DAG: v_cvt_f32_f16_e32 v[[MAG_F32:[0-9]+]], v[[MAG]]
; SI: v_bfi_b32 v[[OUT_F32:[0-9]+]], s[[CONST]], v[[MAG_F32]], v[[SIGN]]
; SI: v_cvt_f16_f32_e32 v[[OUT:[0-9]+]], v[[OUT_F32]]
; VI-DAG: s_movk_i32 s[[CONST:[0-9]+]], 0x7fff
; VI-DAG: v_lshrrev_b32_e32 v[[SIGN_SHIFT:[0-9]+]], 16, v[[SIGN]]
; VI: v_bfi_b32 v[[OUT:[0-9]+]], s[[CONST]], v[[MAG]], v[[SIGN_SHIFT]]
; GCN: buffer_store_short v[[OUT]]
; GCN: s_endpgm
define void @test_copysign_out_f16_mag_f16_sign_f32(
  half addrspace(1)* %arg_out,
  half addrspace(1)* %arg_mag,
  float addrspace(1)* %arg_sign) {
entry:
  %mag = load half, half addrspace(1)* %arg_mag
  %sign = load float, float addrspace(1)* %arg_sign
  %sign.trunc = fptrunc float %sign to half
  %out = call half @llvm.copysign.f16(half %mag, half %sign.trunc)
  store half %out, half addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f16_mag_f16_sign_f64:
; GCN-DAG: buffer_load_ushort v[[MAG:[0-9]+]]
; GCN-DAG: buffer_load_dwordx2 v{{\[}}[[SIGN_LO:[0-9]+]]:[[SIGN_HI:[0-9]+]]{{\]}}
; SI-DAG: s_brev_b32 s[[CONST:[0-9]+]], -2
; SI-DAG: v_cvt_f32_f16_e32 v[[MAG_F32:[0-9]+]], v[[MAG]]
; SI: v_bfi_b32 v[[OUT_F32:[0-9]+]], s[[CONST]], v[[MAG_F32]], v[[SIGN_HI]]
; SI: v_cvt_f16_f32_e32 v[[OUT:[0-9]+]], v[[OUT_F32]]
; VI-DAG: s_movk_i32 s[[CONST:[0-9]+]], 0x7fff
; VI-DAG: v_lshrrev_b32_e32 v[[SIGN_SHIFT:[0-9]+]], 16, v[[SIGN_HI]]
; VI: v_bfi_b32 v[[OUT:[0-9]+]], s[[CONST]], v[[MAG]], v[[SIGN_SHIFT]]
; GCN: buffer_store_short v[[OUT]]
; GCN: s_endpgm
define void @test_copysign_out_f16_mag_f16_sign_f64(
  half addrspace(1)* %arg_out,
  half addrspace(1)* %arg_mag,
  double addrspace(1)* %arg_sign) {
entry:
  %mag = load half, half addrspace(1)* %arg_mag
  %sign = load double, double addrspace(1)* %arg_sign
  %sign.trunc = fptrunc double %sign to half
  %out = call half @llvm.copysign.f16(half %mag, half %sign.trunc)
  store half %out, half addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f16_mag_f32_sign_f16:
; GCN-DAG: buffer_load_dword v[[MAG:[0-9]+]]
; GCN-DAG: buffer_load_ushort v[[SIGN:[0-9]+]]
; SI-DAG: s_brev_b32 s[[CONST:[0-9]+]], -2
; SI-DAG: v_cvt_f16_f32_e32 v[[MAG_TRUNC:[0-9]+]], v[[MAG]]
; SI-DAG: v_cvt_f32_f16_e32 v[[SIGN_F32:[0-9]+]], v[[SIGN]]
; SI-DAG: v_cvt_f32_f16_e32 v[[MAG_F32:[0-9]+]], v[[MAG_TRUNC]]
; SI: v_bfi_b32 v[[OUT_F32:[0-9]+]], s[[CONST]], v[[MAG_F32]], v[[SIGN_F32]]
; SI: v_cvt_f16_f32_e32 v[[OUT:[0-9]+]], v[[OUT_F32]]
; VI-DAG: s_movk_i32 s[[CONST:[0-9]+]], 0x7fff
; VI-DAG: v_cvt_f16_f32_e32 v[[MAG_TRUNC:[0-9]+]], v[[MAG]]
; VI: v_bfi_b32 v[[OUT:[0-9]+]], s[[CONST]], v[[MAG_TRUNC]], v[[SIGN]]
; GCN: buffer_store_short v[[OUT]]
; GCN: s_endpgm
define void @test_copysign_out_f16_mag_f32_sign_f16(
  half addrspace(1)* %arg_out,
  float addrspace(1)* %arg_mag,
  half addrspace(1)* %arg_sign) {
entry:
  %mag = load float, float addrspace(1)* %arg_mag
  %mag.trunc = fptrunc float %mag to half
  %sign = load half, half addrspace(1)* %arg_sign
  %out = call half @llvm.copysign.f16(half %mag.trunc, half %sign)
  store half %out, half addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_out_f16_mag_f64_sign_f16:
; GCN: v_bfi_b32
; GCN: s_endpgm
define void @test_copysign_out_f16_mag_f64_sign_f16(
  half addrspace(1)* %arg_out,
  double addrspace(1)* %arg_mag,
  half addrspace(1)* %arg_sign) {
entry:
  %mag = load double, double addrspace(1)* %arg_mag
  %mag.trunc = fptrunc double %mag to half
  %sign = load half, half addrspace(1)* %arg_sign
  %out = call half @llvm.copysign.f16(half %mag.trunc, half %sign)
  store half %out, half addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_v2f16:
; GCN: v_bfi_b32
; GCN: v_bfi_b32
; GCN: s_endpgm
define void @test_copysign_v2f16(
  <2 x half> addrspace(1)* %arg_out,
  <2 x half> %arg_mag,
  <2 x half> %arg_sign) {
entry:
  %out = call <2 x half> @llvm.copysign.v2f16(<2 x half> %arg_mag, <2 x half> %arg_sign)
  store <2 x half> %out, <2 x half> addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_v3f16:
; GCN: v_bfi_b32
; GCN: v_bfi_b32
; GCN: v_bfi_b32
; GCN: s_endpgm
define void @test_copysign_v3f16(
  <3 x half> addrspace(1)* %arg_out,
  <3 x half> %arg_mag,
  <3 x half> %arg_sign) {
entry:
  %out = call <3 x half> @llvm.copysign.v3f16(<3 x half> %arg_mag, <3 x half> %arg_sign)
  store <3 x half> %out, <3 x half> addrspace(1)* %arg_out
  ret void
}

; GCN-LABEL: {{^}}test_copysign_v4f16:
; GCN: v_bfi_b32
; GCN: v_bfi_b32
; GCN: v_bfi_b32
; GCN: v_bfi_b32
; GCN: s_endpgm
define void @test_copysign_v4f16(
  <4 x half> addrspace(1)* %arg_out,
  <4 x half> %arg_mag,
  <4 x half> %arg_sign) {
entry:
  %out = call <4 x half> @llvm.copysign.v4f16(<4 x half> %arg_mag, <4 x half> %arg_sign)
  store <4 x half> %out, <4 x half> addrspace(1)* %arg_out
  ret void
}
