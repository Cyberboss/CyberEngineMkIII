//! @file CYBWin32Process.hpp Defines CYB::Platform::Process for Win32
#pragma once
namespace CYB {
	namespace Platform {
		//! @file Encapsulates Platform specific functions
		namespace Implementation {
			class Process {
			protected:
				Win32::HANDLE FHandle;
			protected:
				Process(Win32::HANDLE AProcess);
				Process(const Process&) = delete;
				Process(Process&& AMove);
				Process& operator=(Process&& AMove);
				~Process();
			};
		};
	};
};