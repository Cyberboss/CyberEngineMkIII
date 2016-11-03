//! @file CYBPosixFile.cpp Implements CYB::Platform::System::File for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Implementation::File::Init(const System::Path& APath, const API::File::Mode AMode, const API::File::Method AMethod) {
	auto Flags([&]() {
		switch (AMode) {
		case API::File::Mode::READ:
			return O_RDONLY;
		case API::File::Mode::WRITE:
			return O_WRONLY;
		case API::File::Mode::READ_WRITE:
			return O_RDWR;
		default:
			throw Exception::Violation(Exception::Violation::INVALID_ENUM);
		}
	}());

	switch (AMethod) {
	case API::File::Method::ANY:
	case API::File::Method::CREATE:
		Flags |= O_CREAT | O_EXCL;
		break;
	case API::File::Method::EXIST:
		break;
	case API::File::Method::TRUNCATE:
		Flags |= O_CREAT | O_TRUNC;
		break;
	default:
		throw Exception::Violation(Exception::Violation::INVALID_ENUM);
	}

	FDescriptor = Core().FModuleManager.FC.Call<Modules::LibC::open>(APath().CString(), Flags, S_IRUSR | S_IWUSR);

	if (FDescriptor == -1) {
		const auto Error(errno);
		switch (Error) {
		case EEXIST:
			if (AMethod == API::File::Method::ANY) {
				Init(AMode, API::File::Method::EXIST);
				return;
			}
			throw Exception::SystemData(Exception::SystemData::FILE_EXISTS);
		case ENOTDIR:
		case ENOENT:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_FOUND);
		case EACCES:
		default:
			if (AMode == API::File::Mode::READ)
				throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
			else
				throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
	else
		FOpenMethod = AMethod;
}

CYB::Platform::System::File::File(System::Path&& APath, const Mode AMode, Method AMethod) :
	FPath(std::move(APath))
{
	Init(FPath, AMode, AMethod)
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
	const auto Result(Core().FModuleManager.FC.Call<Modules::LibC::fstat>(FDescriptor, &Stat));
	API::Assert::Equal(Result, static_cast<decltype(Result)>(0));
	return static_cast<unsigned long long>(Stat.st_size);
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
	API::Assert::NotEqual(Result, static_cast<decltype(Result)>(-1));

	return static_cast<unsigned long long>(Result);
}

unsigned long long CYB::Platform::System::File::Read(void* const ABuffer, const unsigned long long AMaxAmount) const noexcept {
	const auto Result(Core().FModuleManager.FC.Call<Modules::LibC::open>(FDescriptor, ABuffer, static_cast<size_t>(AMaxAmount)));
	API::Assert::NotEqual(Result, static_cast<decltype(Result)>(-1));
	return static_cast<unsigned long long>(Result);
}

unsigned long long CYB::Platform::System::File::Write(const void* const ABuffer, const unsigned long long AAmount) noexcept {
	const auto Result(Core().FModuleManager.FC.Call<Modules::LibC::write>(FDescriptor, ABuffer, static_cast<size_t>(AAmount)));
	API::Assert::NotEqual(Result, -1);
	return static_cast<unsigned long long>(Result);
}