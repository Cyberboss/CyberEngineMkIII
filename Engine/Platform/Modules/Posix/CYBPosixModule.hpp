//! @file CYBPosixModule.hpp Defines CYB::Platform::Modules::Module for Posix
#pragma once
namespace CYB {
	namespace Platform {
		namespace Modules {
			namespace Implementation {
				//! @brief Contains the Module's base address
				class Module {
				protected:
					void* FModule;	//!< @brief The Module's base address
				protected:
					//! @copydoc CYB::Platform::Modules::Module::Module(const CYB::API::String::CStyle&)
					Module(const API::String::CStyle& AModuleName);
					//! @brief See @ref structors
					Module(const Module&) = delete;
					//! @brief See @ref structors
					Module(Module&& AMove) noexcept;
					//! @brief See @ref structors
					Module& operator=(Module&& AMove) noexcept;
					//! @brief dlcloses the Module
					~Module();
				};
			};
		};
	};
};

