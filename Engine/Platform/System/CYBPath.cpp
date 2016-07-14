//! @file CYBPath.cpp Implements CYB::Platform::System::Path shared functions
#include "CYB.hpp"

using namespace CYB::API::String;

CYB::Platform::System::Path::Path(API::String::UTF8&& APath) {
	SetPath(std::move(APath));
}

CYB::Platform::System::Path::Path(const SystemPath ADirectory) :
	Path(LocateDirectory(ADirectory))
{}

void CYB::Platform::System::Path::CreateDirectories(const UTF8& ABasePath, const API::Container::Deque<UTF8>& APaths) {
	auto CurrentPath(ABasePath);
	class AutoCleanup {
	public:
		API::Container::Stack<UTF8> FAddedPaths;
	public:
		~AutoCleanup() {
			try {
				while (!FAddedPaths.empty()) {
					DeleteDirectory(FAddedPaths.top());
					FAddedPaths.pop();
				}
			}
			catch (CYB::Exception::Base) {} // :-<
		}
	};
	AutoCleanup Manager;

	for (auto& PathAddition : APaths) {
		CurrentPath += DirectorySeparatorChar();
		CurrentPath += PathAddition;
		CreateDirectory(CurrentPath);
		if (&PathAddition != &APaths.back())
			Manager.FAddedPaths.push(CurrentPath);
	}

	Manager.FAddedPaths = API::Container::Stack<UTF8>();
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
	//first ensure we aren't breaking the rules
	if (FPath.RawLength() + AAppendage.RawLength() + 1 > MAX_PATH_BYTES)
		throw Exception::SystemData(Exception::SystemData::PATH_TOO_LONG);

	//verify, file system could have changed and such
	if (!Verify(FPath))
		throw Exception::SystemData(Exception::SystemData::PATH_LOST);

	if (!ACreateIfNonExistant) {
		//try a simple cd
		auto NewPath(FPath + DirectorySeparatorChar() + AAppendage);
		if (Verify(NewPath)) {
			try {
				Evaluate(NewPath);
			}
			catch (Exception::Internal AException) {
				API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::PATH_EVALUATION_FAILURE);
				return false;
			}
			SetPath(std::move(NewPath));
			return true;
		}
		else {
			//Okay, we may be trying to create a new file, check it's parent directory
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
		UTF8 WorkingPath, FirstNewDir;
		{
			//create tokens of directories to create
			API::Container::Deque<UTF8> Tokens;
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
				else if (!Verify(WorkingPath)) {
					return false;
				}
			}
			const Static Ascender(u8"..");
			for (auto& Tok : Tokens)
				if (Tok == Ascender)
					return false;

			//and try to create them
			CreateDirectories(WorkingPath, Tokens);
			FirstNewDir = std::move(Tokens.front());
		}
		//If you enter this block, things MUST get cleaned up after ANY error
		bool MustThrowOOM(false);
		const auto GrimCleanup([&]() {
			try {
				Path Working(*this);
				if (Working.Append(WorkingPath + DirectorySeparatorChar() + FirstNewDir, false, false))
					Delete(true);
			}
			catch (CYB::Exception::SystemData AException2) {
				MustThrowOOM = AException2.FErrorCode == Exception::SystemData::HEAP_ALLOCATION_FAILURE;
			}	//grumble grumble
		});
		try {
			auto NewPath(FPath + DirectorySeparatorChar() + AAppendage);
			Evaluate(NewPath);
			SetPath(std::move(NewPath));
			return true;
		}
		catch (Exception::Internal AException) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::PATH_EVALUATION_FAILURE);
			GrimCleanup();
		}
		catch (Exception::SystemData AException) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::SystemData::HEAP_ALLOCATION_FAILURE, Exception::SystemData::STRING_VALIDATION_FAILURE);
			MustThrowOOM = AException.FErrorCode == Exception::SystemData::HEAP_ALLOCATION_FAILURE;
			if(!MustThrowOOM)
				GrimCleanup();
		}
		if (MustThrowOOM)
			throw Exception::SystemData(Exception::SystemData::HEAP_ALLOCATION_FAILURE);
	}
	return false;
}

void CYB::Platform::System::Path::Delete(bool ARecursive) const {
	if (Verify(FPath)) {
		bool Throw(false);
		Exception::SystemData::ErrorCode ThrowCode(Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
		try {
			if (!IsDirectory())
				DeleteFile(FPath);
			else {
				if (ARecursive) {
					for (auto Entry(Contents()); Entry()->Valid(); ++Entry())
						(*Entry())().Delete(true);	//recursion may be a bad idea here, but I'd like to see someone even TRY and overflow this
					ARecursive = false;
				}
				DeleteDirectory(FPath);
			}
		}
		catch (Exception::SystemData AException) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::SystemData::HEAP_ALLOCATION_FAILURE, Exception::SystemData::DIRECTORY_NOT_EMPTY, Exception::SystemData::FILE_NOT_WRITABLE, Exception::SystemData::PATH_LOST, Exception::SystemData::STRING_VALIDATION_FAILURE);
			Throw = ARecursive || (AException.FErrorCode != Exception::SystemData::PATH_LOST);
			ThrowCode = static_cast<Exception::SystemData::ErrorCode>(AException.FErrorCode);
		}
		if (Throw)
			throw Exception::SystemData(ThrowCode);
	}
}

bool CYB::Platform::System::Path::IsFile(void) const {
	return !IsDirectory();
}

int CYB::Platform::System::Path::ByteLength(void) const noexcept {
	return FPath.RawLength();
}

CYB::API::String::UTF8 CYB::Platform::System::Path::FullFileName(void) const {
	return UTF8();
}

CYB::API::String::UTF8 CYB::Platform::System::Path::FileName(void) const {
	return UTF8();
}

CYB::API::String::UTF8 CYB::Platform::System::Path::Extension(void) const {
	return UTF8();
}

CYB::API::Interop::Object<CYB::API::Path::DirectoryEntry> CYB::Platform::System::Path::Contents(void) const {
	return API::Interop::Object<Implementation::Path::DirectoryEntry>::Upcast<API::Path::DirectoryEntry>(
		API::Allocator().NewObject<Implementation::Path::DirectoryEntry>(*this)
		);
}

CYB::API::Interop::Object<CYB::API::Path>& CYB::Platform::System::Implementation::Path::DirectoryEntry::operator*(void) noexcept {
	return FPathListing;
}

CYB::API::Interop::Object<CYB::API::Path>* CYB::Platform::System::Implementation::Path::DirectoryEntry::operator->(void) noexcept {
	return &FPathListing;
}