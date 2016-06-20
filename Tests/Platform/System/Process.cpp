#include "TestHeader.hpp"

SCENARIO("Getting the running process works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("The running process (me!)") {
		WHEN("Process::GetSelf is called") {
			auto Result(CYB::Platform::System::Process::GetSelf());
			THEN("The process returned is me") {
				CHECK(true);
			}
		}
	}
}

SCENARIO("Process move constructor works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("A Process") {
		auto Proc(CYB::Platform::System::Process::GetSelf());
		WHEN("The process is moved and move assigned") {
			{
				auto Proc2(std::move(Proc));
				Proc = std::move(Proc2);
			}
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}

SCENARIO("Process equivalence works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A Process") {
		auto Proc(CYB::Platform::System::Process::GetSelf());
		WHEN("The process is compared with itself") {
			auto Proc2(CYB::Platform::System::Process::GetSelf());
			THEN("They are the same") {
				CHECK(Proc == Proc2);
			}
		}
		WHEN("The process is compared with baloney") {
			auto Proc2(CYB::Platform::System::Process::GetSelf());
			//TODO fix this test when reforking is implemented
			*reinterpret_cast<unsigned int*>(&Proc2) = static_cast<unsigned int>(-2);
			THEN("They are not the same") {
				CHECK(Proc != Proc2);
			}
		}
	}
}