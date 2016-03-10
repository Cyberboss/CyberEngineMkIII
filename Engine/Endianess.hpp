#pragma once

//!	@file Endianess.hpp Ensures the engine/unit is being compiled on a little endian architecture

namespace CYB {
	namespace Endianess {
		enum class Endianess : unsigned int {
			LITTLE_ENDIAN = 0x00000001,
			BIG_ENDIAN = 0x01000000,
			PDP_ENDIAN = 0x00010000,
			UNKNOWN_ENDIAN = 0xFFFFFFFF
		};
#ifdef TARGET_OS_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4514)	//disable unreferenced function warning
#pragma warning(disable : 6326)	//disable constant comparison warning
#endif
		constexpr Endianess GetEndianess() {
			return
				((1 & 0xFFFFFFFF) == static_cast<unsigned int>(Endianess::LITTLE_ENDIAN))
				? Endianess::LITTLE_ENDIAN
				: ((1 & 0xFFFFFFFF) == static_cast<unsigned int>(Endianess::BIG_ENDIAN))
				? Endianess::BIG_ENDIAN
				: ((1 & 0xFFFFFFFF) == static_cast<unsigned int>(Endianess::PDP_ENDIAN))
				? Endianess::PDP_ENDIAN
				: Endianess::UNKNOWN_ENDIAN;
		}
#ifdef TARGET_OS_WINDOWS
#pragma warning(pop)
#endif
		static_assert(GetEndianess() == Endianess::LITTLE_ENDIAN, "CyberEngineMkIII relies on a little endian architecture");
	}
};