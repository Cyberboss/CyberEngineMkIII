#pragma once

namespace CYB {
	namespace Platform {
		namespace Win32 {
			#define WINBLUE_KBSPRING14	//updated APIs
			#define WIN32_LEAN_AND_MEAN	//kiss
			#include <Windows.h>
			#undef WIN32_LEAN_AND_MEAN
			#undef Yield
			#undef min
			#undef max
		};
	};
};