//! @file CYBModuleMacros.hpp Defines the DEFINE_MODULE macro for easily adding modules
#pragma once
#define NARGS_IMPL(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,x16,x17,x18,N,...) N
#define EXPAND(X) X
#define NARGS(...) EXPAND(NARGS_IMPL(__VA_ARGS__,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))

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
#define APPLY2(X, Y, ...) EXPAND(CAT(APPLY2_, NARGS(__VA_ARGS__))(X, Y, __VA_ARGS__))
#define APPLY2_1(X, Y, x1) X(Y, x1)
#define APPLY2_2(X, Y, x1, x2) X(Y, x1), X(Y, x2)
#define APPLY2_3(X, Y, x1, x2, x3) X(Y, x1), X(Y, x2), X(Y, x3)
#define APPLY2_4(X, Y, x1, x2, x3, x4) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4)
#define APPLY2_5(X, Y, x1, x2, x3, x4, x5) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5)
#define APPLY2_6(X, Y, x1, x2, x3, x4, x5, x6) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6)
#define APPLY2_7(X, Y, x1, x2, x3, x4, x5, x6, x7) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7)
#define APPLY2_8(X, Y, x1, x2, x3, x4, x5, x6, x7, x8) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8)
#define APPLY2_9(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9)
#define APPLY2_10(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10)
#define APPLY2_11(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11)
#define APPLY2_12(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11), X(Y, x12)
#define APPLY2_13(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11), X(Y, x12), X(Y, x13)
#define APPLY2_14(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11), X(Y, x12), X(Y, x13), X(Y, x14)
#define APPLY2_15(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11), X(Y, x12), X(Y, x13), X(Y, x14), X(Y, x15)
#define APPLY2_16(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11), X(Y, x12), X(Y, x13), X(Y, x14), X(Y, x15), X(Y, x16)
#define APPLY2_17(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11), X(Y, x12), X(Y, x13), X(Y, x14), X(Y, x15), X(Y, x16), X(Y, x17)
#define APPLY2_18(X, Y, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18) X(Y, x1), X(Y, x2), X(Y, x3), X(Y, x4), X(Y, x5), X(Y, x6), X(Y, x7), X(Y, x8), X(Y, x9), X(Y, x10), X(Y, x11), X(Y, x12), X(Y, x13), X(Y, x14), X(Y, x15), X(Y, x16), X(Y, x17), X(Y, x18)

#define STRINGIFY(X) #X
#define RESTRINGIFY(Prefix, X) STRINGIFY(Prefix##X)
#define DECLTYPE_EXPAND(X) decltype(::##X)
#define DECLTYPE_REEXPAND(Prefix, X) decltype(::Prefix##X)

//module macro

#define DEFINE_MODULE(Name, ShortName, ...)\
enum class Name: unsigned int {\
	__VA_ARGS__\
};\
const char * const F##Name##Functions[NARGS(__VA_ARGS__)]{\
	APPLY(STRINGIFY,__VA_ARGS__)\
};\
typedef typename Platform::FunctionMapping<Name, NARGS(__VA_ARGS__), APPLY(DECLTYPE_EXPAND,__VA_ARGS__)> FM##Name;\
FM##Name* F##ShortName(nullptr)
