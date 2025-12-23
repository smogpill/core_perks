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

	void operator>>(BinaryInputStream& stream, AssetPack::SubAssetInfo& entry)
	{
		stream >> entry.handle_;
		stream >> entry.offset_;
		stream >> entry.size_;
	}

	bool AssetPack::on_load()
	{
		CP_TRY(Base::on_load());

		AssetEntry& entry = get_entry();
		MappedAssetData data = entry.get_mapped_data();
		BinaryInputStream stream = data.get_stream();
		stream >> sub_assets_;
		return true;
	}

	MappedAssetData AssetPack::map_sub_asset(const AssetHandle& asset)
	{
		return MappedAssetData();
	}

	void AssetPack::unmap_sub_asset(const AssetHandle& asset)
	{

	}

	AssetPack::SubAssetInfo* AssetPack::get_sub_resource_info(const AssetHandle& asset) const
	{
		for (SubAssetInfo& info : sub_assets_)
		{
			if (info.handle_ == asset)
				return &info;
		}
		return nullptr;
	}
}
