; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -instcombine -S | FileCheck %s

target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Should be optimized to one and.
define i1 @test1(i32 %a, i32 %b) {
; CHECK-LABEL: @test1(
; CHECK-NEXT:    [[TMP1:%.*]] = xor i32 %a, %b
; CHECK-NEXT:    [[TMP2:%.*]] = and i32 [[TMP1]], 65280
; CHECK-NEXT:    [[TMP:%.*]] = icmp ne i32 [[TMP2]], 0
; CHECK-NEXT:    ret i1 [[TMP]]
;
  %tmp1 = and i32 %a, 65280
  %tmp3 = and i32 %b, 65280
  %tmp = icmp ne i32 %tmp1, %tmp3
  ret i1 %tmp
}

define <2 x i1> @test1vec(<2 x i32> %a, <2 x i32> %b) {
; CHECK-LABEL: @test1vec(
; CHECK-NEXT:    [[TMP1:%.*]] = xor <2 x i32> %a, %b
; CHECK-NEXT:    [[TMP2:%.*]] = and <2 x i32> [[TMP1]], <i32 65280, i32 65280>
; CHECK-NEXT:    [[TMP:%.*]] = icmp ne <2 x i32> [[TMP2]], zeroinitializer
; CHECK-NEXT:    ret <2 x i1> [[TMP]]
;
  %tmp1 = and <2 x i32> %a, <i32 65280, i32 65280>
  %tmp3 = and <2 x i32> %b, <i32 65280, i32 65280>
  %tmp = icmp ne <2 x i32> %tmp1, %tmp3
  ret <2 x i1> %tmp
}

define i1 @test2(i64 %A) {
; CHECK-LABEL: @test2(
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i64 %A to i8
; CHECK-NEXT:    [[CMP:%.*]] = icmp sgt i8 [[TMP1]], -1
; CHECK-NEXT:    ret i1 [[CMP]]
;
  %and = and i64 %A, 128
  %cmp = icmp eq i64 %and, 0
  ret i1 %cmp
}

; FIXME: Vectors should fold the same way.
define <2 x i1> @test2vec(<2 x i64> %A) {
; CHECK-LABEL: @test2vec(
; CHECK-NEXT:    [[AND:%.*]] = and <2 x i64> %A, <i64 128, i64 128>
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq <2 x i64> [[AND]], zeroinitializer
; CHECK-NEXT:    ret <2 x i1> [[CMP]]
;
  %and = and <2 x i64> %A, <i64 128, i64 128>
  %cmp = icmp eq <2 x i64> %and, zeroinitializer
  ret <2 x i1> %cmp
}

define i1 @test3(i64 %A) {
; CHECK-LABEL: @test3(
; CHECK-NEXT:    [[TMP1:%.*]] = trunc i64 %A to i8
; CHECK-NEXT:    [[CMP:%.*]] = icmp slt i8 [[TMP1]], 0
; CHECK-NEXT:    ret i1 [[CMP]]
;
  %and = and i64 %A, 128
  %cmp = icmp ne i64 %and, 0
  ret i1 %cmp
}

; FIXME: Vectors should fold the same way.
define <2 x i1> @test3vec(<2 x i64> %A) {
; CHECK-LABEL: @test3vec(
; CHECK-NEXT:    [[AND:%.*]] = and <2 x i64> %A, <i64 128, i64 128>
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne <2 x i64> [[AND]], zeroinitializer
; CHECK-NEXT:    ret <2 x i1> [[CMP]]
;
  %and = and <2 x i64> %A, <i64 128, i64 128>
  %cmp = icmp ne <2 x i64> %and, zeroinitializer
  ret <2 x i1> %cmp
}

