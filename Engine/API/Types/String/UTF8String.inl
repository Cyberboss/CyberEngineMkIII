#pragma once

inline CYB::API::String::UTF8::UTF8(Dynamic&& AData) :
	Dynamic(std::move(AData))
{
	if (!Validate(*this))
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
}

inline CYB::API::String::UTF8::UTF8(char* const AData) :
	Dynamic(AData)
{
	if (!Validate(*this))
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
}

inline CYB::API::String::UTF8::UTF8(const CStyle& AData) :
	Dynamic(AData)
{
	if (!Validate(*this))
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
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
	return *this;
}

inline int CYB::API::String::UTF8::Length(void) const noexcept {
	auto Length(0);
	for (auto Current(CString()); *Current != 0; Length += (*Current++ & 0xC0) != 0x80 ? 1 : 0);
	return Length;
}