#include "TestHeader.hpp"

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
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("A valid thread class") {
		ThreadBasicTest TestClass;
		WHEN("The class is used as a parameter to the Thread constructor") {
			CYB::Platform::System::Thread* TestThread(nullptr);
			REQUIRE_NOTHROW(TestThread = new CYB::Platform::System::Thread(TestClass));
			THEN("No errors occur and the thread runs separate from this one") {
				CHECK_COOL_AND_CALM;
				REQUIRE(TestThread != nullptr);
				CYB::Platform::System::Thread::Sleep(1000);
				CHECK(TestClass.FRan);
			}
			delete TestThread;
		}
	};
};

SCENARIO("Thread waiting and deletion work", "[Platform][System][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("A valid thread") {
		ThreadBasicTest TestClass;
		auto TestThread(new CYB::Platform::System::Thread(TestClass));

		//Prevent test from going forever
		CYB::Platform::System::Thread::Sleep(1000);
		REQUIRE(TestThread->IsFinished());

		WHEN("The thread is waited upon") {
			TestThread->Wait();
			THEN("The thread has ran and terminated") {
				CHECK_COOL_AND_CALM;
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
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("A valid thread that will run forever") {
		ThreadCancelTest TestClass;
		CYB::Platform::System::Thread TestThread(TestClass);
		CYB::Platform::System::Thread::Sleep(1000);
		REQUIRE(!TestThread.IsFinished());
		WHEN("The thread is cancelled") {
			TestThread.Cancel();
			THEN("The thread will terminate") {
				CHECK_COOL_AND_CALM;
				CYB::Platform::System::Thread::Sleep(1000);
				CHECK(TestThread.IsFinished());
			}
		}
	};
}

static unsigned int TestGetTime(void) {
#ifdef TARGET_OS_WINDOWS
	return CYB::Platform::Win32::GetTickCount();
#else
	using namespace CYB::Platform::Posix;
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (now.tv_sec * 1000) + (now.tv_nsec / 1000000);
#endif
}

SCENARIO("Thread Sleep works", "[Platform][System][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("A long ass computer time and a current time") {
		const auto Milliseconds(1000);
		const auto StartTime(TestGetTime());
		WHEN("We sleep for that long") {
			CYB::Platform::System::Thread::Sleep(Milliseconds);
			THEN("At least that amount of time has passed when we wake up") {
				CHECK_COOL_AND_CALM;
				CHECK((TestGetTime() - StartTime) >= Milliseconds);
			}
		}
	}
}

SCENARIO("Thread Yield works", "[Platform][System][Threads][Unit]") {
	GIVEN("The correct modules") {
		ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
		ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMRT> RT(CYB::Core().FModuleManager.FRT);
		WHEN("Yield is called") {
			CYB::Platform::System::Thread::Yield();	//what do you want from me?
			THEN("Nothing bad happens and we can only assume the OS did it's job") {
				CHECK_COOL_AND_CALM;
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
	const bool FThrowKnownException;
public:
	ThreadErrorTest(const bool AThrowKnownException) :
		FThrowKnownException(AThrowKnownException)
	{}
	void BeginThreadedOperation(void) final override {
		if (FThrowKnownException)
			throw CYB::Exception::ThreadExceptionTester();
		else
			throw std::exception();
	}
	void CancelThreadedOperation(void) final override {}
};

REDIRECTED_FUNCTION(BadPThreadMutexInit, const void* const, const void* const) {
	return -1;
}

SCENARIO("Thread errors work", "[Platform][System][Threads][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("An invalid thread creation call") {
		auto BCT(K32.Redirect<CYB::Platform::Modules::Kernel32::CreateThread, BadCreateThread>());
		auto BPTC(PThread.Redirect<CYB::Platform::Modules::PThread::pthread_create, BadPThreadCreate>());
		WHEN("Thread creation is attempted") {
			ThreadBasicTest Test;
			CYB::Platform::System::Thread* Thread(nullptr);
			REQUIRE_THROWS_AS(Thread = new CYB::Platform::System::Thread(Test), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK_COOL_AND_CALM;
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::THREAD_CREATION_FAILURE);
				CHECK_FALSE(Test.FRan);
			}
			delete Thread;
		}
	}
#ifndef TARGET_OS_WINDOWS
	GIVEN("An invalid mutex creation call") {
		auto BPMC(PThread.Redirect<CYB::Platform::Modules::PThread::pthread_mutex_init, BadPThreadMutexInit>());
		WHEN("Thread creation is attempted") {
			ThreadBasicTest Test;
			CYB::Platform::System::Thread* Thread(nullptr);
			REQUIRE_THROWS_AS(Thread = new CYB::Platform::System::Thread(Test), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK_COOL_AND_CALM;
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::THREAD_CREATION_FAILURE);
				CHECK_FALSE(Test.FRan);
			}
			delete Thread;
		}
	}
#endif
	GIVEN("A Threadable that will throw an unknown exception") {
		ThreadErrorTest Test(false);
		WHEN("A thread is run created using it") {
			CYB::Platform::System::Thread TestThread(Test);
			THEN("Nothing bad happens") {
				CHECK_COOL_AND_CALM;
			}
		}
	}
	GIVEN("A Threadable that will throw a known exception") {
		ThreadErrorTest Test(true);
		WHEN("A thread is run created using it") {
			CYB::Platform::System::Thread TestThread(Test);
			THEN("Nothing bad happens") {
				CHECK_COOL_AND_CALM;
			}
		}
	}
}