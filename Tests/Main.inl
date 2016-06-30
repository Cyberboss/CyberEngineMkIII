#pragma once

int main(int AArgumentCount, char** AArguments) {
	if (AArgumentCount > 1 && std::strcmp(AArguments[1], "--refork")) {
		return 0;
	}
	return Catch::Session().run(AArgumentCount, AArguments);
}