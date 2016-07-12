//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

using namespace CYB::API::String;

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

bool CYB::Platform::System::Path::Append(const API::String::UTF8& AAppendage, const bool ACreateIfNonExistant, const bool ACreateRecursive) {
	static_cast<void>(AAppendage);
	static_cast<void>(ACreateIfNonExistant);
	static_cast<void>(ACreateRecursive);

	//first ensure we aren't breaking the rules
	if (FPath.RawLength() + AAppendage.RawLength() + 1 > MAX_PATH_BYTES)
		throw Exception::SystemData(Exception::SystemData::PATH_TOO_LONG);

	//verify, file system could have changed and such
	if (!Verify(FPath))
		throw Exception::SystemData(Exception::SystemData::PATH_LOST);

	if (!ACreateIfNonExistant) {
		//try a simple cd
		auto NewPath(FPath + UTF8(Static(u8"/")) + AAppendage);
		try {
			Evaluate(NewPath);
		}
		catch (Exception::Internal AException) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::PATH_EVALUATION_FAILURE);
			return false;
		}
		if (Verify(NewPath)) {
			FPath = std::move(NewPath);
			return true;
		}
		else {
			//Okay, we may be trying to create a new file
			UTF8 Work;
			auto I(NewPath.RawLength() - 1);
			for (; I >= 0; --I)
				if (NewPath.CString()[I] == '/') {
					Work = UTF8(static_cast<const Dynamic&>(NewPath).SubString(I + 1, NewPath.RawLength() - 1 - I));
					break;
				}
			API::Assert::LessThan(-1, I);
			if(Verify(Work)){
				FPath = std::move(NewPath);
				return true;
			}
		}
	}

	return false;
}

void CYB::Platform::System::Path::Delete(const bool ARecursive) const {
	static_cast<void>(ARecursive);
}