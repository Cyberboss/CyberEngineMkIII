//! @file CYBWin32Process.cpp Implements CYB::Platform::Process for Win32

#include "CYB.hpp"

void CYB::Platform::Process::Terminate(void) {
	Implementation::Win32::TerminateProcess(FHandle, 0);
}

CYB::Platform::Process CYB::Platform::Process::GetSelf(void) {
	return Process(Implementation::Win32::GetCurrentProcess());
}

CYB::Platform::Implementation::Process::Process(CYB::Platform::Implementation::Win32::HANDLE AHandle) :
	FHandle(AHandle)
{}

CYB::Platform::Implementation::Process::Process(Process&& AMove) :
	FHandle(AMove.FHandle)
{
	AMove.FHandle = nullptr;
}

CYB::Platform::Implementation::Process& CYB::Platform::Implementation::Process::operator=(Process&& AMove) {
	FHandle = AMove.FHandle;
	AMove.FHandle = nullptr;
	return *this;
}

CYB::Platform::Implementation::Process::~Process() {
	if(FHandle != nullptr)
		Win32::CloseHandle(FHandle);
}
