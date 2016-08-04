//! @file CYBWin32Module.hpp Defines the implementation for CYB::Platform::Modules::Module
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
					//! @copydoc CYB::Platform::Modules::Module::Module(CYB::Platform::Modules::Module&&)
					Module(Module&& AMove) noexcept;
					//! @copydoc CYB::Platform::Modules::Module::operator=()
					Module& operator=(Module&& AMove) noexcept;
					//! @brief Closes the module handle
					~Module();
				};
			};
		};
	};
};
