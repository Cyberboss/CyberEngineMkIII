#pragma once
#define CYB_BUILDING_TESTS
#ifdef _MSC_VER
#pragma warning(disable : 4464)
#else
#include <time.h>
#undef LITTLE_ENDIAN
#undef BIG_ENDIAN
#undef PDP_ENDIAN
#endif

#define ASSERTION_OVERRIDE
unsigned int CheckHCF(void);
#include "../Engine/CYB.hpp"

#include "Utils/RedirectedFunction.hpp"
#include "Utils/CallRedirect.hpp"
#include "Utils/ModuleDependancy.hpp"

namespace CYB {
	namespace Exception {
		extern thread_local unsigned int FLastInstantiatedExceptionCode;
	}
}

#ifdef _MSC_VER
#pragma warning(disable : 4710)
#pragma warning(push)
#pragma warning(disable : 4061)
#pragma warning(disable : 4242)
#pragma warning(disable : 4365)
#pragma warning(disable : 4371)
#else
using namespace CYB::Platform::Posix;
#endif
#include "../SupportLibs/Catch/include/catch.hpp"

#define CHECK_COOL_AND_CALM REQUIRE(CheckHCF() == 0)
#define REQUIRE_HCF(AN) REQUIRE(CheckHCF() == AN)

#ifdef _MSC_VER
#pragma warning(pop)
#endif