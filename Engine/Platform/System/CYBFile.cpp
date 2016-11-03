//! @file CYBFile.cpp Implements CYB::Platform::System::File shared functions
#include "CYB.hpp"

CYB::Platform::System::File::File(const API::Path& APath, const Mode AMode, const Method AMethod) :
	File(System::Path(static_cast<const System::Path&>(APath)), AMode, AMethod)
{}

CYB::Platform::System::File::File(File&& AMove) noexcept:
	Implementation::File(std::move(AMove)),
	FPath(std::move(AMove.FPath))
{}

void CYB::Platform::System::File::Touch(System::Path&& APath) {
	File(std::move(APath), Mode::WRITE, Method::ANY);
}

unsigned long long CYB::Platform::System::File::CursorPosition(void) const noexcept {
	return Seek(0, SeekLocation::CURSOR);
}

const CYB::API::Path& CYB::Platform::System::File::Path(void) const noexcept {
	return FPath;
}

CYB::API::File::Method CYB::Platform::System::File::OpenMethod(void) const noexcept {
	return FOpenMethod;
}