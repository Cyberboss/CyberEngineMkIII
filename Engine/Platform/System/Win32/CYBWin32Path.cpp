//! @file CYBWin32Path.cpp Implements CYB::Platform::System::Path for Win32
#include "CYB.hpp"

#include "../../CYBUTF16String.inl"

using namespace CYB::Platform::Win32;

CYB::API::String::UTF8 CYB::Platform::System::Path::LocateDirectory(const SystemPath ADirectory) {
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
						WithoutDelimiter += CYB::API::String::UTF8(CYB::API::String::Static(u8"/"));
						return WithoutDelimiter;
					}
					throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
				return CYB::API::String::UTF16::ToUTF8(Buffer);
		}
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
	}
	case SystemPath::RESOURCE:
		return GetResourceDirectory();
	case SystemPath::TEMPORARY: {
		API::String::UTF8 Result;
		{
			wchar_t Buffer[MAX_PATH];
			if (MM.FK32.Call<Modules::Kernel32::GetTempPathW>(Win32::DWORD(MAX_PATH), Buffer) == 0)
				throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
			Result = API::String::UTF16::ToUTF8(Buffer);
		}
		Result += API::String::UTF8(API::String::Static(Engine::Parameters::FTempPathName));
	}
	case SystemPath::WORKING:
	{
		wchar_t Buffer[MAX_PATH];
		if (MM.FK32.Call<Modules::Kernel32::GetCurrentDirectoryW>(Win32::DWORD(MAX_PATH), Buffer) == 0)
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
		return API::String::UTF16::ToUTF8(Buffer);
	}
	case SystemPath::USER:
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
}

bool CYB::Platform::System::Path::CreateDirectory(const API::String::UTF8& APath) {
	API::String::UTF16 As16(APath);
	return Core().FModuleManager.FK32.Call<Modules::Kernel32::CreateDirectoryW>(As16.WString(), nullptr) != 0;
}

void CYB::Platform::System::Path::Evaluate(API::String::UTF8& APath) {
	API::String::UTF16 As16(APath);
	wchar_t OutputBuffer[MAX_PATH];
	if (Core().FModuleManager.FShellAPI.Call<Modules::ShellAPI::PathCanonicalizeW>(OutputBuffer, As16.WString()) == FALSE)
		throw Exception::Internal(Exception::Internal::PATH_EVALUATION_FAILURE);
	APath = API::String::UTF16::ToUTF8(OutputBuffer);
}

bool CYB::Platform::System::Path::Verify(const API::String::UTF8& APath) {
	return Core().FModuleManager.FShellAPI.Call<Modules::ShellAPI::PathFileExistsW>(API::String::UTF16(APath).WString()) == TRUE;
}