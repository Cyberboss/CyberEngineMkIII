#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			namespace Implementation {
				using namespace Posix;
				class Process {
				protected:
					pid_t FPID;
					int FExitCode;
					bool FExitCodeReady;
				protected:
					Process(const pid_t APID) noexcept;
					Process(const Process&) = delete;
					Process(Process&& AMove) noexcept = default;
					Process& operator=(Process&& AMove) noexcept = default;
				};
			};
		};
	};
};