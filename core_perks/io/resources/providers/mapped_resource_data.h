// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/io/resources/resource_handle.h"

#if 0

namespace cp
{
	class Resource;

	class MappedResourceData
	{
	public:
		MappedResourceData(const ResourceHandle& resource);
		MappedResourceData(const ResourceHandle& resource, const ResourceHandle& provider, MappedFileRegion&& region);

		bool mapped() const { return region_.is_mapped(); }
		void* data() { return region_.data(); }
		uint64 size() const { return region_.size(); }

	private:
		MappedFileRegion region_;
	};
}

#endif