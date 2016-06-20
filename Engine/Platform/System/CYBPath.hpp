//! @file CYBPath.hpp Defines the Path interface
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief Used for manipulating Paths
			class Path {
			private:
				API::String::UTF8 FPath;
			public:
				static Path GetExecutableDirectory(void);
				static Path GetResourceDirectory(void);
				static Path GetTemporaryDirectory(void);
				static Path GetUserDataDirectory(void);

				static Path WorkingDirectory(void);
				static void SetWorkingDirectory(const Path& APath);

				Path(const API::String::CStyle& AString);
				Path(const Path& ACopy) = default;
				Path(Path&& AMove) noexcept = default;
				Path& operator=(Path&& AMove) noexcept = default;

				void EmptyContents(void) const;

				void CreateAsDirectory(void) const;

				bool IsDirectory(void) const;
				bool IsEmpty(void) const;

				bool IsFile(void) const;

				bool Valid(void) const;

				Path operator+(const Path& AOtherPath) const;
			};
		};
	};
};