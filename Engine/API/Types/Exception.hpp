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
			*/
			Base(API::String::Static&& AMessage, const unsigned int AErrorCode, const Level ALevel) noexcept;
		};
		//! @brief Exceptions indicating an API contract violation. Should not be anticipated
		class Violation : public Base {
		public:
			//! @brief The error code of the exception
			enum ErrorCode : unsigned int {
				INVALID_EXCEPTION_ERROR_CODE, //!< @brief An exception had instantiation attempted with an invalid AErrorCode
				INVALID_HEAP_BLOCK,	//!< @brief A memory block used by the Heap failed to validate
				NEGATIVE_HEAP_ALLOCATION, //!< @brief An allocation was attempted with a negative size value
				UNSUPPORTED_ALLOCATION_AMOUNT, //!< @brief An allocation was attempted with a size above 2047MB
			};
		private:
			/*!
				@brief Get the associated message for an exception
				@param AErrorCode The ErrorCode describing the exception
				@return A description of the exception indicated by @p AErrorCode
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
			*/
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode);
		public:
			/*!
				@brief Construct a Violation exception
				@param AErrorCode The ErrorCode describing the exception
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
				@attention This function is for internal use only
			*/
			Violation(const ErrorCode AErrorCode);
		};
		//! @brief Exceptions caused by external call failures or invalid external data
		class SystemData : public Base{
		public:
			//! @brief The error code of the exception
			enum ErrorCode : unsigned int {
				MEMORY_COMMITAL_FAILURE, //!< @brief Memory could not be commited from a reservation
				MEMORY_PROTECT_FAILURE, //!< @brief Memory protection could not be set
				MEMORY_RELEASE_FAILURE, //!< @brief Memory reservation could not be released
				MEMORY_RESERVATION_FAILURE, //!< @brief Memory could not be reserved
				MODULE_FUNCTION_LOAD_FAILURE, //!< @brief Specific could not be loaded from given Module
				MODULE_LOAD_FAILURE, //!< @brief Module could not be loaded
				MUTEX_INITIALIZATION_FAILURE, //!< @brief Mutex could not be created
				THREAD_CREATION_FAILURE, //!< @brief Thread could not be created
			};
		private:
			/*!
				@brief Get the associated message for an exception
				@param AErrorCode The ErrorCode describing the exception
				@return A description of the exception indicated by @p AErrorCode
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
			*/
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode);
		public:
			/*!
				@brief Construct a SystemData exception
				@param AErrorCode The ErrorCode describing the exception
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
				@attention This function is for internal use only
			*/
			SystemData(const ErrorCode AErrorCode);
		};
		//! @brief Exceptions that are thrown internally in the engine that the unit will never see
		class Internal : public Base {
		public:
			//! @brief The error code of the exception
			enum ErrorCode : unsigned int {
				INVALID_HEAP_FREE_LIST, //!< @brief A heap's free list is incorrect
			};
		private:
			/*!
				@brief Get the associated message for an exception
				@param AErrorCode The ErrorCode describing the exception
				@return A description of the exception indicated by @p AErrorCode
				@par Thread Safety
					This function requires no thread safety
			*/
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode);
		public:
			/*!
				@brief Construct an Internal exception
				@param AErrorCode The ErrorCode describing the exception
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
				@attention This function is for internal use only
			*/
			Internal(const ErrorCode AErrorCode);
		};
		//! @brief Exceptions that compromise the stability of the engine and it must be promptly shutdown
		class Fatal : public Base {
		public:
			//! @brief The error code of the exception
			enum ErrorCode : unsigned int {
				PLEASE_REMOVE_ME_AS_SOON_AS_YOU_ADD_A_REAL_EXCEPTION_HERE, //!< @brief Dummy code, used for code coverage
			};
		private:
			/*!
				@brief Get the associated message for an exception
				@param AErrorCode The ErrorCode describing the exception
				@return A description of the exception indicated by @p AErrorCode
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
			*/
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode);
		public:
			/*!
				@brief Construct a Fatal exception
				@param AErrorCode The ErrorCode describing the exception
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
				@attention This function is for internal use only
			*/
			Fatal(const ErrorCode AErrorCode);
		};
	};
};

#include "Exception.inl"