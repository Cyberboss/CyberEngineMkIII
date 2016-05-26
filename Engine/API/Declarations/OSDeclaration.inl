#pragma once

constexpr CYB::API::Platform::Identifier CYB::API::Platform::Current(void) {
	return
#ifdef TARGET_OS_WINDOWS
		WINDOWS;
#elif defined(TARGET_OS_MAC)
		OSX;
#elif defined(TARGET_OS_LINUX)
		LINUX;
#endif
}