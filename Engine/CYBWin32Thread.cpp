//! @file CYBWin32Thread.cpp Implements CYB::Platform::Thread for Win32
#include "CYB.hpp"

CYB::Platform::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FThread(Win32::CreateThread(nullptr, 0, ThreadProc, &AThreadable, 0, nullptr))
{
	if (FThread == nullptr) {
		//! @todo Log error
		throw Exception::SystemData(Exception::SystemData::ErrorCode::THREAD_CREATION_FAILURE);
	}
}

CYB::Platform::Implementation::Thread::~Thread() {
	Win32::CloseHandle(FThread);
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
	typedef Implementation::Win32::DWORD DWORD;
	return Implementation::Win32::WaitForSingleObject(FThread, 0) == WAIT_OBJECT_0;
}

void CYB::Platform::Thread::Wait(void) const {
	Implementation::Win32::WaitForSingleObject(FThread, INFINITE);
}

void CYB::Platform::Thread::Sleep(const unsigned int AMilliseconds) {
	Implementation::Win32::Sleep(AMilliseconds);
}

void CYB::Platform::Thread::Yield(void) {
	Implementation::Win32::SwitchToThread();
}