//! @file Expressions.inl Implements constexprs found in Expressions.hpp
#pragma once

constexpr unsigned long long CYB::API::ByteConverters::Terabytes(const unsigned long long ANumber) {
	return Gigabytes(ANumber * 1000);
}
constexpr unsigned long long CYB::API::ByteConverters::Gigabytes(const unsigned long long ANumber) {
	return Megabytes(ANumber * 1000);
}
constexpr unsigned long long CYB::API::ByteConverters::Megabytes(const unsigned long long ANumber) {
	return Kilobytes(ANumber * 1000);
}
constexpr unsigned long long CYB::API::ByteConverters::Kilobytes(const unsigned long long ANumber) {
	return ANumber * 1000;
}