#pragma once
//!	@file SystemChecks.hpp Checks the platform that the engine and unit is being compiled on is appropriate

#if !defined(TARGET_OS_WINDOWS) && !defined(TARGET_OS_MAC) && !defined(TARGET_OS_LINUX)
#error CyberEngineMkII must be compiled on Windows, OSX, or Linux
#endif
#if !defined(_M_X64) && !defined(__x86_64__)
#error CyberEngineMkII must be compiled on an amd64 architecture CPU
#endif

#ifdef FAKE_POSIX
#define __x86_64__
#define __LP64__
#endif

//! @cond
#include <cstddef>
//! @endcond

static_assert(
	sizeof(unsigned char) == 1
	&& sizeof(char) == 1
	&& sizeof(unsigned short) == 2
	&& sizeof(short) == 2
	&& sizeof(unsigned int) == 4
	&& sizeof(int) == 4
	&& sizeof(unsigned long long) == 8
	&& sizeof(long long) == 8
	&& sizeof(void*) == 8,
	"A type size assertion failed");
static_assert(CYB::API::Endianess::Get() == CYB::API::Endianess::Type::LITTLE_ENDIAN, "CyberEngineMkIII relies on a little endian architecture");