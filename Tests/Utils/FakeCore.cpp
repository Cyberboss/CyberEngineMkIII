#include "TestHeader.hpp"

class FakeCore {
private:
	byte FBytes[sizeof(CYB::Engine::Core)];
public:
	FakeCore() {
		CYB::Core().SetTestPointer(FBytes);
	}
};

FakeCore FFakeCore;
