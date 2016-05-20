#pragma once

inline CYB::API::Interop::Allocator::Allocator(Heap& AHeap):
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

template <class AType> CYB::API::Object<AType> CYB::API::Interop::Allocator::CopyObject(const Object<AType>& ACopy) {
	return Object<AType>(new (FHeap.Alloc(sizeof(AType))) AType(ACopy()));
}

inline CYB::API::Interop::Allocator& CYB::API::Interop::Allocator::GetAllocator(void) {
	return *FAllocator;
}

inline CYB::API::Interop::Allocator& CYB::Allocator(void) {
	return API::Interop::Allocator::GetAllocator();
}