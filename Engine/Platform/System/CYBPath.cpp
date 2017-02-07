//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

using namespace CYB::API::String;

int CYB::Platform::System::Path::GetIndexOfLastSeperator(const API::String::UTF8& AString, const char ASeparator) noexcept {
	for (auto I(AString.RawLength() - 1); I >= 0; --I)
		if (AString.CString()[I] == ASeparator)
			return I;
	return -1;
}

CYB::Platform::System::Path::Path(API::String::UTF8&& APath) {
	SetPath(std::move(APath));
}

CYB::Platform::System::Path::Path(const SystemPath ADirectory) :
	Path(LocateDirectory(ADirectory))
{}

void CYB::Platform::System::Path::CreateDirectories(UTF8&& ABasePath, const API::Container::Deque<UTF8>& APaths) {
	UTF8 CurrentPath(std::move(ABasePath));
	for (auto& PathAddition : APaths) {
		CurrentPath += DirectorySeparatorChar();
		CurrentPath += PathAddition;
		CreateDirectory(CurrentPath);
	}
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

void CYB::Platform::System::Path::Append(const API::String::UTF8& AAppendage, const bool ACreateIfNonExistant, const bool ACreateRecursive) {
	//first ensure we aren't breaking the rules
	if (FPath.RawLength() + AAppendage.RawLength() + 1 > MAX_PATH_BYTES)
		throw Exception::SystemData(Exception::SystemData::PATH_TOO_LONG);

	//verify, file system could have changed and such
	if (!Verify(FPath))
		throw Exception::SystemData(Exception::SystemData::PATH_LOST);

	auto NewPath(FPath + DirectorySeparatorChar() + AAppendage);
	UTF8 FinalComponent;
	if (!ACreateIfNonExistant) {
		//try a simple cd
		if (!Verify(NewPath)) {
			//Okay, we may be trying to create a new file, check it's parent directory
			const auto I(GetIndexOfLastSeperator(NewPath, *DirectorySeparatorChar()));
			API::Assert::LessThan(0, I);
			FinalComponent = UTF8(static_cast<const Dynamic&>(NewPath).SubString(I, NewPath.RawLength() - I));
			NewPath = UTF8(static_cast<const Dynamic&>(NewPath).SubString(0, I));
			if (!Verify(NewPath))
				throw Exception::SystemData(Exception::SystemData::STREAM_NOT_READABLE);
		}
	}
	else {
		UTF8 WorkingPath;
		//create tokens of directories to create
		API::Container::Deque<UTF8> Tokens;
		if (ACreateRecursive) {
			//! @todo Change this to UTF-8 tokenize, once implemented
			//Tokens = AAppendage.Tokenize(DirectorySeparatorChar());
			auto DynTokens(AAppendage.Tokenize(*DirectorySeparatorChar()));
			for (auto& Tok : DynTokens)
				Tokens.emplace_back(std::move(Tok));
			WorkingPath = UTF8(FPath);
		}
		else {
			const auto I(GetIndexOfLastSeperator(AAppendage, *DirectorySeparatorChar()));
			if (I == -1) {
				Tokens.emplace_back(UTF8(AAppendage));
				WorkingPath = UTF8(FPath);
			}
			else {
				Tokens.emplace_back(UTF8(static_cast<const Dynamic&>(AAppendage).SubString(I + 1, AAppendage.RawLength() - I - 1)));
				WorkingPath = UTF8(static_cast<const Dynamic&>(AAppendage).SubString(0, AAppendage.RawLength()));
				if (!Verify(WorkingPath))
					throw Exception::SystemData(Exception::SystemData::STREAM_NOT_READABLE);
			}
		}
		const Static Ascender(u8"..");
		for (auto& Tok : Tokens)
			if (Tok == Ascender)
				throw Exception::SystemData(Exception::SystemData::STREAM_NOT_READABLE);

		//and try to create them
		CreateDirectories(std::move(WorkingPath), Tokens);
	}
	bool Throw(false);
	try {
		Evaluate(NewPath);
	}
	catch (Exception::Internal AException) {
		API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::PATH_EVALUATION_FAILURE);
		Throw = true;
	}
	if (Throw)
		throw Exception::SystemData(Exception::SystemData::STREAM_NOT_READABLE);
	NewPath += std::move(FinalComponent);
	SetPath(std::move(NewPath));
}

void CYB::Platform::System::Path::Delete(bool ARecursive) {
	bool Throw(false);
	Exception::SystemData::ErrorCode ThrowCode(Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
	try {
		if (!IsDirectory())
			DeleteFile(FPath);
		else if (Verify(FPath)) {
			if (ARecursive) {
				auto Entry(Contents());
				for (auto& It(Entry()); It->Valid(); ++It)
					(*It)().Delete(true);	//recursion may be a bad idea here, but I'd like to see someone even TRY and overflow this
				ARecursive = false;
			}
			DeleteDirectory(FPath);
		}
	}
	catch (Exception::SystemData AException) {
		API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::SystemData::HEAP_ALLOCATION_FAILURE, Exception::SystemData::DIRECTORY_NOT_EMPTY, Exception::SystemData::STREAM_NOT_WRITABLE, Exception::SystemData::PATH_LOST, Exception::SystemData::STRING_VALIDATION_FAILURE);
		Throw = true;
		ThrowCode = static_cast<Exception::SystemData::ErrorCode>(AException.FErrorCode);
		if (!ARecursive && ThrowCode == CYB::Exception::SystemData::PATH_LOST)
			Throw = false;
	}
	if (Throw)
		throw Exception::SystemData(ThrowCode);
}

bool CYB::Platform::System::Path::IsFile(void) const {
	return !IsDirectory();
}

int CYB::Platform::System::Path::ByteLength(void) const noexcept {
	return FPath.RawLength();
}

CYB::API::String::UTF8 CYB::Platform::System::Path::Extension(void) const {
	auto Full(FullName());
	const auto Dot(GetIndexOfLastSeperator(Full, '.') + 1);
	if (Dot == 0)
		return Full;
	return UTF8(static_cast<const Dynamic&>(Full).SubString(Dot, Full.RawLength() - Dot));
}

CYB::API::Interop::Object<CYB::API::Path::DirectoryEntry> CYB::Platform::System::Path::Contents(void) const {
	return API::Interop::Object<Implementation::Path::DirectoryEntry>::Upcast<API::Path::DirectoryEntry>(API::Context().FAllocator.ConstructObject<Implementation::Path::DirectoryEntry, API::Interop::NullConstructor>(*this));
}

CYB::API::Interop::Object<CYB::API::Path>& CYB::Platform::System::Implementation::Path::DirectoryEntry::operator*(void) noexcept {
	return FPathListing;
}

CYB::API::Interop::Object<CYB::API::Path>* CYB::Platform::System::Implementation::Path::DirectoryEntry::operator->(void) noexcept {
	return &FPathListing;
}