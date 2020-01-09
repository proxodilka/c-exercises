#pragma once

template <typename T, typename Dest>
struct rebind_container_type
{
	using type = void*;
	using cont = void*;
};

template <typename T, typename Dest, template<typename, typename...> class V, typename... Args>
struct rebind_container_type<V<T, Args...>, Dest>
{
	using type = T;
	using cont = V<Dest>;
};