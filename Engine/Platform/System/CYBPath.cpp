//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

CYB::Platform::System::Path::Path(const SystemDirectory ADirectory) :
	FPath(LocateDirectory(ADirectory))
{}