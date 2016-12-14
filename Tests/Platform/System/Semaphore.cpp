#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::Platform::Modules;

REDIRECTED_FUNCTION(BadPThreadInit, const void* const, const void* const) {
	return -1;
}

SCENARIO("Semaphores can be constructed", "[Platform][System][Semaphore][Unit]") {
	ModuleDependancy<Kernel32> K32;
	ModuleDependancy<PThread> PThread;
	WHEN("A semaphore is constructed") {
		Semaphore Test;
		THEN("All is well") {
			CHECK(true);
		}
	}
#ifndef TARGET_OS_WINDOWS
	const auto ExpectError([&]() {
		WHEN("A semaphore is constructed") {
			CHECK_THROWS_AS(Semaphore(), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
			}
		}
	});
	GIVEN("A bad pthread mutex init") {
		const auto BadMutex(PThread.Redirect<PThread::pthread_mutex_init, BadPThreadInit>());
		ExpectError();
	}
	GIVEN("A bad pthread cond init") {
		const auto BadMutex(PThread.Redirect<PThread::pthread_mutex_init, BadPThreadInit>());
		ExpectError();
	}
#endif
}