//! @file CYBMain.cpp implments engine initialization functions

#include "CYB.hpp"

void CYB::Engine::Init::Run[[noreturn]](void) {

	Platform::Process::GetSelf().Terminate();

	HCF();
}