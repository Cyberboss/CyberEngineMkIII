#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class VirtualMemory {
			protected:
				static unsigned int SystemPageSize(void);
				static void* PageAlignedUpperBound(void* const AMemory, const unsigned int APageSize);
			};
		};
	};
};