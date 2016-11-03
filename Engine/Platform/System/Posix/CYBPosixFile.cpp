//! @file CYBPosixFile.cpp Implements CYB::Platform::System::File for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::Platform::System::File::File(System::Path&& APath, const Mode AMode, const Method AMethod) :
	FPath(std::move(APath))
{
	auto Flags([&]() {
		switch (AMode) {
		case Mode::READ:
			return O_RDONLY;
		case Mode::WRITE:
			return O_WRONLY;
		case Mode::READ_WRITE:
			return O_RDWR;
		default:
			throw Exception::Violation(Exception::Violation::INVALID_ENUM);
		}
	}());
	switch (AMethod) {
	case Method::ANY:
		Flags |= O_CREAT;
		break;
	case Method::CREATE:
		Flags |= O_EXCL;
		break;
	case Method::EXIST:
		break;
	case Method::TRUNCATE:
		Flags |= O_CREAT | O_TRUNC;
		break;
	default:
		throw Exception::Violation(Exception::Violation::INVALID_ENUM);
	}

	FDescriptor = Core().FModuleManager.FC.Call<Modules::LibC::open>(FPath().CString(), Flags, S_IRUSR | S_IWUSR);

	if (FDescriptor == -1) {
		const auto Error(errno);
		switch (Error) {
		case EEXIST:
			throw Exception::SystemData(Exception::SystemData::FILE_EXISTS);
		case ENOTDIR:
		case ENOENT:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_FOUND);
		case EACCES:
		default:
			if (AMode == Mode::READ)
				throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
			else
				throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
}

CYB::Platform::System::Implementation::File::File(File&& AMove) noexcept :
	FDescriptor(AMove.FDescriptor)
{
	AMove.FDescriptor = -1;
}

CYB::Platform::System::File& CYB::Platform::System::File::operator=(File&& AMove) noexcept {
	FDescriptor = AMove.FDescriptor;
	AMove.FDescriptor = -1;
	return *this;
}

CYB::Platform::System::File::~File() {
	if (FDescriptor != -1)
		Core().FModuleManager.FC.Call<Modules::LibC::close>(FDescriptor);
}

unsigned long long CYB::Platform::System::File::Size(void) const noexcept {
	StatStruct Stat;
	API::Assert::Equal(Core().FModuleManager.FC.Call<Modules::LibC::fstat>(FDescriptor, &Stat), 0);
	return 0;
}

unsigned long long CYB::Platform::System::File::Seek(const long long AOffset, const SeekLocation ALocation) const {
	const auto PosixLocation([&]() {
		switch (ALocation) {
		case SeekLocation::BEGIN:
			return SEEK_SET;
		case SeekLocation::CURSOR:
			return SEEK_CUR;
		case SeekLocation::END:
			return SEEK_END;
		default:
			throw Exception::Violation(Exception::Violation::INVALID_ENUM);
		}
	}());

	const auto Result(Core().FModuleManager.FC.Call<Modules::LibC::lseek>(FDescriptor, static_cast<off_t>(AOffset), PosixLocation));
	API::Assert::NotEqual(-1, Result);

	return static_cast<unsigned long long>(Result);
}

unsigned long long CYB::Platform::System::File::Read(const void* const ABuffer, const unsigned long long AMaxAmount) const {
	static_cast<void>(ABuffer);
	static_cast<void>(AMaxAmount);
	UNIMPLEMENTED;
	return 0;
}

void CYB::Platform::System::File::Write(void* const ABuffer, const unsigned long long AAmount) {
	static_cast<void>(ABuffer);
	static_cast<void>(AAmount);
	UNIMPLEMENTED;
}