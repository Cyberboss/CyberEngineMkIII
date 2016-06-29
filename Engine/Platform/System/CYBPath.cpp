//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

CYB::Platform::System::Path::Path(const SystemPath ADirectory) :
	FPath(LocateDirectory(ADirectory))
{}

const CYB::API::String::UTF8& CYB::Platform::System::Path::operator()(void) const noexcept {
	return FPath;
}