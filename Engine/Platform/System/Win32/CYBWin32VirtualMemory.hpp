//! @file CYBWin32VirtualMemory.hpp Contains the Win32 VirtualMemory implementation
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Various aligment functions
				class VirtualMemory {
				protected:
					/*!
						@brief Retrieve the system page size
						@return The system page size
						@par Thread Safety
							This function requires no thread safety
					*/
					static unsigned int SystemPageSize(void) noexcept;
					/*!
						@brief Get the base address of a page given an address
						@param AMemory The address to reference
						@param APageSize The system page size
						@return The base address of the page @p AMemory lies on
						@par Thread Safety
							This function requires no thread safety
					*/
					static void* PageAlignedUpperBound(void* const AMemory, const unsigned int APageSize) noexcept;
				};
			};
		};
	};
};