#include "TestHeader.hpp"

void CYB::API::Assert::HCF[[noreturn]](void) noexcept {
	FAIL("HCF was called");
	std::terminate();
}

struct unimplEx{
	std::string FData;
};

CATCH_TRANSLATE_EXCEPTION(unimplEx& ex) {
	return ex.FData;
}

void CYB::API::Assert::Unimplemented(const char* const AFunction, const char* const AFile, const unsigned int ALine) {
	throw unimplEx{ std::string("Call to unimplemented function \"") + AFunction + "\" in file \"" + AFile + "\" at line: " + std::to_string(ALine) };
}