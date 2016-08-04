//! @file CYBWin32Thread.hpp Implements CYB::Platform::System::Thread for Win32
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Contains the thread handle and startup function
				class Thread {
				protected:
					Win32::HANDLE FThread;	//!< @brief The thread handle
				private:
					/*!
						@brief The thread startup function called by the kernel.
						@param AThreadable A pointer to the CYB::API::Threadable passed to the constructor
						@return 0
						@par Thread Safety
							This function requires no thread safety
						@attention Do not call this function directly
					*/
					static unsigned long __stdcall ThreadProc(void* const AThreadable) noexcept;
				protected:
					//! @copydoc CYB::Platform::System::Thread::Thread(CYB::API::Threadable&)
					Thread(API::Threadable& AThreadable);
					//! @brief Closes the thread handle
					~Thread();
				};
			};
		};
	};
};