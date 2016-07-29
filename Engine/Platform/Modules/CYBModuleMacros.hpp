//! @file CYBModuleMacros.hpp Defines the DEFINE_MODULE macro for easily adding modules
#pragma once
//! @cond
#define NARGS_IMPL(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,x19,x20,x21,x22,x23,x24,x25,x26,x27,x28,x29,x30,x31,x32,x33,N,...) N
#define EXPAND(X) X
#define NARGS(...) EXPAND(NARGS_IMPL(__VA_ARGS__,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))

#define PRIMITIVE_CAT(X, Y) X ## Y
#define CAT(X, Y) PRIMITIVE_CAT(X, Y)

#define APPLY(X, ...) EXPAND(CAT(APPLY_, NARGS(__VA_ARGS__))(X, __VA_ARGS__))
#define APPLY_1(X, x1) X(x1)
#define APPLY_2(X, x1, x2) X(x1), X(x2)
#define APPLY_3(X, x1, x2, x3) X(x1), X(x2), X(x3)
#define APPLY_4(X, x1, x2, x3, x4) X(x1), X(x2), X(x3), X(x4)
#define APPLY_5(X, x1, x2, x3, x4, x5) X(x1), X(x2), X(x3), X(x4), X(x5)
#define APPLY_6(X, x1, x2, x3, x4, x5, x6) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6)
#define APPLY_7(X, x1, x2, x3, x4, x5, x6, x7) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7)
#define APPLY_8(X, x1, x2, x3, x4, x5, x6, x7, x8) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8)
#define APPLY_9(X, x1, x2, x3, x4, x5, x6, x7, x8, x9) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9)
#define APPLY_10(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10)
#define APPLY_11(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11)
#define APPLY_12(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12)
#define APPLY_13(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13)
#define APPLY_14(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14)
#define APPLY_15(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15)
#define APPLY_16(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16)
#define APPLY_17(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17)
#define APPLY_18(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18)
#define APPLY_19(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19)
#define APPLY_20(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20)
#define APPLY_21(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21)
#define APPLY_22(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22)
#define APPLY_23(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23)
#define APPLY_24(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24)
#define APPLY_25(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25)
#define APPLY_26(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26)
#define APPLY_27(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26), X(x27)
#define APPLY_28(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26), X(x27), X(x28)
#define APPLY_29(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26), X(x27), X(x28), X(x29)
#define APPLY_30(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26), X(x27), X(x28), X(x29), X(x30)
#define APPLY_31(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26), X(x27), X(x28), X(x29), X(x30), X(x31)
#define APPLY_32(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26), X(x27), X(x28), X(x29), X(x30), X(x31), X(x32)
#define APPLY_33(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33) X(x1), X(x2), X(x3), X(x4), X(x5), X(x6), X(x7), X(x8), X(x9), X(x10), X(x11), X(x12), X(x13), X(x14), X(x15), X(x16), X(x17), X(x18), X(x19), X(x20), X(x21), X(x22), X(x23), X(x24), X(x25), X(x26), X(x27), X(x28), X(x29), X(x30), X(x31), X(x32), X(x33)
#define APPLY_NC(X, ...) EXPAND(CAT(APPLY_NC_, NARGS(__VA_ARGS__))(X, __VA_ARGS__))
#define APPLY_NC_1(X, x1) X(x1)
#define APPLY_NC_2(X, x1, x2) X(x1) X(x2)
#define APPLY_NC_3(X, x1, x2, x3) X(x1) X(x2) X(x3)
#define APPLY_NC_4(X, x1, x2, x3, x4) X(x1) X(x2) X(x3) X(x4)
#define APPLY_NC_5(X, x1, x2, x3, x4, x5) X(x1) X(x2) X(x3) X(x4) X(x5)
#define APPLY_NC_6(X, x1, x2, x3, x4, x5, x6) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6)
#define APPLY_NC_7(X, x1, x2, x3, x4, x5, x6, x7) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7)
#define APPLY_NC_8(X, x1, x2, x3, x4, x5, x6, x7, x8) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8)
#define APPLY_NC_9(X, x1, x2, x3, x4, x5, x6, x7, x8, x9) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9)
#define APPLY_NC_10(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10)
#define APPLY_NC_11(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11)
#define APPLY_NC_12(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12)
#define APPLY_NC_13(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13)
#define APPLY_NC_14(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14)
#define APPLY_NC_15(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15)
#define APPLY_NC_16(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16)
#define APPLY_NC_17(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17)
#define APPLY_NC_18(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18)
#define APPLY_NC_19(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19)
#define APPLY_NC_20(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20)
#define APPLY_NC_21(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21)
#define APPLY_NC_22(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22)
#define APPLY_NC_23(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23)
#define APPLY_NC_24(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24)
#define APPLY_NC_25(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25)
#define APPLY_NC_26(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26)
#define APPLY_NC_27(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26) X(x27)
#define APPLY_NC_28(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26) X(x27) X(x28)
#define APPLY_NC_29(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26) X(x27) X(x28) X(x29)
#define APPLY_NC_30(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26) X(x27) X(x28) X(x29) X(x30)
#define APPLY_NC_31(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26) X(x27) X(x28) X(x29) X(x30) X(x31)
#define APPLY_NC_32(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26) X(x27) X(x28) X(x29) X(x30) X(x31) X(x32)
#define APPLY_NC_33(X, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31, x32, x33) X(x1) X(x2) X(x3) X(x4) X(x5) X(x6) X(x7) X(x8) X(x9) X(x10) X(x11) X(x12) X(x13) X(x14) X(x15) X(x16) X(x17) X(x18) X(x19) X(x20) X(x21) X(x22) X(x23) X(x24) X(x25) X(x26) X(x27) X(x28) X(x29) X(x30) X(x31) X(x32) X(x33)

#define STRINGIFY(X) #X
#define STATIC_STRINGIFY(X) API::String::Static(STRINGIFY(X))
#define DECLTYPE_EXPAND(X) decltype(X)

#define CLASSIFY_ENUM(X)\
class X {\
public:\
	enum : unsigned int {\
		Index = static_cast<unsigned int>(InternalIndexes::X),\
	};\
	using FAutoModule = FParentAutoModule;\
};\

#define DEFINE_MODULE(AModuleName, ADiskName, APlatform, AOptionalFunctions, ...)\
namespace CYB {\
	namespace Platform {\
		namespace Modules {\
			using namespace APlatform;\
			using AM##AModuleName = typename Platform::Modules::AutoModule<AOptionalFunctions, NARGS(__VA_ARGS__), APPLY(DECLTYPE_EXPAND, __VA_ARGS__)>;\
			template <> constexpr const char* AM##AModuleName::ModuleName(void){\
				return ADiskName;\
			}\
			template <> inline const CYB::API::String::Static* AM##AModuleName::FunctionNames(void) noexcept {\
				static const CYB::API::String::Static Names[NARGS(__VA_ARGS__)]{ APPLY(STATIC_STRINGIFY, __VA_ARGS__) };\
				return Names;\
			}\
			class AModuleName{\
			private:\
				enum class InternalIndexes: unsigned int {\
					__VA_ARGS__\
				};\
				using FParentAutoModule = AM##AModuleName;\
			public:\
				APPLY_NC(CLASSIFY_ENUM, __VA_ARGS__)\
			};\
		};\
	};\
};\

#define DEFINE_DUMMY_MODULE(AModuleName, ADiskName, APlatform, AOptionalFunctions, ...)\
namespace CYB {\
	namespace Platform {\
		namespace Modules {\
			using AM##AModuleName = AMFake;\
			class AModuleName{\
			private:\
				enum class InternalIndexes: unsigned int {\
					__VA_ARGS__\
				};\
				using FParentAutoModule = AM##AModuleName;\
			public:\
				APPLY_NC(CLASSIFY_ENUM, __VA_ARGS__)\
			};\
		};\
	};\
};

#define OVERRIDE_FUNCTION_NAMES(AAutoModule, ...)\
namespace CYB {\
	namespace Platform {\
		namespace Modules {\
			template<> inline const CYB::API::String::Static* AM##AAutoModule::OverridenNames(void) noexcept {\
				static const API::String::Static Names[NARGS(__VA_ARGS__)]{ __VA_ARGS__ };\
				return Names;\
			}\
		};\
	};\
};
#define DUMMY_OVERRIDE_FUNCTION_NAMES(...)

#ifdef TARGET_OS_WINDOWS
#define DEFINE_WINDOWS_MODULE DEFINE_MODULE
#define DEFINE_POSIX_MODULE DEFINE_DUMMY_MODULE
#define OVERRIDE_WINDOWS_FUNCTION_NAMES OVERRIDE_FUNCTION_NAMES
#define OVERRIDE_POSIX_FUNCTION_NAMES DUMMY_OVERRIDE_FUNCTION_NAMES
#else
#define DEFINE_WINDOWS_MODULE DEFINE_DUMMY_MODULE
#define DEFINE_POSIX_MODULE DEFINE_MODULE
#define OVERRIDE_WINDOWS_FUNCTION_NAMES DUMMY_OVERRIDE_FUNCTION_NAMES
#define OVERRIDE_POSIX_FUNCTION_NAMES OVERRIDE_FUNCTION_NAMES
#endif

#ifdef TARGET_OS_LINUX
#define DEFINE_LINUX_MODULE DEFINE_MODULE
#define OVERRIDE_LINUX_FUNCTION_NAMES OVERRIDE_FUNCTION_NAMES
#else
#define DEFINE_LINUX_MODULE DEFINE_DUMMY_MODULE
#define OVERRIDE_LINUX_FUNCTION_NAMES DUMMY_OVERRIDE_FUNCTION_NAMES
#endif

#ifdef TARGET_OS_MAC
#define DEFINE_OSX_MODULE DEFINE_MODULE
#define OVERRIDE_OSX_FUNCTION_NAMES OVERRIDE_FUNCTION_NAMES
#else
#define DEFINE_OSX_MODULE DEFINE_DUMMY_MODULE
#define OVERRIDE_OSX_FUNCTION_NAMES DUMMY_OVERRIDE_FUNCTION_NAMES
#endif
//! @endcond