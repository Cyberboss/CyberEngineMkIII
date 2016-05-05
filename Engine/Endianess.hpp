#pragma once

//!	@file Endianess.hpp Ensures the engine/unit is being compiled on a little endian architecture

namespace CYB {
	namespace API {
		//! @brief Static Endianess definitions
		class Endianess {
		public:
			//! @brief Endianess types
			enum class Type : unsigned int {
				LITTLE_ENDIAN = 0x00000001,
				BIG_ENDIAN = 0x01000000,
				PDP_ENDIAN = 0x00010000,
				UNKNOWN_ENDIAN = 0xFFFFFFFF
			};
		public:
			/*!
				@brief Get the endianess being compiled
				@return The Type of endianess being compiled
			*/
			static constexpr Type Get();
		};
	};
};
#include "Endianess.inl"