#pragma once
#include "Utils/ForkRegistry.hpp"
ForkRegistry::Function* ForkRegistry::FHead, *ForkRegistry::FTail(nullptr);
int main(int AArgumentCount, char** AArguments) {
	int Result;
	int Base(1);
	if (AArgumentCount >= 1 && BasicCmp(AArguments[0], "--refork"))
		Base = 0;
	if (AArgumentCount >= Base + 1 && BasicCmp(AArguments[Base], "--refork")) {
		if (AArgumentCount >= Base + 2)
			Result = ForkRegistry::ForkTo(AArguments[Base + 1], AArgumentCount - (Base + 2), AArguments + (Base + 2));
		else
			Result = 1;
	}
	else
		Result = Catch::Session().run(AArgumentCount, AArguments);
	return Result;
}