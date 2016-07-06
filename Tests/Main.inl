#pragma once
#include "Utils/ForkRegistry.hpp"
ForkRegistry::Function* ForkRegistry::FHead, *ForkRegistry::FTail(nullptr);
int main(int AArgumentCount, char** AArguments) {
	if (AArgumentCount >= 1 && BasicCmp(AArguments[0], "--refork")) {
		if (AArgumentCount >= 2) 
			return ForkRegistry::ForkTo(AArguments[1], AArgumentCount - 2, AArguments + 2);
		return 1;
	}
	return Catch::Session().run(AArgumentCount, AArguments);
}