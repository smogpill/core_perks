// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec3.h"

namespace cp
{
	template <class T>
	class alignas(alignof(Vec3<T>)) AABox
	{
	public:
		CP_FORCE_INLINE AABox() = default;
		CP_FORCE_INLINE AABox(const Vec3<T>& min, const Vec3<T>& max) : min_(min), max_(max) {}

		//CP_FORCE_INLINE bool empty() const { return min_ > max_; }

		Vec3<T> min_ = 1e15f;
		Vec3<T> max_ = -1e15f;
	};

	using AABoxf = AABox<float>;
	using AABoxd = AABox<double>;
}
