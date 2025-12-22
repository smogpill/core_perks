// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/providers/mapped_region.h"
#include "core_perks/io/assets/asset_handle.h"

namespace cp
{
	class Asset;

	class MappedAssetData
	{
	public:
		MappedAssetData(const UntypedAssetHandle& asset);

		bool mapped() const { return region_.mapped(); }
		void* data() const { return region_.data(); }
		uint64 size() const { return region_.size(); }
		const UntypedAssetHandle& get_asset() const { return asset_; }
		Asset* get_provider() const { return provider_; }

	private:
		UntypedAssetHandle asset_ = nullptr;
		Asset* provider_ = nullptr;
		MappedRegion region_;
	};
}
