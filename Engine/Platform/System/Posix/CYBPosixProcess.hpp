#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			namespace Implementation {
				class Process {
				protected:
					Posix::pid_t FPID;
				protected:
					Process(const Posix::pid_t APID) noexcept;
					Process(const Process&) = delete;
					Process(Process&& AMove) noexcept = default;
					Process& operator=(Process&& AMove) noexcept = default;
				};
			};
		};
	};
};