// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_folder.h"
#include "core_perks/io/assets/asset_manager.h"
#include "core_perks/io/file.h"

namespace cp
{
	CP_DEFINE_CLASS(AssetFolder)
	{
	}

	AssetFolder::AssetFolder(const std::string& path)
		: path_(path)
	{
	}

	AssetFolder::~AssetFolder()
	{
		CP_ASSERT(open_files_.empty());
	}

	bool AssetFolder::on_load()
	{
		CP_TRY(Base::on_load());
		CP_TRY(std::filesystem::exists(path_), "Path does not exist: {}", path_.string());
		AssetManager::get().register_provider(*this);
		return true;
	}

	void AssetFolder::on_unload()
	{
		AssetManager::get().unregister_provider(*this);
		for (AssetInfo* info : open_files_)
			delete info;
		open_files_.clear();
		Base::on_unload();
	}

	MappedAssetData AssetFolder::map_sub_asset(const AssetHandle& asset)
	{
		AssetInfo& info = get_or_open_file(asset);
		if (!info.file_.is_open())
			return MappedAssetData(asset);
		return MappedAssetData(asset, get_handle(), std::move(info.file_.map()));
	}

	void AssetFolder::unmap_sub_asset(const AssetHandle& asset)
	{
		//AssetInfo* info = get_file(asset);
	}

	AssetFolder::AssetInfo& AssetFolder::get_or_open_file(const AssetHandle& asset)
	{
		AssetInfo* info = get_file(asset);
		if (info)
			return *info;
		info = new AssetInfo();
		const std::filesystem::path asset_path = path_ / asset->get_asset_path();
		info->file_.open(asset_path.string(), FileHandle::Mode::READ);
		info->asset_id_hash_ = asset->get_id_hash();
		open_files_.push_back(info);
		return *info;
	}

	AssetFolder::AssetInfo* AssetFolder::get_file(const AssetHandle& asset) const
	{
		if (asset.is_null())
			return nullptr;
		const uint32 id_hash = asset->get_id_hash();
		for (AssetInfo* info : open_files_)
			if (info->asset_id_hash_ == entry.id_hash)
				return info;
		return nullptr;
	}
}
