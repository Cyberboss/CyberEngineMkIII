#pragma once

//keep it function local

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
	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AObject)), *this);
	using namespace std;
	auto Result(InPlaceAllocation<AObject>(Buf.FBuffer, forward<AArgs>(AArguments)...));
	Buf.FBuffer = nullptr;
	return Result;
}

template <class AObject, class AConstructor, typename... AArgs> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::ConstructObject(AArgs&&... AArguments) {
	static_assert(!std::is_abstract<AObject>::value || std::is_same<Constructor<AArgs...>, AConstructor>::value, "Allocatable arguments do not match");
	return Object<AObject>(AllocateObject<AObject, AConstructor>(std::is_abstract<AObject>::type(), std::forward<AArgs>(AArguments)...));
}

template <class AObject, typename... AArgs> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::NewObject(AArgs&&... AArguments) {
	return ConstructObject<AObject, typename AObject::Constructor>(std::forward<AArgs>(AArguments)...);
}

template <class AObject> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::NewObject(void) {
	static_assert(!std::is_abstract<AObject>::value || std::is_same<EmptyConstructor, typename AObject::Constructor>::value, "Allocatable arguments do not match");
	return Object<AObject>(AllocateObject<AObject, EmptyConstructor>(std::is_abstract<AObject>::type()));
}

template <typename AType, typename... AArgs> AType* CYB::API::Interop::Allocator::InPlaceAllocation(void* const ALocation, AArgs&&... AArguments) {
	Assert::NotEqual<void*>(ALocation, nullptr);
	return new (ALocation) AType(std::forward<AArgs>(AArguments)...);
}

template <class AObject> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::CopyObject(const AObject& ACopy) {
	return ConstructObject<AObject, typename AObject::CopyConstructor>(ACopy);
}

template <class AObject> void CYB::API::Interop::Allocator::DeleteObject(AObject* const AAllocated) noexcept {
	if (AAllocated != nullptr) {
		AAllocated->~AObject();
		FHeap.Free(AAllocated);
	}
}