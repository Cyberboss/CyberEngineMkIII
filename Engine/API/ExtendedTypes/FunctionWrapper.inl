//! @file FunctionWrapper.inl Implements functions from CYB::API::Function
#pragma once

template <typename AReturn, typename... AArgs> template <typename... AConstructorArgs> CYB::API::Function<AReturn, AArgs...>::Function(AConstructorArgs&&... AArguments) :
	function(std::allocator_arg, Interop::STLAllocator<byte>(), std::forward<AConstructorArgs>(AArguments...))
{}