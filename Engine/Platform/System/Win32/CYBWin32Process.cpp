#include "CYB.hpp"

#include "../../CYBUTF16.inl"

using namespace CYB::Platform::Win32;

void CYB::Platform::System::Process::Terminate(void) {
	//Self terminating is actually safer than exiting due to locks and shit, also allows for us to kill ourselves and other processes in one line
	//No FK32 because this can be called without Core
	if(Sys::Call(Sys::TERMINATE_PROC, FHandle) == 0)
		throw Exception::Internal(Exception::Internal::PROCESS_TERMINATION_ERROR);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) noexcept {
	//No FK32 because this can be called without Core
	return Process(reinterpret_cast<Win32::HANDLE>(Sys::Call(Sys::GET_CURRENT_PROCESS)));
}

CYB::Platform::System::Implementation::Process::Process(Win32::HANDLE AHandle) noexcept :
	FHandle(AHandle)
{}

CYB::Platform::System::Implementation::Process::Process(Process&& AMove) noexcept :
	FHandle(AMove.FHandle)
{
	AMove.FHandle = nullptr;
}

CYB::Platform::System::Implementation::Process& CYB::Platform::System::Implementation::Process::operator=(Process&& AMove) noexcept {
	FHandle = AMove.FHandle;
	AMove.FHandle = nullptr;
	return *this;
}

CYB::Platform::System::Process::~Process() {
	if(FHandle != nullptr)
		Core().FModuleManager.FK32.Call<Modules::Kernel32::CloseHandle>(FHandle);
}

static HANDLE CreateProcess(const CYB::Platform::System::Path& APath, const CYB::API::String::UTF8& ACommandLine) {

	CYB::API::String::UTF16 ExeAs16(APath()), CmdlAs16(ACommandLine);
	STARTUPINFO StartupInfo{ sizeof(STARTUPINFO), nullptr, nullptr, nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr, 0, 0, 0};
	PROCESS_INFORMATION ProcessInformation;
	auto& MM(CYB::Core().FModuleManager);

	const auto Result(MM.FK32.Call<CYB::Platform::Modules::Kernel32::CreateProcessW>(ExeAs16.WString(), CmdlAs16.WideData(), nullptr, nullptr, FALSE, DWORD(0), nullptr, nullptr, &StartupInfo, &ProcessInformation));

	if (Result == 0) {
		for (;;) {
			const auto Error(MM.FK32.Call<CYB::Platform::Modules::Kernel32::GetLastError>());
			switch (Error) {
			case ERROR_ACCESS_DENIED:
				throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_READABLE);
			case ERROR_FILE_NOT_FOUND:
				throw CYB::Exception::SystemData(CYB::Exception::SystemData::FILE_NOT_FOUND);
			case ERROR_ELEVATION_REQUIRED:
				//try again with elevation by using shell execute
			{
				SHELLEXECUTEINFO ShellExecuteInfo{ sizeof(SHELLEXECUTEINFO), SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS, nullptr, L"runas", ExeAs16.WString(), CmdlAs16.WString(), nullptr, SW_SHOWNORMAL, 0, nullptr, nullptr, 0 };
				ShellExecuteInfo.hIcon = 0;
				ShellExecuteInfo.hMonitor = 0;
				ShellExecuteInfo.hProcess = 0;
				const auto Result2(MM.FShell.Call<CYB::Platform::Modules::Shell::ShellExecuteExW>(&ShellExecuteInfo));
				if (Result2 == TRUE) {
					CYB::API::Assert::LessThan(reinterpret_cast<HINSTANCE>(32), ShellExecuteInfo.hInstApp);
					CYB::API::Assert::NotEqual(ShellExecuteInfo.hProcess, static_cast<HANDLE>(nullptr));
					return ShellExecuteInfo.hProcess;
				}
				continue;	//re-reun error handling
			}
			default:
				BREAK;	//TODO: Remove this eventually
				throw CYB::Exception::Internal(CYB::Exception::Internal::PROCESS_CREATION_ERROR);
			}
		}
	}

	MM.FK32.Call<CYB::Platform::Modules::Kernel32::CloseHandle>(ProcessInformation.hThread);
	return ProcessInformation.hProcess;
}

CYB::Platform::System::Process::Process(const Path& APath, const API::String::UTF8& ACommandLine):
	Implementation::Process(CreateProcess(APath, ACommandLine))
{}

bool CYB::Platform::System::Process::Active(void) const noexcept {
	return Core().FModuleManager.FK32.Call<Modules::Kernel32::WaitForSingleObject>(FHandle, Win32::DWORD(0)) == WAIT_TIMEOUT;
}

bool CYB::Platform::System::Process::operator==(const Process& ARHS) const noexcept {
	if (Active()) {
		const auto Result(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetProcessId>(FHandle));
		if (Result != 0)
			return Result == Core().FModuleManager.FK32.Call<Modules::Kernel32::GetProcessId>(ARHS.FHandle);
	}
	return false;
}