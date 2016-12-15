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
			};
		public:
			API::String::Static FMessage;	//!< @brief An english description of the error, guaranteed to be compatible with CYB::API::String::UTF8
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
				INVALID_ENUM, //!< @brief An operation was attempted with an invalid enum code
				INVALID_INTEROP_CONSTRUCTOR,	//!< @brief A contructor passed to the engine was malformed
				INVALID_OPERATION,	//!< @brief Illegal call given prexisting conditions
				INVALID_PARAMETERS,	//!< @brief The current arrangment of arguments passed the function is invalid
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
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
			*/
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode) noexcept;
		public:
			/*!
				@brief Construct a Violation exception
				@param AErrorCode The ErrorCode describing the exception
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
				@attention This function is for internal use only
			*/
			Violation(const ErrorCode AErrorCode) noexcept;
		};
		//! @brief Exceptions caused by external call failures or invalid external data. Only classifies ones that can pass through the ABI
		class SystemData : public Base{
		public:
			//! @brief The error code of the exception
			enum ErrorCode : unsigned int {
				DIRECTORY_NOT_EMPTY,	//!< @brief Tried to delete a non-empty directory
				FILE_EXISTS, //!< @brief A file that was to be created exists
				FILE_NOT_FOUND,	//!< @brief A required file was not found in the filesystem
				STREAM_NOT_READABLE,	//!< @brief Generic error for read failures. See functions for further documentation
				STREAM_NOT_WRITABLE,	//!< @brief Generic error for write failures. See functions for further documentation
				HEAP_ALLOCATION_FAILURE,	//!< @brief A heap has no block large enough for a requested allocation and expansion failed
				MUTEX_INITIALIZATION_FAILURE, //!< @brief Mutex could not be created
				PATH_LOST,	//!< @brief A previously valid path has become invalidated, most likely due to deletion
				PATH_TOO_LONG,	//!< @brief Attempted to use a path greater than the maximum allowed byte value
				STRING_VALIDATION_FAILURE, //!< @brief A string could not be validated
				SYSTEM_PATH_RETRIEVAL_FAILURE, //!< @brief A system path could not be retrieved
				THREAD_CREATION_FAILURE, //!< @brief Thread could not be created
			};
		private:
			/*!
				@brief Get the associated message for an exception
				@param AErrorCode The ErrorCode describing the exception
				@return A description of the exception indicated by @p AErrorCode
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
			*/
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode) noexcept;
		public:
			/*!
				@brief Construct a SystemData exception
				@param AErrorCode The ErrorCode describing the exception
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
				@attention This function is for internal use only
			*/
			SystemData(const ErrorCode AErrorCode) noexcept;
		};
		//! @brief Exceptions that are thrown internally in the engine that the should never see, these are a superset of SystemData type exceptions. If one of these exceptions appear in a unit, please contant the engine development team
		class Internal : public Base {
		public:
			//! @brief The error code of the exception
			enum ErrorCode : unsigned int {
				MEMORY_COMMITAL_FAILURE, //!< @brief Memory could not be commited from a reservation
				MEMORY_PROTECT_FAILURE, //!< @brief Memory protection could not be set
				MEMORY_RESERVATION_FAILURE, //!< @brief Memory could not be reserved
				MODULE_FUNCTION_LOAD_FAILURE, //!< @brief Specific could not be loaded from given Module
				MODULE_LOAD_FAILURE, //!< @brief Module could not be loaded
				PATH_EVALUATION_FAILURE, //!< @brief Failed to evaluate a Path
				PROCESS_CREATION_ERROR,	//!< @brief Process could not be created for unknown reasons
				PROCESS_EXIT_CODE_UNCHECKABLE, //!< @brief Tried to check the error code of a Process the OS would not allow
				PROCESS_TERMINATION_ERROR, //!< @brief Process could not be terminated, most likely due to insufficient priviledges
			};
		private:
			/*!
				@brief Get the associated message for an exception
				@param AErrorCode The ErrorCode describing the exception
				@return A description of the exception indicated by @p AErrorCode
				@par Thread Safety
					This function requires no thread safety
			*/
			static API::String::Static ErrorMessage(const ErrorCode AErrorCode) noexcept;
		public:
			/*!
				@brief Construct an Internal exception
				@param AErrorCode The ErrorCode describing the exception
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_EXCEPTION_CODE. Thrown if @p AErrorCode is not recognized by the engine
				@attention This function is for internal use only
			*/
			Internal(const ErrorCode AErrorCode) noexcept;
		};
	};
};

#include "Exception.inl"