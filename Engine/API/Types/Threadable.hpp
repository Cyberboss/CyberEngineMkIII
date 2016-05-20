//! @file Threadable.hpp The multithreading API used by the CyberEngine
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
	};
};