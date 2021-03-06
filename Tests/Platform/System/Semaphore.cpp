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
		const auto BadMutex(PThread.Redirect<PThread::pthread_cond_init, BadPThreadInit>());
		ExpectError();
	}
#endif
}

Semaphore* CurrentSem;
unsigned int* LastReturn(nullptr);
bool DelaySleep(false);

class SemBasicTest : public CYB::API::Threadable {
public:
	const unsigned int FRetVal;
public:
	SemBasicTest(const unsigned int ARetVal = 0) :
		FRetVal(ARetVal)
	{}
	void BeginThreadedOperation(void) final override {
		CurrentSem->Wait();
		if(FRetVal > 0)
			*LastReturn = FRetVal;
	}
	void CancelThreadedOperation(void) final override {}
};

struct HookStruct {
	bool FLock;
	void* FImplMutex;
};

/*
	MASSIVE HACK

	This is to hit the call to WaitOne in Semaphore::Wait
	The OS scheduler is pretty smart, so it'll basically never be hit IRL
	This redirects the wait module calls. Uses the parameters to derive Implementation::Semaphore -> Semaphore
	Unlocks the mutex, waits a bit, relocks the mutex and THEN does the call
	This screws with the scheduler and forces WaitOne to be called
	Sign of good design I guess?
	Thank god ENABLE_TEST_HOOKS has no runtime implications
*/

template <> void CYB::Platform::System::Semaphore::Backdoor<HookStruct>(HookStruct& AHooker) {
	auto& Sem(static_cast<Semaphore&>(*reinterpret_cast<CYB::Platform::System::Implementation::Semaphore*>(AHooker.FImplMutex)));
	if (AHooker.FLock)
		Sem.Lock();
	else
		Sem.Unlock();
}

#ifdef TARGET_OS_WINDOWS
using namespace CYB::Platform::Win32;
REDIRECTED_FUNCTION(BadCondWait, PCONDITION_VARIABLE AArg1, PCRITICAL_SECTION AMutex, DWORD AArg3)
#else
using namespace CYB::Platform::Posix;
REDIRECTED_FUNCTION(BadCondWait, pthread_cond_t* AArg1, pthread_mutex_t* AMutex)
#endif
{
	if (DelaySleep) {
		DelaySleep = false;
		HookStruct Hooker{ false, AMutex };
		Semaphore::Backdoor(Hooker);
		Thread::Sleep(100);
		Hooker.FLock = true;
		Semaphore::Backdoor(Hooker);
	}
#ifdef TARGET_OS_WINDOWS
	return ARedirector::CallOriginal(AArg1, AMutex, AArg3);
#else
	return ARedirector::CallOriginal(AArg1, AMutex);
#endif
}

SCENARIO("Semaphores can be waited on and signaled in order", "[Platform][System][Semaphore][Unit]") {
	ModuleDependancy<Kernel32> K32;
	ModuleDependancy<PThread> PT;
	ModuleDependancy<LibC> C;

	GIVEN("A semaphore") {
		Semaphore Sem;
		CurrentSem = &Sem;

		WHEN("A thread is created to wait on the semaphore") {
			SemBasicTest Test;
			Thread SemThread(Test);
			Thread::Sleep(50);
			THEN("The thread is still running") {
				CHECK_FALSE(SemThread.IsFinished());
			}
			Sem.SignalAll();
			Thread::Sleep(50);
			AND_WHEN("The semaphore is signaled") {
				THEN("The thread has finished") {
					CHECK(SemThread.IsFinished());
				}
			}
		}
		const auto DoTest([&]() {
			WHEN("A few threads are created to wait on the semaphores in order") {
				SemBasicTest Test1(1), Test2(2), Test3(3);
				unsigned int Return;
				LastReturn = &Return;
				Thread SemThread1(Test1);
				Thread::Sleep(50);
				Thread SemThread2(Test2);
				Thread::Sleep(50);
				Thread SemThread3(Test3);
				Thread::Sleep(100);

				unsigned int NextExpected(1);

				const auto ClearOne([&](Thread& AThread) {
					Sem.SignalN(1);
					AND_THEN("One thread is cleared") {
						AThread.Wait();
						THEN("The retval is set correctly") {
							CHECK(Return == NextExpected);
						}
					}
					++NextExpected;
				});

				GIVEN("That we wanna test individual signals") {
					ClearOne(SemThread1);
					ClearOne(SemThread2);
					ClearOne(SemThread3);
				}

				AND_THEN("1 is signaled") {
					Sem.SignalN(1);
					Thread::Sleep(50);
					THEN("1 finishes but not 2 and 3") {
						CHECK(SemThread1.IsFinished());
						CHECK_FALSE(SemThread2.IsFinished());
						CHECK_FALSE(SemThread3.IsFinished());
					}
				}

				AND_THEN("2 are signaled") {
					Sem.SignalN(2);
					Thread::Sleep(50);
					THEN("1 and 2 finish but not 3") {
						CHECK(SemThread1.IsFinished());
						CHECK(SemThread2.IsFinished());
						CHECK_FALSE(SemThread3.IsFinished());
					}
				}

				AND_THEN("3 are signaled") {
					Sem.SignalN(3);
					Thread::Sleep(50);
					THEN("They all finish") {
						CHECK(SemThread1.IsFinished());
						CHECK(SemThread2.IsFinished());
						CHECK(SemThread3.IsFinished());
					}
				}

				Sem.SignalAll();
			}
		});

		DoTest();
		
		AND_WHEN("We do some crazy hacks to beat the OS scheduler") {
			DelaySleep = true;
			const auto Thing1(K32.Redirect<Kernel32::SleepConditionVariableCS, BadCondWait>());
			const auto Thing2(PT.Redirect<PThread::pthread_cond_wait, BadCondWait>());
			DoTest();
		}

	}
}