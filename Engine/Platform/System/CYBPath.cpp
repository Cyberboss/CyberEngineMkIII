//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

CYB::Platform::System::Path::Path(const SystemPath ADirectory) :
	FPath(LocateDirectory(ADirectory))
{}