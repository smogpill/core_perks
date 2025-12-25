// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/assets/providers/mapped_asset_data.h"

namespace cp
{
	class BinaryOutputStream;
	class AssetLoader;
	class MappedAssetData;
	class AssetEntry;

	class Asset
	{
		CP_CLASS(Asset);
	public:
		virtual ~Asset() = default;

		virtual bool on_load() { return true; }
		virtual void on_unload() {}
		virtual bool on_dependency_loaded(AssetEntry& dependency) { return true; }
		virtual bool on_all_dependencies_loaded() { return true; }
		virtual void on_store(cp::BinaryOutputStream& stream) const {}
		virtual MappedAssetData map_sub_asset(const AssetHandle& asset) = 0 { return MappedAssetData(asset); }
		virtual void unmap_sub_asset(const AssetHandle& asset) = 0 {}
		AssetEntry& get_entry() const { return *entry_; }
		AssetHandle get_handle() const { return AssetHandle(entry_); }

	protected:
	private:
		friend class AssetEntry;

		uint32 version_ = 0;
		AssetEntry* entry_ = nullptr;
		std::vector<AssetHandle> dependencies_;
		std::vector<AssetHandle> dependents_;
	};
}
