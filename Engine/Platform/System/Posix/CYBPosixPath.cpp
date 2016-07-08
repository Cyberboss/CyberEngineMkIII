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
			API::String::Dynamic Work(AsStatic);
			if (ADirectory == SystemPath::EXECUTABLE) {
				auto Last(0);
				for(auto I(0U); I < Work.RawLength(); ++I)
					if (Work.CString()[I] == '/')
						Last = I;
				Work.Shrink(Last + 1);
			}
			return Work;
		}
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);}
	case SystemPath::RESOURCE:
		return GetResourceDirectory();
	case SystemPath::TEMPORARY: {
		API::String::UTF8 Result(API::String::Static(u8"/tmp/"));
		Result += API::String::UTF8(API::String::Static(Engine::Parameters::FTempPathName));
		if(!CreateDirectory(Result))
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
		return Result;
	}
	case SystemPath::USER:
	case SystemPath::WORKING:
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
}

bool CYB::Platform::System::Path::CreateDirectory(const API::String::UTF8& APath) {
	const auto Result(Core().FModuleManager.FC.Call<Modules::LibC::mkdir>(APath.CString(), 0777));
	if (Result != 0) 
		switch (errno) {
		case EEXIST:
			return true;
		case ENOTDIR:
			throw Exception::SystemData(Exception::SystemData::PATH_LOST);
		default:
			return false;
		}
	return true;
}

void CYB::Platform::System::Path::Evaluate(API::String::UTF8& APath) {
	//In order to follow the policy of not evaluating symlinks we simply readlink the '.' operator
	auto NewPath(APath + CYB::API::String::UTF8(CYB::API::String::Static(u8"/.")));
	char ThePath[PATH_MAX];
	auto const Result(Core().FModuleManager.FC.Call<Modules::LibC::realpath>(NewPath.CString(), ThePath));
	if (Result != ThePath)
		throw Exception::Internal(Exception::Internal::PATH_EVALUATION_FAILURE);
	APath = CYB::API::String::UTF8(CYB::API::String::Static(ThePath));
}

bool CYB::Platform::System::Path::Verify(const API::String::UTF8& APath) {
	StatStruct ST;
	return Sys::Call(Sys::STAT, const_cast<char*>(APath.CString()), &ST) == 0;
}