// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/providers/mapped_asset_data.h"

namespace cp
{
	MappedAssetData::MappedAssetData(const AssetHandle& asset)
		: asset_(asset)
	{
	}

	MappedAssetData::MappedAssetData(const AssetHandle& asset, const AssetHandle& provider, MappedRegion&& region)
		: asset_(asset), provider_(provider), region_(std::move(region))
	{
	}
}
