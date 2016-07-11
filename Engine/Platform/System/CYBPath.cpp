//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

CYB::Platform::System::Path::Path(const SystemPath ADirectory) :
	FPath(LocateDirectory(ADirectory))
{}

const CYB::API::String::UTF8& CYB::Platform::System::Path::operator()(void) const noexcept {
	return FPath;
}

CYB::API::String::UTF8 CYB::Platform::System::Path::GetResourceDirectory(void) {
	auto FirstPath(LocateDirectory(SystemPath::EXECUTABLE));
	FirstPath += CYB::API::String::UTF8(CYB::API::String::Static(u8"../Resources"));
	bool Throw(false);
	try {
		Evaluate(FirstPath);
	}
	catch (Exception::Internal AException) {
		API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::PATH_EVALUATION_FAILURE);
		Throw = true;
	}
	if(Throw)
		throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
	return FirstPath;
}