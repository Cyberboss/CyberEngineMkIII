#pragma once

//keep it function local
#define AFB \
class AutoFreeBuffer {\
public:\
	Allocator& FAllocator;\
	void* FBuffer;\
public:\
	AutoFreeBuffer(void* const ABuffer, Allocator& AAllocator) :\
		FAllocator(AAllocator),\
		FBuffer(ABuffer)\
	{}\
	~AutoFreeBuffer() {\
		if (FBuffer != nullptr)\
			FAllocator.FHeap.Free(FBuffer);\
	}\
}

inline CYB::API::Interop::Allocator::Allocator(Heap& AHeap) noexcept :
	FHeap(AHeap)
{}

template <class AObject, class AConstructor, typename... AArgs> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::NewObject(AArgs&&... AArguments) {
	static_assert(!std::is_abstract<AObject>::value
		|| std::is_same<Constructor<AArgs...>, AConstructor>::value,
		"Allocatable arguments do not match");
	if (std::is_abstract<AObject>::value) {
		AConstructor Construction(std::forward<AArgs>(AArguments)...);
		return Object<AObject>(static_cast<AObject*>(InteropAllocation(Allocatable::GetID<AObject>(), Construction)));
	}
	AFB;
	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AObject)), *this);
	using namespace std;
	Object<AObject> Result(InPlaceAllocation<AObject>(Buf.FBuffer, typename is_abstract<AObject>::type(), std::forward<AArgs>(AArguments)...));
	Buf.FBuffer = nullptr;
	return Result;
}

template <class AObject> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::NewObject(void) {
	static_assert(!std::is_abstract<AObject>::value
		|| std::is_same<EmptyConstructor, typename AObject::Constructor>::value,
		"Allocatable arguments do not match");
	if (std::is_abstract<AObject>::value) {
		Constructor<void> Construction;
		return Object<AObject>(static_cast<AObject*>(InteropAllocation(Allocatable::GetID<AObject>(), Construction)));
	}
	AFB;
	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AObject)), *this);
	using namespace std;
	Object<AObject> Result(InPlaceAllocation<AObject>(Buf.FBuffer, typename is_abstract<AObject>::type()));
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

template <class AObject> CYB::API::Interop::Object<AObject> CYB::API::Interop::Allocator::CopyObject(const AObject& ACopy) {
	static_assert(!std::is_abstract<AObject>::value
		|| std::is_same<Constructor<const AObject&>, typename AObject::CopyConstructor>::value,
		"Allocatable not copyable");
	if (std::is_abstract<AObject>::value) {
		typename AObject::CopyConstructor Construction(ACopy);
		return Object<AObject>(InteropAllocation(Allocatable::GetID<AObject>(), Construction));
	}
	AFB;
	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AObject)), *this);
	Object<AObject> Result(InPlaceAllocation<AObject>(Buf.FBuffer, std::is_abstract<AObject>::type, ACopy));
	Buf.FBuffer = nullptr;
	return Result;
}

template <class AObject> void CYB::API::Interop::Allocator::DeleteObject(AObject* const AAllocated) noexcept {
	if (AAllocated != nullptr) {
		AAllocated->~AObject();
		FHeap.Free(AAllocated);
	}
}
#undef AFB