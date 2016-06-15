//! @file CStyleString.inl Implements functions from CYB::API::String::CStyle
#pragma once

inline CYB::API::String::CStyle::CStyle(char* const AData) noexcept :
	FData(AData),
	FLength(CalculateByteLength())
{}

inline unsigned int CYB::API::String::CStyle::CalculateByteLength(void) const noexcept {
	unsigned int Length(0);
	if (FData != nullptr)
		for (; FData[Length] != 0; ++Length)
			Assert::LessThan<unsigned int>(Length, std::numeric_limits<unsigned int>().max());
	return Length;
}

inline const char* CYB::API::String::CStyle::CString(void) const noexcept {
	return FData;
}

inline unsigned int CYB::API::String::CStyle::RawLength(void) const noexcept {
	return FLength;
}