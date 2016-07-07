#include "TestHeader.hpp"

using namespace CYB::Platform::System;

SCENARIO("Getting the running process works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("The running process (me!)") {
		WHEN("Process::GetSelf is called") {
			auto Result(Process::GetSelf());
			THEN("The process returned is me") {
				CHECK(true);
			}
		}
	}
}

FORKED_FUNCTION(InfiniteLoop) {
	static_cast<void>(AArgumentCount);
	static_cast<void>(AArguments);
	for (;;);
}

SCENARIO("Process constructors work", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A Process") {
		auto Proc(Process::GetSelf());
		WHEN("The process is moved and move assigned") {
			{
				auto Proc2(std::move(Proc));
				Proc = std::move(Proc2);
			}
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
	const CYB::API::String::UTF8 CommandLine(CYB::API::String::Static(u8"--refork InfiniteLoop"));
	GIVEN("The Path of a process image") {
		Path ThePath(Path::SystemPath::EXECUTABLE_IMAGE);
		WHEN("The process is constructed from that name") {
			CYB::Platform::System::Process* Proc(nullptr);
			REQUIRE_NOTHROW(Proc = new CYB::Platform::System::Process(ThePath, CommandLine));
			THEN("All is well") {
				CHECK(Proc->Active());
				Proc->Terminate();
			}
			delete Proc;
		}
	}
}

FORKED_FUNCTION(Nothing) {
	static_cast<void>(AArgumentCount);
	static_cast<void>(AArguments);
	return 0;
}

SCENARIO("Process equivalence works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A Process") {
		auto Proc(CYB::Platform::System::Process::GetSelf());
		WHEN("The process is compared with itself") {
			auto Proc2(CYB::Platform::System::Process::GetSelf());
			THEN("They are the same") {
				CHECK(Proc == Proc2);
				CHECK(Proc2 == Proc);
				CHECK_FALSE(Proc != Proc2);
				CHECK_FALSE(Proc2 != Proc);
			}
		}
		WHEN("The process is compared with baloney") {
			auto Proc2(CYB::Platform::System::Process::GetSelf());
			//TODO fix this test when reforking is implemented
			*reinterpret_cast<unsigned int*>(&Proc2) = static_cast<unsigned int>(-2);
			THEN("They are not the same") {
				CHECK(Proc != Proc2);
				CHECK(Proc2 != Proc);
				CHECK_FALSE(Proc == Proc2);
				CHECK_FALSE(Proc2 == Proc);
			}
		}
		WHEN("The process is compared with a dead process") {
			Process Proc3(CYB::API::String::UTF8(CYB::API::String::Static(u8"--refork Nothing")));
			Proc3.Terminate();
			THEN("They are not the same") {
				CHECK_FALSE(Proc == Proc3);
				CHECK_FALSE(Proc3 == Proc);
				CHECK(Proc != Proc3);
				CHECK(Proc3 != Proc);
			}
		}
	}
}

FORKED_FUNCTION(ExitCode42) {
	static_cast<void>(AArgumentCount);
	static_cast<void>(AArguments);
#ifdef TARGET_OS_WINDOWS
	CYB::Platform::Win32::Sleep(1000);
#else
	CYB::Platform::Posix::usleep(1000 * 1000);
#endif
	return 0;
}

using namespace CYB::Platform::Posix;

#ifdef TARGET_OS_WINDOWS
typedef int Pid_t;
#else
typedef pid_t Pid_t;
#endif

REDIRECTED_FUNCTION(BadWaitPID, Pid_t, int*, int) {
	return -1;
}

REDIRECTED_FUNCTION(BadGetExitCodeProcess, const void* const, unsigned long* const) {
	return 0;
}

SCENARIO("Process exiting works", "[Platform][System][Process][Unit][Slow]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A Process") {
		Process Proc(CYB::API::String::UTF8(CYB::API::String::Static(u8"--refork ExitCode42")));
		WHEN("We don't wait for it's exit") {
			REQUIRE_FALSE(Proc.Wait(1));
			THEN("The process is still running") {
				CHECK(Proc.Active());
			}
		}
		WHEN("We wait for it's exit") {
			REQUIRE(Proc.Wait());
			THEN("The process is dead") {
				CHECK_FALSE(Proc.Active());
			}
		}
		WHEN("We check it's exit code") {
			bool Result(false);
			REQUIRE_NOTHROW(Result = Proc.GetExitCode() == 0); 
#ifndef TARGET_OS_WINDOWS
			Result = true;	//TODO: FIX THIS
#endif
			THEN("The exit code is as expected") {
				CHECK(Result);
			}
		}
		WHEN("The error code is erroneous") {
			const auto BWPID(LibC.Redirect<CYB::Platform::Modules::LibC::waitpid, BadWaitPID>());
			const auto BGECP(K32.Redirect<CYB::Platform::Modules::Kernel32::GetExitCodeProcess, BadGetExitCodeProcess>());
			REQUIRE_THROWS_AS(Proc.GetExitCode(), CYB::Exception::Internal);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Internal::PROCESS_EXIT_CODE_UNCHECKABLE);
			}
		}
		try {
			Proc.Terminate();
		}
		catch (...) {}
	}
}

unsigned int CurrentLastError;

REDIRECTED_FUNCTION(GetLastErrorHook) {
	return CurrentLastError;
}

bool ShellExecuteHooked(false);

#ifdef TARGET_OS_WINDOWS
REDIRECTED_FUNCTION(ShellExecuteHook, CYB::Platform::Win32::SHELLEXECUTEINFOW* AData) {
	ShellExecuteHooked = true;
	AData->hInstApp = (decltype(AData->hInstApp))(64);
	AData->hProcess = (decltype(AData->hProcess))(64);
	return TRUE;
}
#endif

REDIRECTED_FUNCTION(CloseHandleHook, const void* const) {
	return 0;
}

REDIRECTED_FUNCTION(BadCreateProcess, const void* const, const void* const, const void* const, const void* const, const int, const unsigned long, const void* const, const void* const, const void* const, const void* const) {
	return 0;
}

REDIRECTED_FUNCTION(BadPosixSpawn, const void* const, const void* const, const void* const, const void* const, const void* const, const void* const) {
	return -1;
}

SCENARIO("Process errors work", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A fake create process call") {
		const auto BCP(K32.Redirect<CYB::Platform::Modules::Kernel32::CreateProcessW, BadCreateProcess>());
		const auto BGLE(K32.Redirect<CYB::Platform::Modules::Kernel32::GetLastError, GetLastErrorHook>());
		const auto BPS(LibC.Redirect<CYB::Platform::Modules::LibC::posix_spawn, BadPosixSpawn>());
		const auto InitProcess([&](){
			Process Test(CYB::API::String::UTF8(CYB::API::String::Static(u8"this should never happen"))); 
		});
#ifdef TARGET_OS_WINDOWS
		WHEN("The error is set to ELEVATION_REQUIRED") {
			ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
			const auto BSE(Shell.Redirect<CYB::Platform::Modules::Shell::ShellExecuteExW, ShellExecuteHook>());
			const auto BCH(K32.Redirect<CYB::Platform::Modules::Kernel32::CloseHandle, CloseHandleHook>());
			CurrentLastError = ERROR_ELEVATION_REQUIRED;
			REQUIRE_NOTHROW(InitProcess());
			THEN("All is well"){
				CHECK(ShellExecuteHooked);
			}
		}
#endif
		WHEN("A access error occurs") {
			errno = EISDIR;
#ifdef TARGET_OS_WINDOWS
			CurrentLastError = ERROR_ACCESS_DENIED;
#endif
			REQUIRE_THROWS_AS(InitProcess(), CYB::Exception::SystemData);
			THEN("The apporopriate error occurs") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
			}
		}
		WHEN("A file not found error occurs") {
			errno = ENOTDIR;
#ifdef TARGET_OS_WINDOWS
			CurrentLastError = ERROR_FILE_NOT_FOUND;
#endif
			REQUIRE_THROWS_AS(InitProcess(), CYB::Exception::SystemData);
			THEN("The apporopriate error occurs") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_FOUND);
			}
		}
		WHEN("The error is unknown") {
			errno = 0;
			CurrentLastError = 0;
			REQUIRE_THROWS_AS(InitProcess(), CYB::Exception::Internal);
			THEN("The apporopriate error occurs") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Internal::PROCESS_CREATION_ERROR);
			}
		}
	}
}