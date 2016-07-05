#pragma once

inline CYB::API::String::UTF8::UTF8(Dynamic&& AData) :
	Dynamic(std::move(AData))
{
	if (!Validate(*this))
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
	CalculateLength();
}

inline CYB::API::String::UTF8::UTF8(const CStyle& AData) :
	Dynamic(AData)
{
	if (!Validate(*this))
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
	CalculateLength();
}

inline bool CYB::API::String::UTF8::Validate(const CStyle& AString) noexcept {
	for (auto I(0), IX(AString.RawLength()); I < IX; ++I)
	{
		auto Char(static_cast<unsigned char>(AString.CString()[I]));
		int OneLessNumberOfChars;
		if (0x00 <= Char && Char <= 0x7F)
			OneLessNumberOfChars = 0; // 0bbbbbbb
		else if ((Char & 0xE0) == 0xC0)
			OneLessNumberOfChars = 1; // 110bbbbb
		else if (Char == 0xED && I<(IX - 1) && (static_cast<unsigned char>(AString.CString()[I + 1]) & 0xA0) == 0xA0)
			return false; //U+d800 to U+dfff
		else if ((Char & 0xF0) == 0xE0)
			OneLessNumberOfChars = 2; // 1110bbbb
		else if ((Char & 0xF8) == 0xF0)
			OneLessNumberOfChars = 3; // 11110bbb
		else
			return false;

		for (auto J(0); J < OneLessNumberOfChars && I < IX; ++J) // n bytes matching 10bbbbbb follow ?
			if ((++I == IX) || ((static_cast<unsigned char>(AString.CString()[I]) & 0xC0) != 0x80))
				return false;
	}
	return true;
}

inline CYB::API::String::UTF8 CYB::API::String::UTF8::operator+(const UTF8& ARHS) const {
	return UTF8(Dynamic::operator+(ARHS));
}

inline CYB::API::String::UTF8& CYB::API::String::UTF8::operator+=(const UTF8& ARHS) {
	Dynamic::operator+=(ARHS);
	CalculateLength();
	return *this;
}

inline int CYB::API::String::UTF8::ByteIndexOfChar(const int ACharIndex) const noexcept {
	Assert::LessThan(ACharIndex, Length());
	auto I(0);
	for (auto Count(0); I < RawLength() && Count <= ACharIndex; ++I)
		if ((FData[I] & 0xC0) != 0x80)
			++Count;
	return I - 1;
}

template <typename ALambda> void CYB::API::String::UTF8::IterateCodepoints(const ALambda AIterator) const noexcept(noexcept(AIterator(0,0))) {
	auto ContinuationCount(0U);
	for (auto I(0), Index(0); I < RawLength(); ++I) {
		if ((FData[I] & 0xC0) != 0x80) {
			unsigned int Codepoint(0);
			for (;; --ContinuationCount) {
				reinterpret_cast<char*>(&Codepoint)[ContinuationCount] = FData[I - ContinuationCount];
				if (ContinuationCount == 0)
					break;
			}
			if (!AIterator(Codepoint, Index))
				break;
			++Index;
		}
		else
			++ContinuationCount;
	}
}

inline const char& CYB::API::String::UTF8::operator[](const int AIndex) const noexcept {
	return FData[ByteIndexOfChar(AIndex)];
}

inline void CYB::API::String::UTF8::Shrink(const int AMaxChars) noexcept {
	if (AMaxChars == 0) {
		*this = UTF8();
	}
	else if (Length() > AMaxChars + 1) {
		const auto Index(ByteIndexOfChar(AMaxChars));
		FData[Index] = 0;
		CStyle::FLength = Index;
		FLength = AMaxChars;
		API::Assert::True(Validate(*this));
	}
}

inline int CYB::API::String::UTF8::Length(void) const noexcept {
	return FLength;
}

inline void CYB::API::String::UTF8::CalculateLength(void) noexcept {
	FLength = 0;
	if (RawLength() > 0)
		for (auto Current(CString()); *Current != 0; FLength += (*Current++ & 0xC0) != 0x80 ? 1 : 0);
}

inline CYB::API::String::UTF8 CYB::API::String::UTF8::SubString(const int AIndex, const int ALength) const {
	Assert::LessThan(AIndex, Length());
	Assert::LessThan(AIndex + ALength, Length());
	return UTF8(Dynamic::SubString(ByteIndexOfChar(AIndex), ByteIndexOfChar(AIndex + ALength)));
}