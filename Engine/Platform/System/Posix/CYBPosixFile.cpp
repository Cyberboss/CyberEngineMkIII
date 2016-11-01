//! @file CYBPosixFile.cpp Implements CYB::Platform::System::File for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::Platform::System::File::File(System::Path&& APath, const Mode AMode, const Method AMethod) :
	FPath(std::move(APath))
{
	static_cast<void>(AMode);
	static_cast<void>(AMethod);
	UNIMPLEMENTED;
}

CYB::Platform::System::Implementation::File::File(File&& AMove) noexcept {
	static_cast<void>(AMove);
	UNIMPLEMENTED;
}

CYB::Platform::System::File& CYB::Platform::System::File::operator=(File&& AMove) noexcept {
	static_cast<void>(AMove);
	UNIMPLEMENTED;
	return *this;
}

CYB::Platform::System::File::~File() {
	UNIMPLEMENTED;
}