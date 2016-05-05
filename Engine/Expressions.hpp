//! @file Expressions.hpp Helpful constexprs
#pragma once

namespace CYB {
	namespace API {
		class ByteConverters {
		public:
			/*!
				@brief Get the true byte value of some amount of terabytes
				@param AAmount The number of terabytes
				@return The number of bytes in @p AAmount TB
			*/
			static constexpr unsigned long long Terabytes(const unsigned long long AAmount);
			/*!
				@brief Get the true byte value of some amount of gigabytes
				@param AAmount The number of gigabytes
				@return The number of bytes in @p AAmount GB
			*/
			static constexpr unsigned long long Gigabytes(const unsigned long long AAmount);
			/*!
				@brief Get the true byte value of some amount of megabytes
				@param AAmount The number of megabytes
				@return The number of bytes in @p AAmount MB
			*/
			static constexpr unsigned long long Megabytes(const unsigned long long AAmount);
			/*!
				@brief Get the true byte value of some amount of kilobytes
				@param AAmount The number of kilobytes
				@return The number of bytes in @p AAmount KB
			*/
			static constexpr unsigned long long Kilobytes(const unsigned long long AAmount);
		};
	};
};

#include "Expressions.inl"
