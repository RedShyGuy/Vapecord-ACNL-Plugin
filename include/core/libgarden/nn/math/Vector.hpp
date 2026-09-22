#pragma once

#include <core/libgarden/nn/types.h>

#include <array>
#include <cmath>

#include <core/libgarden/gardenex/util/index_sequence.hpp>

namespace nn::math
{
	template<typename T, size_t N>
	struct Vector;

	template<size_t I>
	FORCE_INLINE constexpr auto&& get(auto&& self)
	{
		if constexpr (I == 0) return std::forward<decltype(self)>(self).x;
		if constexpr (I == 1) return std::forward<decltype(self)>(self).y;
		if constexpr (I == 2) return std::forward<decltype(self)>(self).z;
		if constexpr (I == 3) return std::forward<decltype(self)>(self).w;
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr Vector<T, N> operator+(const Vector<T, N>& left, std::common_with<Vector<T, N>> auto&& right)
	{
		return Operation<T, N>([] FORCE_INLINE(auto l, auto r) static
		{
			return l + r;
		}, std::forward<decltype(left)>(left), std::forward<decltype(right)>(right));
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr auto&& operator+=(Vector<T, N>& self, std::common_with<Vector<T, N>> auto&& right)
	{
		Operation<N>([&self, right]<size_t I> FORCE_INLINE
		{
			get<I>(self) += get<I>(right);
		});
		return self;
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr Vector<T, N> operator-(const Vector<T, N>& left, std::common_with<Vector<T, N>> auto&& right)
	{
		return Operation<T, N>([] FORCE_INLINE(auto l, auto r) static
		{
			return l - r;
		}, std::forward<decltype(left)>(left), std::forward<decltype(right)>(right));
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr auto&& operator-=(Vector<T, N>& self, std::common_with<Vector<T, N>> auto&& right)
	{
		Operation<N>([&self, right]<size_t I> FORCE_INLINE
		{
			get<I>(self) -= get<I>(right);
		});
		return self;
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr Vector<T, N> operator*(const Vector<T, N>& left, T&& right)
	{
		return Operation<T, N>([] FORCE_INLINE(auto l, auto r) static
		{
			return l * r;
		}, std::forward<decltype(left)>(left), std::forward<T>(right));
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr Vector<T, N> operator*(T&& left, const Vector<T, N>& right)
	{
		return operator*(std::forward<decltype(right)>(right), std::forward<T>(left));
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr auto&& operator*=(Vector<T, N>& self, T&& right)
	{
		Operation<N>([&self, right]<size_t I> FORCE_INLINE
		{
			get<I>(self) *= right;
		});
		return self;
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr Vector<T, N> operator/(const Vector<T, N>& left, T&& right)
	{
		return Operation<T, N>([] FORCE_INLINE(auto l, auto r) static
		{
			return l / r;
		}, std::forward<decltype(left)>(left), std::forward<T>(right));
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr Vector<T, N> operator/(T&& left, const Vector<T, N>& right)
	{
		return operator/(std::forward<decltype(right)>(right), std::forward<T>(left));
	}

	template<typename T, size_t N>
	FORCE_INLINE constexpr auto&& operator/=(Vector<T, N>& self, T&& right)
	{
		Operation<N>([&self, right]<size_t I> FORCE_INLINE
		{
			get<I>(self) /= right;
		});
		return self;
	}

	template<size_t N, typename F, typename R>
	FORCE_INLINE constexpr auto Operation(F&& fn, R&& combine)
	{
		return util::for_index_sequence<N>(std::forward<F>(fn), std::forward<R>(combine));
	}

	template<size_t N, typename F>
	FORCE_INLINE constexpr void Operation(F&& fn)
	{
		util::for_index_sequence<N>(std::forward<F>(fn));
	}

	template<typename T, size_t N, typename F, typename L, typename R>
	FORCE_INLINE constexpr auto Operation(F&& fn, L&& left, R&& right)
	{
		using Self = Vector<T, N>;
		return util::create<Self, N>([&]<size_t I> FORCE_INLINE
		{
			if constexpr (std::is_convertible_v<L, Self> && std::is_convertible_v<R, Self>)
				return fn(get<I>(left), get<I>(right));
			else if constexpr (std::is_convertible_v<L, Self>)
				return fn(get<I>(left), std::forward<R>(right));
			else if constexpr (std::is_convertible_v<R, Self>)
				return fn(std::forward<L>(left), get<I>(right));
		});
	}

	template<typename T>
	struct Vector<T, 2>
	{
		using Self = Vector<T, 2>;
		friend constexpr bool operator==(const Self& left, const Self& right) = default;
		T x, y;
	};
	
	template<typename T>
	struct Vector<T, 3>
	{
		using Self = Vector<T, 3>;
		friend constexpr bool operator==(const Self& left, const Self& right) = default;
		T x, y, z;
	};

	template<typename T>
	struct Vector<T, 4>
	{
		using Self = Vector<T, 4>;
		friend constexpr bool operator==(const Self& left, const Self& right) = default;
		T x, y, z, w;
	};

	namespace def
	{
		using Vector2s8 = Vector<s8, 2>;
		using Vector3s8 = Vector<s8, 3>;
		using Vector2u8 = Vector<u8, 2>;
		using Vector3u8 = Vector<u8, 3>;
		using Vector2s16 = Vector<s16, 2>;
		using Vector3s16 = Vector<s16, 3>;
		using Vector2u16 = Vector<u16, 2>;
		using Vector3u16 = Vector<u16, 3>;
		using Vector2u32 = Vector<u32, 2>;
		using Vector3u32 = Vector<u32, 3>;
		using Vector2i = Vector<s32, 2>;
		using Vector3i = Vector<s32, 3>;
		using Vector2 = Vector<float, 2>;
		using Vector3 = Vector<float, 3>;
		using Vector4 = Vector<float, 4>;

		static_assert(sizeof(Vector2) == 2 * sizeof(float));
		static_assert(sizeof(Vector3) == 3 * sizeof(float));
		static_assert(sizeof(Vector4) == 4 * sizeof(float));
	}

	using namespace def;
}

using namespace nn::math::def;
