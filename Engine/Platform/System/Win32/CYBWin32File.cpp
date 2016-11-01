//! @file CYBWin32File.cpp Implements CYB::Platform::System::File for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

CYB::Platform::System::File::File(const API::Path& APath, const Mode AMode, const Method AMethod) :
	FPath(static_cast<const System::Path&>(APath))
{
	static_cast<void>(APath);
	static_cast<void>(AMode);
	static_cast<void>(AMethod);
	UNIMPLEMENTED;
}