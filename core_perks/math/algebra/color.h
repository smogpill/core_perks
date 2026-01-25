// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class Color
	{
	public:
		Color() : r_(0), g_(0), b_(0), a_(255) {}
		union
		{
			struct { uint8 r_, g_, b_, a_; };
			struct { uint8 x_, y_, z_, w_; };
			uint32 rgba_;
		};
	};
}
