//! @file Thread.hpp The multithreading API used by the CyberEngine
#pragma once
namespace CYB {
	namespace API {
		//! @brief The basic multithreading interface
		class Threadable {
		public:
			/*!
				@brief Called at the start of a threading operation in a new thread by the internal thread manager at most once. Should not be called by unit
				@par Calling Thread
					This function will be called by the engine in what should be considered a new thread, however it may be running in the ThreadPool depending on the calling context
				@par Exception Safety
					This function will have exceptions throw from it logged at the WARN level and then ignored
			*/
			virtual void BeginThreadedOperation(void) = 0;
			/*!
				@brief Called by the internal thread manager to request cancellation of a thread at most once. When called the Threadable should do it's best to immediately return from BeginThreadedOperation. Should not be called by unit
				@par Calling Thread
					This function may be called by the engine in any thread at any time during the object's lifetime
				@par Exception Safety
					This function will have exceptions throw from it logged at the WARN level and then ignored
			*/
			virtual void CancelThreadedOperation(void) = 0;
		};
		//! @brief A mutlithreading interface with a number of jobs in mind
		class ThreadableTaskset : public Threadable {
		private:
			const unsigned int FNumTasks;	//!< @brief The total number of tasks to perform
			std::atomic_bool FCancelFlag;	//!< @brief The cancellation flag
		protected:
			/*!
				@brief Construct a ThreadableTaskset
				@param ANumTasks The number of tasks to run on this taskset
				@param AContinueOnError Whether or not tasks should continue after an error
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			ThreadableTaskset(const unsigned int ANumTasks) :
				FNumTasks(ANumTasks),
				FCancelFlag(false)
			{}
		public:
			/*!
				@brief Called at the start of a threading operation in a new thread by the internal thread manager. Will call DoTask in the order of 0-(FNumTasks - 1) provided an error does not occur in doing so. On termination the cancel flag will be cleared
				@par Calling Thread
					This function will be called by the engine in what should be considered a new thread, however it may be running in the ThreadPool depending on the calling context
				@par Exception Safety
					Throwing an exception to this function will cause all remaining tasks to not be run and have the exception propagate to the internal thread manager where they will be logged at the DEV level and then ignored
			*/
			void BeginThreadedOperation(void) final override {
				for (unsigned int I(0); I < FNumTasks && !FCancelFlag.load(std::memory_order_relaxed) && DoTask(I); ++I);
				FCancelFlag.store(false, std::memory_order_release);
			}
			/*!
				@brief Called by the internal thread manager to request cancellation of a thread. May also be called by the unit. When called no further tasks will be run after the current one completes
				@par Calling Thread
					This function may be called by the engine in any thread at any time during the object's lifetime
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			void CancelThreadedOperation(void) final override {
				FCancelFlag.store(true, std::memory_order_relaxed);
			}
			/*!
				@brief Called by BeginThreadedOperation FNumTasks times with an incrementing @p ATask value. Each time it is called task number @p ATask should be run
				@par Calling Thread
					The function will in the thread that the object's BeginThreadedOperation runs on
				@par Exception Safety
					This function may throw exceptions which will then be logged and ignored. This will cancel further tasks
			*/
			virtual bool DoTask(const unsigned int ATask) = 0;
		};
	};
};