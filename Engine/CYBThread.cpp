//! @file CYBThread.cpp Shared function implementations for CYB::Platform::Thread
#include "CYB.hpp"

CYB::Platform::Thread::Thread(API::Threadable& AThreadable) :
	Implementation::Thread(AThreadable),
	FThreadable(AThreadable)
{}
CYB::Platform::Thread::~Thread() {
	if (!IsFinished()) {
		//! @todo Log dev warning
		Wait();
	}
}

void CYB::Platform::Thread::Cancel(void) {
	if (!IsFinished())
		FThreadable.CancelThreadedOperation();
}
