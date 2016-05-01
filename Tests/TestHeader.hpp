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
#include "../Engine/CYB.hpp"
#ifdef _MSC_VER
#pragma warning(disable : 4710)
#pragma warning(push)
#pragma warning(disable : 4061)
#pragma warning(disable : 4242)
#pragma warning(disable : 4365)
#pragma warning(disable : 4371)
#else
using namespace CYB::Platform::Implementation::Posix;
#endif
#include "../SupportLibs/Catch/single_include/catch.hpp"
#ifdef _MSC_VER
#pragma warning(pop)
#endif