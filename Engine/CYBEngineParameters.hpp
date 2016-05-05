//! @file CYBEngineParameters.hpp Engine compilation configuration variables
#pragma once

namespace CYB {
	namespace Engine {
		//! @brief Compilation configuration variables
		class Parameters {
		public:
			enum : unsigned long long {
				ENGINE_HEAP_INITIAL_COMMIT_SIZE = API::ByteConverters::Megabytes(5),	//!< @brief The inital amount of memory to be commited by the engine heap
				HEAP_RESERVATION_SIZE = API::ByteConverters::Terabytes(1),	//!< @brief The amount of address space reserved by CYB::Engine::Heap
			};
		};
	};
};