#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			class Path;
			namespace Implementation {
				class Process {
				protected:
					Win32::HANDLE FHandle;
				private:
					static Win32::HANDLE CreateProcess(const System::Path& APath, const API::String::UTF8& ACommandLine);
				protected:
					Process() noexcept;
					Process(const System::Path& APath, const API::String::UTF8& ACommandLine);
					Process(const Process&) = delete;
					Process(Process&& AMove) noexcept;
					Process& operator=(Process&& AMove) noexcept;
				};
			};
		};
	};
};