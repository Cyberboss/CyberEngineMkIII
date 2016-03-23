//! @file CYBPosixThread.hpp Defines CYB::Platform::Thread for Posix
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Thread {
			private:
				struct ThreadData {
					API::Threadable* FThreadable;
					std::atomic_bool* FRunningPointer;
				};
			protected:
				Posix::pthread_t FThread;
				std::atomic_bool FRunning;
			private:
				void* ThreadProc(void* const AThreadData);
			protected:
				Thread(API::Threadable& AThreadable);
			};
		};
	};
};