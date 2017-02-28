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
		if (AsUTF.RawLength() > 0) {
			if (AsUTF.CString()[0] == '-') {
				if (AsUTF.RawLength() > 1 && AsUTF.CString()[1] == '-') {
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
	}

	return Result;
}

unsigned int CYB::Engine::Helpers::CommandLine::RunHandler(Callback ACallback, const int AFullNameKey, const int ADescriptionKey, const API::String::CStyle& AShortFlag, const API::String::CStyle& ALongFlag, const unsigned int ANumExpectedTokens, const unsigned int ANumOptionalTokens, const unsigned long long AMaxInvocations) const {
	API::Container::Deque<const API::String::Dynamic*> Work;
	API::String::Dynamic NotEnoughParams(u8"Not enough parameters for command: ");
	auto InvocationCount(0U);
	for (auto I(FTokens.begin()); InvocationCount < AMaxInvocations && I != FTokens.end(); ++I) {
		if ((I->FType == TokenType::SINGLE_LETTER_KEY && I->FEntry == AShortFlag) 
			|| (I->FType == TokenType::EXTENDED_KEY && I->FEntry == ALongFlag)) {
			auto Remaining(ANumExpectedTokens);
			//Make sure we have enough normal tokens to fill requirements
			auto J(I + 1);
			const auto AddRemaining([&]() {
				for (; Remaining > 0 && J != FTokens.end() && J->FType == TokenType::NORMAL; ++J, --Remaining)
					Work.emplace_back(&(J->FEntry));
			});
			AddRemaining();
			if (Remaining == 0) {
				//found all required, check for optional
				Remaining = ANumOptionalTokens;
				AddRemaining();
				if (!ACallback(Work))
					Platform::System::Process::GetSelf().Terminate();	//Stop immediately, no consideration
				++InvocationCount;
			}
			else
				API::Context().FLogger.Log(NotEnoughParams + I->FEntry, API::Logger::Level::WARN);
			Work.clear();
		}
	}
	static_cast<void>(AFullNameKey);
	static_cast<void>(ADescriptionKey);
	return InvocationCount;
}