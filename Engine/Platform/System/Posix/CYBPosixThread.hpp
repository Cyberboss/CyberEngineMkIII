#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			namespace Implementation {
				class Thread {
				private:
					struct ThreadData {
						API::Threadable* const FThreadable;
						Posix::pthread_mutex_t* const FRunningLock;
						std::atomic_bool* const FRunning;
					};
				private:
					Posix::pthread_mutex_t FRunningLock;
				protected:
					Posix::pthread_t FThread;
					std::atomic_bool FRunning;
				private:
					static void* ThreadProc(void* const AThreadData) noexcept;

					void DestroyMutex(void) noexcept;
				protected:
					Thread(API::Threadable& AThreadable);
					~Thread();
				};
			};
		};
	};
};