//! @file CYBOSXSyscalls.hpp Defines linked and syscall numbers for OSX
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief OSX syscall numbers have to be incremented by 0x2000000 to work on 64-bit
				class Sys {
				public:
					//! @brief Call numbers for OSX
					enum CallNumber : long long {
						EXIT = 0x2000000 + 1,	//!< @brief sys_exit
						EXIT_PROC = -1,	//!< @brief Aliases EXIT
						LOAD_LIBRARY = -2,	//!< @brief dlopen
						LOAD_SYMBOL = -3,	//!< @brief dlsym
						CLOSE_LIBRARY = -4,	//!< @brief dlclose
						GET_CURRENT_PROCESS = -5,	//!< @brief getpid
						LSTAT = -6,	//!< @brief lstat
						FSTAT = -7, //!< @brief fstat
					};
				};
			};
		};
	};
};