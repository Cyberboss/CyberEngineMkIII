//! @file CYBPosixPath.cpp Implements CYB::Platform::System::Path for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::API::String::UTF8 CYB::Platform::System::Path::LocateDirectory(const SystemPath ADirectory) {
	auto& MM(Core().FModuleManager);
	switch (ADirectory) {
	case SystemPath::EXECUTABLE_IMAGE:
	case SystemPath::EXECUTABLE: {
		char ThePath[PATH_MAX];
		if (MM.FC.Call<Modules::LibC::readlink>("/proc/self/exe", ThePath, PATH_MAX) > 0) {
			API::String::Static AsStatic(ThePath);
			API::String::UTF8 Work(AsStatic);
			if (ADirectory == SystemPath::EXECUTABLE) {
				auto Last(0);
				for(auto I(0U); I < Work.RawLength(); ++I)
					if (Work.CString()[I] == '/')
						Last = I;
				Work.Shrink(Last + 1);
			}
			return Work;
		}
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
	}
	case SystemPath::RESOURCE:
	case SystemPath::TEMPORARY:
	case SystemPath::USER:
	case SystemPath::WORKING:
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
}