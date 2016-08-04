//! @file CYBLinuxSyscalls.hpp Define system and linked call numbers for Linux
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Contains call numbers for Linux
				class Sys {
				public:
					//! @brief Call numbers for Linux
					enum CallNumber : long long {
						EXIT = 60,	//!< @brief sys_exit
						EXIT_PROC = -1,	//!< @brief Aliases EXIT
						LOAD_LIBRARY = -2,	//!< @brief dlopen
						LOAD_SYMBOL = -3,	//!< @brief dlsym
						CLOSE_LIBRARY = -4,	//!< @brief dlclose
						GET_CURRENT_PROCESS = -5,	//!< @brief getpid
						LSTAT = -6,	//!< @brief lstat
					};
				};
			};
		};
	};
};