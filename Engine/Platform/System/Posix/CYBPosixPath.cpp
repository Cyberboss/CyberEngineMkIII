//! @file CYBPosixPath.cpp Implements CYB::Platform::System::Path for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::API::String::UTF8 CYB::Platform::System::Path::LocateDirectory(const SystemDirectory ADirectory) {
	auto& MM(Core().FModuleManager);
	switch (ADirectory) {
	case SystemDirectory::EXECUTABLE: {
		char ThePath[PATH_MAX];
		if (MM.FC.Call<Modules::LibC::readlink>("/proc/self/exe", ThePath, PATH_MAX) > 0) {
			API::String::Static AsStatic(ThePath);
			API::String::UTF8 Work(AsStatic);
			auto Last(0);
			Work.IterateCodepoints(
			[&](const unsigned int ACodepoint, const int AIndex) {
				if (ACodepoint == '/')
					Last = AIndex;
				return true;
			}
			);
			Work.Shrink(Last + 1);
			return Work;
		}
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
	}
	case SystemDirectory::RESOURCE:
	case SystemDirectory::TEMPORARY:
	case SystemDirectory::USER:
	case SystemDirectory::WORKING:
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
}