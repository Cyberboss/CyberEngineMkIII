#pragma once

namespace CYB {
	namespace Platform {
		//! @brief Includes headers for Windows platforms
		namespace Win32 {
			//! @cond
#ifdef TARGET_OS_WINDOWS
			#define WINBLUE_KBSPRING14	//updated APIs
			#define WIN32_LEAN_AND_MEAN	//kiss
			#include <Windows.h>
			#include <Shellapi.h>
			#include <Shlwapi.h>
			#undef WIN32_LEAN_AND_MEAN
			#undef Yield
			#undef min
			#undef max
			#undef CreateProcess
#endif
			//! @endcond
		};
	};
};