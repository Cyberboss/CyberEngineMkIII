//! @file CYBThread.cpp Shared function implementations for CYB::Platform::Thread
#include "CYB.hpp"

CYB::Platform::Thread::Thread(API::Threadable& AThreadable) :
	Implementation::Thread(AThreadable),
	FThreadable(AThreadable),
	FCancelSubmitted(false)
{}
CYB::Platform::Thread::~Thread() {
	Wait();
}

void CYB::Platform::Thread::Cancel(void) {
	if (!FCancelSubmitted && !IsFinished()) {
		FCancelSubmitted = true;
		FThreadable.CancelThreadedOperation();
	}
}
