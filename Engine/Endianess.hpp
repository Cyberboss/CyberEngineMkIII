#pragma once

//!	@file Endianess.hpp Ensures the engine/unit is being compiled on a little endian architecture

namespace CYB {
	namespace API {
		//! @brief Static Endianess definitions
		namespace Endianess {
			//! @brief Endianess types
			enum class Type : unsigned int {
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
			/*!
				@brief Get the endianess being compiled
				@return The Type of endianess being compiled
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			constexpr Type Get() {
				return
					((1 & 0xFFFFFFFF) == static_cast<unsigned int>(Type::LITTLE_ENDIAN))
					? Type::LITTLE_ENDIAN
					: ((1 & 0xFFFFFFFF) == static_cast<unsigned int>(Type::BIG_ENDIAN))
					? Type::BIG_ENDIAN
					: ((1 & 0xFFFFFFFF) == static_cast<unsigned int>(Type::PDP_ENDIAN))
					? Type::PDP_ENDIAN
					: Type::UNKNOWN_ENDIAN;
			}
#ifdef TARGET_OS_WINDOWS
#pragma warning(pop)
#endif
			static_assert(Get() == Type::LITTLE_ENDIAN, "CyberEngineMkIII relies on a little endian architecture");
		};
	};
};