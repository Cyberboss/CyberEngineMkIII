//! @file CYBPosixFile.cpp Implements CYB::Platform::System::File for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Implementation::File::Init(const System::Path& APath, const API::File::Mode AMode, API::File::Method& AMethod) {

	if (AMode == API::File::Mode::READ && AMethod == API::File::Method::TRUNCATE)
		throw Exception::Violation(Exception::Violation::INVALID_PARAMETERS);

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
				AMethod = API::File::Method::EXIST;
				Init(APath, AMode, AMethod);
				return;
			}
		case EISDIR:
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

	//Check it's actually a file because otherwise we aren't supposed to have it open
	if (!(S_ISREG(StatFD().st_mode)))
		throw Exception::SystemData(Exception::SystemData::FILE_EXISTS);

	if (AMethod == API::File::Method::ANY)
		AMethod = API::File::Method::CREATE;
}
 
CYB::Platform::System::File::File(Path&& APath, const Mode AMode, const Method AMethod) :
	FPath(std::move(APath)),
	FOpenMode(AMode),
	FOpenMethod(AMethod)
{
	try {
		Init(FPath, AMode, FOpenMethod);
	}
	catch (Exception::SystemData& AException) {
		if (FDescriptor != -1) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::SystemData::FILE_EXISTS, Exception::SystemData::FILE_NOT_READABLE);
			Close();
		}
		throw;
	}
}

CYB::Platform::System::Implementation::File::File(File&& AMove) noexcept :
	FDescriptor(AMove.FDescriptor) {
	AMove.FDescriptor = -1;
}

CYB::Platform::System::File& CYB::Platform::System::File::operator=(File&& AMove) noexcept {
	Close();
	FDescriptor = AMove.FDescriptor;
	AMove.FDescriptor = -1;
	FOpenMode = AMove.FOpenMode;
	FOpenMethod = AMove.FOpenMethod;
	return *this;
}

void CYB::Platform::System::File::Close(void) const noexcept {
	if (FDescriptor != -1)
		Core().FModuleManager.FC.Call<Modules::LibC::close>(FDescriptor);
}

unsigned long long CYB::Platform::System::File::Size(const System::Path& APath) {
	StatStruct Stat;
	const auto Result(Sys::Call(Sys::LSTAT, const_cast<char*>(APath().CString()), &Stat));
	if(Result == 0)
		return static_cast<unsigned long long>(Stat.st_size);
	else if(Result == EACCES)
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
	throw Exception::SystemData(Exception::SystemData::FILE_NOT_FOUND);
}


StatStruct CYB::Platform::System::Implementation::File::StatFD(void) const {
	StatStruct Stat;
	const auto Result(static_cast<int>(System::Sys::Call(Sys::FSTAT, FDescriptor, &Stat)));
	if (Result != 0)
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
	return Stat;
}

unsigned long long CYB::Platform::System::File::Size(void) const {
	return static_cast<unsigned long long>(StatFD().st_size);
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

	unsigned long long Result(Core().FModuleManager.FC.Call<Modules::LibC::lseek>(FDescriptor, static_cast<off_t>(AOffset), PosixLocation));
	//See http://man7.org/linux/man-pages/man2/lseek.2.html, there is no way besides the user seeking past
	if (Result == static_cast<decltype(Result)>(-1))
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);

	return Result;
}

unsigned long long CYB::Platform::System::File::Read(void* const ABuffer, const unsigned long long AMaxAmount) const {
	if (FOpenMode == Mode::WRITE)
		throw Exception::Violation(Exception::Violation::INVALID_OPERATION);
	unsigned long long Result(Core().FModuleManager.FC.Call<Modules::LibC::read>(FDescriptor, ABuffer, static_cast<size_t>(AMaxAmount)));
	if (Result == static_cast<decltype(Result)>(-1))
		Result = 0;
	return Result;
}

unsigned long long CYB::Platform::System::File::Write(const void* const ABuffer, const unsigned long long AAmount) {
	if (FOpenMode == Mode::READ)
		throw Exception::Violation(Exception::Violation::INVALID_OPERATION);
	unsigned long long Result(Core().FModuleManager.FC.Call<Modules::LibC::write>(FDescriptor, ABuffer, static_cast<size_t>(AAmount)));
	if (Result == static_cast<decltype(Result)>(-1))
		Result = 0;
	return Result;
}