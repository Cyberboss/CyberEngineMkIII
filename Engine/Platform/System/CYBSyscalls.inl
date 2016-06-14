#pragma once

template <typename... Args> unsigned long long CYB::Platform::System::Sys::Call(const CallNumber ACallNumber, Args&&... AArgs) noexcept {
	VerifyArgumentCount(ACallNumber, sizeof...(AArgs));
	return DoCall(ACallNumber, std::forward<Args>(AArgs)...);
}