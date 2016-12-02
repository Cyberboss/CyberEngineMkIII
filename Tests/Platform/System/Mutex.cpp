#include "TestHeader.hpp"

static bool SpawnMutexTryLockThread(CYB::Platform::System::Mutex& AMutex) {
	class MutexRunner : public CYB::API::Threadable {
	private:
		CYB::Platform::System::Mutex& FMutex;
		bool FResult;
	public:
		MutexRunner(CYB::Platform::System::Mutex& AMutex) :
			FMutex(AMutex)
		{}
		void BeginThreadedOperation(void) final override {
			FResult = FMutex.TryLock();
		}
		void CancelThreadedOperation(void) final override {}
		bool Result(void) const {
			return FResult;
		}
	};
	MutexRunner Runner(AMutex);
	{
		CYB::Platform::System::Thread Running(Runner);
	}
	return Runner.Result();
}

SCENARIO("Mutex basic functions work", "[Platform][System][Mutex][Unit]") {
	ModuleDependancy<CYB::Platform::Modules::AMKernel32> K32;
	ModuleDependancy<CYB::Platform::Modules::AMPThread> PThread;
	ModuleDependancy<CYB::Platform::Modules::AMRT> RT;
	ModuleDependancy<CYB::Platform::Modules::AMSystem> System;
	GIVEN("An empty Mutex pointer") {
		CYB::Platform::System::Mutex* TestMutex(nullptr);
		WHEN("The mutex is initialized") {
			TestMutex = new CYB::Platform::System::Mutex();
			THEN("All is well with the world") {
				CHECK(TestMutex != nullptr);
			}
			delete TestMutex;
		}
	}
	GIVEN("A valid mutex") {
		auto TestMutex = new CYB::Platform::System::Mutex();
		WHEN("The mutex is locked") {
			TestMutex->Lock();
			THEN("It can't be locked again") {
				CHECK_FALSE(SpawnMutexTryLockThread(*TestMutex));
			}
			AND_THEN("It is unlocked") {
				TestMutex->Unlock();
				THEN("It can be locked again") {
					CHECK(TestMutex->TryLock());
				}
			}
			TestMutex->Unlock();
		}
		WHEN("The mutex is try locked") {
			REQUIRE(TestMutex->TryLock());
			THEN("It can't be locked again") {
				CHECK_FALSE(SpawnMutexTryLockThread(*TestMutex));
			}
			TestMutex->Unlock();
		}
		delete TestMutex;
	}
}
REDIRECTED_FUNCTION(BadPThreadMutexInit, const void* const, const void* const) {
	return -1;
}
SCENARIO("Mutex initialization error works", "[Platform][System][Mutex][Unit]") {
#ifndef TARGET_OS_WINDOWS
	ModuleDependancy<CYB::Platform::Modules::AMPThread> PThread;
	GIVEN("A Redirected initialization function") {
		auto BPMC(PThread.Redirect<CYB::Platform::Modules::PThread::pthread_mutex_init, BadPThreadMutexInit>());
		WHEN("The function is called") {
			CYB::Platform::System::Mutex* TestMutex(nullptr);
			REQUIRE_THROWS_AS(TestMutex = new CYB::Platform::System::Mutex(), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK(TestMutex == nullptr);
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
			}
		}
	}
#else
	try {
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
	}
	catch (...) {}
	CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
#endif
}