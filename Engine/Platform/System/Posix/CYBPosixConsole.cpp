//! @file CYBPosixConsole.cpp Implements CYB::Platform::System::Console's Log and Show for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Console::Show(void) noexcept { /* nothing to do here */ }

void CYB::Platform::System::Console::WriteOut(const CYB::API::String::CStyle& AMessage, const bool AError) noexcept {
	//fire and forget approach
	Core().FModuleManager.Call<Modules::LibC::write>(AError ? STDERR_FILENO : STDOUT_FILENO, AMessage.CString(), AMessage.RawLength());
}