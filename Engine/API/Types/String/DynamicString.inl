#pragma once

inline CYB::API::String::Dynamic::Dynamic() noexcept :
	CStyle(nullptr)
{}

inline CYB::API::String::Dynamic::Dynamic(char* const AData) noexcept :
	CStyle(AData)
{}

inline CYB::API::String::Dynamic::Dynamic(const CStyle& AData) :
	CStyle(CopyCStyle(AData))
{}

inline CYB::API::String::Dynamic::Dynamic(const Dynamic& ACopy) :
	Dynamic(static_cast<const CStyle&>(ACopy))
{}

inline CYB::API::String::Dynamic::Dynamic(Dynamic&& AMove) noexcept :
	CStyle(AMove.FData)
{
	AMove.FData = nullptr;
}

inline CYB::API::String::Dynamic& CYB::API::String::Dynamic::operator=(Dynamic&& AMove) noexcept(!Platform::IsDebug()) {
	DeallocateData();
	FData = AMove.FData;
	FLength = AMove.FLength;
	AMove.FData = nullptr;
	return *this;
}

inline CYB::API::String::Dynamic::~Dynamic() {
	DeallocateData();
}

inline char* CYB::API::String::Dynamic::CopyCStyle(const CStyle& AData) {
	if (AData.RawLength() > 0) {
		Assert::LessThan(AData.RawLength(), std::numeric_limits<int>::max());
		auto Data(static_cast<char*>(Allocator().FHeap.Alloc(static_cast<int>(AData.RawLength() + 1))));
		std::copy(AData.CString(), AData.CString() + AData.RawLength() + 1, Data);
		return Data;
	}
	return nullptr;
}

inline void CYB::API::String::Dynamic::DeallocateData(void) {
	Allocator().FHeap.Free(FData);
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
		const auto TotalSize(static_cast<unsigned long long>(ARHS.RawLength() + ARHS.RawLength() + 1));
		Assert::LessThanOrEqual(TotalSize, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
		FData = static_cast<char*>(Allocator().FHeap.Realloc(FData, static_cast<int>(TotalSize)));
		std::copy(ARHS.CString(), ARHS.CString() + ARHS.RawLength() + 1, FData + RawLength());
	}
	return *this;
}

inline int CYB::API::String::Dynamic::Length(void) const noexcept {
	return RawLength();
}