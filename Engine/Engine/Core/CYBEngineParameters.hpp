//! @file CYBEngineParameters.hpp Engine compilation configuration variables
#pragma once

namespace CYB {
	namespace Engine {
		//! @brief Compilation configuration variables
		class Parameters {
		public:
			//! @brief Various engine constants
			enum : unsigned long long {
				VERSION_MAJOR = 3,	//!< @brief The major engine version
				VERSION_MINOR = 0,	//!< @brief The minor engine version
				VERSION_REVISION = 0,	//!< @brief The current engine revision
				COPYRIGHT_YEAR = 2016,	//!< @brief The engine's copyright year

				ENGINE_HEAP_INITIAL_COMMIT_SIZE = API::ByteConverters::Megabytes(5),	//!< @brief The inital amount of memory to be commited by the engine heap
				LOGGER_HEAP_INITIAL_COMMIT_SIZE = API::ByteConverters::Megabytes(1),	//!< @brief The inital amount of memory to be commited by the Logger heap

				HEAP_RESERVATION_SIZE = API::ByteConverters::Terabytes(1) / 4,	//!< @brief The amount of address space reserved by CYB::Engine::Heaps
			};
		public:
			static const char* const FTempPathName;	//!< @brief The directory in the temp folder to use
		public:
			/*!
				@brief Creates the correct CYB::API::EngineInformation struct
				@return A populated CYB::API::EngineInformation
				@par Thread Safety
					This function require no thread safety
			*/
			static API::EngineInformation CreateEngineInformation(void) noexcept;
		};
	};
};