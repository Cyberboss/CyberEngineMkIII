#pragma once

inline CYB::API::Interop::Allocator::Allocator(Heap& AHeap) noexcept :
	FHeap(AHeap)
{
	FAllocator = this;
}

template <class AType> CYB::API::Object<AType> CYB::API::Interop::Allocator::NewObject(void) {
	Interop::EmptyConstructor Constructor;
	return Object<AType>(NewObject(Interop::Allocatable::GetID<AType>(), Constructor));
}

template <class AType, typename... AArgs> CYB::API::Object<AType> CYB::API::Interop::Allocator::NewObject(AArgs&&... AArguments) {
	static_assert(ParameterPack<AArgs...>::template PPEqual<AType::ConstructorArguments>(), "Allocatable arguments do not match");
	Interop::Constructor<AArgs...> Constructor(std::forward<AArgs>(AArguments)...);
	return Object<AType>(NewObject(Interop::Allocatable::GetID<AType>(), Constructor));
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