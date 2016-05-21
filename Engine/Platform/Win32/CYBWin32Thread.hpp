#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Thread {
			protected:
				Win32::HANDLE FThread;
			private:
				static unsigned long __stdcall ThreadProc(void* const AThreadable);
			protected:
				Thread(API::Threadable& AThreadable);
				~Thread();
			};
		};
	};
};