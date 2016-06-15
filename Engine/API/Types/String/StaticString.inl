//! @file StaticString.inl Implements functions from CYB::API::String::Static
#pragma once

inline CYB::API::String::Static::Static() noexcept :
	Static("")
{}

inline CYB::API::String::Static::Static(const char* const AData) noexcept :
	CStyle(const_cast<char*>(AData))	//Bjarne forgive us
{}

inline unsigned int CYB::API::String::Static::Length(void) const noexcept {
	return RawLength();
}