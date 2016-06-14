#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				class Thread {
				protected:
					Win32::HANDLE FThread;
				private:
					static unsigned long __stdcall ThreadProc(void* const AThreadable) noexcept;
				protected:
					Thread(API::Threadable& AThreadable);
					~Thread();
				};
			};
		};
	};
};