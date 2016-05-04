//! @file CYBLinuxModule.cpp Defines CYB::Platform::Implementation::Module::Suffix (Posix) for Linux
#pragma once
constexpr const char* CYB::Platform::Implementation::Module::Suffix(void) {
	return u8".so";
}