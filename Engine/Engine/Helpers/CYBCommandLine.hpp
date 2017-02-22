//! @file CYBCommandLine.hpp Contains the Comm
#pragma once

namespace CYB {
	namespace Engine {
		namespace Helpers {
			//! @brief Command line string parser and manager
			class CommandLine {
			public:
				/*!
					@brief Parse the command line into tokens
					@param ANumArguments The number of provided command line arguments
					@param AArguments The array of command line arguments
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				CommandLine(const unsigned int ANumArguments, const oschar_t* const* const AArguments);
			};
		};
	};
};