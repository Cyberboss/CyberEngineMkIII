#include "TestHeader.hpp"

namespace Fake {
	class Allocator : public CYB::API::Interop::Allocator {
	public:
		Allocator();
		~Allocator();
		CYB::API::Interop::Allocatable* NewObject(const CYB::API::Interop::Allocatable::ID AID, const CYB::API::Interop::EmptyConstructor& AConstructor) final override;
	};

	class Core {
	private:
		byte FBytes[sizeof(CYB::Engine::Core)];
		Allocator* FAllocator;
	public:
		Core(const unsigned long long ASize);
		~Core();

		static void ResetToFakeCorePointer(void);
		static void NullifySingleton(void);
	};

	class Heap : public CYB::API::Heap {
	public:
		void* Alloc(const int AAmount) final override;
		void* Realloc(void* const AOld, const int ANewSize) final override;
		void Free(void* const AOld) RELEASE_NOEXCEPT final override;
	};
	class SysCalls {
	public:
		struct Args {
			CYB::Platform::System::Sys::Union64 FArg1,
				FArg2,
				FArg3,
				FArg4,
				FArg5,
				FArg6;
		};
		typedef unsigned long long(*CallPointer)(Args& AArgs);
	private:
		struct MapList {
			MapList* FNext;
			CallPointer FFunction;
			CYB::Platform::System::Sys::CallNumber FCallNumber;
		};
	public:
		static MapList* FMapList;
	public:
		static void OverrideCall(const CYB::Platform::System::Sys::CallNumber ACallNumber, CallPointer ACallPointer);
		static void ResetCall(const CYB::Platform::System::Sys::CallNumber ACallNumber);
	};
};

class SysCallOverride {
private:
	const CYB::Platform::System::Sys::CallNumber FCallNumber;
public:
	SysCallOverride(CYB::Platform::System::Sys::CallNumber ACallNumber, Fake::SysCalls::CallPointer ACallPointer);
	~SysCallOverride();
};