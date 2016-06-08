#include "TestHeader.hpp"

static std::atomic_uint_fast32_t FHCFCalled(0);

void CYB::API::Assert::HCF(void) {
	std::terminate();
}

unsigned int CheckHCF(void) {
	return FHCFCalled.exchange(0);
}