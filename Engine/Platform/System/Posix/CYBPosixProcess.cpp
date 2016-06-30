#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Process::Terminate(void) noexcept {
	if (FPID == static_cast<pid_t>(Sys::Call(Sys::GET_CURRENT_PROCESS)))
		Sys::Call(Sys::EXIT_PROC);
	else
		Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, SIGKILL);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) noexcept {
	return Process(static_cast<pid_t>(Sys::Call(Sys::GET_CURRENT_PROCESS)));
}

CYB::Platform::System::Implementation::Process::Process(const pid_t APID) noexcept :
	FPID(APID)
{}

static pid_t SpawnProcess(const CYB::Platform::System::Path& APath, const CYB::API::String::UTF8& ACommandLine) {

	//Parse ACommandLine for spaces
	const char** Argv(nullptr);

	//TODO: replace this shit once you make a real vector/deque
	struct UTF8Link {
		CYB::API::String::UTF8 FString;
		UTF8Link* FNext;
	};

	UTF8Link* Base(nullptr), *Work(Base);

	const auto Cleanup([&]() {
		for (auto Current(Base); Current != nullptr; Current = Base->FNext, CYB::Allocator().FHeap.Free(Base), Base = Current);
		CYB::Allocator().FHeap.Free(Argv);
	});
	if (ACommandLine.RawLength() > 0) {
		unsigned int Count(0), Last(0);
		try {
			ACommandLine.IterateCodepoints([&](const unsigned int ACodepoint, const int AIndex) {
				if (ACodepoint == ' ') {
					Argv = static_cast<const char**>(CYB::Allocator().FHeap.Realloc(Argv, Count + 1));

					if (Work == nullptr) {
						Base = static_cast<UTF8Link*>(CYB::Allocator().FHeap.Alloc(sizeof(UTF8Link)));
						Work = Base;
					}
					else
						Work = static_cast<UTF8Link*>(CYB::Allocator().FHeap.Alloc(sizeof(UTF8Link)));

					Work->FNext = nullptr;
					CYB::Allocator().InPlaceAllocation<CYB::API::String::UTF8>(&Work->FString, ACommandLine.SubString(Last, AIndex));
					Last = AIndex + 1;

					++Count;
				}
				return true;
			});
			Argv = static_cast<const char**>(CYB::Allocator().FHeap.Alloc(Count * sizeof(const char*)));

			Work = Base;
			for (auto I(0U); Work != nullptr; Work = Work->FNext, ++I)
				Argv[I] = Work->FString.CString();
		}
		catch (...) {
			Cleanup();
			throw;
		}
	}

	pid_t PID;
	const auto Result(CYB::Core().FModuleManager.FC.Call<CYB::Platform::Modules::LibC::posix_spawn>(&PID, APath().CString(), nullptr, nullptr, const_cast<char**>(Argv), environ));
	Cleanup();

	if (Result != 0)
		switch (errno) {
		case ENOTDIR:
		case ENOENT:
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_FOUND);
		case EISDIR:
		case ENOEXEC:
		case ELIBBAD:
		case EINVAL:
			throw CYB::Exception::Internal(CYB::Exception::Internal::FILE_NOT_EXECUTABLE);
		case EACCES:
		case ETXTBSY:
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_READABLE);
		default:
			throw CYB::Exception::Internal(CYB::Exception::Internal::PROCESS_CREATION_ERROR);
		}

	return PID;
}

CYB::Platform::System::Process::Process(const Path& APath, const API::String::UTF8& ACommandLine) :
	Implementation::Process(SpawnProcess(APath, ACommandLine))
{}

bool CYB::Platform::System::Process::Active(void) const noexcept {
	return Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, 0) == 0 
		|| errno == EPERM;
}

bool CYB::Platform::System::Process::operator==(const Process& ARHS) const noexcept {
	return (FPID == ARHS.FPID) && Active();
}