#pragma once

#define REDIRECTED_FUNCTION(AName, ...) \
template <class ARedirector> class AName {\
public:\
	static auto RedirectedFunction(__VA_ARGS__);\
};\
template <class ARedirector> auto AName<ARedirector>::RedirectedFunction(__VA_ARGS__)