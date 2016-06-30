#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Process::Terminate(void) {
	if (FPID == static_cast<pid_t>(Sys::Call(Sys::GET_CURRENT_PROCESS))) {
		Sys::Call(Sys::EXIT_PROC);
	}
	else if (Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, SIGKILL) == -1)
		throw Exception::Internal(Exception::Internal::PROCESS_TERMINATION_ERROR);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) noexcept {
	return Process(static_cast<pid_t>(Sys::Call(Sys::GET_CURRENT_PROCESS)));
}

CYB::Platform::System::Implementation::Process::Process(const pid_t APID) noexcept :
	FPID(APID),
	FExitCodeReady(false)
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

CYB::Platform::System::Process::~Process() {
	//try to reap
	int ExitCode;
	Core().FModuleManager.FC.Call<Modules::LibC::waitpid>(FPID, &ExitCode, WNOHANG);
}

bool CYB::Platform::System::Process::Active(void) const noexcept {
	return Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, 0) == 0 
		|| errno == EPERM;
}

bool CYB::Platform::System::Process::operator==(const Process& ARHS) const noexcept {
	return (FPID == ARHS.FPID) && Active();
}

bool CYB::Platform::System::Process::Wait(const unsigned int AMilliseconds) {
	for (auto I(0U); I < AMilliseconds && FExitCodeReady; Thread::Sleep(1), ++I) {
		const auto Result(Core().FModuleManager.FC.Call<Modules::LibC::waitpid>(FPID, &FExitCode, 0));
		if (Result == -1)
			throw Exception::Internal(Exception::Internal::PROCESS_EXIT_CODE_UNCHECKABLE);
		FExitCodeReady = Result == FPID;
	}
	return FExitCodeReady;
	//Wonky supposedly thread safe implementation
	/*
	auto& LibC(Core().FModuleManager.FC);
	const auto WaitRequired(FReapStatus.fetch_add(1, std::memory_order_acquire));
	if (-255 <= WaitRequired && WaitRequired <= 0) {
		if (WaitRequired == 1) {
			if (AMilliseconds == 0) {
				if (LibC.Call<Modules::LibC::waitpid>(FPID, &FExitCode, 0) == -1) {
					FReapStatus.store(1, std::memory_order_relaxed);
					throw Exception::Internal(Exception::Internal::PROCESS_EXIT_CODE_UNCHECKABLE);
				}
				FReapStatus.store(-255, std::memory_order_release);
			}
			else {
				for (auto I(0U); I < AMilliseconds; Thread::Sleep(1), ++I) {
					if (LibC.Call<Modules::LibC::waitpid>(FPID, &FExitCode, 0) == -1) {
						FReapStatus.store(1, std::memory_order_relaxed);
						throw Exception::Internal(Exception::Internal::PROCESS_EXIT_CODE_UNCHECKABLE);
					}
					FReapStatus.store(-255, std::memory_order_release);
					break;
				}
			}
		}
		else if (AMilliseconds == 0)
			long long LastResult(2);
		for (; LastResult > 1; Thread::Sleep(1), LastResult = FReapStatus.fetch_add(1, std::memory_order_relaxed));
		if (LastResult == 1)
			return Wait();
		else {
			long long LastResult(2);
			auto I(0U);
			for (; I < AMilliseconds && LastResult > 1; Thread::Sleep(1), ++I, LastResult = FReapStatus.fetch_add(1, std::memory_order_relaxed));
			if (LastResult == 1)
				return Wait(AMilliseconds - I);
		}
		const auto Result(FReapStatus.load(std::memory_order_relaxed));
		return -255 <= Result && Result <= 0;
	}
	return true;
	*/
}

int CYB::Platform::System::Process::GetExitCode(void) {
	Wait();
	return FExitCode;
}