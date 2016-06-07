#include "TestHeader.hpp"

static std::atomic_uint_fast32_t FHCFCalled(0);

void CYB::API::HCF(void) {
	++FHCFCalled;
}

void CYB::API::Assert(const bool AExpression) {
	if (!AExpression)
		HCF();
}

unsigned int CheckHCF(void) {
	return FHCFCalled.exchange(0);
}