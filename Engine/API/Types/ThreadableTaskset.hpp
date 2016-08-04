//! @file ThreadableTaskset.hpp Defines CYB::API::ThreadableTaskset
#pragma once
namespace CYB {
	namespace API {

		//! @brief A mutlithreading interface with a number of jobs in mind
		class ThreadableTaskset : public Threadable {
		private:
			const unsigned int FNumTasks;	//!< @brief The total number of tasks to perform
			std::atomic_bool FCancelFlag;	//!< @brief The cancellation flag
		protected:
			/*!
				@brief Construct a ThreadableTaskset
				@param ANumTasks The number of tasks to run on this taskset
				@par Thread Safety
					This function requires no thread safety
			*/
			ThreadableTaskset(const unsigned int ANumTasks) noexcept;
		public:
			/*!
				@brief Called at the start of a threading operation in a new thread by the internal thread manager. Will call DoTask in the order of 0-(FNumTasks - 1) provided an error does not occur in doing so. On termination the cancel flag will be cleared
				@par Calling Thread
					This function will be called by the engine in what should be considered a new thread, however it may be running in the ThreadPool depending on the calling context
				@attention Throwing an exception to this function will cause all remaining tasks to not be run and have the exception propagate to the internal thread manager where they will be logged at the DEV level and then ignored
			*/
			void BeginThreadedOperation(void) final override;
			/*!
				@brief Called by the internal thread manager to request cancellation of a thread. May also be called by the unit. When called no further tasks will be run after the current one completes
				@par Calling Thread
					This function may be called by the engine in any thread at any time during the object's lifetime
			*/
			void CancelThreadedOperation(void) noexcept final override;
			/*!
				@brief Called by BeginThreadedOperation FNumTasks times with an incrementing @p ATask value. Each time it is called task number @p ATask should be run
				@param ATask The task number to run
				@return true to continue task execution, false otherwise
				@par Calling Thread
					The function will in the thread that the object's BeginThreadedOperation runs on
			*/
			virtual bool DoTask(const unsigned int ATask) = 0;
		};
	};
};

#include "ThreadableTaskset.inl"