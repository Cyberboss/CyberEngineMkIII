//! @file Exception.hpp Defines the exception classes of the engine
#pragma once
namespace CYB {
	//! @brief The namespace containing engine exception classes
	namespace Exception {
		//! @brief The base exception recognized by the entire engine
		class Base {
		public:
			//! @brief The extended type of the exception
			enum class Level : byte {
				VIOLATION,	//!< Developer error
				SYSTEM_DATA,	//!< System/Data error
				INTERNAL,	//!< Engine error
				UNIT,	//!< Unit error
				FATAL,	//!< Fatal error
			};
		public:
			//! @todo Make the error message allocated
			API::String::Static FMessage;	//!< @brief An english description of the error
			const unsigned int FErrorCode;	//!< @brief The assigned error code
			const Level FLevel;	//!< @brief The type of the exception
		protected:
			/*!
				@brief Construct a Base exception
				@param AMessage A english description of the error
				@param AErrorCode The assigned error code
				@param ALevel The type of the exception
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			Base(API::String::Static&& AMessage, const unsigned int AErrorCode, const Level ALevel);
		};
		class Violation : public Base {
		public:
			enum ErrorCode : unsigned int {
			};
		public:
			Violation(const ErrorCode AErrorCode);
		};
		class SystemData : public Base{
		public:
			enum ErrorCode : unsigned int {
				MEMORY_COMMITAL_FAILURE,
				MEMORY_PROTECT_FAILURE,
				MEMORY_RELEASE_FAILURE,
				MEMORY_RESERVATION_FAILURE,
				MODULE_FUNCTION_LOAD_FAILURE,
				MODULE_LOAD_FAILURE,
				THREAD_CREATION_FAILURE,
			};
		private:
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode);
		public:
			SystemData(const ErrorCode AErrorCode);
		};
		class Internal : public Base {
		public:
			enum ErrorCode : unsigned int {
			};
		public:
			Internal(const ErrorCode AErrorCode);
		};
		class Fatal : public Base {
		public:
			enum ErrorCode : unsigned int {
			};
		public:
			Fatal(const ErrorCode AErrorCode);
		};
	};
};

#include "Exception.inl"