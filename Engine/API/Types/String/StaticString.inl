//! @file StaticString.inl Implements functions from CYB::API::String::Static
#pragma once

inline CYB::API::String::Static::Static() noexcept :
	Static(nullptr)
{}

inline CYB::API::String::Static::Static(const char* const AData) noexcept :
	CStyle(const_cast<char*>(AData), -1)	//Valid af since the data is immutable
{}

inline int CYB::API::String::Static::Length(void) const noexcept {
	return RawLength();
}