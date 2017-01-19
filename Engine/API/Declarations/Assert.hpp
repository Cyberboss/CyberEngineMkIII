#pragma once
//!	@file Assert.hpp Defines assertion functions and macros

//! @brief No operation macro
#define NOP static_cast<void>(nullptr)
//! @brief Breaks code execution at line used if DEBUG is defined. Must be defined by user otherwise.
#define BREAK NOP

#ifdef DEBUG
#undef BREAK
#ifdef TARGET_OS_WINDOWS
#define BREAK __debugbreak()
#elif defined(TARGET_OS_MAC)
#define BREAK __builtin_trap()
#else
#define BREAK __asm__("int3")
#endif
#endif

//! @brief Used for hardcore unreachable code paths when Assert::HCF is not enough. Should generally be avoided in units
#define UNREACHABLE

#ifdef TARGET_OS_WINDOWS
#define UNREACHABLE __assume(false)
#else
#define UNREACHABLE __builtin_unreachable()
#endif

//! @brief Quick macro for supplying CYB::API::Assert::Unimplemented with the correct parameters
#define UNIMPLEMENTED CYB::API::Assert::Unimplemented(__FUNCTION__, __FILE__, __LINE__)

namespace CYB {
	namespace API {
		//! @brief Contains assertion functions
		class Assert {
		public:
			/*!
				@brief Indicates unreachable code. Implementation can be overriden by defining ASSERTION_OVERRIDE
				@par Thread Safety
					This function requires no thread safety
			*/
			static void HCF[[noreturn]](void) noexcept;
			/*!
				@brief Indicates an unimplemented function, calls HCF if ASSERTION_OVERRIDE is not defined
				@param AFunction Should be __FUNCTION__ 
				@param AFile Should be __FILE__
				@param ALine Should be __LINE__
				@par Thread Safety
					This function requires no thread safety
			*/
			static void Unimplemented(const char* const AFunction, const char* const AFile, const unsigned int ALine) noexcept;
			/*!
				@brief Assertion function. @p AExpression should always be evaluated
				@param AExpression The expression to check. Will call HCF if false
				@par Thread Safety
					This function requires no thread safety
			*/
			static void True(const bool AExpression) noexcept;
			/*!
				@brief False assertion function. @p AExpression should always be evaluated
				@param AExpression The expression to check. Will call HCF if true
				@par Thread Safety
					This function requires no thread safety
			*/
			static void False(const bool AExpression) noexcept;
			/*!
				@brief Equivalence assertion function. May not be evaluated
				@param ALHS the left hand side of the equation
				@param ARHS the right hand side of the equation
				@tparam AType The type of @p ALHS and @p ARHS
				@par Thread Safety
					This function requires no thread safety
			*/
			template <typename AType> static void Equal(const AType& ALHS, const AType& ARHS) noexcept;
			/*!
				@brief Equal against one or more things assertion function. May not be evaluated
				@param ALHS the left hand side of the equations
				@param ARHS the right hand side of the current equations
				@param ARHSs the right hand sides of the remaining equations
				@tparam AType The type of @p ALHS
				@tparam AOtherTypes The types of @p ARHSs must be comparable with AType
				@par Thread Safety
					This function requires no thread safety
			*/
			template <typename AType, typename... AOtherTypes> static void Equal(const AType& ALHS, const AType& ARHS, AOtherTypes&&... ARHSs) noexcept;
			/*!
				@brief Unequivalence assertion function. May not be evaluated
				@param ALHS the left hand side of the equation
				@param ARHS the right hand side of the equation
				@tparam AType The type of @p ALHS and @p ARHS
				@par Thread Safety
					This function requires no thread safety
			*/
			template <typename AType> static void NotEqual(const AType& ALHS, const AType& ARHS) noexcept;
			/*!
				@brief Less than assertion function. May not be evaluated
				@param ALHS the left hand side of the equation
				@param ARHS the right hand side of the equation
				@tparam AType The type of @p ALHS and @p ARHS
				@par Thread Safety
					This function requires no thread safety
			*/
			template <typename AType> static void LessThan(const AType& ALHS, const AType& ARHS) noexcept;
			/*!
				@brief Less than or equal assertion function. May not be evaluated
				@param ALHS the left hand side of the equation
				@param ARHS the right hand side of the equation
				@tparam AType The type of @p ALHS and @p ARHS
				@par Thread Safety
					This function requires no thread safety
			*/
			template <typename AType> static void LessThanOrEqual(const AType& ALHS, const AType& ARHS) noexcept;
		};
	};
};
#include "Assert.inl"