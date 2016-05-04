//! @file CStyleString.inl Implements functions from CYB::API::String::CStyle
#pragma once

inline CYB::API::String::CStyle::CStyle(char* const AData) :
	FData(AData),
	FLength(CalculateByteLength())
{}

inline unsigned long long CYB::API::String::CStyle::CalculateByteLength(void) const {
	unsigned long long Length(0);
	for (; FData[Length] != 0; ++Length);
	return Length;
}

inline char* CYB::API::String::CStyle::CString(void) {
	return FData;
}

inline const char* CYB::API::String::CStyle::CString(void) const {
	return FData;
}

inline unsigned long long CYB::API::String::CStyle::RawLength(void) const {
	return FLength;
}