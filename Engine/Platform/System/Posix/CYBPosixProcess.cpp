#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Process::Terminate(void) {
	if (FPID == static_cast<pid_t>(Sys::Call(Sys::GET_CURRENT_PROCESS))) {
		Sys::Call(Sys::EXIT_PROC);
	}
	else if (Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, SIGKILL) == -1)
		throw Exception::Internal(Exception::Internal::PROCESS_TERMINATION_ERROR);
}

CYB::Platform::System::Implementation::Process::Process() noexcept :
	FPID(static_cast<pid_t>(System::Sys::Call(Sys::GET_CURRENT_PROCESS))),
	FExitCodeReady(false)
{}


CYB::Platform::System::Implementation::Process::Process(const Path& APath, const API::String::UTF8& ACommandLine) :
	FPID(SpawnProcess(APath, ACommandLine)),
	FExitCodeReady(false)
{}

pid_t CYB::Platform::System::Implementation::Process::SpawnProcess(const CYB::Platform::System::Path& APath, const CYB::API::String::Dynamic& ACommandLine) {

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
	struct AutoCleanup {
	private:
		const decltype(Cleanup) FCleanup;
	public:
		AutoCleanup(const decltype(Cleanup)& ACleanup) :
			FCleanup(ACleanup)
		{}
		~AutoCleanup() {
			FCleanup();
		}
	};

	AutoCleanup AC(Cleanup);

	if (ACommandLine.RawLength() > 0) {
		unsigned int Count(0), Last(0);
		const auto AddRange([&](const unsigned int AStart, const unsigned int AEnd) {
			if (Work == nullptr) {
				Base = static_cast<UTF8Link*>(CYB::Allocator().FHeap.Alloc(sizeof(UTF8Link)));
				Work = Base;
			}
			else {
				Work->FNext = static_cast<UTF8Link*>(CYB::Allocator().FHeap.Alloc(sizeof(UTF8Link)));
				Work = Work->FNext;
			}
			Work->FNext = nullptr;
			CYB::Allocator().InPlaceAllocation<CYB::API::String::Dynamic>(&Work->FString, ACommandLine.SubString(AStart, AEnd - Last));
			Last = AEnd + 1;

			++Count;
		});
		for (unsigned int I(0); I < ACommandLine.RawLength(); ++I)
			if (ACommandLine.CString()[I] == ' ')
				AddRange(Last, I);
		AddRange(Last, ACommandLine.RawLength());

		Argv = static_cast<const char**>(CYB::Allocator().FHeap.Alloc(Count * sizeof(const char*)));

		Work = Base;
		for (auto I(0U); Work != nullptr; Work = Work->FNext, ++I)
			Argv[I] = Work->FString.CString();
	}

	pid_t PID;
	const auto Result(CYB::Core().FModuleManager.FC.Call<CYB::Platform::Modules::LibC::posix_spawn>(&PID, APath().CString(), nullptr, nullptr, const_cast<char**>(Argv), environ));

	if (Result != 0)
		switch (errno) {
		case ENOTDIR:
		case ENOENT:
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_FOUND);
		case EISDIR:
		case ENOEXEC:
		case ELIBBAD:
		case EINVAL:
		case EACCES:
		case ETXTBSY:
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_READABLE);
		default:
			throw CYB::Exception::Internal(CYB::Exception::Internal::PROCESS_CREATION_ERROR);
		}

	return PID;
}

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
	if (!FExitCodeReady) {
		const auto WaitForever(AMilliseconds == 0);
		pid_t Result(0);
		if (WaitForever)
			Result = Core().FModuleManager.FC.Call<Modules::LibC::waitpid>(FPID, &FExitCode, 0);
		else
			for (auto I(0U); I < AMilliseconds && Result == 0; Thread::Sleep(1), ++I)
				Result = Core().FModuleManager.FC.Call<Modules::LibC::waitpid>(FPID, &FExitCode, WNOHANG);

		if (Result == -1)
			throw Exception::Internal(Exception::Internal::PROCESS_EXIT_CODE_UNCHECKABLE);

		if (Result != 0) {
			API::Assert::Equal(FPID, Result);
			FExitCode = WEXITSTATUS(FExitCode);
			FExitCodeReady = true;
		}
	}
	return FExitCodeReady;}

int CYB::Platform::System::Process::GetExitCode(void) {
	Wait();
	return FExitCode;
}