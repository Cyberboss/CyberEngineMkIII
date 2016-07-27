#pragma once

#include "Backdoor.hpp"
#include "RedirectedFunction.hpp"
#include "CallRedirect.hpp"
#include "ModuleDependancy.hpp"
#include "Fakes.hpp"
#include "ForkRegistry.hpp"

namespace CYB {
	namespace Exception {
		extern thread_local unsigned int FLastInstantiatedExceptionCode;
	};
};

#include "HCF.inl"

#define CHECK_EXCEPTION_CODE(AExceptionCode) CHECK(CYB::Exception::FLastInstantiatedExceptionCode == static_cast<unsigned int>(AExceptionCode))

inline std::string GetCYBExceptionTypeName(const CYB::Exception::Base::Level ALevel) {
	using namespace CYB::Exception;
	switch (ALevel) {
	case Base::Level::VIOLATION:
		return "Violation";
	case Base::Level::SYSTEM_DATA:
		return "System Data";
	case Base::Level::INTERNAL:
		return "Internal";
	case Base::Level::UNIT:
		return "Unit";
	case Base::Level::FATAL:
		return "Fatal";
	default:
		return "Unknown CYB-based";
	}
}

CATCH_TRANSLATE_EXCEPTION(CYB::Exception::Base& AException) {
	return GetCYBExceptionTypeName(AException.FLevel) + " exception. Error code: " + std::to_string(AException.FErrorCode) + ": " + AException.FMessage.CString();
}