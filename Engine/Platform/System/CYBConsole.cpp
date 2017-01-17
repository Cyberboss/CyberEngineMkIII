//! @file CYBConsole.cpp Implements CYB::Platform::System::Console shared functions
#include "CYB.hpp"

#include <ctime>

CYB::Platform::System::Console::Console() :
	FConsolePath(u8"stdout/stderr")
{}

void CYB::Platform::System::Console::Log(const API::String::CStyle& AMessage, const Level ALevel) {
	const char* LevelString;
	bool IsError(false);
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
		IsError = true;
		Show();
		break;
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
	auto Time(time(0));   // get time now
	auto Now(localtime(&Time));
	const auto Hour(Now->tm_hour), Min(Now->tm_min), Sec(Now->tm_sec);
	char Buffer[50];
	memset(Buffer, 0, 50);	//memset instead of Buffer[Length] = 0 because coverage

	const auto Length(sprintf(Buffer, u8"[%02d:%02d:%02d]", Hour, Min, Sec));

	API::Assert::LessThan(-1, Length);
	API::Assert::LessThan(Length, 50);

	WriteOut(API::String::Static(Buffer), IsError);
	WriteOut(API::String::Static(LevelString), IsError);
	WriteOut(AMessage, IsError);
	WriteOut(API::String::Static("\n"), IsError);
}

void CYB::Platform::System::Console::Flush(void) const noexcept {}

const CYB::API::String::CStyle& CYB::Platform::System::Console::CurrentLog(void) const noexcept {
	return FConsolePath;
}