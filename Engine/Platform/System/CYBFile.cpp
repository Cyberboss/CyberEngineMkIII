//! @file CYBFile.cpp Implements CYB::Platform::System::File shared functions
#include "CYB.hpp"

CYB::Platform::System::File::File(const API::Path& APath, const Mode AMode, const Method AMethod) :
	File(System::Path(static_cast<const System::Path&>(APath)), AMode, AMethod)
{
#ifdef DEBUG
	using namespace API::String;
	Dynamic Message(u8"Opened file: ");
	Message += APath();
	Message += Static(" in mode: ");
	switch (AMode) {
	case Mode::READ:
		Message += Static("READ");
		break;
	case Mode::WRITE:
		Message += Static("WRITE");
		break;
	case Mode::READ_WRITE:
		Message += Static("READ_WRITE");
		break;
	default:
		UNREACHABLE;
	}
	Message += Static(" with method: ");
	switch (AMethod) {
	case Method::ANY:
		Message += Static("ANY");
		break;
	case Method::CREATE:
		Message += Static("CREATE");
		break;
	case Method::EXIST:
		Message += Static("EXIST");
		break;
	case Method::TRUNCATE:
		Message += Static("TRUNCATE");
		break;
	default:
		UNREACHABLE;
	}
	API::Context().FLogger.Log(Message, API::Logger::Level::DEV);
#endif
}

void CYB::Platform::System::File::Touch(System::Path&& APath) {
	File(std::move(APath), Mode::WRITE, Method::ANY);
}

CYB::Platform::System::File::~File() {
	Close();
}

void CYB::Platform::System::File::Capabilities(Mode& AMode, bool& ASeek) const noexcept {
	ASeek = true;
	AMode = FOpenMode;
}

unsigned long long CYB::Platform::System::File::CursorPosition(void) const noexcept {
	return Seek(0, SeekLocation::CURSOR);
}

const CYB::API::Path& CYB::Platform::System::File::GetPath(void) const noexcept {
	return FPath;
}

CYB::API::File::Method CYB::Platform::System::File::OpenMethod(void) const noexcept {
	return FOpenMethod;
}