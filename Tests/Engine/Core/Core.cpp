#include "TestHeader.hpp"

//Heavy whiteboxing below

struct CoreConstructor {
	byte FBytes[sizeof(CYB::Engine::Core)];
	const unsigned int FNumArguments;
	const oschar_t* const* const FArguments;
};

template<> void CYB::Engine::Core::Backdoor<CoreConstructor>(CoreConstructor& AConstructor) {
	new (static_cast<void*>(AConstructor.FBytes)) Core(AConstructor.FNumArguments, AConstructor.FArguments);
}

SCENARIO("Core instantiation", "[Engine][Core][Functional]") {
	Fake::Core::NullifySingleton();
	GIVEN("The core") {
		typedef CYB::Engine::Core TheCore;
		WHEN("It is instantiated") {
			CoreConstructor Constructor{ {}, 0, nullptr };
			TheCore::Backdoor(Constructor);
			THEN("The engine is properly initialized"){
				CHECK(static_cast<void*>(&CYB::Core()) == static_cast<void*>(Constructor.FBytes));
			}
		}
	}
	Fake::Core::ResetToFakeCorePointer();
}