//! @file ParameterPackIndexer.hpp Defines the template for indexing parameter packed template arguments
#pragma once
namespace CYB {
	namespace API {
		class ParameterPackIndexer {
		public:
			template<unsigned int Index, typename Type, typename... Remaining> struct GetType {
				typedef typename GetType<Index - 1, Remaining...>::type type;
			};
			template<typename Type, typename... Remaining> struct GetType<0, Type, Remaining...> {
				typedef Type type;
			};
		};
	};
};