//! @file CYBFile.cpp Implements CYB::Platform::System::File shared functions
#include "CYB.hpp"

CYB::Platform::System::File::File(const API::Path& APath, const Mode AMode, const Method AMethod) :
	FPath(static_cast<const System::Path&>(APath))
{
	static_cast<void>(APath);
	static_cast<void>(AMode);
	static_cast<void>(AMethod);
	UNIMPLEMENTED;
}

CYB::Platform::System::File::File(System::Path&& APath, const Mode AMode, const Method AMethod) :
	FPath(std::move(APath))
{
	static_cast<void>(AMode);
	static_cast<void>(AMethod);
	UNIMPLEMENTED;
}

CYB::Platform::System::File::File(File&& AMove) noexcept :
	FPath(std::move(AMove.FPath))
{
	UNIMPLEMENTED;
}

unsigned long long CYB::Platform::System::File::Size(void) const noexcept {
	UNIMPLEMENTED;
	return 0;
}

unsigned long long CYB::Platform::System::File::CursorPosition(void) const noexcept {
	UNIMPLEMENTED;
	return 0;
}

void CYB::Platform::System::File::Seek(const long long AOffset, const SeekLocation ALocation) const {
	static_cast<void>(AOffset);
	static_cast<void>(ALocation);
	UNIMPLEMENTED;
}

unsigned long long CYB::Platform::System::File::Read(const void* const ABuffer, const unsigned long long AMaxAmount) const {
	static_cast<void>(ABuffer);
	static_cast<void>(AMaxAmount);
	UNIMPLEMENTED;
	return 0;
}

void CYB::Platform::System::File::Write(void* const ABuffer, const unsigned long long AAmount) {
	static_cast<void>(ABuffer);
	static_cast<void>(AAmount);
	UNIMPLEMENTED;
}

const CYB::API::Path& CYB::Platform::System::File::Path(void) const noexcept {
	return FPath;
}