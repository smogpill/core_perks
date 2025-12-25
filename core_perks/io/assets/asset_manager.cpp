// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_manager.h"
#include "core_perks/io/assets/asset_loader.h"
#include "core_perks/io/assets/asset_entry.h"
#include "core_perks/containers/vector_extensions.h"

namespace cp
{
	AssetManager::~AssetManager()
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(map_.empty());
	}

	void AssetManager::set_assets_path(const std::string& path)
	{
		assets_path_ = path;
	}

	void AssetManager::set_cache_path(const std::string& path)
	{
		cache_path_ = path;
	}

	void AssetManager::destroy_entry(AssetEntry& entry)
	{
		mutex_.lock();
		auto it = map_.find(entry.get_id().hash());
		map_.erase(it);
		mutex_.unlock();
	}

	void AssetManager::add_request(const AssetHandle& request)
	{
		std::scoped_lock lock(mutex_);
		requests_.push(request);
	}

	void AssetManager::process_requests()
	{
		for (;;)
		{
			AssetHandle handle;
			{
				std::scoped_lock lock(mutex_);
				if (requests_.empty())
					break;
				handle = requests_.back();
				requests_.pop();
			}
			if (handle.entry_)
				handle.entry_->update_async();
		}
	}

	void AssetManager::on_entry_updated(AssetEntry& entry)
	{
		process_requests();
	}

	MappedAssetData AssetManager::map_asset(const AssetHandle& asset)
	{
		std::scoped_lock lock(mutex_);
		for (Asset* provider : providers_ | std::views::reverse)
		{
			MappedAssetData data = provider->map_sub_asset(asset);
			if (data.data() != nullptr)
				return std::move(data);
		}
		return MappedAssetData(asset);
	}

	void AssetManager::register_provider(Asset& provider)
	{
		std::scoped_lock lock(mutex_);
		CP_ASSERT(!contains(providers_, &provider));
		providers_.push_back(&provider);
	}

	void AssetManager::unregister_provider(Asset& provider)
	{
		std::scoped_lock lock(mutex_);
		erase_first(providers_, &provider);
	}

	AssetEntry* AssetManager::get_or_create_entry(const HashedString& id, const Type& type)
	{
		std::scoped_lock lock(mutex_);
		auto it = map_.find(id.hash());
		if (it == map_.end())
		{
			AssetEntry* entry = new AssetEntry(id, type);
			map_[id.hash()] = entry;
			return entry;
		}
		else
		{
			return it->second;
		}
	}
}
