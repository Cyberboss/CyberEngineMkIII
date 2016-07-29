#pragma once

template <typename AType, typename... ARemaining> template <unsigned int AIndex, typename AUnused> class CYB::API::ParameterPack<AType, ARemaining...>::Indexer {
public:
	using FType = typename ParameterPack<ARemaining...>::template Indexer<AIndex - 1>::FType;
};

template <typename AType, typename... ARemaining> template <typename AUnused> class CYB::API::ParameterPack<AType, ARemaining...>::Indexer<0, AUnused> {
public:
	using FType = AType;
};

template <typename AType, typename... ARemaining> constexpr unsigned int CYB::API::ParameterPack<AType, ARemaining...>::Size(void) {
	return (sizeof...(ARemaining)) + 1;
}