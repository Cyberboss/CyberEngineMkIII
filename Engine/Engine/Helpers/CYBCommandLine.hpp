//! @file CYBCommandLine.hpp Contains the Comm
#pragma once

namespace CYB {
	namespace Engine {
		namespace Helpers {
			//! @brief Command line string parser and manager
			class CommandLine {
			public:
				//! @todo replace this with CYB::Function
				/*!
					@brief Called to handle a valid command line parameters
					@par AParameters
						The expected parameters followed by any optional parameters
					@par Return Value
						true to continue execution, false to exit the process. Returning false will cause the call to add the handler to throw a CYB::Exception::Fatal with the SHUTDOWN flag
					@par Thread Safety
						This will be called on the same thread it was added during the call that adds it
					@par Exception Safety
						Exceptions will be propagated to the function that added the handler
				*/
				using Callback = std::add_pointer<bool(const API::Container::Deque<const API::String::Dynamic*>& AParameters)>::type;
			private:
				//! @brief Recognized token variation
				enum class TokenType : unsigned char {
					NORMAL,	//!< @brief Simple string
					SINGLE_LETTER_KEY,	//!< @brief '-' or '/' prefix
					EXTENDED_KEY,	//!< @brief '--' prefix
				};
				//! @brief A parsed command line token
				struct Token {
					API::String::Dynamic FEntry;	//!< @brief The string
					TokenType FKeyType;	//!< @brief The type of prefix the string is if any
				};
			private:
				API::Container::Vector<Token> FTokens;	//!< @brief The ordered array of tokens
			private:
				/*!
					@brief Parse the command line into tokens
					@param ANumArguments The number of provided command line arguments
					@param AArguments The array of command line arguments
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

				//! @todo Fix this definition once translation keys are added
				/*!
					@brief Adds a command line flag handler
					@param ACallback A callback to run for each valid invocation of this flag
					@param AFullNameKey The translation key of the functional name of the command
					@param ADescriptionKey The translation key of a descriptive text for this command
					@param AShortFlag The short, case-sensitive flag to activate this command
					@param ALongFlag The long, case-sensitive flag to activate this command
					@param ANumExpectedTokens The number of expected required tokens
					@param ANumOptionalTokens The number of possible optional tokens after expected tokens
					@param AMaxInvocations The number of times this command can be validly repeated
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Fatal Error code: CYB::Exception::Fatal::SHUTDOWN. Thrown if a run handler requests an engine shutdown
					@attention Throws dependant on called Callback
				*/
				void RunHandler(Callback ACallback, const int AFullNameKey, const int ADescriptionKey, const API::String::CStyle& AShortFlag, const API::String::CStyle& ALongFlag, const unsigned int ANumExpectedTokens, const unsigned int ANumOptionalTokens, const unsigned long long AMaxInvocations) const;
			};
		};
	};
};