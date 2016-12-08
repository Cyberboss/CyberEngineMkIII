//! @file CYBWin32File.hpp Defines the Win32 implementation for CYB::Platform::System::File
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief The CS, CV pair handler
				class Semaphore {
				protected:
					Win32::CRITICAL_SECTION FCritSection;	//!< @brief The mutex
					Win32::CONDITION_VARIABLE FCondVar;	//!< @brief The condition variable
				protected:
					//! @brief See @ref structors
					Semaphore() noexcept = default;
				};
			};
		};
	};
};