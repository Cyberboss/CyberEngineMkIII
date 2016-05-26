#include "TestHeader.hpp"

SCENARIO("Test that thread creation works as intended", "[Platform][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("A valid thread class") {
		class ThreadBasicTest : public CYB::API::Threadable {
		public:
			bool FRan = false;
		public:
			void BeginThreadedOperation(void) final override {
				FRan = true;
			}
			void CancelThreadedOperation(void) final override {}
		};
		ThreadBasicTest TestClass;
		WHEN("The class is used as a parameter to the Thread constructor") {
			CYB::Platform::Thread* TestThread(nullptr);
			REQUIRE_NOTHROW(TestThread = new CYB::Platform::Thread(TestClass));
			THEN("No errors occur and the thread runs separate from this one") {
				REQUIRE(TestThread != nullptr);
				CYB::Platform::Thread::Sleep(1000);
				REQUIRE(TestClass.FRan);
			}
			delete TestThread;
		}
	};
};

SCENARIO("Test that thread waiting and deletion works as intended", "[Platform][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("A valid thread") {
		class ThreadBasicTest : public CYB::API::Threadable {
		public:
			bool FRan = false;
		public:
			void BeginThreadedOperation(void) final override {
				FRan = true;
			}
			void CancelThreadedOperation(void) final override {}
		};
		ThreadBasicTest TestClass;
		auto TestThread(new CYB::Platform::Thread(TestClass));

		//Prevent test from going forever
		CYB::Platform::Thread::Sleep(1000);
		REQUIRE(TestThread->IsFinished());

		WHEN("The thread is waited upon") {
			TestThread->Wait();
			THEN("The thread has ran and terminated") {
				REQUIRE(TestClass.FRan);
			}
			delete TestThread;
		}
		WHEN("The thread is deleted") {
			delete TestThread;
			THEN("The thread is ran and terminated") {
				REQUIRE(TestClass.FRan);
			}
		}
	};
};

SCENARIO("Test that threaded operations can be canceled", "[Platform][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("A valid thread that will run forever") {
		class ThreadCancelTest : public CYB::API::Threadable {
		private:
			std::atomic_bool FRunning;
		public:
			ThreadCancelTest() :
				FRunning(true)
			{}
			void BeginThreadedOperation(void) final override {
				while (FRunning.load(std::memory_order_relaxed))
					CYB::Platform::Thread::Yield();
			}
			void CancelThreadedOperation(void) final override {
				FRunning.store(false, std::memory_order_relaxed);
			}
		};
		ThreadCancelTest TestClass;
		CYB::Platform::Thread TestThread(TestClass);
		CYB::Platform::Thread::Sleep(1000);
		REQUIRE(!TestThread.IsFinished());
		WHEN("The thread is cancelled") {
			TestThread.Cancel();
			THEN("The thread will terminate") {
				CYB::Platform::Thread::Sleep(1000);
				REQUIRE(TestThread.IsFinished());
			}
		}
	};
}

static unsigned int TestGetTime(void) {
#ifdef TARGET_OS_WINDOWS
	return CYB::Platform::Implementation::Win32::GetTickCount();
#else
	using namespace CYB::Platform::Implementation::Posix;
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (now.tv_sec * 1000) + (now.tv_nsec / 1000000);
#endif
}

SCENARIO("Test basic sleep works", "[Platform][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("A long ass computer time and a current time") {
		const auto Milliseconds(1000);
		const auto StartTime(TestGetTime());
		WHEN("We sleep for that long") {
			CYB::Platform::Thread::Sleep(Milliseconds);
			THEN("At least that amount of time has passed when we wake up") {
				REQUIRE((TestGetTime() - StartTime) >= Milliseconds);
			}
		}
	}
}

TEST_CASE("Test basic yield works", "[Platform][Threads][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	CYB::Platform::Thread::Yield();	//what do you want from me?
}