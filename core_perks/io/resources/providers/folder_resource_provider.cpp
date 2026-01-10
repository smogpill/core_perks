// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/providers/folder_resource_provider.h"
#include "core_perks/io/file/mapped_file_region.h"

namespace cp
{
	FolderResourceProvider::FolderResourceProvider(const std::string& path)
		: folder_path_(path)
	{
	}

	MappedResourceData FolderResourceProvider::map_resource(ResourceEntry& entry)
	{
		ResourceInfo& info = get_or_open_file(entry);
		if (!info.file_.is_open())
			return MappedResourceData();
		MappedFileRegion region = info.file_.map(FileHandle::Access::READ_ONLY);
		return MappedResourceData(*this, entry, std::move(region));
	}

	void FolderResourceProvider::unmap_asset(MappedResourceData& data)
	{
		//ResourceInfo* info = get_file(data);
	}

	ResourceInfo& FolderResourceProvider::get_or_open_file(const ResourceEntry& entry)
	{
		ResourceInfo* info = get_file(entry);
		if (info)
			return *info;
		ResourceInfo* new_info = new ResourceInfo();
		const std::filesystem::path asset_path = folder_path_ / entry.get_resource_path();
		new_info->file_.open(asset_path.string(), FileHandle::Mode::READ);
		new_info->resource_id_hash_ = entry.get_id_hash();
		open_files_.push_back(new_info);
		return *new_info;
	}

	ResourceInfo* FolderResourceProvider::get_file(const ResourceEntry& entry) const
	{
		for (ResourceInfo* info : open_files_)
		{
			if (info->resource_id_hash_ == entry.get_id_hash())
				return info;
		}
		return nullptr;
	}
}
