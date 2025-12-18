// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/assets/asset_handle.h"
#include "core_perks/io/assets/asset_manager.h"
#include "core_perks/io/assets/asset_entry.h"

namespace cp
{
	UntypedAssetHandle::UntypedAssetHandle(const std::string& id, const Type& type)
		: entry_(AssetManager::get().get_or_create_entry(id, type))
	{
	}

	UntypedAssetHandle::UntypedAssetHandle(AssetEntry* entry)
		: entry_(entry)
	{
	}

	void UntypedAssetHandle::set_id(const std::string& id, const Type& type)
	{
		entry_ = AssetManager::get().get_or_create_entry(id, type);
	}

	void UntypedAssetHandle::set_resource(Asset* resource)
	{
		if (entry_)
		{
			entry_->set(resource);
		}
		else
		{
			delete resource;
		}
	}

	void UntypedAssetHandle::unload_async()
	{
		if (entry_)
			entry_->unload_async();
	}

	void UntypedAssetHandle::store_async(std::function<void(bool)> on_done)
	{
		if (entry_)
			entry_->store_async(std::move(on_done));
	}

	void UntypedAssetHandle::load_async(std::function<void(bool)> on_done)
	{
		if (entry_)
			entry_->load_async(std::move(on_done));
	}

	std::string UntypedAssetHandle::get_name() const
	{
		return entry_ ? entry_->get_name() : nullptr;
	}

	const std::string& UntypedAssetHandle::get_id() const
	{
		static const std::string empty_string;
		return entry_ ? entry_->get_id() : empty_string;
	}

	Asset* UntypedAssetHandle::get() const
	{
		return entry_ ? entry_->get() : nullptr;
	}
}
