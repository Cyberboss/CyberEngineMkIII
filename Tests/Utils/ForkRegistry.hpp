#pragma once
bool BasicCmp(const char* const A, const char* const B);

class ForkRegistry {
public:
	typedef int(*FunctionPointer)(const int AArgumentCount, const char* const* const AArguments);
	class Function {
	public:
		const FunctionPointer FFunction;
		Function* FNext;
		const char* const FCommand;
	public:
		inline Function(const FunctionPointer APointer, const char* const ACommand) :
			FFunction(APointer),
			FNext(nullptr),
			FCommand(ACommand)
		{
			RegisterFork(this);
		}
	};
private:
	static Function* FHead, *FTail;
public:
	static void RegisterFork(Function* const AFunction) {
		if (FTail == nullptr) {
			FHead = AFunction;
			FTail = AFunction;
		}
		else {
			FTail->FNext = AFunction;
			FTail = AFunction;
		}
	}
	static int ForkTo(const char* const ACommand, const int ARemainingArgs, const char* const* const AArgs) {
		Function* Result(nullptr);
		for (auto Current(FHead); Current != nullptr; Current = Current->FNext)
			if (BasicCmp(ACommand, Current->FCommand)) {
				if (Result != nullptr)
					std::terminate();
				Result = Current;
			}
		if (Result == nullptr)
			std::terminate();
		return Result->FFunction(ARemainingArgs, AArgs);
	}
};

#define FORKED_FUNCTION(AName) int AName(const int AArgumentCount, const char* const* AArguments);\
ForkRegistry::Function AName##Func(AName, #AName);\
int AName(const int AArgumentCount, const char* const* AArguments)