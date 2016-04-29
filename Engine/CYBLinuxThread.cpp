//! @file CYBLinuxThread.cpp Implements CYB::Platform::Thread for Linux
#include "CYB.hpp"

void CYB::Platform::Thread::Yield(void) {
	Implementation::Posix::sched_yield();
}