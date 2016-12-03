//! @file CYBOSXThread.cpp Implements CYB::Platform::System::Thread for OSX
#include "CYB.hpp"

void CYB::Platform::System::Thread::Yield(void) noexcept {
	Core().FModuleManager.Call<Modules::System::sched_yield>();
}