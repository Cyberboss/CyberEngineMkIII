//! @file CStyleString.inl Implements functions from CYB::API::String::CStyle
#pragma once

inline CYB::API::String::CStyle::CStyle(char* const AData) :
	FData(AData),
	FLength(CalculateByteLength())
{}

inline unsigned int CYB::API::String::CStyle::CalculateByteLength(void) const {
	unsigned int Length(0);
	for (; FData[Length] != 0; ++Length)
		Assert(Length < std::numeric_limits<unsigned int>().max());
	return Length;
}

inline const char* CYB::API::String::CStyle::CString(void) const {
	return FData;
}

inline unsigned int CYB::API::String::CStyle::RawLength(void) const {
	return FLength;
}