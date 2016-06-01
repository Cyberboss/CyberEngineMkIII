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

SCENARIO("Test that thread creation works as intended", "[Platform][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("A valid thread class") {
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
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("A valid thread") {
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

SCENARIO("Test that threaded operations can be canceled", "[Platform][Threads][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("A valid thread that will run forever") {
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
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMRT> RT(CYB::Core().FModuleManager.FRT);
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

SCENARIO("Test basic yield works", "[Platform][Threads][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMRT> RT(CYB::Core().FModuleManager.FRT);
	CYB::Platform::Thread::Yield();	//what do you want from me?
}

typedef unsigned long(*WindowsThreadStartRoutine)(void* const);

REDIRECTED_FUNCTION(BadCreateThread, void* const, const unsigned long long, WindowsThreadStartRoutine const, void* const, const unsigned long, unsigned long*) {
	return static_cast<void*>(nullptr);
}

REDIRECTED_FUNCTION(BadPThreadCreate, void* const, void* const, void* const, void* const) {
	return -1;
}

class ThreadErrorTest : public CYB::API::Threadable {
public:
	void BeginThreadedOperation(void) final override {
		throw std::exception();
	}
	void CancelThreadedOperation(void) final override {}
};

SCENARIO("Test thread system errors work", "[Platform][Threads][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMRT> RT(CYB::Core().FModuleManager.FRT);
	GIVEN("An invalid thread creation call") {
		auto BCT(K32.Redirect<CYB::Platform::ModuleDefinitions::Kernel32::CreateThread, BadCreateThread>());
		auto BPTC(PThread.Redirect<CYB::Platform::ModuleDefinitions::PThread::pthread_create, BadPThreadCreate>());
		WHEN("Thread creation is attempted") {
			ThreadBasicTest Test;
			CYB::Platform::Thread* Thread(nullptr);
			REQUIRE_THROWS_AS(Thread = new CYB::Platform::Thread(Test), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::THREAD_CREATION_FAILURE);
				CHECK_FALSE(Test.FRan);
			}
			delete Thread;
		}
	}
}