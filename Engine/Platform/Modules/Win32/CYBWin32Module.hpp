//! @file CYBWin32Module.hpp Defines CYB::Platform::Modules::Module for Win32
#pragma once
namespace CYB {
	namespace Platform {
		namespace Modules {
			namespace Implementation {
				//! @brief Contains the Module handle
				class Module {
				protected:
					Win32::HMODULE FModule;	//!< @brief The Module handle
				protected:
					//! @copydoc CYB::Platform::Modules::Module::Module(const CYB::API::String::CStyle&)
					Module(const API::String::CStyle& AModuleName);
					Module(const Module&) = delete;
					Module(Module&& AMove) noexcept;	//!< @brief See @ref structors
					Module& operator=(Module&& AMove) noexcept;	//!< @brief See @ref structors
					//! @brief Closes the module handle
					~Module();
				};
			};
		};
	};
};
