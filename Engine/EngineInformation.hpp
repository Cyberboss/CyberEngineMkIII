#pragma once
namespace CYB {
	namespace API {
		class EngineInformation {
		public:
			const char* const FEngineName,	//!< @brief Engine name
				* const FEngineCopyrightString;	//!< @brief Engine copyright
			const unsigned short FCopyrightYear;	//!< @brief Engine copyright year
			const byte FVersionMajor,	//!< @brief Engine major version
				FVersionMinor;	//!< @brief Engine minor version
			const unsigned int FVersionRevision;	//!< @brief Engine release revision
			const char* const FCompileDate;	//!< @brief Engine compile date
			const char* const FCompileTime;	//!< @brief Engine compile time
		};
	};
};