// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Asset;
	class AssetEntry;

	class UntypedAssetHandle
	{
	public:
		UntypedAssetHandle() = default;
		UntypedAssetHandle(const std::string& id, const Type& type);
		~UntypedAssetHandle() = default;
		//UntypedResourceHandle(const UntypedResourceHandle& other) : entry_(other.entry_) {}

		void release() { entry_.release(); }
		void load_async(std::function<void(bool)> on_done = [](bool) {});
		void store_async(std::function<void(bool)> on_done = [](bool) {});
		void unload_async();
		std::string get_name() const;
		const std::string& get_id() const;
		//auto operator=(const UntypedResourceHandle& other) { entry_ = other.entry_; }
		operator bool() const { return entry_ != nullptr; }

	protected:
		UntypedAssetHandle(AssetEntry* entry);

		void set_id(const std::string& id, const Type& type);
		void set_resource(Asset* resource);
		Asset* get() const;

		cp::RefPtr<AssetEntry> entry_;
	};

	template <class T>
	class AssetHandle : public UntypedAssetHandle
	{
		using Base = UntypedAssetHandle;
	public:
		using Base::UntypedAssetHandle;
		using Base::operator=;

		AssetHandle() = default;
		AssetHandle(const std::string& id) : Base(id, T::get_type_static()) {}
		/*
		ResourceHandle() = default;
		ResourceHandle(const std::string& id);
		ResourceHandle(const ResourceHandle& other) : entry_(other.entry_) {}
		*/

		void set_id(const std::string& id) { Base::set_id(id, T::get_type_static()); }
		//void ReloadAsync();
		T* get() const { return static_cast<T*>(Base::get()); }
		void set_resource(T* resource) { Base::set_resource(resource); }
		//auto operator=(const ResourceHandle& other) -> ResourceHandle& { entry_ = other.entry_; return *this; }

	private:
		friend class AssetManager;
		friend class AssetLoader;
	};
}
