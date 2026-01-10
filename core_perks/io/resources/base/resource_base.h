// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/math/numerical/hash.h"
#include "core_perks/patterns/hashed_string.h"

namespace cp
{
	enum class ResourceState
	{
		// Stable states
		NONE,
		READY,
		FAILED,

		// Transitions
		LOADING,
		WAITING_DEPENDENCIES,
		READYING,
		PROCESSING,
		STORING,
		UNLOADING
	};

	union ResourcePriority
	{
		uint32 raw_ = 0;
		struct
		{
			uint32 unused : 32;
		} bits_;
	};

	class ResourceID : public HashedString
	{
	public:
		using HashedString::HashedString;
		using HashedString::operator=;

		static const ResourceID& get_empty();
	};
}