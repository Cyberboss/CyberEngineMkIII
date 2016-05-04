//! @file StaticString.inl Implements functions from CYB::API::String::Static
#pragma once

inline CYB::API::String::Static::Static(char* const AData) :
	CStyle(AData)
{}

inline unsigned long long CYB::API::String::Static::Length(void) const {
	return RawLength();
}