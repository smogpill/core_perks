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

	namespace impl
	{
		template <class T> CP_FORCE_INLINE T abs(const T& x) { return std::abs(x); }
		template <class T> CP_FORCE_INLINE T fma(const T& x, const T& y, const T&z)  { return std::fma(x, y, z); }
		template <class T> CP_FORCE_INLINE T sign(const T& x) { return (T(0) < x) - (x < T(0)); }
		template <class T> CP_FORCE_INLINE T copysign(const T& x, const T& y) { return std::copysign(x, y); }
		template <class T> CP_FORCE_INLINE T sqrt(const T& x) { return std::sqrt(x); }
		template <class T> CP_FORCE_INLINE T rsqrt(const T& x) { return T(1) / sqrt(x); }
		template <class T> CP_FORCE_INLINE T exp(const T& x) { return std::exp(x); }
		template <class T> CP_FORCE_INLINE T log(const T& x) { return std::log(x); }
		template <class T> CP_FORCE_INLINE T pow(const T& x, const T& y) { return std::pow(x, y); }
		template <class T> CP_FORCE_INLINE T exp2(const T& x) { return std::exp2(x); }
		template <class T> CP_FORCE_INLINE T log2(const T& x) { return std::log2(x); }
		template <class T> CP_FORCE_INLINE T floor(const T& x) { return std::floor(x); }
		template <class T> CP_FORCE_INLINE T ceil(const T& x) { return std::ceil(x); }
		template <class T> CP_FORCE_INLINE T round(const T& x) { return std::round(x); }
		template <class T> CP_FORCE_INLINE T trunc(const T& x) { return std::trunc(x); }
		template <class T> CP_FORCE_INLINE T fract(const T& x) { return x - floor(x); }
		template <class T> CP_FORCE_INLINE T min(const T& x, const T& y) { return std::min(x, y); }
		template <class T> CP_FORCE_INLINE T max(const T& x, const T& y) { return std::max(x, y); }
		template <class T> CP_FORCE_INLINE T clamp(const T& x, const T& low, const T& high) { return std::clamp(x, low, high); }
		template <class T> CP_FORCE_INLINE T saturate(const T& x) { return clamp(x, T(0), T(1)); }

		template <class T> CP_FORCE_INLINE T mod(const T& x, const T& y)
		{
			CP_ASSERT(std::isfinite(y));
			CP_ASSERT(y);
			return fma(-y, trunc(x / y), x);
		}

		template <class T> CP_FORCE_INLINE T wrap(const T& x, const T& low, const T& high)
		{
			CP_ASSERT(low < high);
			const auto range = high - low;
			return x - range * floor((x - low) / range);
		}

		/*
		* /// Requires testing
		CP_FORCE_INLINE T log2(float x)
		{
			uint32 c = std::bit_cast<uint32>(x);
			c = (c >> 23) - 127;
			return std::bit_cast<T>(c);
		}*/
	}

#define CP_ELEMENTWISE_1(NAME, A) template <class T> CP_FORCE_INLINE T NAME(const T& A) { return elementwise([](const auto& A) { return impl::NAME(A); }, A); }
#define CP_ELEMENTWISE_2(NAME, A, B) template <class T> CP_FORCE_INLINE T NAME(const T& A, const T& B) { return elementwise([](const auto& A, const auto& B) { return impl::NAME(A, B); }, A, B); }
#define CP_ELEMENTWISE_3(NAME, A, B, C) template <class T> CP_FORCE_INLINE T NAME(const T& A, const T& B, const T& C) { return elementwise([](const auto& A, const auto& B, const auto& C) { return impl::NAME(A, B, C); }, A, B, C); }

	CP_ELEMENTWISE_1(abs, x);
	CP_ELEMENTWISE_3(fma, x, y, z);
	CP_ELEMENTWISE_1(sign, x);
	CP_ELEMENTWISE_2(copysign, x, y);
	CP_ELEMENTWISE_1(sqrt, x);
	CP_ELEMENTWISE_1(rsqrt, x);
	CP_ELEMENTWISE_1(exp, x);
	CP_ELEMENTWISE_1(log, x);
	CP_ELEMENTWISE_2(pow, x, y);
	CP_ELEMENTWISE_1(exp2, x);
	CP_ELEMENTWISE_1(log2, x);
	CP_ELEMENTWISE_1(floor, x);
	CP_ELEMENTWISE_1(ceil, x);
	CP_ELEMENTWISE_1(round, x);
	CP_ELEMENTWISE_1(trunc, x);
	CP_ELEMENTWISE_1(fract, x);
	CP_ELEMENTWISE_2(min, x, y);
	CP_ELEMENTWISE_2(max, x, y);
	CP_ELEMENTWISE_3(clamp, x, low, high);
	CP_ELEMENTWISE_1(saturate, x);
	CP_ELEMENTWISE_2(mod, x, y);
	CP_ELEMENTWISE_3(wrap, x, low, high);
}
