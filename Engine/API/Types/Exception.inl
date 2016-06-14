//! @file Exception.inl Implements functions from CYB::Exception
#pragma once

inline CYB::Exception::Base::Base(API::String::Static&& AMessage, const unsigned int AErrorCode, const Level ALevel) noexcept :
	FMessage(std::move(AMessage)),
	FErrorCode(AErrorCode),
	FLevel(ALevel)
{}