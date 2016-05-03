//! @file CYBModuleDefinitions.inl Defines needed modules used by the platform layer

#ifndef DOXY

#ifdef TARGET_OS_WINDOWS
DEFINE_MODULE(Kernel32, Win32, DiscardVirtualMemory)
#endif

#endif