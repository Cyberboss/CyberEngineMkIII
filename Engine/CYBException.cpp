//! @file CYBExcpetion.cpp Exception constructors of the non-unit kind
#include "CYB.hpp"

const char* CYB::Exception::SystemData::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case MEMORY_RESERVATION_FAILURE:
		return "Failed to reserve virtual memory from the OS.";
	case MEMORY_COMMITAL_FAILURE:
		return "Failed to commit virtual memory from the OS.";
	case MEMORY_DISCARD_FAILURE:
		return "Failed to discard virtual memory to the OS.";
	case MEMORY_PROTECT_FAILURE:
		return "Failed to implement requested virtual memory protection level.";
	case MEMORY_RELEASE_FAILURE:
		return "Failed to return virtual memory to the OS.";
	case THREAD_CREATION_FAILURE:
		return "OS failed to create requested thread.";
	default:
		CYB::API::HCF();
	}
}

CYB::Exception::SystemData::SystemData(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::SYSTEM_DATA)
{}