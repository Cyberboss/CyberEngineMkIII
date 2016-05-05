//! @file Assert.inl Implements API assertion functions
#pragma once
inline void CYB::API::HCF[[noreturn]](void){
#ifdef DEBUG
	BREAK;
#endif
#ifdef TARGET_OS_WINDOWS
	__assume(false);
#else
	__builtin_unreachable();
#endif
}
inline void CYB::API::Assert(const bool AExpression) {
	if (!AExpression)
		HCF();
}