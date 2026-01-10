// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/mapped_resource_data.h"

namespace cp
{
	class ResourceProvider
	{
	public:
		virtual ~ResourceProvider() = default;

		virtual MappedFileRegion map_resource(const ResourceID& id) = 0 { return MappedFileRegion(); }
		virtual void store_resource_async(const ResourceHandle& resource, std::function<void(bool)> on_done = [](bool) {}) = 0 { on_done(false); }

	protected:
	};
}
