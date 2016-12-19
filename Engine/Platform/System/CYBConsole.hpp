//! @file CYBConsole.hpp Defines the stdout interface
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief Contains the basic File interface. Does not perform locking of any kind, be aware of possible race conditions
			class Console : public API::Logger {
			private:
				const API::String::Static FConsolePath;	//!< @brief The 'path' of the console
			public:
				/*!
					@brief Contructs a console interface and sets up the fake Path and Logger
					@par Thread Safety
						This function may only be called once
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if FTempPath could not be allocated
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE. Thrown if FTempPath not be retrieved
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if FTempPath does not validate
				*/
				Console();

				/*!
					@brief Show a window with console output
					@par Thread Safety
						This function may only be called once
				*/
				void Show(void) noexcept;

				//! @copydoc CYB::API::Logger::Log()
				void Log(const API::String::CStyle& AMessage, const Level ALevel) noexcept final override;

				/*!
					@brief Returns the console path
					@return "stdout"
					@par Thread Safety
						This function requires no thread saftey
				*/
				const API::String::CStyle& CurrentLog(void) const noexcept final override;
			};
		};
	};
};