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


CYB::Platform::System::Implementation::Process::Process(const System::Path& APath, const API::String::UTF8& ACommandLine) :
	FPID(SpawnProcess(APath, ACommandLine)),
	FExitCodeReady(false)
{}

pid_t CYB::Platform::System::Implementation::Process::SpawnProcess(const System::Path& APath, const CYB::API::String::Dynamic& ACommandLine) {

	//Parse ACommandLine for spaces
	API::Container::Deque<CYB::API::String::Dynamic> Work;
	API::Container::Vector<const char*> Argv;

	const auto HasArguments(ACommandLine.RawLength() > 0);
	if (HasArguments) {

		Work = ACommandLine.Tokenize(' ');

		Argv.reserve(Work.size() + 1);

		for (auto& Word : Work)
			Argv.emplace_back(Word.CString());
		Argv.emplace_back(nullptr);
	}

	pid_t PID;
	const auto Result(CYB::Core().FModuleManager.FC.Call<CYB::Platform::Modules::LibC::posix_spawn>(&PID, APath().CString(), nullptr, nullptr, HasArguments ? const_cast<char**>(&Argv[0]) : nullptr, environ));

	if (Result != 0)
		HandleSpawnError();

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