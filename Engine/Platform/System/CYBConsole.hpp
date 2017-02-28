//! @file CYBConsole.hpp Defines the stdout interface
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief Contains the basic File interface. Does not perform locking of any kind, be aware of possible race conditions
			class Console : public API::Logger {
			private:
				Mutex FLock;	//!< @brief To prevent simultaneous writes
				const API::String::Static FConsolePath;	//!< @brief The 'path' of the console
				std::atomic_bool FDevLog; 	//!< @brief Flag for enabling/disabling DEV logs
			private:
				/*!
					@brief Write to either stdout or stderr depending on @p AError
					@param AMessage The message to write
					@param AError If true, @p AMessage will be written to stderr instead of stdout
					@par Thread Safety
						This function requires synchronization
				*/
				static void WriteOut(const API::String::CStyle& AMessage, const bool AError) noexcept;
			public:
				/*!
					@brief Runs the CommandLine handler to become a dedicated holder process
					@param ACL The CommandLine to use to run the handler
					@par Thread Safety
						This function requries no thread safety
				*/
				static void CheckCommandLineForParent(API::CommandLine& ACL) noexcept;

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
					@brief Show a window with console output. Will automatically be called if an error is logged
					@par Thread Safety
						This function requries no thread safety
				*/
				void Show(void) noexcept;

				/*!
					@brief Spawn a holder for the console so commands sent through it won't affect the process
					@par Thread Safety
						This function requires sychronization at the object level
				*/
				void CreateHolder(void) const noexcept;

				/*!
					@copydoc CYB::API::Logger::Log()
					Errors will be written to stderr, everything else will be written to stdout
				*/
				void Log(const API::String::CStyle& AMessage, const Level ALevel) final override;

				/*!
					@copydoc CYB::API::Logger::Flush()
					This version always returns immediately
				*/
				void Flush(void) const noexcept final override;

				/*!
					@brief Returns the console path
					@return "stdout"
					@par Thread Safety
						This function requires no thread saftey
				*/
				const API::String::CStyle& CurrentLog(void) const noexcept final override;

				//! @copydoc CYB::API::Logger::SetDebugLogging()
				void SetDebugLogging(const bool AEnable) noexcept final override;
			};
		};
	};
};