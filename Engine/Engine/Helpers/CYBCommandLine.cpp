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

void CYB::Engine::Helpers::CommandLine::RunHandler(Callback ACallback, const int AFullNameKey, const int ADescriptionKey, const API::String::CStyle& AShortFlag, const API::String::CStyle& ALongFlag, const unsigned int ANumExpectedTokens, const unsigned int ANumOptionalTokens, unsigned long long AMaxInvocations) const {
	API::Container::Deque<const API::String::Dynamic*> Work;
	for (auto I(FTokens.begin()); AMaxInvocations > 0 && I != FTokens.end(); ++I) {
		if ((I->FType == TokenType::SINGLE_LETTER_KEY && I->FEntry == AShortFlag) 
			|| (I->FType == TokenType::EXTENDED_KEY && I->FEntry == ALongFlag)) {
			auto Remaining(ANumExpectedTokens);
			//Make sure we have enough normal tokens to fill requirements
			auto J(I + 1);
			for (; Remaining > 0 && J->FType == TokenType::NORMAL && J != FTokens.end(); ++J, --Remaining)
				Work.emplace_back(&(J->FEntry));
			if (Remaining == 0) {
				//found all required, check for optional
				Remaining = ANumOptionalTokens;
				for (; Remaining > 0 && J->FType == TokenType::NORMAL && J != FTokens.end(); ++J, --Remaining)
					Work.emplace_back(&(J->FEntry));
				if(!ACallback(Work))
					Platform::System::Process::GetSelf().Terminate();	//Stop immediately, no consideration
				--AMaxInvocations;
			}
			Work.clear();
		}
	}
}