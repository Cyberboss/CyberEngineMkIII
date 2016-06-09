#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			namespace Implementation {
				class Process {
				protected:
					Posix::pid_t FPID;
				protected:
					Process(const Posix::pid_t APID);
					Process(const Process&) = delete;
					Process(Process&& AMove) = default;
					Process& operator=(Process&& AMove) = default;
				};
			};
		};
	};
};