//! @file CYBLogger.cpp Implements CYB::Engine::Logger
#include "CYB.hpp"

const CYB::API::Path& CYB::Engine::Logger::CurrentLog(void) const noexcept {
	return FFile.GetPath();
}