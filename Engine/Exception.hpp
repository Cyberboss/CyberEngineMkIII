//! @file Exception.hpp Defines the exception classes of the engine
#pragma once
namespace CYB {
	namespace Exception {
		class Base {
		public:
			enum class Level : byte {
				VIOLATION,
				SYSTEM_DATA,
				INTERNAL,
				UNIT,
				FATAL,
			};
		public:
			const char* const FMessage;
			const unsigned int FErrorCode;
			const Level FLevel;
		protected:
			Base(const char* const AMessage, const unsigned int AErrorCode, const Level ALevel) :
				FMessage(AMessage),
				FErrorCode(AErrorCode),
				FLevel(ALevel)
			{}
		};
		class Violation : public Base {
		public:
			enum ErrorCode : unsigned int {
			};
		public:
			Violation(const ErrorCode AErrorCode);
		};
		class SystemData : public Base{
		public:
			enum ErrorCode : unsigned int {
				THREAD_CREATION_FAILURE,
			};
		public:
			SystemData(const ErrorCode AErrorCode);
		};
		class Internal : public Base {
		public:
			enum ErrorCode : unsigned int {
			};
		public:
			Internal(const ErrorCode AErrorCode);
		};
		class Unit : public Base {
		public:
			enum ErrorCode : unsigned int {
			};
		public:
			Unit(const char* const AMessage, const ErrorCode AErrorCode);
		};
		class Fatal : public Base {
		public:
			enum ErrorCode : unsigned int {
			};
		public:
			Fatal(const ErrorCode AErrorCode);
		};
	};
};