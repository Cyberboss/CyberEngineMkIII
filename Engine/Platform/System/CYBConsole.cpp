//! @file CYBConsole.cpp Implements CYB::Platform::System::Console shared functions
#include "CYB.hpp"

CYB::Platform::System::Console::Console() :
	FConsolePath(u8"stdout")
{}

void CYB::Platform::System::Console::Log(const API::String::CStyle& AMessage, const Level ALevel) {
	static_cast<void>(AMessage);

	char* LevelString;
	switch (ALevel) {
	case Level::DEV:
		LevelString = ": Debug: ";
		break;
	case Level::INFO:
		LevelString = ": Info: ";
		break;
	case Level::WARN:
		LevelString = ": Warning: ";
		break;
	case Level::ERR:
		LevelString = ": ERROR: ";
		break;
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
	static_cast<void>(LevelString);
}

void CYB::Platform::System::Console::Flush(void) const noexcept {}

const CYB::API::String::CStyle& CYB::Platform::System::Console::CurrentLog(void) const noexcept {
	return FConsolePath;
}