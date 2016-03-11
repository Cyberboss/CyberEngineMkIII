//! @file CYBMain.cpp implments engine initialization functions

#include "CYB.hpp"

void CYB::Engine::Init::Run[[noreturn]](void) {

	Platform::Process::Terminate(Platform::Process::GetSelf());

	HCF();
}