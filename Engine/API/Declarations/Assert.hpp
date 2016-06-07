#pragma once
//!	@file Assert.hpp Defines assertion functions and macros

//! @brief No operation macro
#define NOP static_cast<void>(nullptr)
//! @brief Breaks code execution at line used if DEBUG is defined. Must be defined by user otherwise.
#define BREAK NOP

#undef BREAK
#ifdef TARGET_OS_WINDOWS
#define BREAK __debugbreak()
#elif defined(TARGET_OS_MAC)
#define BREAK __builtin_trap()
#else
#define BREAK __asm__("int3")
#endif

namespace CYB {
	namespace API {
		/*!
			@brief Indicates unreachable code
			@par Thread Safety
				This function requires no thread safety
			@par Exception Safety
				This function does not throw exceptions
		*/
		void HCF(void);
		/*!
			@brief Assertion function. @p AExpression should always be evaluated
			@param AExpression The expression to check. Will call HCF if false
			@par Thread Safety
				This function requires no thread safety
			@par Exception Safety
				This function does not throw exceptions
		*/
		void Assert(const bool AExpression);
	};
};
#ifndef ASSERTION_OVERRIDE
#include "Assert.inl"
#endif