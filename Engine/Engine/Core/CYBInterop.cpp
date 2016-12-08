//! @file CYBInterop.cpp Implements various engine interop functions
#include "CYB.hpp"

//Context

CYB::API::Interop::Context& CYB::API::Interop::Context::GetContext(void) noexcept {
	return Core().CurrentContext();
}

//Allocator
template <class AAllocatable> AAllocatable* CYB::Engine::Allocator::DoAllocation(API::Interop::Constructor<void>& AConstructor) {
	if (!AConstructor.Valid<AAllocatable>())
		throw Exception::Violation(Exception::Violation::INVALID_INTEROP_CONSTRUCTOR);

	auto& TrueConstructor(static_cast<typename AAllocatable::Constructor&>(AConstructor));
	
	class AutoCleanup {
	private:
		API::Heap& FHeap;
	public:
		void* FLocation;
	public:
		AutoCleanup(API::Heap& AHeap) :
			FHeap(AHeap),
			FLocation(AHeap.Alloc(sizeof(AAllocatable)))
		{}
		~AutoCleanup() {
			FHeap.Free(FLocation);
		}
		void* Release(void) noexcept {
			auto Result(FLocation);
			FLocation = nullptr;
			return Result;
		}
	};

	AutoCleanup Location(FHeap);
	TrueConstructor.template Construct<AAllocatable>(Location.FLocation);
	auto Result(static_cast<AAllocatable*>(Location.Release()));
	return Result;
}

void* CYB::Engine::Allocator::InteropAllocation(const API::Interop::Allocatable::ID AID, API::Interop::Constructor<void>& AConstructor) {
	using namespace API::Interop;
	switch (AID) {
	case Allocatable::ID::File: //return DoAllocation<Platform::System::File>(AConstructor);
	case Allocatable::ID::Mutex: return DoAllocation<Platform::System::Mutex>(AConstructor);
	case Allocatable::ID::Path: return DoAllocation<Platform::System::Path>(AConstructor);
	case Allocatable::ID::Semaphore: return DoAllocation<Platform::System::Semaphore>(AConstructor);
	case Allocatable::ID::NULL_ID:
	default:
		throw Exception::Violation(Exception::Violation::INVALID_ENUM);
	}
}

CYB::Engine::Allocator::Allocator(API::Heap& AHeap) noexcept :
	API::Interop::Allocator(AHeap)
{}