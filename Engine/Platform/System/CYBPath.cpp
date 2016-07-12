//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

using namespace CYB::API::String;

CYB::Platform::System::Path::Path(const SystemPath ADirectory) {
	bool Throw(false);
	try {
		SetPath(LocateDirectory(ADirectory));
	}
	catch (Exception::Internal AException) {
		API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
		Throw = true;
	}
	if (Throw)
		throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
}

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
	static_cast<void>(ACreateRecursive);
	try {
		//first ensure we aren't breaking the rules
		if (FPath.RawLength() + AAppendage.RawLength() + 1 > MAX_PATH_BYTES)
			throw Exception::SystemData(Exception::SystemData::PATH_TOO_LONG);

		//verify, file system could have changed and such
		if (!Verify(FPath))
			throw Exception::SystemData(Exception::SystemData::PATH_LOST);

		if (!ACreateIfNonExistant) {
			//try a simple cd
			auto NewPath(FPath + DirectorySeparatorChar() + AAppendage);
			try {
				Evaluate(NewPath);
			}
			catch (Exception::Internal AException) {
				API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::PATH_EVALUATION_FAILURE);
				return false;
			}
			if (Verify(NewPath)) {
				SetPath(std::move(NewPath));
				return true;
			}
			else {
				//Okay, we may be trying to create a new file
				UTF8 Work;
				auto I(NewPath.RawLength() - 1);
				for (; I > 0; --I)
					if (NewPath.CString()[I] == '/') {
						Work = UTF8(static_cast<const Dynamic&>(NewPath).SubString(0, I));
						break;
					}
				API::Assert::LessThan(0, I);
				if (Verify(Work)) {
					SetPath(std::move(NewPath));
					return true;
				}
			}
		}
		else {
			//create tokens of directories to create
			API::Container::Deque<UTF8> Tokens;
			UTF8 WorkingPath;
			if (ACreateRecursive) {
				//TODO: Change this to UTF-8 tokenize, once implemented
				//Tokens = AAppendage.Tokenize(DirectorySeparatorChar());
				auto DynTokens(AAppendage.Tokenize('/'));
				for (auto& Tok : DynTokens)
					Tokens.emplace_back(std::move(Tok));
				WorkingPath = UTF8(FPath);
			}
			else {
				auto I(AAppendage.RawLength() - 1);
				for (; I > 0; --I)
					if (AAppendage.CString()[I] == '/') {
						Tokens.emplace_back(UTF8(static_cast<const Dynamic&>(AAppendage).SubString(I + 1, AAppendage.RawLength() - I - 1)));
						WorkingPath = UTF8(static_cast<const Dynamic&>(AAppendage).SubString(0, AAppendage.RawLength()));
						break;
					}
				if (I == 0) {
					Tokens.emplace_back(UTF8(AAppendage));
					WorkingPath = UTF8(FPath);
				}
				else if(!Verify(WorkingPath)) {
					return false;
				}
			}
			//and try to create them
			if (TryCreateDirectories(WorkingPath, Tokens)) {
				try {
					SetPath(FPath + DirectorySeparatorChar() + AAppendage);
					return true;
				}
				catch (Exception::Internal AException) {
					API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
					try {
						Path Working(*this);
						Working.Append(WorkingPath + DirectorySeparatorChar() + Tokens.front(), false, false);
						Delete(true);
					}
					catch (CYB::Exception::Base) {}	//grumble grumble
				}
			}
			else
				throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
	catch (Exception::Internal AException) {
		API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
	}
	return false;
}

void CYB::Platform::System::Path::Delete(const bool ARecursive) const {
	static_cast<void>(ARecursive);
}

CYB::API::String::Static CYB::Platform::System::Path::DirectorySeparatorChar(void) noexcept {
	return Static(u8"/");
}