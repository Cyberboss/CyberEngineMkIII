//! @file CYBException.cpp Exception constructors of the non-unit kind
#include "CYB.hpp"

namespace CYB {
	namespace Exception {
		//! @brief Used to better verify tests, should be optimized out by the linker. See: http://stackoverflow.com/questions/1229430/how-do-i-prevent-my-unused-global-variables-being-compiled-out
		thread_local unsigned int FLastInstantiatedExceptionCode;
	};
};

CYB::API::String::Static CYB::Exception::Violation::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode)	{
	case INVALID_ENUM:
		return API::String::Static(u8"An operation was attempted with an invalid enum code");
	case INVALID_HEAP_BLOCK:
		return API::String::Static(u8"A Block failed to validate during a Heap operation");
	case NEGATIVE_HEAP_ALLOCATION:
		return API::String::Static(u8"Alloc or Realloc called with a negative size value");
	case UNSUPPORTED_ALLOCATION_AMOUNT:
		return API::String::Static(u8"An allocation was attempted with a size greater than 2047MB");
	default:
		throw Violation(INVALID_ENUM);
	}
}
CYB::Exception::Violation::Violation(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::VIOLATION)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}

CYB::API::String::Static CYB::Exception::SystemData::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case HEAP_ALLOCATION_FAILURE:
		return API::String::Static(u8"Current heap has no block large enough for a requested allocation and expansion failed");
	case MUTEX_INITIALIZATION_FAILURE:
		return API::String::Static(u8"Failed to initialize a new mutex.");
	case STRING_VALIDATION_FAILURE:
		return API::String::Static(u8"A string could not be validated");
	case SYSTEM_PATH_RETRIEVAL_FAILURE:
		return API::String::Static(u8"A system path could not be retrieved");
	case THREAD_CREATION_FAILURE:
		return API::String::Static(u8"OS failed to create requested thread.");
	default:
		throw Violation(Violation::INVALID_ENUM);
	}
}

CYB::Exception::SystemData::SystemData(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::SYSTEM_DATA)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}
CYB::API::String::Static CYB::Exception::Internal::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case FAILED_TO_CONVERT_UTF16_STRING:
		return API::String::Static(u8"Failed to convert between a UTF-8 and UTF-16 string.");
	case MEMORY_COMMITAL_FAILURE:
		return API::String::Static(u8"Failed to commit virtual memory from the OS.");
	case MEMORY_PROTECT_FAILURE:
		return API::String::Static(u8"Failed to implement requested virtual memory protection level.");
	case MEMORY_RESERVATION_FAILURE:
		return API::String::Static(u8"Failed to reserve virtual memory from the OS.");
	case MEMORY_RELEASE_FAILURE:
		return API::String::Static(u8"Failed to return virtual memory to the OS.");
	case MODULE_FUNCTION_LOAD_FAILURE:
		return API::String::Static(u8"Failed to load a requested function from a module.");
	case MODULE_LOAD_FAILURE:
		return API::String::Static(u8"Failed to load a requested module.");
	default:
		throw Violation(Violation::INVALID_ENUM);
	}
}

CYB::Exception::Internal::Internal(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::INTERNAL)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}

CYB::API::String::Static CYB::Exception::Fatal::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case PLEASE_REMOVE_ME_AS_SOON_AS_YOU_ADD_A_REAL_EXCEPTION_HERE:
		return API::String::Static(u8"Seriously, do it");
	default:
		throw Violation(Violation::INVALID_ENUM);
	}
}

CYB::Exception::Fatal::Fatal(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::FATAL)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}