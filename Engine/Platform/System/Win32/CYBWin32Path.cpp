//! @file CYBWin32Path.cpp Implements CYB::Platform::System::Path for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;
using namespace CYB::API::String;

CYB::API::String::UTF8 CYB::Platform::System::Path::LocateDirectory(const SystemPath ADirectory) {
	if (ADirectory != SystemPath::EXECUTABLE_IMAGE
		&& ADirectory != SystemPath::EXECUTABLE
		&& ADirectory != SystemPath::RESOURCE
		&& ADirectory != SystemPath::TEMPORARY
		&& ADirectory != SystemPath::USER
		&& ADirectory != SystemPath::WORKING)
		throw Exception::Violation(Exception::Violation::INVALID_ENUM);
	auto& MM(Core().FModuleManager);
	switch (ADirectory) {
	case SystemPath::EXECUTABLE_IMAGE:
	case SystemPath::EXECUTABLE: {
		auto Module(MM.FK32.Call<Modules::Kernel32::GetModuleHandleW>(nullptr));
		if (Module != nullptr) {
			wchar_t Buffer[MAX_PATH];
			if (MM.FK32.Call<Modules::Kernel32::GetModuleFileNameW>(Module, Buffer, DWORD(MAX_PATH)) != 0)
				if (ADirectory == SystemPath::EXECUTABLE) {
					if (MM.FShellAPI.Call<Modules::ShellAPI::PathRemoveFileSpecW>(Buffer) != 0) 
						return API::String::UTF16::ToUTF8(Buffer);
					throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
			return API::String::UTF16::ToUTF8(Buffer);
		}
		throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
	}
	case SystemPath::RESOURCE:
		return GetResourceDirectory();
	case SystemPath::TEMPORARY: {
		API::String::UTF8 Result;
		{
			wchar_t Buffer[MAX_PATH];
			if (MM.FK32.Call<Modules::Kernel32::GetTempPathW>(Win32::DWORD(MAX_PATH), Buffer) == 0)
				throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
			Result = API::String::UTF16::ToUTF8(Buffer);
		}
		Result += API::String::UTF8(API::String::Static(Engine::Parameters::FTempPathName));
		bool Throw(false);
		Exception::SystemData::ErrorCode ThrowCode(Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
		try {
			CreateDirectory(Result);
		}
		catch (Exception::SystemData AException) {
			Throw = true;
			ThrowCode = static_cast<Exception::SystemData::ErrorCode>(AException.FErrorCode);
			API::Assert::Equal<unsigned int>(ThrowCode, Exception::SystemData::FILE_NOT_WRITABLE, Exception::SystemData::HEAP_ALLOCATION_FAILURE);
			if(ThrowCode == Exception::SystemData::FILE_NOT_WRITABLE)
				ThrowCode = Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE;
		}
		if(Throw)
			throw Exception::SystemData(ThrowCode);
		return Result;
	}
	case SystemPath::WORKING:
	{
		wchar_t Buffer[MAX_PATH];
		if (MM.FK32.Call<Modules::Kernel32::GetCurrentDirectoryW>(Win32::DWORD(MAX_PATH), Buffer) == 0)
			throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
		return API::String::UTF16::ToUTF8(Buffer);
	}
	case SystemPath::USER:
	{
		struct AutoFreeBuffer {
		private:
			wchar_t* const FBuffer;
		public:
			AutoFreeBuffer(wchar_t* const ABuffer) :
				FBuffer(ABuffer)
			{}
			~AutoFreeBuffer() {
				Core().FModuleManager.FOLE.Call<Modules::Ole32::CoTaskMemFree>(FBuffer);
			}
		};
		wchar_t* Buffer;
		//https://msdn.microsoft.com/en-us/library/windows/desktop/dd378457(v=vs.85).aspx
		//GUID for roaming app data
		if (MM.FShell.Call<Modules::Shell::SHGetKnownFolderPath>(GUID{ 0xF1B32785, 0x6FBA, 0x4FCF, 0x9D, 0x55, 0x7B, 0x8E, 0x7F, 0x15, 0x70, 0x91 }, DWORD(0), nullptr, &Buffer) != S_OK)
			throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);

		AutoFreeBuffer AFB(Buffer);
		auto Result(API::String::UTF16::ToUTF8(Buffer));
		return Result;
	}
	default:
		__assume(false);	//HCF is too indirect for coverage
	}
}

void CYB::Platform::System::Path::CreateDirectory(const API::String::UTF8& APath) {
	API::String::UTF16 As16(APath);
	if (Core().FModuleManager.FK32.Call<Modules::Kernel32::CreateDirectoryW>(As16.WString(), nullptr) == 0
		&& Core().FModuleManager.FK32.Call<Modules::Kernel32::GetLastError>() != ERROR_ALREADY_EXISTS)
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
}

void CYB::Platform::System::Path::DeleteDirectory(const API::String::UTF8& APath) {
	API::String::UTF16 As16(APath);
	if (Core().FModuleManager.FK32.Call<Modules::Kernel32::RemoveDirectoryW>(As16.WString()) == FALSE) {
		const auto Error(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetLastError>());
		switch (Error) {
		case ERROR_FILE_NOT_FOUND:
			break;	//contract fufilled
		case ERROR_DIR_NOT_EMPTY:
			throw Exception::SystemData(Exception::SystemData::DIRECTORY_NOT_EMPTY);
		default:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
}


void CYB::Platform::System::Path::DeleteFile(const API::String::UTF8& APath) {
	API::String::UTF16 As16(APath);
	auto& K32(Core().FModuleManager.FK32);
	if (K32.Call<Modules::Kernel32::DeleteFileW>(As16.WString()) == FALSE) {
		const auto Error(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetLastError>());
		switch (Error) {
		case ERROR_FILE_NOT_FOUND:
			break;	//contract fufilled
		case ERROR_ACCESS_DENIED: {
			//try once more with no RO attribute
			auto Attributes(K32.Call<Modules::Kernel32::GetFileAttributesW>(As16.WString()));
			if (Attributes != INVALID_FILE_ATTRIBUTES) {
				Attributes &= ~FILE_ATTRIBUTE_READONLY;
				if (K32.Call<Modules::Kernel32::SetFileAttributesW>(As16.WString(), Attributes) != FALSE && K32.Call<Modules::Kernel32::DeleteFileW>(As16.WString()) != FALSE)
					break;
			}
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
		default:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
}

void CYB::Platform::System::Path::Evaluate(API::String::UTF8& APath) {
	API::String::UTF16 As16(APath);
	wchar_t OutputBuffer[MAX_PATH];
	if (Core().FModuleManager.FShellAPI.Call<Modules::ShellAPI::PathCanonicalizeW>(OutputBuffer, As16.WString()) == FALSE)
		throw Exception::Internal(Exception::Internal::PATH_EVALUATION_FAILURE);
	APath = API::String::UTF16::ToUTF8(OutputBuffer);
}

bool CYB::Platform::System::Path::Verify(const API::String::UTF8& APath) const {
	API::String::UTF16 As16;
	const bool Local(&APath == &FPath);
	if (!Local)
		As16 = API::String::UTF16(APath);
	const API::String::UTF16& Ref(Local ? FWidePath : As16);

	return Core().FModuleManager.FShellAPI.Call<Modules::ShellAPI::PathFileExistsW>(Ref.WString()) == TRUE;
}

void CYB::Platform::System::Path::SetPath(API::String::UTF8&& APath) {
	FWidePath = API::String::UTF16(APath);
	FPath = std::move(APath);
}

bool CYB::Platform::System::Path::IsDirectory(void) const {
	const auto Attributes(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetFileAttributesW>(FWidePath.WString()));
	if (Attributes == INVALID_FILE_ATTRIBUTES) {
		const auto Error(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetLastError>());
		switch (Error) {
		case ERROR_FILE_NOT_FOUND:
			throw Exception::SystemData(Exception::SystemData::PATH_LOST);
		default:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
		}
	}
	return (Attributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
}

void CYB::Platform::System::Path::NavigateToParentDirectory(void) {
	//docs say it has to be MAX_PATH
	//examples say otherwise, but this function is deprecated so lets humor them
	wchar_t Buffer[MAX_PATH];
	API::Assert::Equal(FWidePath.RawLength() % 2, 0);
	std::copy(FWidePath.WString(), FWidePath.WString() + FWidePath.RawLength(), Buffer);

	//This won't work with '/' chars so convert them here
	//Only the last one though because that's all we need
	for (auto I(FWidePath.RawLength() / 2); I >= 0 ; --I)
		if (Buffer[I] == L'/') {
			Buffer[I] = L'\\';
			break;
		}
	auto& SAPI(Core().FModuleManager.FShellAPI);
	const auto Result1(SAPI.Call<Modules::ShellAPI::PathIsRootW>(Buffer));
	if (Result1 != FALSE)
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);

	const auto Result2(SAPI.Call<Modules::ShellAPI::PathRemoveFileSpecW>(Buffer));
	if (Result2 == 0)
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);	//this doesn't seem to happen irl but whatever
	SetPath(UTF16::ToUTF8(Buffer));
}

CYB::API::String::UTF8 CYB::Platform::System::Path::FullName(void) const {
	const auto Result(Core().FModuleManager.FShellAPI.Call<Modules::ShellAPI::PathFindFileNameW>(const_cast<wchar_t*>(FWidePath.WString())));
	if (*Result == 0)
		throw Exception::SystemData(Exception::SystemData::STRING_VALIDATION_FAILURE);
	return UTF16::ToUTF8(Result);
}

CYB::API::String::UTF8 CYB::Platform::System::Path::Name(void) const {
	const auto File(Core().FModuleManager.FShellAPI.Call<Modules::ShellAPI::PathFindFileNameW>(const_cast<wchar_t*>(FWidePath.WString()))),
		Extension(Core().FModuleManager.FShellAPI.Call<Modules::ShellAPI::PathFindExtensionW>(const_cast<wchar_t*>(FWidePath.WString())));
	if (*File == 0)
		throw Exception::SystemData(Exception::SystemData::STRING_VALIDATION_FAILURE);
	if (*Extension == 0)
		return UTF16::ToUTF8(File);

	wchar_t Buffer[MAX_PATH];
	std::memset(Buffer, 0, sizeof(Buffer));
	std::copy(File, Extension, Buffer);

	return UTF16::ToUTF8(Buffer);
}


//DirectoryEntry

CYB::Platform::System::Implementation::Path::DirectoryEntry::DirectoryEntry(const System::Path& APath) :
	FOriginalPath(APath),
	FPathListing(nullptr)
{
	WIN32_FIND_DATA FindData;
	{
		UTF16 Query(UTF8(FOriginalPath() + API::Path::DirectorySeparatorChar() + Static(u8"*")));
		FFindHandle = Core().FModuleManager.FK32.Call<Modules::Kernel32::FindFirstFileW>(Query.WString(), &FindData);
	}
	if (FFindHandle == INVALID_HANDLE_VALUE) {
		const auto Error(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetLastError>());
		switch (Error) {
		case ERROR_FILE_NOT_FOUND:
			break;	//can really only happen if a drive is enumed and is empty
		case ERROR_ACCESS_DENIED:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
		default:
			throw Exception::SystemData(Exception::SystemData::PATH_LOST);
		}
	}
	else
		AssignOrRecurse(std::move(FindData));
}

CYB::Platform::System::Implementation::Path::DirectoryEntry::~DirectoryEntry() {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::FindClose>(FFindHandle);
}

void CYB::Platform::System::Implementation::Path::DirectoryEntry::operator++(void) {
	WIN32_FIND_DATA FindData;
	if (Core().FModuleManager.FK32.Call<Modules::Kernel32::FindNextFileW>(FFindHandle, &FindData) == 0)
		FPathListing = API::Interop::Object<API::Path>(nullptr);
	else
		AssignOrRecurse(std::move(FindData));
}

void CYB::Platform::System::Implementation::Path::DirectoryEntry::AssignOrRecurse(WIN32_FIND_DATA&& AFindData) {
	UTF8 Conversion;
	{
		auto FindData(std::move(AFindData));
		Conversion = UTF16::ToUTF8(FindData.cFileName);
		if (Conversion == Static(u8".") || Conversion == Static(u8"..")) {
			operator++();
			return;
		}
	}
	FPathListing = API::Interop::Object<System::Path>::Upcast<API::Path>(API::Context().FAllocator.NewObject<System::Path, System::Path::InternalConstructor>(FOriginalPath() + API::Path::DirectorySeparatorChar() + Conversion));
}

const UTF16& CYB::Platform::System::Implementation::Path::WidePath(void) const noexcept {
	return FWidePath;
}