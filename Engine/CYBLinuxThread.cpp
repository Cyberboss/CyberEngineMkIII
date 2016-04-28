//! @file CYBLinuxThread.cpp Implements CYB::Platform::Thread for Linux
#include "CYB.hpp"

void CYB::Platform::Thread::Yield(void) {
	Sys::Call(Sys::YIELD);
}