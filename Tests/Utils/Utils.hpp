#pragma once

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