#pragma once

inline CYB::API::Interop::Allocator::Allocator(Heap& AHeap) noexcept :
	FHeap(AHeap)
{
	FAllocator = this;
}

template <class AType> CYB::API::Interop::Object<AType> CYB::API::Interop::Allocator::NewObject(void) {
	static_assert(!std::is_abstract<AType>::value 
		|| std::is_same<Interop::EmptyConstructor, typename AType::Constructor>::value,
		"Allocatable arguments do not match");
	if (std::is_abstract<AType>::value) {
		Interop::EmptyConstructor Constructor;
		return Object<AType>(NewObject(Interop::Allocatable::GetID<AType>(), Constructor));
	}
	return Object<AType>(InPlaceAllocation<AType>(FHeap.Alloc(sizeof(AType))));
}

template <class AType, typename... AArgs> CYB::API::Interop::Object<AType> CYB::API::Interop::Allocator::NewObject(AArgs&&... AArguments) {
	static_assert(!std::is_abstract<AType>::value 
		|| std::is_same<Interop::Constructor<AArgs...>, typename AType::Constructor>::value,
		"Allocatable arguments do not match");
	if (std::is_abstract<AType>::value) {
		Interop::Constructor<AArgs...> Constructor(std::forward<AArgs>(AArguments)...);
		return Object<AType>(NewObject(Interop::Allocatable::GetID<AType>(), Constructor));
	}
	return Object<AType>(InPlaceAllocation<AType>(FHeap.Alloc(sizeof(AType)), std::forward<AArgs>(AArguments)...));
}

inline CYB::API::Interop::Allocator& CYB::API::Interop::Allocator::GetAllocator(void) noexcept {
	return *FAllocator;
}

inline CYB::API::Interop::Allocator& CYB::Allocator(void) noexcept {
	return API::Interop::Allocator::GetAllocator();
}

template <typename AType, typename... AArgs> AType* CYB::API::Interop::Allocator::InPlaceAllocation(void* const ALocation, AArgs&&... AArguments) {
	Assert::NotEqual<void*>(ALocation, nullptr);
	return new (ALocation) AType(std::forward<AArgs>(AArguments)...);
}

inline void CYB::API::Interop::Allocator::Delete(Allocatable* const AAllocatable) noexcept {
	if (AAllocatable != nullptr) {
		AAllocatable->~Allocatable();
		FHeap.Free(AAllocatable);
	}
}