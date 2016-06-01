#pragma once

#define REDIRECTED_FUNCTION(AName, ...) \
template <class ARedirector> class AName {\
public:\
	static auto Redirect(__VA_ARGS__);\
};\
template <class ARedirector> auto AName<ARedirector>::Redirect(__VA_ARGS__)