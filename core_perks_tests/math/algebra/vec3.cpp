// Vibeout (https://github.com/smogpill/vibeout)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"

namespace cp
{
	CP_TEST(Vec3f, basic)
	{
		Vec3f v;
		CP_EXPECT(v.x_ == 0.0f);
		CP_EXPECT(v.y_ == 0.0f);
		CP_EXPECT(v.z_ == 0.0f);
	}
}
