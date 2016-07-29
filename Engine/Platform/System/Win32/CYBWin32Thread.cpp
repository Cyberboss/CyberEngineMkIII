#include "CYB.hpp"

CYB::Platform::System::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FThread(Core().FModuleManager.FK32.Call<Modules::Kernel32::CreateThread>(nullptr, 0U, ThreadProc, &AThreadable, 0U, nullptr))
{
	if (FThread == nullptr)
		throw Exception::SystemData(Exception::SystemData::THREAD_CREATION_FAILURE);
}

CYB::Platform::System::Implementation::Thread::~Thread() {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::CloseHandle>(FThread);
}

unsigned long __stdcall CYB::Platform::System::Implementation::Thread::ThreadProc(void* const AThreadable) noexcept {
	try {
		static_cast<API::Threadable*>(AThreadable)->BeginThreadedOperation();
	}
	catch (CYB::Exception::Base AException) {
		// TODO Log error
		static_cast<void>(AException);
	}
	catch (...) {
		// TODO Log error
	}
	return 0;
}
bool CYB::Platform::System::Thread::IsFinished(void) const noexcept {
	using DWORD = Win32::DWORD;
	return Core().FModuleManager.FK32.Call<Modules::Kernel32::WaitForSingleObject>(FThread, 0U) == WAIT_OBJECT_0;
}

void CYB::Platform::System::Thread::Wait(void) const noexcept {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::WaitForSingleObject>(FThread, INFINITE);
}

void CYB::Platform::System::Thread::Sleep(const unsigned int AMilliseconds) noexcept {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::Sleep>(AMilliseconds);
}

void CYB::Platform::System::Thread::Yield(void) noexcept {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::SwitchToThread>();
}