#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Process {
			protected:
				const Posix::pid_t FPID;
			protected:
				Process(const Posix::pid_t APID);
				Process(const Process&) = delete;
				Process(Process&& AMove);
			};
		};
	};
};