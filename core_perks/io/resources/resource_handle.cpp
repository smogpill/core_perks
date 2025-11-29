// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
	ResourceHandleBase::ResourceHandleBase(const std::string& id)
		: _holder(ResourceManager::Get().GetOrCreateHolder(id))
	{
	}

	void ResourceHandleBase::SetID(const std::string& id)
	{
		_holder = ResourceManager::Get().GetOrCreateHolder(id);
	}

	void ResourceHandleBase::SetResource(Resource* resource)
	{
		if (_holder)
		{
			_holder->Set(resource);
		}
		else
		{
			delete resource;
		}
	}

	void ResourceHandleBase::UnloadAsync()
	{
		if (_holder)
			_holder->UnloadAsync();
	}

	void ResourceHandleBase::StoreAsync(std::function<void(bool)> onDone)
	{
		if (_holder)
			_holder->StoreAsync(std::move(onDone));
	}

	void ResourceHandleBase::LoadAsync(std::function<Resource* ()> createFunction)
	{
		if (_holder)
			_holder->LoadAsync(std::move(createFunction));
	}
}
