// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_holder.h"

namespace cp
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager(const std::string& assetsPath);
		~ResourceManager();

		auto GetAssetsPath() const -> const std::string& { return _assetsPath; }

	private:
		friend class ResourceHolder;
		friend class ResourceHandleBase;

		auto GetOrCreateHolder(const std::string& id) -> ResourceHolder*;
		void DestroyHolder(ResourceHolder& holder);

		mutable std::mutex _mutex;
		std::unordered_map<std::string, ResourceHolder*> _map;
		std::queue<ResourceHandleBase*> _loadQueue;
		std::string _assetsPath;
	};
}
