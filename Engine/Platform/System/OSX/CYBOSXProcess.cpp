//! @file CYBOSXProcess.cpp Implements CYB::Platform::System::Process for OSX
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Implementation::Process::HandleSpawnError[[noreturn]](void) {
	switch (errno) {
	case ENOTDIR:
	case ENOENT:
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_FOUND);
	case EISDIR:
	case ENOEXEC:
	case EINVAL:
	case EACCES:
	case ETXTBSY:
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_READABLE);
	default:
		throw CYB::Exception::Internal(CYB::Exception::Internal::PROCESS_CREATION_ERROR);
	}
}