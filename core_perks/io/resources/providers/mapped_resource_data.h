// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/file.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/streams/binary_input_stream.h"

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
		const ResourceHandle& get_resource() const { return resource_; }
		const ResourceHandle& get_provider() const { return provider_; }
		BinaryInputStream get_stream() { return BinaryInputStream(data(), size()); }

	private:
		ResourceHandle resource_;
		ResourceHandle provider_;
		MappedFileRegion region_;
	};
}
