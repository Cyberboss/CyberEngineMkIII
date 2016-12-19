#pragma once

#ifdef CYB_UNIT_IMPLEMENTATION
CYB::API::Interop::Context* CYB::API::Interop::Context::FContext;
#endif

inline CYB::API::Interop::Context& CYB::API::Context(void) noexcept {
	return Interop::Context::GetContext();
}

inline CYB::API::Interop::Context::Context(Allocator& AAllocator, Logger& ALogger) noexcept :
	FAllocator(AAllocator),
	FLogger(ALogger)
{}

#ifndef CORE_DEFINED_CONTEXT
inline CYB::API::Interop::Context& CYB::API::Interop::Context::GetContext(void) noexcept {
	return *FContext;
}
#endif