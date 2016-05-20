//! @file ParameterPack.hpp Defines the template for working with parameter packed template arguments
#pragma once
namespace CYB {
	namespace API {
		/*!
			@brief Contains information about a template parameter pack
			@tparam AType The first type in the pack
			@tparam ARemaining The remaining types in the pack
		*/
		template <typename AType, typename... ARemaining> class ParameterPack {
		public:
			/*!
				@brief A class which can be used to get the type at AIndex of the pack
				@tparam AIndex The index of the type to retrieve
				@tparam AUnused This type is not used and should remain void
			*/
			template <unsigned int AIndex, typename AUnused = void> class Indexer;
		public:
			/*!
				@brief Returns the size of the parameter pack
				@return The size of the parameter pack
			*/
			static constexpr unsigned int Size(void);

			/*!
				@brief Check total equivalance with another parameter pack
				@return true if the types are equivalent, false otherwise
			*/
			template <typename AOtherType, typename... AOtherTypes> static constexpr bool Equal(void) {
				return Size() == ParameterPack<AOtherType, AOtherTypes...>::Size()
					&& ParameterPack<ARemaining...>::template Equal<AOtherTypes...>()
					&& std::is_same<AOtherType,AType>::value;
			}
			/*!
				@brief Check total equivalance with another ParameterPack specialization
				@return true if the types are equivalent, false otherwise
			*/
			template <typename AOtherPP> static constexpr bool PPEqual(void) {
				return AOtherPP::template Equal<AType, ARemaining...>();
			}
		};
	};
};

#include "ParameterPack.inl"