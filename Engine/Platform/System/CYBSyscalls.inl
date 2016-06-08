#pragma once

template <typename... Args> static unsigned long long CYB::Platform::System::Sys::Call(const CallNumber ACallNumber, Args&&... AArgs) {
	VerifyArgumentCount(ACallNumber, sizeof...(AArgs));
	return DoCall(ACallNumber, std::forward<Args>(AArgs)...);
}