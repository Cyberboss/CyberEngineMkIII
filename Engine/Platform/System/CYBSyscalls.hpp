//! @file CYBSyscalls.hpp Defines the System Call interface
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			/*! 
				@brief Contains the functionality for making asm syscalls and linked calls
				@par Linked Calls
					Certain calls must be linked in, namely inline functions, shared library functions, and process termination functions due to the fact that they must be used without the ModuleManager being instantiated. Use this API for linked calls that aren't from the approves C++ library (E.g. memory_fence, vector, etc).
			*/
			class Sys : public Implementation::Sys {
			public:
				//! @brief Used for passing arguments to calls to this class. If the call in question requires const, const_casting away constness for this intermediary is well defined.
				union Union64 {
				public:
					void* FPointer;	//!< @brief The argument interpreted as a pointer
					unsigned long long FNumber;	//!< @brief The argument interpreted as a number
				public:
					/*!
						@brief Construct a Union64 from a pointer
						@param APointer The pointer to use
					*/
					constexpr Union64(void* const APointer) noexcept;
					/*!
						@brief Construct a Union64 from a number
						@param ANumber The number to use
						@par Thread Safety
							This function requires no thread safety
					*/
					constexpr Union64(const unsigned long long ANumber) noexcept;
				};
			private:
				/*!
					@brief Check if a certain call has been redirected. Used for testing, always evaluates to false in the runtime
					@param ACallNumber The call number to check for redirection
					@return true if call @p ACallNumber has been redirected, false otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				static bool CallRedirected(const CallNumber ACallNumber) noexcept;
				/*!
					@brief Calls a redirected call using the parameters
					@param ACallNumber The call number that has been redirected. CallRedirected must evaluate to true with this argument
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@param AArg4 An argument to the call
					@param AArg5 An argument to the call
					@param AArg6 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
					@attention Throws based on called function
				*/
				static unsigned long long RedirectedCall(const CallNumber ACallNumber, const Union64 AArg1 = Union64(nullptr), const Union64 AArg2 = Union64(nullptr), const Union64 AArg3 = Union64(nullptr), const Union64 AArg4 = Union64(nullptr), const Union64 AArg5 = Union64(nullptr), const Union64 AArg6 = Union64(nullptr));

				/*!
					@brief Do an asm syscall for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The system call to do
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long DoCall(const CallNumber ACallNumber) noexcept;
				/*!
					@brief Do an asm syscall for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The system call to do
					@param AArg1 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1) noexcept;
				/*!
					@brief Do an asm syscall for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The system call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2) noexcept;
				/*!
					@brief Do an asm syscall for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The system call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3) noexcept;
				/*!
					@brief Do an asm syscall for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The system call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@param AArg4 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4) noexcept;
				/*!
					@brief Do an asm syscall for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The system call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@param AArg4 An argument to the call
					@param AArg5 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5) noexcept;
				/*!
					@brief Do an asm syscall for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The system call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@param AArg4 An argument to the call
					@param AArg5 An argument to the call
					@param AArg6 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5, const Union64 AArg6) noexcept;
				
				/*!
					@brief Do linked call for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The linked call to do
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long LinkedCall(const CallNumber ACallNumber) noexcept;
				
				/*!
					@brief Do linked call for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The linked call to do
					@param AArg1 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long LinkedCall(const CallNumber ACallNumber, const Union64 AArg1) noexcept;
				
				/*!
					@brief Do linked call for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The linked call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2) noexcept;
				
				/*!
					@brief Do linked call for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The linked call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3) noexcept;
				
				/*!
					@brief Do linked call for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The linked call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@param AArg4 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4) noexcept;
				
				/*!
					@brief Do linked call for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The linked call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@param AArg4 An argument to the call
					@param AArg5 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5) noexcept;
				
				/*!
					@brief Do linked call for @p ACallNumber. The number of arguments must match system specification
					@param ACallNumber The linked call to do
					@param AArg1 An argument to the call
					@param AArg2 An argument to the call
					@param AArg3 An argument to the call
					@param AArg4 An argument to the call
					@param AArg5 An argument to the call
					@param AArg6 An argument to the call
					@return The result of the call
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned long long LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5, const Union64 AArg6) noexcept;

				/*!
					@brief Checks the number of arguments for syscalls and calls HCF if something is wrong
					@param ACallNumber The call number whose argument count is to be checked
					@param ANumArgs The number of arguments in the call
				*/
				static void VerifyArgumentCount(const CallNumber ACallNumber, const unsigned long long ANumArgs) noexcept;
			public:
				/*!
					@brief Do a linked or system call
					@tparam Args The arguments to the call, must all be Union64
					@param ACallNumber The number of the call to make
					@param AArgs The arguments to the call
					@return The result of the call
					@par Thread Safety
						This function requires no Thread Safety
				*/
				template <typename... Args> static unsigned long long Call(const CallNumber ACallNumber, Args&&... AArgs) noexcept;
			};
		};
	};
};

#include "CYBSyscalls.inl"