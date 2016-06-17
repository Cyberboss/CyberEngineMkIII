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
		void* Alloc(const int AAmount);
		void* Realloc(void* const AOld, const int ANewSize);
		void Free(void* const AOld);
	};
}