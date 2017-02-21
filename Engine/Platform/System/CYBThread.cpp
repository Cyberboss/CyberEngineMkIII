//! @file CYBThread.cpp Shared function implementations for CYB::Platform::Thread
#include "CYB.hpp"

void CYB::Platform::System::Thread::RunThread(API::Threadable& AThreadable) noexcept {
	Core().DefaultContext();
	try {
		const auto ThreadID(Core().ThreadID());
		API::Assert::LessThan(1ULL, ThreadID);	//1 is the main thread, and if we've overflowed, someone screwed up somewhere
		if(ThreadID > 2)	//not 
			Engine::Context::GetContext().FLogger.Log(API::String::Static(u8"Thread started."), API::Logger::Level::DEV);

		AThreadable.BeginThreadedOperation();

		Engine::Context::GetContext().FLogger.Log(API::String::Static(u8"Thread finished."), API::Logger::Level::DEV);
	}
	catch (CYB::Exception::Base AException) {
		auto& Logger(Engine::Context::GetContext().FLogger);
		Logger.Log(API::String::Static(u8"Unhandled CYB exception crashed this thread. Error message follows: "), API::Logger::Level::ERR);
		Logger.Log(AException.FMessage, API::Logger::Level::ERR);
	}
	catch (...) {
		Engine::Context::GetContext().FLogger.Log(API::String::Static(u8"Unhandled unknown exception crashed this thread with no survivors."), API::Logger::Level::ERR);
	}
}

CYB::Platform::System::Thread::Thread(API::Threadable& AThreadable) :
	Implementation::Thread(AThreadable),
	FCancelSubmitted(false)
{}

CYB::Platform::System::Thread::~Thread() {
	Wait();
}

void CYB::Platform::System::Thread::Cancel(void) {
	if (!FCancelSubmitted && !IsFinished()) {
		FCancelSubmitted = true;
		FThreadable.CancelThreadedOperation();
	}
}