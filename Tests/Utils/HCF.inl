#pragma once

inline void CYB::API::Assert::HCF[[noreturn]](void) noexcept {
	FAIL("HCF was called");
	std::terminate();
}