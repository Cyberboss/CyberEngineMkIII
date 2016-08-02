#include "TestHeader.hpp"

void CYB::API::Assert::HCF[[noreturn]](void) noexcept {
	FAIL("HCF was called");
	std::terminate();
}

void CYB::API::Assert::Unimplemented[[noreturn]](const char* const AFunction, const char* const AFile, const unsigned int ALine) {
	FAIL(std::string("Call to unimplemented function \"") + AFunction + "\" in file \"" + AFile + "\" at line: " + std::to_string(ALine));
	HCF();
}