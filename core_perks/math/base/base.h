// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	using simd128 = __m128;

	template <class T> concept HasXY = requires(const T& t) { t.x_; t.y_; };
	template <class T> concept HasXYZ = requires(const T& t) { t.x_; t.y_; t.z_; };
	template <class T> concept HasXYZW = requires(const T& t) { t.x_; t.y_; t.z_; t.w_; };

	template <typename Func, typename T>
	CP_FORCE_INLINE T elementwise(Func&& func, const T& a)
	{
		if constexpr (HasXYZW<T>)
			return T(elementwise(func, a.x_), elementwise(func, a.y_), elementwise(func, a.z_), elementwise(func, a.w_));
		else if constexpr (HasXYZ<T>)
			return T(elementwise(func, a.x_), elementwise(func, a.y_), elementwise(func, a.z_));
		else if constexpr (HasXY<T>)
			return T(elementwise(func, a.x_), elementwise(func, a.y_));
		else
			return func(a);
	}

	template <typename Func, typename T>
	CP_FORCE_INLINE T elementwise(Func&& func, const T& a, const T& b)
	{
		if constexpr (HasXYZW<T>)
			return T(elementwise(func, a.x_, b.x_), elementwise(func, a.y_, b.y_), elementwise(func, a.z_, b.z_), elementwise(func, a.w_, b.w_));
		else if constexpr (HasXYZ<T>)
			return T(elementwise(func, a.x_, b.x_), elementwise(func, a.y_, b.y_), elementwise(func, a.z_, b.z_));
		else if constexpr (HasXY<T>)
			return T(elementwise(func, a.x_, b.x_), elementwise(func, a.y_, b.y_));
		else
			return func(a, b);
	}

	template <typename Func, typename T>
	CP_FORCE_INLINE T elementwise(Func&& func, const T& a, const T& b, const T& c)
	{
		if constexpr (HasXYZW<T>)
			return T(elementwise(func, a.x_, b.x_, c.x_), elementwise(func, a.y_, b.y_, c.z_), elementwise(func, a.z_, b.z_, c.z_), elementwise(func, a.w_, b.w_, c.w_));
		else if constexpr (HasXYZ<T>)
			return T(elementwise(func, a.x_, b.x_, c.x_), elementwise(func, a.y_, b.y_, c.z_), elementwise(func, a.z_, b.z_, c.z_));
		else if constexpr (HasXY<T>)
			return T(elementwise(func, a.x_, b.x_, c.x_), elementwise(func, a.y_, b.y_, c.z_));
		else
			return func(a, b, c);
	}

#define CP_ELEMENTWISE_1(X, IMPL) (const T& X) { return elementwise([](const auto& X) IMPL, X); }
#define CP_ELEMENTWISE_2(X, Y, IMPL) (const T& X, const T& Y) { return elementwise([](const auto& X, const auto& Y) IMPL, X, Y); }
#define CP_ELEMENTWISE_3(X, Y, Z, IMPL) (const T& X, const T& Y, const T& Z) { return elementwise([](const auto& X, const auto& Y, const auto& Z) IMPL, X, Y, Z); }

	template <class T> CP_FORCE_INLINE T abs CP_ELEMENTWISE_1(x, { return std::abs(x); });
	template <class T> CP_FORCE_INLINE T fma CP_ELEMENTWISE_3(x, y, z, { return std::fma(x, y, z); });
	template <class T> CP_FORCE_INLINE T sign CP_ELEMENTWISE_1(x, { return (T(0) < x) - (x < T(0)); });
	template <class T> CP_FORCE_INLINE T copysign CP_ELEMENTWISE_2(x, y, { return std::copysign(x, y); });
	template <class T> CP_FORCE_INLINE T sqrt CP_ELEMENTWISE_1(x, { return std::sqrt(x); });
	template <class T> CP_FORCE_INLINE T rsqrt CP_ELEMENTWISE_1(x, { return T(1) / sqrt(x); });
	template <class T> CP_FORCE_INLINE T exp CP_ELEMENTWISE_1(x, { return std::exp(x); });
	template <class T> CP_FORCE_INLINE T log CP_ELEMENTWISE_1(x, { return std::log(x); });
	template <class T> CP_FORCE_INLINE T pow CP_ELEMENTWISE_2(x, y, { return std::pow(x, y); });
	template <class T> CP_FORCE_INLINE T exp2 CP_ELEMENTWISE_1(x, { return std::exp2(x); });
	template <class T> CP_FORCE_INLINE T log2 CP_ELEMENTWISE_1(x, { return std::log2(x); });
	template <class T> CP_FORCE_INLINE T floor CP_ELEMENTWISE_1(x, { return std::floor(x); });
	template <class T> CP_FORCE_INLINE T ceil CP_ELEMENTWISE_1(x, { return std::ceil(x); });
	template <class T> CP_FORCE_INLINE T round CP_ELEMENTWISE_1(x, { return std::round(x); });
	template <class T> CP_FORCE_INLINE T trunc CP_ELEMENTWISE_1(x, { return std::trunc(x); });
	template <class T> CP_FORCE_INLINE T fract CP_ELEMENTWISE_1(x, { return x - floor(x); });
	template <class T> CP_FORCE_INLINE T min CP_ELEMENTWISE_2(x, y, { return std::min(x, y); });
	template <class T> CP_FORCE_INLINE T max CP_ELEMENTWISE_2(x, y, { return std::max(x, y); });
	template <class T> CP_FORCE_INLINE T clamp CP_ELEMENTWISE_3(x, low, high, { return std::clamp(x, low, high); });
	template <class T> CP_FORCE_INLINE T saturate CP_ELEMENTWISE_1(x, { return clamp(x, T(0), T(1)); });

	template <class T> CP_FORCE_INLINE T mod CP_ELEMENTWISE_2(x, y,
		{
			CP_ASSERT(std::isfinite(y));
			CP_ASSERT(y);
			return fma(-y, trunc(x / y), x);
		});

	template <class T> CP_FORCE_INLINE T wrap CP_ELEMENTWISE_3(x, low, high,
		{
			CP_ASSERT(low < high);
			const auto range = high - low;
			return x - range * floor((x - low) / range);
		});

	/// Requires testing
	/*
	CP_FORCE_INLINE T log2(float x)
	{
		uint32 c = std::bit_cast<uint32>(x);
		c = (c >> 23) - 127;
		return std::bit_cast<T>(c);
	}*/
}
