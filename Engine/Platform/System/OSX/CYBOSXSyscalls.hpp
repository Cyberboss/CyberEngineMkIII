#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				class Sys {
				public:
					enum CallNumber : long long {
						EXIT = 0x2000000 + 1,
						EXIT_PROC = -1,
						LOAD_LIBRARY = -2,
						LOAD_SYMBOL = -3,
						CLOSE_LIBRARY = -4,
						GET_CURRENT_PROCESS = -5,
						LSTAT = -6,
					};
				};
			};
		};
	};
};