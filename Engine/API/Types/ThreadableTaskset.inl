#pragma once

CYB::API::ThreadableTaskset::ThreadableTaskset(const unsigned int ANumTasks) noexcept:
	FNumTasks(ANumTasks),
	FCancelFlag(false)
{}

void CYB::API::ThreadableTaskset::BeginThreadedOperation(void) {
	for (unsigned int I(0); I < FNumTasks && !FCancelFlag.load(std::memory_order_relaxed); ++I) {
		if (!DoTask(I))
			break;
		FCancelFlag.store(false, std::memory_order_release);
	}
}

void CYB::API::ThreadableTaskset::CancelThreadedOperation(void) noexcept {
	FCancelFlag.store(true, std::memory_order_relaxed);
}