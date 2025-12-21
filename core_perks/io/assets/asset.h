// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryOutputStream;
	class AssetLoader;

	class Asset
	{
		CP_CLASS(Asset);
	public:
		virtual ~Asset() = default;

		virtual bool on_load(AssetLoader& loader) = 0 { return true; }
		virtual bool on_dependency_loaded(Asset& dependency) { return true; }
		virtual bool on_all_dependencies_loaded() { return true; }
		virtual void on_store(cp::BinaryOutputStream& stream) const {}

	private:
		uint32 _version = 0;
		std::vector<UntypedAssetHandle> dependencies_;
		std::vector<UntypedAssetHandle> dependents_;
	};
}
