#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				class VirtualMemory {
				protected:
					static unsigned int SystemPageSize(void) noexcept;
					static void* PageAlignedUpperBound(void* const AMemory, const unsigned int APageSize) noexcept;
				};
			};
		};
	};
};