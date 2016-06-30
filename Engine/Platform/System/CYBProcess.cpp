//! @file CYBProcess.cpp Shared function implementations for CYB::Platform::Process
#include "CYB.hpp"

CYB::Platform::System::Process::Process(const Path& APath, const API::String::UTF8& ACommandLine) :
	Implementation::Process(APath, ACommandLine)
{}

CYB::Platform::System::Process::Process(const API::String::UTF8& ACommandLine) :
	Process(Path(Path::SystemPath::EXECUTABLE_IMAGE), ACommandLine)
{}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) noexcept {
	return Process();
}

bool CYB::Platform::System::Process::operator!=(const Process& ARHS) const noexcept {
	return !(*this == ARHS);
}