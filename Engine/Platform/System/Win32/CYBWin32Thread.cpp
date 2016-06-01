#include "CYB.hpp"

CYB::Platform::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FThread(Core().FModuleManager.FK32.Call<Modules::Kernel32::CreateThread>(nullptr, 0U, ThreadProc, &AThreadable, 0U, nullptr))
{
	if (FThread == nullptr) {
		//! @todo Log error
		throw Exception::SystemData(Exception::SystemData::ErrorCode::THREAD_CREATION_FAILURE);
	}
}

CYB::Platform::Implementation::Thread::~Thread() {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::CloseHandle>(FThread);
}

unsigned long __stdcall CYB::Platform::Implementation::Thread::ThreadProc(void* const AThreadable) {
	try {
		try {
			static_cast<API::Threadable*>(AThreadable)->BeginThreadedOperation();
		}
		catch (CYB::Exception::Base AException) {
			//! @todo Log error
			static_cast<void>(AException);
		}
	}
	catch (...) {}
	return 0;
}
bool CYB::Platform::Thread::IsFinished(void) const {
	typedef Win32::DWORD DWORD;
	return Core().FModuleManager.FK32.Call<Modules::Kernel32::WaitForSingleObject>(FThread, 0U) == WAIT_OBJECT_0;
}

void CYB::Platform::Thread::Wait(void) const {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::WaitForSingleObject>(FThread, INFINITE);
}

void CYB::Platform::Thread::Sleep(const unsigned int AMilliseconds) {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::Sleep>(AMilliseconds);
}

void CYB::Platform::Thread::Yield(void) {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::SwitchToThread>();
}