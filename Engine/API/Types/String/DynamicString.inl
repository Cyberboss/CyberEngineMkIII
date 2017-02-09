#pragma once

inline CYB::API::String::Dynamic::Dynamic() noexcept :
	CStyle(nullptr, -1)
{}

inline CYB::API::String::Dynamic::Dynamic(const unsigned long long AValue) :
	Dynamic(MakeNumberString(AValue, false))
{}

inline CYB::API::String::Dynamic::Dynamic(const long long AValue) :
	Dynamic(MakeNumberString(static_cast<unsigned long long>(AValue < 0 ? -AValue : AValue), AValue < 0))
{}

inline CYB::API::String::Dynamic::Dynamic(char* const AData) noexcept :
	CStyle(AData, -1)
{}

inline CYB::API::String::Dynamic::Dynamic(const char* const AData, const int ALength) :
	CStyle(CopyCStyle(Static(AData), ALength), ALength)
{}

inline CYB::API::String::Dynamic::Dynamic(const CStyle& AData, const int ALength) :
	CStyle(CopyCStyle(AData, ALength), ALength)
{}

inline CYB::API::String::Dynamic::Dynamic(const Dynamic& ACopy) :
	Dynamic(ACopy, ACopy.RawLength())
{}

inline CYB::API::String::Dynamic::Dynamic(Dynamic&& AMove) noexcept :
	CStyle(AMove.FData, AMove.FLength)
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
	API::Assert::LessThanOrEqual(-1, ALength);
	Assert::LessThanOrEqual(ALength, AData.RawLength());
	if (ALength == -1)
		ALength = AData.RawLength();
	if (ALength != 0 && AData.RawLength() > 0) {
		Assert::LessThan(AData.RawLength(), std::numeric_limits<int>::max());
		auto Data(static_cast<char*>(Context().FAllocator.FHeap.Alloc(static_cast<int>(ALength + 1))));
		std::copy(AData.CString(), AData.CString() + ALength, Data);
		Data[ALength] = 0;
		return Data;
	}
	return nullptr;
}

inline CYB::API::String::Dynamic CYB::API::String::Dynamic::MakeNumberString(const unsigned long long AValue, const bool ANegative) {
	const auto BufferLength(24); //Max possible size is 20 or something, up it to the 64 bit boundary
	char Buffer[BufferLength];
	const auto Length(sprintf(Buffer, u8"-%llu", AValue));
	Assert::LessThan(0, Length);
	Assert::LessThanOrEqual(Length, BufferLength);
	//single buffer trickery
	if(ANegative)
		return Dynamic(Buffer, Length);
	return Dynamic(Buffer + 1, Length - 1);
}

template <typename ALambda> bool CYB::API::String::Dynamic::BuildAndPopulateBuffer(const int ASize, const ALambda APopulateData, Dynamic& ADynamic) {
	auto Data(static_cast<char*>(Context().FAllocator.FHeap.Alloc(ASize + 1)));
	if (APopulateData(Data)) {
		Data[ASize] = 0;
		ADynamic = Dynamic(Data);
		return true;
	}
	return false;
}

inline void CYB::API::String::Dynamic::DeallocateData(void) {
	Context().FAllocator.FHeap.Free(FData);
	FData = nullptr;
}

inline CYB::API::String::Dynamic CYB::API::String::Dynamic::operator+(const CStyle& ARHS) const {
	if (ARHS.CString() == nullptr)
		return *this;
	else if (CString() == nullptr)
		return Dynamic(ARHS);

	const auto TotalLength(static_cast<unsigned long long>(RawLength() + ARHS.RawLength()));
	Assert::LessThanOrEqual(TotalLength + 1, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	auto Data(static_cast<char*>(Context().FAllocator.FHeap.Alloc(static_cast<int>(TotalLength + 1))));
	std::copy(CString(), CString() + RawLength(), Data);
	std::copy(ARHS.CString(), ARHS.CString() + ARHS.RawLength() + 1, Data + RawLength());
	return Dynamic(Data);
}

inline CYB::API::String::Dynamic& CYB::API::String::Dynamic::operator+=(const CStyle& ARHS) {
	if (ARHS.RawLength() > 0) {
		const auto TotalSize(static_cast<unsigned long long>(RawLength() + ARHS.RawLength() + 1));
		Assert::LessThanOrEqual(TotalSize, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
		FData = static_cast<char*>(Context().FAllocator.FHeap.Realloc(FData, static_cast<int>(TotalSize)));
		std::copy(ARHS.CString(), ARHS.CString() + ARHS.RawLength() + 1, FData + RawLength());
		FLength = static_cast<int>(TotalSize) - 1;
	}
	return *this;
}

inline CYB::API::String::Dynamic CYB::API::String::Dynamic::SubString(const int AIndex, const int ALength) const {
	return Dynamic(CString() + AIndex, ALength);
}

inline CYB::API::Container::Deque<CYB::API::String::Dynamic> CYB::API::String::Dynamic::Tokenize(const char ASeparator) const {
	Container::Deque<Dynamic> Work;
	auto Last(0);
	const auto AddRange([&](const int AStart, const int AEnd) {
		Work.emplace_back(SubString(AStart, AEnd - Last));
		Last = AEnd + 1;
	});
	for (auto I(0); I < RawLength(); ++I)
		if (CString()[I] == ASeparator)
			AddRange(Last, I);
	AddRange(Last, RawLength());
	return Work;
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

inline CYB::API::String::Dynamic CYB::API::String::Dynamic::FromData(char* const AData) noexcept {
	return Dynamic(AData);
}