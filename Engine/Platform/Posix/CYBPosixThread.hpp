#pragma once
namespace CYB {
	namespace Platform {
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
				static void* ThreadProc(void* const AThreadData);

				void DestroyMutex(void);
			protected:
				Thread(API::Threadable& AThreadable);
				~Thread();
			};
		};
	};
};