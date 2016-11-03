//! @file CYBWin32File.cpp Implements CYB::Platform::System::File for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

CYB::Platform::System::File::File(System::Path&& APath, const Mode AMode, const Method AMethod) :
	FPath(std::move(APath))
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
			throw Exception::Violation(Exception::Violation::INVALID_ENUM);
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
			throw Exception::Violation(Exception::Violation::INVALID_ENUM);
		}
	}());
																								//we don't care if somebody sneaks a peek at us, just no touching ;)
	FHandle = K32.Call<Modules::Kernel32::CreateFileW>(FPath.WidePath().WString(), DesiredAccess, static_cast<DWORD>(FILE_SHARE_READ | FILE_SHARE_DELETE), nullptr, CreationDisposition, static_cast<DWORD>(FILE_ATTRIBUTE_NORMAL), nullptr);

	if (FHandle == INVALID_HANDLE_VALUE) {
		const auto Error(K32.Call<Modules::Kernel32::GetLastError>());
		API::Assert::NotEqual<DWORD>(Error, ERROR_INVALID_PARAMETER);
		switch (Error) {
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_INVALID_NAME:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_FOUND);
		case ERROR_FILE_EXISTS:
			throw Exception::SystemData(Exception::SystemData::FILE_EXISTS);
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

CYB::Platform::System::Implementation::File::File(File&& AMove) noexcept :
	FHandle(AMove.FHandle)
{
	AMove.FHandle = INVALID_HANDLE_VALUE;
}

CYB::Platform::System::File& CYB::Platform::System::File::operator=(File&& AMove) noexcept {
	FHandle = AMove.FHandle;
	AMove.FHandle = INVALID_HANDLE_VALUE;
	return *this;
}

CYB::Platform::System::File::~File() {
	if (FHandle != INVALID_HANDLE_VALUE)
		Core().FModuleManager.FK32.Call<Modules::Kernel32::CloseHandle>(FHandle);
}

unsigned long long CYB::Platform::System::File::Size(const System::Path& APath) {
	auto& K32(Core().FModuleManager.FK32);

	WIN32_FILE_ATTRIBUTE_DATA Attributes;
	if (GetFileAttributesExW(APath.WidePath().WString(), GetFileExInfoStandard, &Attributes) != 0) {
		LARGE_INTEGER Size;
		Size.HighPart = static_cast<LONG>(Attributes.nFileSizeHigh);
		Size.LowPart = Attributes.nFileSizeLow;
		return static_cast<unsigned long long>(Size.QuadPart);
	}

	const auto Error(K32.Call<Modules::Kernel32::GetLastError>());
	if (Error == ERROR_ACCESS_DENIED || Error == ERROR_SHARING_VIOLATION)
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
	else
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_FOUND);
}

unsigned long long CYB::Platform::System::File::Size(void) const noexcept {
	LARGE_INTEGER Size;
	API::Assert::NotEqual(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetFileSizeEx>(FHandle, &Size), 0);
	return static_cast<unsigned long long>(Size.QuadPart);
}

unsigned long long CYB::Platform::System::File::CursorPosition(void) const noexcept {
	LARGE_INTEGER Distance, Location;
	Distance.QuadPart = 0;
	API::Assert::NotEqual(Core().FModuleManager.FK32.Call<Modules::Kernel32::SetFilePointerEx>(FHandle, Distance, &Location, static_cast<DWORD>(FILE_CURRENT)), 0);
	return static_cast<unsigned long long>(Location.QuadPart);
}