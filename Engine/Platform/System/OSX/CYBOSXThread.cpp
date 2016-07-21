#include "CYB.hpp"

void CYB::Platform::System::Thread::Yield(void) noexcept {
	Core().FModuleManager.FSystem.Call<Modules::System::sched_yield>();
}