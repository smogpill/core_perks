// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/file.h"
#include "core_perks/io/assets/providers/mapped_asset_data.h"
#include "core_perks/io/assets/asset_handle.h"
#include "core_perks/io/streams/binary_input_stream.h"

namespace cp
{
	class Asset;

	class MappedAssetData
	{
	public:
		MappedAssetData(const AssetHandle& asset);
		MappedAssetData(const AssetHandle& asset, const AssetHandle& provider, MappedRegion&& region);

		bool mapped() const { return region_.is_mapped(); }
		void* data() { return region_.data(); }
		uint64 size() const { return region_.size(); }
		const AssetHandle& get_asset() const { return asset_; }
		const AssetHandle& get_provider() const { return provider_; }
		BinaryInputStream get_stream() { return BinaryInputStream(data(), size()); }

	private:
		AssetHandle asset_;
		AssetHandle provider_;
		MappedRegion region_;
	};
}
