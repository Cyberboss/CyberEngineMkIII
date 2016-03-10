//! @file CYBWin32Implementation.hpp Includes headers for implementations that use the Windows API
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			namespace Win32 {
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
			};
		};
	};
};

#include "CYBWin32Process.hpp"
