//! @file CYBWin32Headers.hpp Includes headers for Windows platforms
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
			//Shlobj.h has too many shitty inline functions
			//defining the exported ones here
#pragma region ShlobjDefs
			HRESULT SHGetKnownFolderPath(
			_In_     REFKNOWNFOLDERID rfid,
				_In_     DWORD            dwFlags,
				_In_opt_ HANDLE           hToken,
				_Out_    PWSTR            *ppszPath
				);
#pragma endregion
			#undef WIN32_LEAN_AND_MEAN
			#undef Yield
			#undef min
			#undef max
			#undef CreateProcess
			#undef CreateDirectory
#endif
			//! @endcond
		};
	};
};

#include "CYBUTF16String.hpp"