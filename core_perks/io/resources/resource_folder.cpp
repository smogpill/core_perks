// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_folder.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/file/file_handle.h"

#if 0

namespace cp
{
	CP_DEFINE_CLASS(ResourceFolder)
	{
	}

	ResourceFolder::ResourceFolder(const std::string& path)
		: path_(path)
	{
	}

	ResourceFolder::~ResourceFolder()
	{
		CP_ASSERT(open_files_.empty());
	}

	bool ResourceFolder::on_ready()
	{
		CP_TRY(Base::on_ready());
		CP_TRY(std::filesystem::exists(path_), "Path does not exist: {}", path_.string());
		ResourceManager::get().register_provider(*this);
		return true;
	}

	void ResourceFolder::on_unready()
	{
		ResourceManager::get().unregister_provider(*this);
		for (ResourceInfo* info : open_files_)
			delete info;
		open_files_.clear();
		Base::on_unready();
	}

	MappedResourceData ResourceFolder::map_sub_resource(const ResourceHandle& resource)
	{
		ResourceInfo& info = get_or_open_file(resource);
		if (!info.file_.is_open())
			return MappedResourceData(resource);
		return MappedResourceData(resource, get_handle(), std::move(info.file_.map()));
	}

	void ResourceFolder::unmap_sub_resource(const ResourceHandle& resource)
	{
		//ResourceInfo* info = get_file(resource);
	}

	ResourceFolder::ResourceInfo& ResourceFolder::get_or_open_file(const ResourceHandle& resource)
	{
		ResourceInfo* info = get_file(resource);
		if (info)
			return *info;
		info = new ResourceInfo();
		const std::filesystem::path resource_path = path_ / resource->get_resource_path();
		info->file_.open(resource_path.string(), FileHandle::Mode::READ);
		info->resource_id_hash_ = resource->get_id().hash();
		open_files_.push_back(info);
		return *info;
	}

	ResourceFolder::ResourceInfo* ResourceFolder::get_file(const ResourceHandle& resource) const
	{
		if (resource.is_null())
			return nullptr;
		const uint32 id_hash = resource->get_id_hash();
		for (ResourceInfo* info : open_files_)
			if (info->resource_id_hash_ == entry.id_hash)
				return info;
		return nullptr;
	}
}

#endif