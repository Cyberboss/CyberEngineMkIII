#pragma once

inline CYB::API::Interop::Allocator::Allocator(Heap& AHeap) noexcept :
	FHeap(AHeap)
{}

template <class AObject, class AConstructor, typename... AArgs> AObject* CYB::API::Interop::Allocator::AllocateObject(const std::true_type AIgnored, AArgs&&... AArguments) {
	static_cast<void>(AIgnored);
	AConstructor Construction(std::forward<AArgs>(AArguments)...);
	return static_cast<AObject*>(InteropAllocation(Allocatable::GetID<AObject>(), Construction));
}

template <class AObject, class AConstructor, typename... AArgs> AObject* CYB::API::Interop::Allocator::AllocateObject(const std::false_type AIgnored, AArgs&&... AArguments) {
	static_cast<void>(AIgnored);
	UniquePointer<void> Buf(FHeap.Alloc(sizeof(AObject)));
	auto Result(InPlaceAllocation<AObject>(Buf.get(), std::forward<AArgs>(AArguments)...));
	Buf.release();
	return Result;
}

template <class AObject, class AConstructor, typename... AArgs> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::ConstructObject(AArgs&&... AArguments) {
	static_assert(!std::is_abstract<AObject>::value || std::is_same<Constructor<AArgs...>, AConstructor>::value, "Allocatable arguments do not match");
	using AbstractType = std::is_abstract<AObject>;
	return Object<AObject>(AllocateObject<AObject, AConstructor>(typename AbstractType::type(), std::forward<AArgs>(AArguments)...));
}

template <class AObject, typename... AArgs> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::NewObject(AArgs&&... AArguments) {
	return ConstructObject<AObject, typename AObject::Constructor>(std::forward<AArgs>(AArguments)...);
}

template <class AObject> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::NewObject(void) {
	static_assert(!std::is_abstract<AObject>::value || std::is_same<EmptyConstructor, typename AObject::Constructor>::value, "Allocatable arguments do not match");
	using AbstractType = std::is_abstract<AObject>;
	return Object<AObject>(AllocateObject<AObject, EmptyConstructor>(typename AbstractType::type()));
}

template <typename AType, typename... AArgs> AType* CYB::API::Interop::Allocator::InPlaceAllocation(void* const ALocation, AArgs&&... AArguments) {
	Assert::NotEqual<void*>(ALocation, nullptr);
	return new (ALocation) AType(std::forward<AArgs>(AArguments)...);
}

template <class AObject> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::CopyObject(const AObject& ACopy) {
	return ConstructObject<AObject, typename AObject::CopyConstructor>(ACopy);
}

template <typename AObject> void CYB::API::Interop::Allocator::DeleteObject(AObject* const AAllocated) noexcept {
	if (AAllocated != nullptr) {
		AAllocated->~AObject();
		FHeap.Free(AAllocated);
	}
}