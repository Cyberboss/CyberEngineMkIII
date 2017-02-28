//! @file CYBWin32Console.cpp Implements CYB::Platform::System::Console's Log and Show for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

void CYB::Platform::System::Console::Show(void) noexcept {
	Core().FModuleManager.Call<Modules::Kernel32::AllocConsole>();
	Core().FModuleManager.Call<Modules::Kernel32::SetConsoleOutputCP>(static_cast<UINT>(CP_UTF8));
	//! @todo when command line is implemnted, refork another process which holds onto this console so it doesn't close when the program exits. See http://stackoverflow.com/a/979116
}

void CYB::Platform::System::Console::CheckCommandLineForParent(API::CommandLine& ACL) noexcept {
	ACL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>& AHandle) {
		static_cast<void>(AHandle);
		return false;	//We want to die now
	}, 0, 0, API::String::Static("rfcl"), API::String::Static(), 1, 0, 1);
}

void CYB::Platform::System::Console::WriteOut(const CYB::API::String::CStyle& AMessage, const bool AError) noexcept {
	//fire and forget approach
	auto& MM(Core().FModuleManager);
	DWORD OutWritten;
	MM.Call<Modules::Kernel32::WriteConsoleA>(MM.Call<Modules::Kernel32::GetStdHandle>(AError ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE), AMessage.CString(), static_cast<DWORD>(AMessage.RawLength()), &OutWritten, nullptr);
}