// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/asset.h"
#include "core_perks/io/assets/asset_handle.h"

namespace cp
{
	class AssetPack : public Asset
	{
		CP_CLASS(AssetPack);
	public:
		void add_resource(const UntypedAssetHandle& handle);

	private:
		std::vector<UntypedAssetHandle> resources_;
	};
}
