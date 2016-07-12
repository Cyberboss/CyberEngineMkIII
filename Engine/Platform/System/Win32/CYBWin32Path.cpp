//! @file CYBWin32Path.cpp Implements CYB::Platform::System::Path for Win32
#include "CYB.hpp"

#include "../../SystemHeaders/CYBUTF16String.inl"

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
					if (MM.FShellAPI.Call<Modules::ShellAPI::PathRemoveFileSpecW>(Buffer) != 0) {
						auto WithoutDelimiter(API::String::UTF16::ToUTF8(Buffer));
						WithoutDelimiter += API::String::UTF8(API::String::Static(u8"/"));
						return WithoutDelimiter;
					}
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
		if (!CreateDirectory(Result))
			throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
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
		__assume(false);
	}
}

bool CYB::Platform::System::Path::CreateDirectory(const API::String::UTF8& APath) {
	API::String::UTF16 As16(APath);
	if (Core().FModuleManager.FK32.Call<Modules::Kernel32::CreateDirectoryW>(As16.WString(), nullptr) != 0)
		return true;
	
	if (Core().FModuleManager.FK32.Call<Modules::Kernel32::GetLastError>() == ERROR_ALREADY_EXISTS)
		return true;

	return false;
}

bool CYB::Platform::System::Path::TryCreateDirectories(const UTF8& ABasePath, const API::Container::Deque<UTF8>& APaths) {
	static_cast<void>(ABasePath);
	static_cast<void>(APaths);
	return false;
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