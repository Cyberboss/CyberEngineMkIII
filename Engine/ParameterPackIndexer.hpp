//! @file ParameterPackIndexer.hpp Defines the template for indexing parameter packed template arguments
#pragma once
namespace CYB {
	namespace API {
		/*! 
			@brief Indexes template parameter packs
			@tparam AIndex The index to retrieve
			@tparam AType The first type in the parameter pack
			@tparam ARemaining the other types in the parameter pack
		*/
		template<unsigned int AIndex, typename AType, typename... ARemaining> class ParameterPackIndexer {
		public:
			typedef typename ParameterPackIndexer<AIndex - 1, ARemaining...>::type type;	//!< @brief The type of the indexed template parameter
		};
		template<typename AType, typename... ARemaining> class ParameterPackIndexer<0, AType, ARemaining...> {
		public:
			typedef AType type;
		};
	};
};