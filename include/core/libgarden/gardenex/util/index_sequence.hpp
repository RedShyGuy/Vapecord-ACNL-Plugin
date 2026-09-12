#pragma once

#include <cstdlib>
#include <utility>
#include <tuple>

#ifndef FORCE_INLINE
#define FORCE_INLINE [[gnu::always_inline]]
#endif

namespace util
{

	namespace detail
	{
		template<bool Cond, typename TIndex = size_t, size_t... I>
		FORCE_INLINE constexpr auto for_index_sequence_impl(auto&& fn, std::index_sequence<I...>)
		{
			if constexpr (!Cond)
				(fn.template operator() < static_cast<TIndex>(I) > (), ...);
			else
				return  (fn.template operator() < static_cast<TIndex>(I) > () || ...);
		}

		template<typename TIndex, size_t... I>
		FORCE_INLINE constexpr auto for_index_sequence_impl(auto&& fn, auto&& combine, std::index_sequence<I...>)
		{
			return combine(fn.template operator() < static_cast<TIndex>(I) > ()...);
		}

		template<template<typename...> typename T, size_t... I>
		FORCE_INLINE constexpr auto create_impl(auto&& fn, std::index_sequence<I...>)
		{
			return T { fn.template operator() < I > () ... };
		}

		template<template<typename, size_t> typename T, size_t... I>
		FORCE_INLINE constexpr auto create_impl(auto&& fn, std::index_sequence<I...>)
		{
			return T { fn.template operator() < I > () ... };
		}

		template<typename T, size_t... I>
		FORCE_INLINE constexpr auto create_impl(auto&& fn, std::index_sequence<I...>)
		{
			return T { fn.template operator() < I > () ... };
		}
	}

	template<size_t N>
	FORCE_INLINE constexpr void for_index_sequence(auto&& fn)
	{
		detail::for_index_sequence_impl<false>(std::forward<decltype(fn)>(fn), std::make_index_sequence<N>());
	}

	template<size_t N>
	FORCE_INLINE constexpr auto for_index_sequence_conditional(auto&& fn)
	{
		return detail::for_index_sequence_impl<true>(std::forward<decltype(fn)>(fn), std::make_index_sequence<N>());
	}

	template<size_t N>
	FORCE_INLINE constexpr auto for_index_sequence(auto&& fn, auto&& combine)
	{
		return detail::for_index_sequence_impl<size_t>(std::forward<decltype(fn)>(fn), std::forward<decltype(combine)>(combine), std::make_index_sequence<N>());
	}

	template<template<typename, size_t> typename T, size_t N>
	FORCE_INLINE constexpr auto create(auto&& fn)
	{
		return detail::create_impl<T>(std::forward<decltype(fn)>(fn), std::make_index_sequence<N>());
	}

	template<template<typename...> typename T, size_t N>
	FORCE_INLINE constexpr auto create(auto&& fn)
	{
		return detail::create_impl<T>(std::forward<decltype(fn)>(fn), std::make_index_sequence<N>());
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr auto create(auto&& fn)
	{
		return detail::create_impl<T>(std::forward<decltype(fn)>(fn), std::make_index_sequence<N>());
	}

	template<size_t N>
	FORCE_INLINE constexpr auto repeat(auto&& fn)
	{
		if constexpr (N == 0) return;
		else if constexpr (N == 1) return fn.template operator() < N - 1 > ();
		else return fn.template operator() < N - 1 > (repeat<N - 1>(std::forward<decltype(fn)>(fn)));
	}

	template<size_t N, auto Fn>
	FORCE_INLINE constexpr auto repeat()
	{
		if constexpr (N == 0) return;
		else if constexpr (N == 1) return Fn.template operator() < N - 1 > ();
		else return Fn.template operator() < N - 1, repeat<N - 1, Fn>() > ();
	}
}
