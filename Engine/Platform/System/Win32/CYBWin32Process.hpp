#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
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
};