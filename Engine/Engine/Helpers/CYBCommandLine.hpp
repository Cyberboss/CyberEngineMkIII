//! @file CYBCommandLine.hpp Contains the definition of CYB::Engine::Helpers::CommandLine
#pragma once

namespace CYB {
	namespace Engine {
		namespace Helpers {
			//! @brief Command line string parser and manager
			class CommandLine : public API::CommandLine {
				ENABLE_TEST_HOOKS
			private:
				//! @brief Recognized token variations
				enum class TokenType : unsigned char {
					NORMAL,	//!< @brief Simple string
					SINGLE_LETTER_KEY,	//!< @brief '-' or '/' prefix
					EXTENDED_KEY,	//!< @brief '--' prefix
				};
				//! @brief A parsed command line token
				struct Token {
					API::String::Dynamic FEntry;	//!< @brief The string
					TokenType FType;	//!< @brief The type of prefix the string is if any
				};
			private:
				API::Container::Vector<Token> FTokens;	//!< @brief The ordered array of tokens
			private:
				/*!
					@brief Parse the command line into tokens
					@param ANumArguments The number of provided command line arguments
					@param AArguments The array of command line arguments
					@return The parsed tokens list
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if Windows failed to convert a string
				*/
				static API::Container::Vector<Token> ParseTokens(const unsigned int ANumArguments, const oschar_t* const* const AArguments);
			public:
				/*!
					@brief Parse the command line into tokens
					@param ANumArguments The number of provided command line arguments
					@param AArguments The array of command line arguments
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if Windows failed to convert a string
				*/
				CommandLine(const unsigned int ANumArguments, const oschar_t* const* const AArguments);

				void RunHandler(Callback ACallback, const int AFullNameKey, const int ADescriptionKey, const API::String::CStyle& AShortFlag, const API::String::CStyle& ALongFlag, const unsigned int ANumExpectedTokens, const unsigned int ANumOptionalTokens, unsigned long long AMaxInvocations) const final override;
			};
		};
	};
};