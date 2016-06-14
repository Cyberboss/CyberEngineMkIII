#pragma once

inline void CYB::API::Assert::HCF[[noreturn]](void) noexcept {
	BREAK;
	std::terminate();
}