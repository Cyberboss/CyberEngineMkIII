#pragma once

inline CYB::API::Interop::Allocator::Allocator(Heap& AHeap) noexcept :
FHeap(AHeap)
{}

template <class AAllocatable, class AConstructor
#ifndef TARGET_OS_WINDOWS
	= typename AAllocatable::Constructor
#endif
	, typename... AArgs> CYB::API::Interop::Object<AAllocatable> CYB::API::Interop::Allocator::NewObject(AArgs&&... AArguments) {
	static_assert(!std::is_abstract<AAllocatable>::value
		|| std::is_same<Constructor<AArgs...>, AConstructor>::value,
		"Allocatable arguments do not match");
	if (std::is_abstract<AAllocatable>::value) {
		AConstructor Construction(std::forward<AArgs>(AArguments)...);
		return Object<AAllocatable>(static_cast<AAllocatable*>(InteropAllocation(Allocatable::GetID<AAllocatable>(), Construction)));
	}
	class AutoFreeBuffer {
	public:
		Allocator& FAllocator;
		void* FBuffer;
	public:
		AutoFreeBuffer(void* const ABuffer, Allocator& AAllocator) :
			FAllocator(AAllocator),
			FBuffer(ABuffer)
		{}
		~AutoFreeBuffer() {
			if (FBuffer != nullptr)
				FAllocator.FHeap.Free(FBuffer);
		}
	};
	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AAllocatable)), *this);
	using namespace std;
	Object<AAllocatable> Result(InPlaceAllocation<AAllocatable>(Buf.FBuffer, typename is_abstract<AAllocatable>::type(), std::forward<AArgs>(AArguments)...));
	Buf.FBuffer = nullptr;
	return Result;
}

template <class AAllocatable> CYB::API::Interop::Object<AAllocatable> CYB::API::Interop::Allocator::NewObject(void) {
	static_assert(!std::is_abstract<AAllocatable>::value
		|| std::is_same<EmptyConstructor, typename AAllocatable::Constructor>::value,
		"Allocatable arguments do not match");
	if (std::is_abstract<AAllocatable>::value) {
		Constructor<void> Construction;
		return Object<AAllocatable>(static_cast<AAllocatable*>(InteropAllocation(Allocatable::GetID<AAllocatable>(), Construction)));
	}
	class AutoFreeBuffer {
	public:
		Allocator& FAllocator;
		void* FBuffer;
	public:
		AutoFreeBuffer(void* const ABuffer, Allocator& AAllocator) :
			FAllocator(AAllocator),
			FBuffer(ABuffer)
		{}
		~AutoFreeBuffer() {
			if (FBuffer != nullptr)
				FAllocator.FHeap.Free(FBuffer);
		}
	};
	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AAllocatable)), *this);
	using namespace std;
	Object<AAllocatable> Result(InPlaceAllocation<AAllocatable>(Buf.FBuffer, typename is_abstract<AAllocatable>::type()));
	Buf.FBuffer = nullptr;
	return Result;
}

template <typename AType, typename... AArgs> AType* CYB::API::Interop::Allocator::InPlaceAllocation(void* const ALocation, std::false_type AIgnored, AArgs&&... AArguments) {
	static_cast<void>(AIgnored);
	Assert::NotEqual<void*>(ALocation, nullptr);
	return new (ALocation) AType(std::forward<AArgs>(AArguments)...);
}

template <typename AType, typename... AArgs> AType* CYB::API::Interop::Allocator::InPlaceAllocation(void* const, std::true_type, AArgs&&...) noexcept {
	//TODO: Log this error
	Assert::HCF();
}

template <typename AType, typename... AArgs> AType* CYB::API::Interop::Allocator::InPlaceAllocation(void* const ALocation, AArgs&&... AArguments) {
	return InPlaceAllocation<AType, AArgs...>(ALocation, std::false_type(), std::forward<AArgs>(AArguments)...);
}

template <class AAllocatable> CYB::API::Interop::Object<AAllocatable> CYB::API::Interop::Allocator::CopyObject(const AAllocatable& ACopy) {
	static_assert(!std::is_abstract<AAllocatable>::value
		|| std::is_same<Constructor<const AAllocatable&>, typename AAllocatable::CopyConstructor>::value,
		"Allocatable not copyable");
	if (std::is_abstract<AAllocatable>::value) {
		typename AAllocatable::CopyConstructor Construction(ACopy);
		return Object<AAllocatable>(InteropAllocation(Allocatable::GetID<AAllocatable>(), Construction));
	}
	class AutoFreeBuffer {
	public:
		Allocator& FAllocator;
		void* FBuffer;
	public:
		AutoFreeBuffer(void* const ABuffer, Allocator& AAllocator) :
			FAllocator(AAllocator),
			FBuffer(ABuffer)
		{}
		~AutoFreeBuffer() {
			if (FBuffer != nullptr)
				FAllocator.FHeap.Free(FBuffer);
		}
	};
	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AAllocatable)), *this);
	Object<AAllocatable> Result(InPlaceAllocation<AAllocatable>(Buf.FBuffer, std::is_abstract<AAllocatable>::type, ACopy));
	Buf.FBuffer = nullptr;
	return Result;
}
inline void CYB::API::Interop::Allocator::Delete(Allocatable* const AAllocatable) noexcept {
	if (AAllocatable != nullptr) {
		AAllocatable->~Allocatable();
		FHeap.Free(AAllocatable);
	}
}