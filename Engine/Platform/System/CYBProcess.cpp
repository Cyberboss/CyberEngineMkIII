//! @file CYBProcess.cpp Shared function implementations for CYB::Platform::Process
#include "CYB.hpp"

void CYB::Platform::System::Process::Terminate(Process&& AProcess) noexcept {
	AProcess.Terminate();
}

CYB::Platform::System::Process::Process(const API::String::UTF8& ACommandLine) :
	Process(Path(Path::SystemPath::EXECUTABLE_IMAGE), ACommandLine)
{}

bool CYB::Platform::System::Process::operator!=(const Process& ARHS) const noexcept {
	return !(*this == ARHS);
}