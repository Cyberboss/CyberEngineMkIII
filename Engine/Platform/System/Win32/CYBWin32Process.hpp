#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			namespace Implementation {
				class Process {
				protected:
					Win32::HANDLE FHandle;
				protected:
					Process(Win32::HANDLE AProcess) noexcept;
					Process(const Process&) = delete;
					Process(Process&& AMove) noexcept;
					Process& operator=(Process&& AMove) noexcept;
					~Process();
				};
			};
		};
	};
};