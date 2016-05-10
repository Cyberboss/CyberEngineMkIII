#include "TestHeader.hpp"

SCENARIO("Test that thread creation works as intended", "[Platform][Threads][Slow]") {
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
			try {
				TestThread = new CYB::Platform::Thread(TestClass);
			}
			catch (...) {}
			THEN("No errors occur and the thread runs separate from this one") {
				REQUIRE(TestThread != nullptr);
				CYB::Platform::Thread::Sleep(1000);
				REQUIRE(TestClass.FRan);
			}
			delete TestThread;
		}
	};
};

SCENARIO("Test that thread waiting and deletion works as intended", "[Platform][Threads][Slow]") {
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

SCENARIO("Test that threaded operations can be cancelled", "[Platform][Threads][Slow]") {
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