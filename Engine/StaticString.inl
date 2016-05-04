//! @file StaticString.inl Implements functions from CYB::API::String::Static
#pragma once

inline CYB::API::String::Static::Static(const char* const AData) :
	CStyle(const_cast<char*>(AData))	//Bjarne forgive us
{}

inline CYB::API::String::Static::Static(Static&& AMove) :
	CStyle(std::move(AMove))
{}

inline CYB::API::String::Static& CYB::API::String::Static::operator=(Static&& AMove) {
	FData = AMove.FData;
	FLength = AMove.FLength;
	return *this;
}

inline unsigned int CYB::API::String::Static::Length(void) const {
	return RawLength();
}