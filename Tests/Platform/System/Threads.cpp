#include "TestHeader.hpp"

#include <chrono>

class ThreadBasicTest : public CYB::API::Threadable {
public:
	bool FRan = false;
public:
	void BeginThreadedOperation(void) final override {
		FRan = true;
	}
	void CancelThreadedOperation(void) final override {}
};

SCENARIO("Thread creation works", "[Platform][System][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
	ModuleDependancy<CYB::Platform::Modules::LibC> LibC;
	ModuleDependancy<CYB::Platform::Modules::PThread> PThread;
	ModuleDependancy<CYB::Platform::Modules::RT> RT;
	ModuleDependancy<CYB::Platform::Modules::System> System;
	GIVEN("A valid thread class") {
		ThreadBasicTest TestClass;
		WHEN("The class is used as a parameter to the Thread constructor") {
			CYB::Platform::System::Thread* TestThread(nullptr);
			REQUIRE_NOTHROW(TestThread = new CYB::Platform::System::Thread(TestClass));
			THEN("No errors occur and the thread runs separate from this one") {
				REQUIRE(TestThread != nullptr);
				CYB::Platform::System::Thread::Sleep(1000);
				CHECK(TestClass.FRan);
			}
			delete TestThread;
		}
	};
};

SCENARIO("Thread Wait and deletion work", "[Platform][System][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
	ModuleDependancy<CYB::Platform::Modules::LibC> LibC;
	ModuleDependancy<CYB::Platform::Modules::PThread> PThread;
	ModuleDependancy<CYB::Platform::Modules::RT> RT;
	ModuleDependancy<CYB::Platform::Modules::System> System;
	GIVEN("A valid thread") {
		ThreadBasicTest TestClass;
		auto TestThread(new CYB::Platform::System::Thread(TestClass));

		//Prevent test from going forever
		CYB::Platform::System::Thread::Sleep(1000);
		REQUIRE(TestThread->IsFinished());

		WHEN("The thread is waited upon") {
			TestThread->Wait();
			THEN("The thread has ran and terminated") {
				CHECK(TestClass.FRan);
			}
			delete TestThread;
		}
		WHEN("The thread is deleted") {
			delete TestThread;
			THEN("The thread is ran and terminated") {
				CHECK(TestClass.FRan);
			}
		}
	};
};

class ThreadCancelTest : public CYB::API::Threadable {
private:
	std::atomic_bool FRunning;
public:
	ThreadCancelTest() :
		FRunning(true)
	{}
	void BeginThreadedOperation(void) final override {
		while (FRunning.load(std::memory_order_relaxed))
			CYB::Platform::System::Thread::Yield();
	}
	void CancelThreadedOperation(void) final override {
		FRunning.store(false, std::memory_order_relaxed);
	}
};

SCENARIO("Threaded operations can be cancelled", "[Platform][System][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
	ModuleDependancy<CYB::Platform::Modules::LibC> LibC;
	ModuleDependancy<CYB::Platform::Modules::PThread> PThread;
	ModuleDependancy<CYB::Platform::Modules::RT> RT;
	ModuleDependancy<CYB::Platform::Modules::System> System;
	GIVEN("A valid thread that will run forever") {
		ThreadCancelTest TestClass;
		CYB::Platform::System::Thread TestThread(TestClass);
		CYB::Platform::System::Thread::Sleep(1000);
		REQUIRE(!TestThread.IsFinished());
		WHEN("The thread is cancelled") {
			TestThread.Cancel();
			THEN("The thread will terminate") {
				CYB::Platform::System::Thread::Sleep(1000);
				CHECK(TestThread.IsFinished());
			}
		}
	};
}

static long long TestGetTime(void) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

SCENARIO("Thread Sleep works", "[Platform][System][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
	ModuleDependancy<CYB::Platform::Modules::LibC> LibC;
	ModuleDependancy<CYB::Platform::Modules::PThread> PThread;
	ModuleDependancy<CYB::Platform::Modules::RT> RT;
	ModuleDependancy<CYB::Platform::Modules::System> System;
	GIVEN("A long ass computer time and a current time") {
		const auto Milliseconds(1000);
		const auto StartTime(TestGetTime());
		WHEN("We sleep for that long") {
			CYB::Platform::System::Thread::Sleep(Milliseconds);
			THEN("At least that amount of time has passed when we wake up") {
				CHECK((TestGetTime() - StartTime) >= Milliseconds);
			}
		}
	}
}

SCENARIO("Thread Yield works", "[Platform][System][Threads][Unit]") {
	GIVEN("The correct modules") {
		ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
		ModuleDependancy<CYB::Platform::Modules::RT> RT;
		ModuleDependancy<CYB::Platform::Modules::System> System;
		WHEN("Yield is called") {
			CYB::Platform::System::Thread::Yield();	//what do you want from me?
			THEN("Nothing bad happens and we can only assume the OS did it's job") {
				CHECK(true);
			}
		}
	}
}

typedef unsigned long(*WindowsThreadStartRoutine)(void* const);

REDIRECTED_FUNCTION(BadCreateThread, void* const, const unsigned long long, WindowsThreadStartRoutine const, void* const, const unsigned long, unsigned long*) {
	return static_cast<void*>(nullptr);
}

REDIRECTED_FUNCTION(BadPThreadCreate, void* const, void* const, void* const, void* const) {
	return -1;
}

namespace CYB {
	namespace Exception {
		class ThreadExceptionTester : public Base {
		public:
			ThreadExceptionTester() :
				Base(u8"Nothing to see here", 0, CYB::Exception::Base::Level::UNIT)
			{}
		};
	}
}

class ThreadErrorTest : public CYB::API::Threadable {
private:
	const bool FThrowKnownException, FThrowOnCancellation;
	std::atomic_bool FCancelled;
public:
	ThreadErrorTest(const bool AThrowKnownException, const bool AThrowOnCancellation) :
		FThrowKnownException(AThrowKnownException),
		FThrowOnCancellation(AThrowOnCancellation),
		FCancelled(false)
	{}
	void BeginThreadedOperation(void) final override {
		if (!FThrowOnCancellation) {
			if (FThrowKnownException)
				throw CYB::Exception::ThreadExceptionTester();
			else
				throw std::exception();
		}
		while (!FCancelled.load(std::memory_order_relaxed))
			CYB::Platform::System::Thread::Yield();
	}
	void CancelThreadedOperation(void) final override {
		FCancelled.store(true, std::memory_order_relaxed);
		if (FThrowOnCancellation) {
			if (FThrowKnownException)
				throw CYB::Exception::ThreadExceptionTester();
			else
				throw std::exception();
		}
	}
};

REDIRECTED_FUNCTION(BadPThreadMutexInit, const void* const, const void* const) {
	return -1;
}

SCENARIO("Thread errors work", "[Platform][System][Threads][Unit]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
	ModuleDependancy<CYB::Platform::Modules::PThread> PThread;
	ModuleDependancy<CYB::Platform::Modules::RT> RT;
	ModuleDependancy<CYB::Platform::Modules::System> System;
	GIVEN("An invalid thread creation call") {
		auto BCT(K32.Redirect<CYB::Platform::Modules::Kernel32::CreateThread, BadCreateThread>());
		auto BPTC(PThread.Redirect<CYB::Platform::Modules::PThread::pthread_create, BadPThreadCreate>());
		WHEN("Thread creation is attempted") {
			ThreadBasicTest Test;
			CYB::Platform::System::Thread* Thread(nullptr);
			REQUIRE_THROWS_AS(Thread = new CYB::Platform::System::Thread(Test), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::THREAD_CREATION_FAILURE);
				CHECK_FALSE(Test.FRan);
			}
			delete Thread;
		}
	}
	GIVEN("A Threadable that will throw an unknown exception on start") {
		ThreadErrorTest Test(false, false);
		WHEN("A thread is run created using it") {
			CYB::Platform::System::Thread TestThread(Test);
			THEN("Nothing bad happens") {
				CHECK(true);
			}
		}
	}
	GIVEN("A Threadable that will throw a known exception on start") {
		ThreadErrorTest Test(true, false);
		WHEN("A thread is run created using it") {
			CYB::Platform::System::Thread TestThread(Test);
			THEN("Nothing bad happens") {
				CHECK(true);
			}
		}
	}
	GIVEN("A Threadable that will throw an unknown exception on cancel") {
		ThreadErrorTest Test(false, true);
		CYB::Platform::System::Thread TestThread(Test);
		WHEN("A thread is cancelled created using it") {
			TestThread.Cancel();
			THEN("Nothing bad happens") {
				CHECK(true);
			}
		}
	}
	GIVEN("A Threadable that will throw a known exception on cancel") {
		ThreadErrorTest Test(true, true);
		CYB::Platform::System::Thread TestThread(Test);
		WHEN("A thread is cancelled created using it") {
			TestThread.Cancel();
			THEN("Nothing bad happens") {
				CHECK(true);
			}
		}
	}
}