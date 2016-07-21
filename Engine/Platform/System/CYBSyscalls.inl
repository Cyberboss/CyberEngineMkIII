#pragma once

template <typename... Args> unsigned long long CYB::Platform::System::Sys::Call(const CallNumber ACallNumber, Args&&... AArgs) noexcept {
	if (CallRedirected(ACallNumber))
		return RedirectedCall(ACallNumber, std::forward<Args>(AArgs)...);
#ifndef TARGET_OS_WINDOWS
		if (ACallNumber >= 0) {
			VerifyArgumentCount(ACallNumber, sizeof...(AArgs));
			return DoCall(ACallNumber, std::forward<Args>(AArgs)...);
		}
		else 
#endif
	return LinkedCall(ACallNumber, std::forward<Args>(AArgs)...);
}