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
			AND_WHEN("The semaphore is signaled") {
				Thread::Sleep(50);
				THEN("The thread has finished") {
					CHECK(SemThread.IsFinished());
				}
			}
		}
		WHEN("A few threads are created to wait on the semaphore in order") {
			SemBasicTest Test1(1), Test2(2), Test3(3);
			unsigned int Return;
			LastReturn = &Return;
			Thread SemThread1(Test1);
			Thread::Sleep(50);
			Thread SemThread2(Test2);
			Thread::Sleep(50);
			Thread SemThread3(Test3);
			Thread::Sleep(50);

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
	}
}