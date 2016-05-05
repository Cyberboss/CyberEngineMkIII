//! @file Endianess.inl Implements functions from CYB::API::Endianess
#pragma once
#ifdef TARGET_OS_WINDOWS
#pragma warning (push)
#pragma warning (disable : 6326)
#endif
constexpr CYB::API::Endianess::Type CYB::API::Endianess::Get(void) {
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
#pragma warning (pop)
#endif