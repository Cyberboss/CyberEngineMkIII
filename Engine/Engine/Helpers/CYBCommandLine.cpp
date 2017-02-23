//! @file CYBCommandLine.cpp Implements CYB::Engine::Helpers::CommandLine
#include "CYB.hpp"

CYB::Engine::Helpers::CommandLine::CommandLine(const unsigned int ANumArguments, const oschar_t* const* const AArguments) :
	FTokens(ParseTokens(ANumArguments, AArguments))
{}

CYB::API::Container::Vector<CYB::Engine::Helpers::CommandLine::Token> CYB::Engine::Helpers::CommandLine::ParseTokens(const unsigned int ANumArguments, const oschar_t* const* const AArguments) {
	CYB::API::Container::Vector<CYB::Engine::Helpers::CommandLine::Token> Result;
	Result.reserve(ANumArguments);

	//Make no assumptions about argv[0]
	for (auto I(0U); I < ANumArguments; ++I) {
		API::String::Dynamic AsUTF(
#ifdef TARGET_OS_WINDOWS	//Do this nowhere else in the engine tree
			API::String::UTF16::ToUTF8(AArguments[I])
#else
			AArguments[I]
#endif
		);
		TokenType TT;
		if (AsUTF.CString()[0] == '-') {
			if (AsUTF.CString()[1] == '-') {
				TT = TokenType::EXTENDED_KEY;
				AsUTF = AsUTF.SubString(2, -1);
			}
			else {
				TT = TokenType::SINGLE_LETTER_KEY;
				AsUTF = AsUTF.SubString(1, -1);
			}
		}
		else if (AsUTF.CString()[0] == '/') {
			TT = TokenType::SINGLE_LETTER_KEY;
			AsUTF = AsUTF.SubString(1, -1);
		}
		else
			TT = TokenType::NORMAL;
		Result.emplace_back(Token{ std::move(AsUTF), TT });
	}

	return Result;
}