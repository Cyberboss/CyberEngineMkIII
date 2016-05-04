#pragma once
namespace CYB {
	namespace API {
		class EngineInformation {
		public:
			const API::String::Static FEngineName,	//!< @brief Engine name
				FEngineCopyrightString;	//!< @brief Engine copyright
			const unsigned short FCopyrightYear;	//!< @brief Engine copyright year
			const byte FVersionMajor,	//!< @brief Engine major version
				FVersionMinor;	//!< @brief Engine minor version
			const unsigned int FVersionRevision;	//!< @brief Engine release revision
			const API::String::Static FCompileDate,	//!< @brief Engine compile date
				FCompileTime;	//!< @brief Engine compile time
		};
	};
};