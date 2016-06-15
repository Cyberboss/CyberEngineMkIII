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
	case INVALID_EXCEPTION_ERROR_CODE:
		return API::String::Static(u8"An exception had instantiation attempted with an invalid error code");
	case INVALID_HEAP_BLOCK:
		return API::String::Static(u8"A Block failed to validate during a Heap operation");
	default:
		throw Violation(INVALID_EXCEPTION_ERROR_CODE);
	}
}
CYB::Exception::Violation::Violation(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::VIOLATION)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}

CYB::API::String::Static CYB::Exception::SystemData::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case MEMORY_RESERVATION_FAILURE:
		return API::String::Static(u8"Failed to reserve virtual memory from the OS.");
	case MEMORY_COMMITAL_FAILURE:
		return API::String::Static(u8"Failed to commit virtual memory from the OS.");
	case MEMORY_PROTECT_FAILURE:
		return API::String::Static(u8"Failed to implement requested virtual memory protection level.");
	case MEMORY_RELEASE_FAILURE:
		return API::String::Static(u8"Failed to return virtual memory to the OS.");
	case MODULE_FUNCTION_LOAD_FAILURE:
		return API::String::Static(u8"Failed to load a requested function from a module.");
	case MODULE_LOAD_FAILURE:
		return API::String::Static(u8"Failed to load a requested module.");
	case MUTEX_INITIALIZATION_FAILURE:
		return API::String::Static(u8"Failed to initialize a new mutex.");
	case THREAD_CREATION_FAILURE:
		return API::String::Static(u8"OS failed to create requested thread.");
	default:
		throw Violation(Violation::INVALID_EXCEPTION_ERROR_CODE);
	}
}

CYB::Exception::SystemData::SystemData(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::SYSTEM_DATA)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}
CYB::API::String::Static CYB::Exception::Internal::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case PLEASE_REMOVE_ME_AS_SOON_AS_YOU_ADD_A_REAL_EXCEPTION_HERE:
		return API::String::Static(u8"Seriously, do it");
	default:
		throw Violation(Violation::INVALID_EXCEPTION_ERROR_CODE);
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
		throw Violation(Violation::INVALID_EXCEPTION_ERROR_CODE);
	}
}

CYB::Exception::Fatal::Fatal(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::FATAL)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}