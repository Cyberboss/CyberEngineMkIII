//! @file CommandLine.hpp Defines the CommandLine interface
namespace CYB {
	namespace API {
		//! @brief An interface for running callbacks based on command line parameters
		class CommandLine {
		public:
			//! @todo replace this with CYB::Function
			/*!
				@brief Called to handle a valid command line parameters
				@par AParameters
					The expected parameters followed by any optional parameters
				@par Return Value
					true to continue execution, false to exit the process
				@par Thread Safety
					This will be called on the same thread it was added during the call that adds it
				@par Exception Safety
					Exceptions will be propagated to the function that added the handler
			*/
			using Callback = std::add_pointer<bool(const API::Container::Deque<const API::String::Dynamic*>& AParameters)>::type;
		public:
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
				@return The number of times this command was invoked
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				@attention Throws dependant on called Callback
			*/
			virtual unsigned int RunHandler(Callback ACallback, const int AFullNameKey, const int ADescriptionKey, const API::String::CStyle& AShortFlag, const API::String::CStyle& ALongFlag, const unsigned int ANumExpectedTokens, const unsigned int ANumOptionalTokens, const unsigned long long AMaxInvocations) const = 0;
		};
	};
};