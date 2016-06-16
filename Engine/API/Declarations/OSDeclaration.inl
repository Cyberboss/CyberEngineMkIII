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

constexpr bool CYB::API::Platform::IsDebug(void) {
	return
#ifdef DEBUG
		true;
#else
		false;
#endif
}

constexpr bool CYB::API::Platform::IsTest(void) {
	return
#ifdef CYB_BUILDING_TESTS
		true;
#else
		false;
#endif
}