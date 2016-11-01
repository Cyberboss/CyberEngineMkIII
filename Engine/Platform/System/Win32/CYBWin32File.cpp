//! @file CYBWin32File.cpp Implements CYB::Platform::System::File for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

CYB::Platform::System::File::File(const API::Path& APath, const Mode AMode, const Method AMethod) :
	FPath(static_cast<const System::Path&>(APath))
{
	auto& K32(Core().FModuleManager.FK32);

	const auto DesiredAccess([&]() -> DWORD {
		switch (AMode)
		{
		case Mode::READ:
			return GENERIC_READ;
		case Mode::WRITE:
			return GENERIC_WRITE;
		case Mode::READ_WRITE:
			return GENERIC_READ | GENERIC_WRITE;
		default:
			__assume(false);	//HCF is too indirect for coverage
		}
	}()), CreationDisposition([&]() -> DWORD {
		switch (AMethod) {
		case Method::ANY:
			return OPEN_ALWAYS;
		case Method::CREATE:
			return CREATE_NEW;
		case Method::EXIST:
			return OPEN_EXISTING;
		case Method::TRUNCATE:
			return TRUNCATE_EXISTING;
		default:
			__assume(false);	//HCF is too indirect for coverage
		}
	}());

																												//we don't care if somebody sneaks a peek at us, just no touching ;)
	FHandle = K32.Call<Modules::Kernel32::CreateFileW>(static_cast<const System::Path&>(APath).WidePath().WString(), DesiredAccess, static_cast<DWORD>(FILE_SHARE_READ | FILE_SHARE_DELETE), nullptr, CreationDisposition, static_cast<DWORD>(FILE_ATTRIBUTE_NORMAL), nullptr);

	if (FHandle == INVALID_HANDLE_VALUE) {
		const auto Error(K32.Call<Modules::Kernel32::GetLastError>());
		API::Assert::NotEqual<DWORD>(Error, ERROR_INVALID_PARAMETER);
		switch (Error) {
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_INVALID_NAME:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_FOUND);
		case ERROR_SHARING_VIOLATION:
		case ERROR_ACCESS_DENIED:
		default:
			if (AMode == Mode::READ)
				throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
			else
				throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
}

CYB::Platform::System::File::~File() {
	if (FHandle != INVALID_HANDLE_VALUE)
		Core().FModuleManager.FK32.Call<Modules::Kernel32::CloseHandle>(FHandle);
}