//! @file Context.hpp Defines the class which is given to access the engine either inside it or within a unit
#pragma once
namespace CYB {
	namespace API {
		class Logger;
		namespace Interop {
			//! @brief Class which gives access to the engine
			class Context {
			private:
				static Context* FContext;	//!< @brief The context reference for the unit. This is not used inside the engine
			public:
				Allocator& FAllocator;	//!< @brief The Allocator
				Logger& FLogger;	//!< @brief The Logger
			protected:
				/*!
					@brief Setup the context
					@param AAllocator The Allocator
					@param AAllocator The Logger
					@par Thread Safety
						This function requires no thread safety
				*/
				Context(Allocator& AAllocator, Logger& ALogger) noexcept;
			public:
				/*!
					@brief Get the API's Context
					@return The API Context
					@par Thread Safety
						This function requires no thread safety
				*/
				static Context& GetContext(void) noexcept;

				//! @brief See @ref structors
				Context(const Context&) = delete;
			};
		};
		//! @copydoc CYB::API::Interop::Context::GetContext
		API::Interop::Context& Context(void) noexcept;
	};
};