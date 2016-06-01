//! @file CYBFile.hpp Defines the filesystem interface
//! @todo Document
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief Contains the universal file interface
			class File {
			public:
				enum class Mode {
					CREATE,
					READ,
					WRITE,
					READ_WRITE,
				};
			public:
				static void Delete(const Path& APath);
				static bool Exists(const Path& APath);
				static unsigned long long Size(const Path& APath);

				File(const Path& APath, const Mode AMode);
				File(const File&) = delete;
				File(File&& AMove);
				File& operator=(File&& AMove);

				unsigned long long Size(void);

				unsigned long long CursorPosition(void) const;
				unsigned long long Seek(void) const;

				unsigned long long Read(void* const ABuffer, const unsigned long long AAmount) const;
				unsigned long long Write(void* const ABuffer, const unsigned long long AAmount) const;
			};
		};
	};
};