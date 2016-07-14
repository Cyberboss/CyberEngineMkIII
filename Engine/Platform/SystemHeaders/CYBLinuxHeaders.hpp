//! @file CYBLinuxHeaders.hpp Includes headers for Linux platforms
#pragma once
namespace CYB {
	namespace Platform {
		namespace Posix {
#ifdef TARGET_OS_LINUX
			#include <linux/limits.h>
			#include <gnu/lib-names.h>
#endif
		}
	}
}