#pragma once
//!	@file Preprocessor.hpp Does the preprocessing required by the Engine
#ifdef DOXY
//! @brief Should be defined or not by user before all inclusions of CyberEngine.hpp. Enables engine debugging layers. Also ensure definition in unit matches CyberEngine executable you launch with(CyberEngine.exe for no debug, CyberEngineD.exe for debug). Must be defined by user
#define DEBUG
//! @brief Define to make localized strings not use plain english in debug mode. Must be defined by user
#define FORCE_LOCALIZATION
#endif

#include "SystemChecks.hpp"
#include "Endianess.hpp"

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
		inline void HCF[[noreturn]](void){
#ifdef DEBUG
			BREAK;
#endif
#ifdef TARGET_OS_WINDOWS
			__assume(false);
#else
			__builtin_unreachable();
#endif
		}
		/*!
			@brief Assertion function. @p AExpression should always be evaluated
			@param AExpression The expression to check. Will call HCF if false
			@par Thread Safety
				This function requires no thread safety
			@par Exception Safety
				This function does not throw exceptions
		*/
		inline void Assert(const bool AExpression) {
			if (!AExpression)
				HCF();
		}
	};
};