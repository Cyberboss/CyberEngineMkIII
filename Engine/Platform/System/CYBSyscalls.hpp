//! @file CYBSyscalls.hpp Defines the System Call interface
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			//Note, keep public inheritance for call number access
			class Sys : public Implementation::Sys {
			public:
				union Union64 {
				public:
					void* FPointer;
					unsigned long long FNumber;
				public:
					Union64(void* const APointer) :
						FPointer(APointer)
					{}
					Union64(const unsigned long long ANumber) :
						FNumber(ANumber)
					{}
				};
			private:
				static unsigned long long DoCall(const CallNumber ACallNumber);
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1);
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2);
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3);
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4);
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5);
				static unsigned long long DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5, const Union64 AArg6);
				static void VerifyArgumentCount(const CallNumber ACallNumber, const unsigned long long ANumArgs);
			public:
				template <typename... Args> static unsigned long long Call(const CallNumber ACallNumber, Args&&... AArgs);
			};
		};
	};
};

#include "CYBSyscalls.inl"