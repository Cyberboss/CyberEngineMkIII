//! @file CYBThread.hpp Contains the platform thread definition
#pragma once
namespace CYB {
	namespace Platform {
		//! @brief Definition of a platform thread object
		class Thread : private Implementation::Thread {
		public:
			//! @brief The states of a thread
			enum class State {
				IDLE,		//!< @brief The thread is not running and can be started
				RUNNING,	//!< @brief The thread is running
				FINISHED,	//!< @brief The thread has finished running
			};
		private:
			API::Threadable& FThreadable;	//!< @brief A reference to the Threadable object this Thread will run
		public:
			/*!
				@brief Construct a Thread. Once returned, the thread will be scheduled to run
				@param AThreadable A reference to the threadable object the Thread will run
			*/
			Thread(API::Threadable& AThreadable);
			Thread(const Thread&) = delete;
			~Thread();

			void Cancel(void);
			bool IsFinished(void) const;
			void Wait(void) const;

			static void Yield(void);
		};
	};
};