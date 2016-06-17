#pragma once

#include "Backdoor.hpp"
#include "Fakes.hpp"
#include "RedirectedFunction.hpp"
#include "CallRedirect.hpp"
#include "ModuleDependancy.hpp"

namespace CYB {
	namespace Exception {
		extern thread_local unsigned int FLastInstantiatedExceptionCode;
	};
};

#include "HCF.inl"

#define CHECK_EXCEPTION_CODE(AExceptionCode) CHECK(CYB::Exception::FLastInstantiatedExceptionCode == static_cast<unsigned int>(AExceptionCode))