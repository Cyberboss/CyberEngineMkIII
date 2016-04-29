//! @file CYBWin32Implementation.hpp Includes headers for implementations that use the Windows API
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			namespace Win32 {
				#define WIN32_LEAN_AND_MEAN
				#include <Windows.h>
				#undef WIN32_LEAN_AND_MEAN
				#undef Yield
				#undef min
				#undef max
			};
		};
	};
};

#include "CYBWin32Syscalls.hpp"
#include "CYBWin32Thread.hpp"
#include "CYBWin32Process.hpp"
#include "CYBWin32VirtualMemory.hpp"
#include "CYBWin32Module.hpp"