#include "TestHeader.hpp"

void CYB::API::Assert::HCF[[noreturn]](void) noexcept {
	FAIL("HCF was called");
}

void CYB::API::Assert::Unimplemented(const char* const AFunction, const char* const AFile, const unsigned int ALine) noexcept {
	WARN(std::string("Call to unimplemented function \"") + AFunction + "\" in file \"" + AFile + "\" at line: " + std::to_string(ALine));
}