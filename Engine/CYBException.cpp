//! @file CYBExcpetion.cpp Exception constructors of the non-unit kind
#include "CYB.hpp"

namespace CYB {
	namespace Exception {
		//! @brief Used to better verify tests, should be optimized out by the linker. See: http://stackoverflow.com/questions/1229430/how-do-i-prevent-my-unused-global-variables-being-compiled-out
		thread_local unsigned int FLastInstantiatedExceptionCode;
	}
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
	case MUTEX_DESTRUCTION_FAILURE:
		return API::String::Static(u8"Failed to destroy a mutex.");
	case THREAD_CREATION_FAILURE:
		return API::String::Static(u8"OS failed to create requested thread.");
	default:
		CYB::API::HCF();
	}
}

CYB::Exception::SystemData::SystemData(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::SYSTEM_DATA)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}
CYB::API::String::Static CYB::Exception::Internal::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case ErrorCode::HEAP_CORRUPTION:
		return API::String::Static(u8"The blocks of the heap have been corrupted sometime between now and the last usage of the heap. The heap is now unusable.");
	default:
		CYB::API::HCF();
	}
}

CYB::Exception::Internal::Internal(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::INTERNAL)
{
	FLastInstantiatedExceptionCode = AErrorCode;
}