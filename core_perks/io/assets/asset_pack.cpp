// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_pack.h"
#include "core_perks/containers/vector_extensions.h"

namespace cp
{
	CP_DEFINE_CLASS(AssetPack)
	{
	}

	namespace asset_pack
	{
		struct Entry
		{
			std::string asset_id_;
			uint64 offset_ = 0;
			uint64 size_ = 0;
		};
		struct Header
		{
			std::vector<Entry> entries_;
		};
	}

	void AssetPack::add_resource(const UntypedAssetHandle& handle)
	{
		if (!contains(resources_, handle))
			resources_.push_back(handle);
	}

	bool AssetPack::on_load(const MappedAssetData& data)
	{
		CP_TRY(Base::on_load(data));
		return false;
	}

	MappedAssetData AssetPack::map_asset(AssetEntry& entry)
	{
		return MappedAssetData();
	}

	void AssetPack::unmap_asset(MappedAssetData& data)
	{
	}
}
