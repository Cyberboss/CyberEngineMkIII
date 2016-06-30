#pragma once

int main(int AArgumentCount, char** AArguments) {
	if (AArgumentCount >= 1 && std::strcmp(AArguments[0], "--refork") == 0) {
		if (AArgumentCount >= 2 && std::strcmp(AArguments[1], "--loop") == 0)
			for (;;);
		return 0;
	}
	return Catch::Session().run(AArgumentCount, AArguments);
}