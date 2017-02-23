//! @file CYBCommandLine.cpp Implements CYB::Engine::Helpers::CommandLine
#include "CYB.hpp"

CYB::Engine::Helpers::CommandLine::CommandLine(const unsigned int ANumArguments, const oschar_t* const* const AArguments) :
	FTokens(ParseTokens(ANumArguments, AArguments))
{}

CYB::API::Container::Vector<CYB::Engine::Helpers::CommandLine::Token> CYB::Engine::Helpers::CommandLine::ParseTokens(const unsigned int ANumArguments, const oschar_t* const* const AArguments) {
	CYB::API::Container::Vector<CYB::Engine::Helpers::CommandLine::Token> Result;
	Result.reserve(ANumArguments + 1);

	using OSCharConverter =
#ifdef TARGET_OS_WINDOWS	//Do not do this anywhere else in the Engine tree
		API::String::UTF16;
#else
		API::String::UTF8;
#endif

	//Make no assumptions about argv[0]
	static_cast<void>(AArguments);

	return Result;
}