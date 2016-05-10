#include "TestHeader.hpp"

#include "../SupportLibs/Catch/include/internal/catch_impl.hpp"

int main(int AArgumentCount, char** AArguments){
	return Catch::Session().run(AArgumentCount, AArguments);
}