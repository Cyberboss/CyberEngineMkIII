//! @file CYBLinuxPath.cpp Implements CYB::Platform::System::Path for Linux
#include "CYB.hpp"

using namespace CYB::Platform::Posix;
using namespace CYB::API::String;

CYB::API::String::Dynamic CYB::Platform::System::Implementation::Path::GetExecutableImage(void) {
	char ThePath[PATH_MAX];
	if (CYB::Core().FModuleManager.FC.Call<Modules::LibC::readlink>("/proc/self/exe", ThePath, PATH_MAX) > 0)
		return API::String::Dynamic(static_cast<const char*>(ThePath));
	throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
}