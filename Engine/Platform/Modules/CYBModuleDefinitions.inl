//! @file CYBModuleDefinitions.inl Defines needed modules used by the platform layer

//! @cond
#ifdef TARGET_OS_WINDOWS
DEFINE_MODULE(Kernel32, Win32, true, DiscardVirtualMemory)
#endif
//! @endcond