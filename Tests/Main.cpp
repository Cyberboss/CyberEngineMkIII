#define CATCH_CONFIG_MAIN  
#ifdef _MSC_VER
#pragma warning(disable : 4061)
#pragma warning(disable : 4242)
#pragma warning(disable : 4365)
#pragma warning(disable : 4371)
#pragma warning(disable : 4464)
#pragma warning(disable : 4710)
#include "../SupportLibs/Catch/single_include/catch.hpp"
#else
#include "VirtualMemory.cpp"
#include "Modules.cpp"
#endif