//! @file CYBThread.cpp Shared function implementations for CYB::Platform::Thread
#include "CYB.hpp"

CYB::Platform::System::Thread::Thread(API::Threadable& AThreadable) :
	Implementation::Thread(AThreadable),
	FCancelSubmitted(false)
{}

CYB::Platform::System::Thread::~Thread() {
	Wait();
}

void CYB::Platform::System::Thread::Cancel(void) noexcept {
	if (!FCancelSubmitted && !IsFinished()) {
		FCancelSubmitted = true;
		try {
			FThreadable.CancelThreadedOperation();
		}
		catch (CYB::Exception::Base AException) {
			static_cast<void>(AException);
			//! @todo Log error
		}
		catch (...) {
			//! @todo Log error
		}
	}
}
