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
#define ENABLE_TEST_HOOKS public: template <typename AHook> static void Backdoor(AHook& AHooker);

#include "../Engine/CYB.hpp"

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

#include "Utils/Utils.hpp"

#ifdef _MSC_VER
#pragma warning(pop)
#endif