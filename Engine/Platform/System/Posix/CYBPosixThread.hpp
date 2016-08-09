#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			namespace Implementation {
				using namespace Posix;
				//! @brief PThread stuff
				class Thread {
				protected:
					API::Threadable& FThreadable;	//!< @brief A reference to the Threadable object this Thread will run
					pthread_t FThread;	//!< @brief The PThread
					std::atomic_bool FRunning;	//!< @brief The running indicator
				private:
					/*!
						@brief The thread startup function called by the pthread library
						@param AThread A pointer to the Thread that called pthread_create
						@return nullptr
						@par Thread Safety
							This function requires no thread safety
						@attention Do not call this function directly
					*/
					static void* ThreadProc(void* const AThread) noexcept;
				protected:
					//! @copydoc CYB::Platform::System::Thread::Thread(CYB::API::Threadable&)
					Thread(API::Threadable& AThreadable);
				};
			};
		};
	};
};