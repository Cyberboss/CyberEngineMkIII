//! @file CYBExcpetion.cpp Exception constructors of the non-unit kind
#include "CYB.hpp"

const char* CYB::Exception::SystemData::ErrorMessage(const ErrorCode AErrorCode) {
	switch (AErrorCode) {
	case MEMORY_RESERVATION_FAILURE:
		return "Failed to reserve virtual memory from the OS.";
	case MEMORY_COMMITAL_FAILURE:
	case MEMORY_DISCARD_FAILURE:
	case MEMORY_PROTECT_FAILURE:
	case MEMORY_RELEASE_FAILURE:
	case THREAD_CREATION_FAILURE:
	default:
		CYB::API::HCF();
	}
}

CYB::Exception::SystemData::SystemData(const ErrorCode AErrorCode) :
	Base(ErrorMessage(AErrorCode), AErrorCode, Level::SYSTEM_DATA)
{}