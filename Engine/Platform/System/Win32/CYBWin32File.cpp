//! @file CYBWin32File.cpp Implements CYB::Platform::System::File for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

CYB::Platform::System::File::File(System::Path&& APath, const Mode AMode, const Method AMethod) :
	FPath(std::move(APath))
{
	if (AMode == Mode::READ && AMethod == Method::TRUNCATE)
		throw Exception::Violation(Exception::Violation::INVALID_PARAMETERS);

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

	const auto FilePath(FPath.WidePath().WString());
																								//Maintain POSIX compatibility, no locking
	FHandle = K32.Call<Modules::Kernel32::CreateFileW>(FilePath, DesiredAccess, static_cast<DWORD>(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE), nullptr, CreationDisposition, static_cast<DWORD>(FILE_ATTRIBUTE_NORMAL), nullptr);

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
	else if (AMethod == Method::ANY) {
		if (K32.Call<Modules::Kernel32::GetLastError>() == ERROR_ALREADY_EXISTS)
			FOpenMethod = Method::EXIST;
		else
			FOpenMethod = Method::CREATE;
	}
	else
		FOpenMethod = AMethod;
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
	throw Exception::SystemData(Exception::SystemData::FILE_NOT_FOUND);
}

unsigned long long CYB::Platform::System::File::Size(void) const noexcept {
	LARGE_INTEGER Size;
	API::Assert::NotEqual(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetFileSizeEx>(FHandle, &Size), 0);
	return static_cast<unsigned long long>(Size.QuadPart);
}

unsigned long long CYB::Platform::System::File::Seek(const long long AOffset, const SeekLocation ALocation) const {
	LARGE_INTEGER Distance, Location;
	Distance.QuadPart = AOffset;

	const auto WinLocation([&]() -> DWORD {
		switch (ALocation) {
		case SeekLocation::BEGIN:
			return FILE_BEGIN;
		case SeekLocation::CURSOR:
			return FILE_CURRENT;
		case SeekLocation::END:
			return FILE_END;
		default:
			throw Exception::Violation(Exception::Violation::INVALID_ENUM);
		}
	}());

	API::Assert::NotEqual(Core().FModuleManager.FK32.Call<Modules::Kernel32::SetFilePointerEx>(FHandle, Distance, &Location, WinLocation), 0);

	return static_cast<unsigned long long>(Location.QuadPart);
}

unsigned long long CYB::Platform::System::File::Read(void* const ABuffer, const unsigned long long AMaxAmount) const noexcept {
	DWORD BytesRead;
	API::Assert::NotEqual(Core().FModuleManager.FK32.Call<Modules::Kernel32::ReadFile>(FHandle, ABuffer, static_cast<DWORD>(AMaxAmount), &BytesRead, nullptr), 0);
	return BytesRead;
}

unsigned long long CYB::Platform::System::File::Write(const void* const ABuffer, const unsigned long long AAmount) noexcept {
	DWORD BytesWritten;
	API::Assert::NotEqual(Core().FModuleManager.FK32.Call<Modules::Kernel32::WriteFile>(FHandle, ABuffer, static_cast<DWORD>(AAmount), &BytesWritten, nullptr), 0);
	return BytesWritten;
}