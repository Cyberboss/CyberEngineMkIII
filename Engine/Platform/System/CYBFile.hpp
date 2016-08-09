//! @file CYBFile.hpp Defines the filesystem interface
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
				enum class SeekOffset {
					CURSOR,
					BEGIN,
					END,
				};
			public:
				static void Touch(const Path& APath);
				static unsigned long long Size(const Path& APath);

				File(const Path& APath, const Mode AMode);
				File(const File&) = delete;
				File(File&& AMove);
				File& operator=(File&& AMove);

				unsigned long long Size(void) noexcept;

				unsigned long long CursorPosition(void) const noexcept;
				void Seek(const unsigned long long AOffest, const SeekOffset AOffset) const;

				unsigned long long Read(void* const ABuffer, const unsigned long long AMaxAmount) const;
				unsigned long long Write(void* const ABuffer, const unsigned long long AMaxAmount) const;
			};
		};
	};
};
//! @todo Use File::Touch in Path tests after it's implemented