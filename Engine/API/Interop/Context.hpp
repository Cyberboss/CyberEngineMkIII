//! @file Context.hpp Defines the class which is given to access the engine either inside it or within a unit
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			//! @brief Class which gives access to the engine
			class Context {
			private:
				static thread_local Context* FContext;	//!< @brief The context reference for the unit
			public:
				Allocator& FAllocator;	//!< @brief The Allocator
			public:
				/*!
					@brief Get the API Context
					@return The API Context
					@par Thread Safety
						This function requires no thread safety
				*/
				static Context& GetContext(void) noexcept;

				/*!
					@brief Setup the context
					@param AAllocator The allocator
					@par Thread Safety
						This function requires no thread safety
				*/
				Context(Allocator& AAllocator) noexcept;
			};
		};
		//! @copydoc CYB::API::Interop::Context::GetContext
		API::Interop::Context& Context(void) noexcept;
	};
};