//! @file CYBConsole.cpp Implements CYB::Platform::System::Console shared functions
#include "CYB.hpp"

CYB::Platform::System::Console::Console() :
	FConsolePath(u8"stdout")
{}

void CYB::Platform::System::Console::Log(const API::String::CStyle& AMessage, const Level ALevel) noexcept {
	static_cast<void>(AMessage);
	static_cast<void>(ALevel);
}

const CYB::API::String::CStyle& CYB::Platform::System::Console::CurrentLog(void) const noexcept {
	return FConsolePath;
}