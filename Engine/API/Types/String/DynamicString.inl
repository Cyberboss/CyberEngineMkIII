#pragma once

inline CYB::API::String::Dynamic::Dynamic() noexcept :
	CStyle(nullptr)
{}

inline CYB::API::String::Dynamic::Dynamic(char* const AData) noexcept :
	CStyle(AData)
{}

inline CYB::API::String::Dynamic::Dynamic(const char* const AData, const int ALength) :
	CStyle(CopyCStyle(Static(AData), ALength))
{}

inline CYB::API::String::Dynamic::Dynamic(const CStyle& AData, const int ALength) :
	CStyle(CopyCStyle(AData, ALength))
{}

inline CYB::API::String::Dynamic::Dynamic(const Dynamic& ACopy) :
	Dynamic(static_cast<const CStyle&>(ACopy))
{}

inline CYB::API::String::Dynamic::Dynamic(Dynamic&& AMove) noexcept :
	CStyle(AMove.FData)
{
	AMove.FData = nullptr;
}

inline CYB::API::String::Dynamic& CYB::API::String::Dynamic::operator=(Dynamic&& AMove) noexcept {
	DeallocateData();
	FData = AMove.FData;
	FLength = AMove.FLength;
	AMove.FData = nullptr;
	return *this;
}

inline CYB::API::String::Dynamic::~Dynamic() {
	DeallocateData();
}

inline char* CYB::API::String::Dynamic::CopyCStyle(const CStyle& AData, int ALength) {
	Assert::LessThanOrEqual(ALength, AData.RawLength());
	if (ALength == -1)
		ALength = AData.RawLength();
	if (AData.RawLength() > 0) {
		Assert::LessThan(AData.RawLength(), std::numeric_limits<int>::max());
		auto Data(static_cast<char*>(Allocator().FHeap.Alloc(static_cast<int>(ALength + 1))));
		std::copy(AData.CString(), AData.CString() + ALength + 1, Data);
		return Data;
	}
	return nullptr;
}

template <typename ALambda> bool CYB::API::String::Dynamic::BuildAndPopulateBuffer(const int ASize, const ALambda APopulateData, Dynamic& ADynamic) {
	auto Data(static_cast<char*>(Allocator().FHeap.Alloc(ASize + 1)));
	if (APopulateData(Data)) {
		Data[ASize] = 0;
		ADynamic = Dynamic(Data);
		return true;
	}
	return false;
}

inline void CYB::API::String::Dynamic::DeallocateData(void) {
	Allocator().FHeap.Free(FData);
	FData = nullptr;
}

inline CYB::API::String::Dynamic CYB::API::String::Dynamic::operator+(const CStyle& ARHS) const {
	const auto TotalLength(static_cast<unsigned long long>(RawLength() + ARHS.RawLength()));
	if (TotalLength > 0) {
		Assert::LessThanOrEqual(TotalLength + 1, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
		auto Data(static_cast<char*>(Allocator().FHeap.Alloc(static_cast<int>(TotalLength + 1))));
		std::copy(CString(), CString() + RawLength(), Data);
		std::copy(ARHS.CString(), ARHS.CString() + ARHS.RawLength() + 1, Data + RawLength());
		return Dynamic(Data);
	}
	return Dynamic();
}

inline CYB::API::String::Dynamic& CYB::API::String::Dynamic::operator+=(const CStyle& ARHS) {
	if (ARHS.RawLength() > 0) {
		const auto TotalSize(static_cast<unsigned long long>(RawLength() + ARHS.RawLength() + 1));
		Assert::LessThanOrEqual(TotalSize, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
		FData = static_cast<char*>(Allocator().FHeap.Realloc(FData, static_cast<int>(TotalSize)));
		std::copy(ARHS.CString(), ARHS.CString() + ARHS.RawLength() + 1, FData + RawLength());
	}
	return *this;
}

inline CYB::API::String::Dynamic CYB::API::String::Dynamic::SubString(const int AIndex, const int ALength) const {
	return Dynamic(CString() + AIndex, ALength);
}

inline void CYB::API::String::Dynamic::Shrink(const int AMaxBytes) noexcept {
	Assert::LessThan(-1, AMaxBytes);
	if (AMaxBytes == 0)
		DeallocateData();
	else if (RawLength() > AMaxBytes + 1) 
		FData[AMaxBytes] = 0;
	FLength = CalculateByteLength();
}

inline int CYB::API::String::Dynamic::Length(void) const noexcept {
	return RawLength();
}