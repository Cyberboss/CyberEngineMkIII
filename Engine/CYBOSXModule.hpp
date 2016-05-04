//! @file CYBOSXModule.cpp Defines CYB::Platform::Implementation::Module::Suffix (Posix) for OSX
#pragma once
constexpr const char* CYB::Platform::Implementation::Module::Suffix(void) {
	return u8".dylib";
}