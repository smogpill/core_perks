// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource_holder.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
	template <class T> class ResourceHandle;

	class ResourceLoader
	{
	public:
		struct DependencyOptions
		{
			bool _optional = false;
		};

		ResourceLoader(ResourceHolder& holder);
		template <class T>
		auto AddDependency(const std::string& id, const DependencyOptions& options = DependencyOptions()) -> ResourceHandle<T>;
		auto GetId() const -> const std::string&;
		auto GetAssetPath() const -> std::string;
		bool ReadAsBinaryFile(std::vector<uint8>& content);
		bool PathExists() const;

	private:
		ResourceHolder& _holder;
	};

	template <class T>
	auto ResourceLoader::AddDependency(const std::string& id, const DependencyOptions& options) -> ResourceHandle<T>
	{
		ResourceHandle<T> handle(id);
		if (handle._holder->PathExists() || !options._optional)
		{
			handle._holder->_loadingParent = &_holder;
			_holder.AddLoadingDependency();
			handle.LoadAsync();
		}
		return handle;
	}
}
