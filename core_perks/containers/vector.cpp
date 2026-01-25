// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/containers/vector.h"

namespace cp
{
	uint32 grow_vector_capacity(uint32 capacity, uint32 desired_capacity)
	{
		CP_ASSERT(desired_capacity > capacity);
		const uint32 min_capacity = 8;
		if (capacity < min_capacity)
			capacity = min_capacity;
		while (capacity < desired_capacity)
			capacity = capacity + (capacity >> 1); // Grow by 1.5x
		return capacity;
	}
}
