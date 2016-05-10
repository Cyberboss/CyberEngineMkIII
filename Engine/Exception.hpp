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
				MEMORY_COMMITAL_FAILURE, //!< @brief Memory could not be commited from a reservation
				MEMORY_PROTECT_FAILURE, //!< @brief Memory protection could not be set
				MEMORY_RELEASE_FAILURE, //!< @brief Memory reservation could not be released
				MEMORY_RESERVATION_FAILURE, //!< @brief Memory could not be reserved
				MODULE_FUNCTION_LOAD_FAILURE, //!< @brief Specific could not be loaded from given Module
				MODULE_LOAD_FAILURE, //!< @brief Module could not be loaded
				MUTEX_DESTRUCTION_FAILURE, //!< @brief Mutex could not be destroyed
				MUTEX_INITIALIZATION_FAILURE, //!< @brief Mutex could not be created
				THREAD_CREATION_FAILURE, //!< @brief Thread could not be created
			};
		private:
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode);
		public:
			SystemData(const ErrorCode AErrorCode);
		};
		class Internal : public Base {
		public:
			enum ErrorCode : unsigned int {
				HEAP_CORRUPTION,
			};
		private:
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode);
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