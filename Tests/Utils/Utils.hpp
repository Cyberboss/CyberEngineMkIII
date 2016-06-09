#pragma once

#include "Fakes.hpp"
#include "RedirectedFunction.hpp"
#include "CallRedirect.hpp"
#include "ModuleDependancy.hpp"

unsigned int CheckHCF(void);
namespace CYB {
	namespace Exception {
		extern thread_local unsigned int FLastInstantiatedExceptionCode;
	};
};

#define CHECK_COOL_AND_CALM REQUIRE(CheckHCF() == 0)
#define REQUIRE_HCF(AN) REQUIRE(CheckHCF() == AN)