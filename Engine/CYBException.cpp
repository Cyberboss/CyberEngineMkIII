//! @file CYBExcpetion.cpp Exception constructors of the non-unit kind
#include "CYB.hpp"

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
	case THREAD_CREATION_FAILURE:
		return API::String::Static(u8"OS failed to create requested thread.");
	default:
		CYB::API::HCF();
	}
}

CYB::Exception::SystemData::SystemData(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::SYSTEM_DATA)
{}