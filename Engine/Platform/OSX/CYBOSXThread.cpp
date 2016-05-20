#include "CYB.hpp"

void CYB::Platform::Thread::Yield(void) {
	Implementation::Posix::sched_yield();
}