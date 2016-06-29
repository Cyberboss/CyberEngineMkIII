//! @file CYBPath.hpp Defines the Path interface
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			/*!
				@brief Used for manipulating Paths
				@attention Only UTF-8 encodedable paths are supported
			*/
			class Path {
			public:
				enum class SystemPath {
					EXECUTABLE_IMAGE,	//!< @brief The path from which the engine was launched
					EXECUTABLE,	//!< @brief The directory from which the engine was launched
					RESOURCE,	//!< @brief The directory where read only resources
					TEMPORARY,	//!< @brief The directory for storing data relevant only to this execution
					USER,	//!< @brief The directory for storing permanent data associated with the OS' current user
					WORKING,	//!< @brief The current application working directory
				};
			private:
				API::String::UTF8 FPath;	//!< @brief The underlying string
			private:
				/*!
					@brief Get the string path of a SystemDirectory
					@param ADirectory The type of SystemDirectory to look up
					@return The string path of @p ADirectory
					@par Thread Safety
						Use of SystemDirectory::WORKING must be synchronized with SetAsWorkingDirectory. Otherwise, this function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the specified path could not be retrieved
				*/
				static API::String::UTF8 LocateDirectory(const SystemPath ADirectory);
			public:
				/*!
					@brief Get the Path of a SystemDirectory
					@param ADirectory The type of SystemDirectory to look up
					@par Thread Safety
						Use of SystemDirectory::WORKING must be synchronized with SetAsWorkingDirectory. Otherwise, this function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the specified path could not be retrieved
				*/
				Path(const SystemPath ADirectory);
				Path(API::String::UTF8&& AString);
				Path(const Path& ACopy) = default;
				Path(Path&& AMove) noexcept = default;
				Path& operator=(Path&& AMove) RELEASE_NOEXCEPT = default;

				void EmptyContents(void) const;

				void CreateAsDirectory(const bool ARecursive) const;

				bool IsDirectory(void) const;
				bool IsFile(void) const;
				bool Exists(void) const;
				bool CanRead(void) const;
				bool CanWrite(void) const;

				bool Valid(void) const; 
				void SetAsWorkingDirectory(void) const;

				void Evaluate(void);

				Path operator+(const Path& AOtherPath) const;
				/*!
					@brief Public access to the underlying string
					@return The UTF8 string representing the Path
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				const API::String::UTF8& operator()(void) const noexcept;
			};
		};
	};
};