// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/algebra/vec4.h"

namespace cp
{
	template <class T>
	class alignas(alignof(Vec4<T>)) Mat34
	{
	public:
		union
		{
			struct { Vec4<T> x_, y_, z_; };
			Vec4<T> xyz_[3] = { Vec4<T>::unit_x(), Vec4<T>::unit_y(), Vec4<T>::unit_z() };
		};
	};
}
