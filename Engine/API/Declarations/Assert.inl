//! @file Assert.inl Implements API assertion functions
#pragma once

#ifndef ASSERTION_OVERRIDE
inline void CYB::API::Assert::HCF[[noreturn]](void){
#ifdef DEBUG
	BREAK;
#endif
#ifdef TARGET_OS_WINDOWS
	__assume(false);
#else
	__builtin_unreachable();
#endif
}
#endif
inline void CYB::API::Assert::True(const bool AExpression) {
	if (!AExpression)
		HCF();
}

inline void CYB::API::Assert::False(const bool AExpression) {
	if (AExpression)
		HCF();
}

template <typename AType> void CYB::API::Assert::Equal(const AType& ALHS, const AType& ARHS) {
	if (ALHS != ARHS)
		HCF();
}

template <typename AType> void CYB::API::Assert::NotEqual(const AType& ALHS, const AType& ARHS) {
	if (ALHS == ARHS)
		HCF();
}

template <typename AType> void CYB::API::Assert::LessThan(const AType& ALHS, const AType& ARHS) {
	if (ALHS >= ARHS)
		HCF();
}

template <typename AType> void CYB::API::Assert::LessThanOrEqual(const AType& ALHS, const AType& ARHS) {
	if (ALHS > ARHS)
		HCF();
}