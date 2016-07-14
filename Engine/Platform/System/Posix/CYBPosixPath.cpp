//! @file CYBPosixPath.cpp Implements CYB::Platform::System::Path for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;
using namespace CYB::API::String;

CYB::API::String::UTF8 CYB::Platform::System::Path::LocateDirectory(const SystemPath ADirectory) {
	auto& MM(Core().FModuleManager);
	switch (ADirectory) {
	case SystemPath::EXECUTABLE_IMAGE:
	case SystemPath::EXECUTABLE: {
		char ThePath[PATH_MAX];
		if (MM.FC.Call<Modules::LibC::readlink>("/proc/self/exe", ThePath, PATH_MAX) > 0) {
			API::String::Static AsStatic(ThePath);
			API::String::Dynamic Work(AsStatic);
			if (ADirectory == SystemPath::EXECUTABLE) {
				auto Last(0);
				for(auto I(0U); I < Work.RawLength(); ++I)
					if (Work.CString()[I] == '/')
						Last = I;
				Work.Shrink(Last + 1);
			}
			return Work;
		}
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);}
	case SystemPath::RESOURCE:
		return GetResourceDirectory();
	case SystemPath::TEMPORARY: {
		API::String::UTF8 Result(API::String::Static(u8"/tmp/"));
		Result += API::String::UTF8(API::String::Static(Engine::Parameters::FTempPathName)); 
		bool Throw(false);
		try {
			CreateDirectory(Result);
		}
		catch (Exception::SystemData AException) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::SystemData::FILE_NOT_WRITABLE);
			Throw = true;
		}
		if (Throw)
			throw Exception::SystemData(Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
		return Result;
	}
	case SystemPath::WORKING: {
		API::String::UTF8 CurrentDir(API::String::Static(u8"."));
		bool Throw(false);
		try {
			Evaluate(CurrentDir);
		}
		catch (Exception::Internal AException) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::PATH_EVALUATION_FAILURE);
			Throw = true;
		}
		if(Throw)
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
		return CurrentDir;
	}
	case SystemPath::USER: {
		const API::String::Static XDGDir(MM.FC.Call<Modules::LibC::getenv>("XDG_CONFIG_HOME")),
			HOMEDir(MM.FC.Call<Modules::LibC::getenv>("HOME"));
		if (XDGDir.RawLength() > 0)
			return API::String::UTF8(XDGDir);
		else if (HOMEDir.RawLength() > 0)
			return API::String::UTF8(HOMEDir);
		else {
			const auto BufferSize(MM.FC.Call<Modules::LibC::sysconf>(_SC_GETPW_R_SIZE_MAX));
			if (BufferSize != -1) {
				const auto UID(MM.FC.Call<Modules::LibC::getuid>());	//they say this can't fail
				PasswdStruct PS, *Out;
				API::String::UTF8 FinalPath;
				auto Buffer(static_cast<char*>(API::Allocator().FHeap.Alloc(BufferSize)));
				const auto Result(MM.FC.Call<Modules::LibC::getpwuid_r>(UID, &PS, Buffer, BufferSize, &Out));
				const auto Succeeded(Result == 0 && Out == &PS);
				if(Succeeded)
					FinalPath = API::String::UTF8(API::String::Static(PS.pw_dir));
				API::Allocator().FHeap.Free(Buffer);
				if (Succeeded)
					return FinalPath;
			}
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
		}
	}
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
}

void CYB::Platform::System::Path::CreateDirectory(const API::String::UTF8& APath) {
	const auto Result(Core().FModuleManager.FC.Call<Modules::LibC::mkdir>(APath.CString(), 0777));
	if (!(Result == 0 || errno == EEXIST))
		throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
}

void CYB::Platform::System::Path::DeleteFile(const API::String::UTF8& APath) {
	if (Core().FModuleManager.FC.Call<Modules::LibC::unlink>(APath.CString()) != 0) {
		const auto Error(errno);
		API::Assert::NotEqual(EISDIR, Error);
		switch (Error) {
		case ENOTDIR:
		case ENOENT:
			break;	//contract fufilled
		default:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
}

void CYB::Platform::System::Path::DeleteDirectory(const API::String::UTF8& APath) {
	if (Core().FModuleManager.FC.Call<Modules::LibC::rmdir>(APath.CString()) != 0) {
		const auto Error(errno);
		switch (Error) {
		case ENOENT:
			break;	//contract fufilled
		case EEXIST:	//Posix 2001
		case ENOTEMPTY:
			throw Exception::SystemData(Exception::SystemData::DIRECTORY_NOT_EMPTY);
		default:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_WRITABLE);
		}
	}
}

void CYB::Platform::System::Path::Evaluate(API::String::UTF8& APath) {
	auto NewPath(APath + CYB::API::String::UTF8(CYB::API::String::Static(u8"/.")));
	char ThePath[PATH_MAX];
	auto const Result(Core().FModuleManager.FC.Call<Modules::LibC::realpath>(NewPath.CString(), ThePath));
	if (Result != ThePath)
		throw Exception::Internal(Exception::Internal::PATH_EVALUATION_FAILURE);
	APath = CYB::API::String::UTF8(CYB::API::String::Static(ThePath));
}

bool CYB::Platform::System::Path::Verify(const API::String::UTF8& APath) const {
	StatStruct ST;
	return Sys::Call(Sys::LSTAT, const_cast<char*>(APath.CString()), &ST) == 0;
}

void CYB::Platform::System::Path::SetPath(API::String::UTF8&& APath) {
	FPath = std::move(APath);
}

bool CYB::Platform::System::Path::IsDirectory(void) const {
	StatStruct ST;	if (Sys::Call(Sys::LSTAT, const_cast<char*>(FPath.CString()), &ST) != 0)
		throw Exception::SystemData(Exception::SystemData::PATH_LOST);
	using namespace CYB::Platform::Posix;
	return S_ISDIR(ST.st_mode);
}

void CYB::Platform::System::Path::NavigateToParentDirectory(void) {

}

bool CYB::Platform::System::Path::CanRead(void) const {
	return false;
}

bool CYB::Platform::System::Path::CanWrite(void) const {
	return false;
}

bool CYB::Platform::System::Path::CanExecute(void) const {
	return false;
}

CYB::Platform::System::Implementation::Path::DirectoryEntry::DirectoryEntry(const System::Path& APath) :
	FPathListing(nullptr),
	FDirectory(Core().FModuleManager.FC.Call<Modules::LibC::opendir>(APath().CString()))
{
	if (FDirectory == nullptr) {
		const auto Error(errno);
		switch (Error) {
		case EACCES:
			throw Exception::SystemData(Exception::SystemData::FILE_NOT_READABLE);
		default:
			throw Exception::SystemData(Exception::SystemData::PATH_LOST);
		}
	}

	operator++();
}

CYB::Platform::System::Implementation::Path::DirectoryEntry::~DirectoryEntry() {
	Core().FModuleManager.FC.Call<Modules::LibC::closedir>(FDirectory);
}

void CYB::Platform::System::Implementation::Path::DirectoryEntry::operator++(void) {
	DirStruct* Result(nullptr);
	if (Core().FModuleManager.FC.Call<Modules::LibC::readdir_r>(FDirectory, &FEntry, &Result) != 0 || Result != &FEntry)
		FPathListing = API::Interop::Object<API::Path>(nullptr);
	else
		FPathListing = API::Interop::Object<System::Path>::Upcast<API::Path>(API::Allocator().NewObject<System::Path>(UTF8(Static(FEntry.d_name))));
}