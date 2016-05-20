#pragma once

inline CYB::API::Allocator::Allocator(Heap& AHeap):
	FHeap(AHeap)
{
	FAllocator = this;
}

template <class AType> CYB::API::Object<AType> CYB::API::Allocator::NewObject(void) {
	Interop::EmptyConstructor Constructor;
	return Object<AType>(NewObject(Interop::Allocatable::GetID<AType>(), Constructor));
}

template <class AType, typename... AArgs> CYB::API::Object<AType> CYB::API::Allocator::NewObject(AArgs&&... AArguments) {
	static_assert(ParameterPack<AArgs...>::template PPEqual<AType::ConstructorArguments>(), "Allocatable arguments do not match");
	Interop::Constructor<AArgs...> Constructor(std::forward<AArgs>(AArguments)...);
	return Object<AType>(NewObject(Interop::Allocatable::GetID<AType>(), Constructor));
}

template <class AType> CYB::API::Object<AType> CYB::API::Allocator::CopyObject(const Object<AType>& ACopy) {
	return Object<AType>(new (FHeap.Alloc(sizeof(AType))) AType(ACopy()));
}

inline CYB::API::Allocator& CYB::API::Allocator::GetAllocator(void) {
	return *FAllocator;
}

inline CYB::API::Allocator& CYB::Allocator(void) {
	return API::Allocator::GetAllocator();
}