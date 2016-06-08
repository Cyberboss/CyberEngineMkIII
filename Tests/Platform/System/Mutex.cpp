#include "TestHeader.hpp"

static bool FNoopDeleteCriticalSectionCalled(false);
REDIRECTED_FUNCTION(NoopDeleteCriticalSection, CYB::Platform::Win32::CRITICAL_SECTION* const) {
	FNoopDeleteCriticalSectionCalled = true;
}

static bool SpawnMutexTryLockThread(const CYB::Platform::System::Mutex& AMutex) {
	class MutexRunner : public CYB::API::Threadable {
	private:
		const CYB::Platform::System::Mutex& FMutex;
		bool FResult;
	public:
		MutexRunner(const CYB::Platform::System::Mutex& AMutex):
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

SCENARIO("Basic Mutex functions work", "[Platform][System][Mutex][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("An empty Mutex pointer") {
		CYB::Platform::System::Mutex* TestMutex(nullptr);
		WHEN("The mutex is initialized") {
			TestMutex = new CYB::Platform::System::Mutex();
			THEN("All is well with the world") {
				CHECK_COOL_AND_CALM;
			}
			delete TestMutex;
		}
	}
	GIVEN("A valid mutex") {
		auto TestMutex = new CYB::Platform::System::Mutex();
		WHEN("The mutex is locked") {
			TestMutex->Lock();
			THEN("It can't be locked again") {
				CHECK_COOL_AND_CALM;
				CHECK_FALSE(SpawnMutexTryLockThread(*TestMutex));
			}
			AND_THEN("It is unlocked") {
				TestMutex->Unlock();
				THEN("It can be locked again") {
					CHECK_COOL_AND_CALM;
					CHECK(TestMutex->TryLock());
				}
			}
		}
		WHEN("The mutex is try locked") {
			REQUIRE(TestMutex->TryLock());
			THEN("It can't be locked again") {
				CHECK_COOL_AND_CALM;
				CHECK_FALSE(SpawnMutexTryLockThread(*TestMutex));
			}
		}
		delete TestMutex;
	}
}