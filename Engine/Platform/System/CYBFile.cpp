//! @file CYBFile.cpp Implements CYB::Platform::System::File shared functions
#include "CYB.hpp"

CYB::Platform::System::File::File(const API::Path& APath, const Mode AMode, Method AMethod) :
	File(System::Path(static_cast<const System::Path&>(APath)), AMode, AMethod)
{
	using namespace API::String;
	Dynamic Message(u8"Opened file: ");
	Message += APath();
	Message += Static(" in mode: ");

	//weird ifs because coverage and we can guarantee no default case
	if (AMode == Mode::READ) 
		Message += Static("READ");
	else if (AMode == Mode::WRITE)
		Message += Static("WRITE");
	else{
		API::Assert::Equal(AMode, Mode::READ_WRITE);
		Message += Static("READ_WRITE");
	}

	Message += Static(" with method: ");

	if (AMethod == Method::ANY) {
		Message += Static("ANY to ");
		AMethod = FOpenMethod;
	}

	if (AMethod == Method::CREATE)
		Message += Static("CREATE");
	else if (AMethod == Method::EXIST)
		Message += Static("EXIST");
	else {
		API::Assert::NotEqual(AMethod, Method::ANY);
		if (AMethod == Method::TRUNCATE)
			Message += Static("TRUNCATE");
	}

	API::Context().FLogger.Log(Message, API::Logger::Level::DEV);
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