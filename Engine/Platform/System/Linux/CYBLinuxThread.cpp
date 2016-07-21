#include "CYB.hpp"

void CYB::Platform::System::Thread::Yield(void) noexcept {
	Core().FModuleManager.FRT.Call<Modules::RT::sched_yield>();
}