//! @file CYBWin32VirtualMemory.hpp Contains the Win32 Sys implementation
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Used for Linked calls only
				class Sys {
				public:
					//! @brief Win32 Linked call numbers
					enum CallNumber : long long {
						TERMINATE_PROC = -1,	//!< @brief See <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms686714(v=vs.85).aspx">TerminateProcess</a> 
						LOAD_LIBRARY = -2,	//!< @brief See <a href="https://msdn.microsoft.com/en-ca/library/windows/desktop/ms684175(v=vs.85).aspx">LoadLibrary</a> 
						LOAD_SYMBOL = -3,	//!< @brief See <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms683212(v=vs.85).aspx">GetProcAddress</a> 
						CLOSE_LIBRARY = -4,	//!< @brief See <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152(v=vs.85).aspx">FreeLibrary</a> 
						GET_CURRENT_PROCESS = -5,	//!< @brief See <a href="https://msdn.microsoft.com/en-us/library/windows/desktop/ms683179(v=vs.85).aspx">GetCurrentProcess</a> 
					};
				};
			};
		};
	};
};
