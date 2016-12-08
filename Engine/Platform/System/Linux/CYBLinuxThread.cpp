//! @file CYBLinuxThread.cpp Implements CYB::Platform::System::Thread for Linux
#include "CYB.hpp"

void CYB::Platform::System::Thread::Yield(void) noexcept {
	Core().FModuleManager.Call<Modules::RT::sched_yield>();
}