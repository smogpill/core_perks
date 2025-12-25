// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Asset; class AssetEntry; class BinaryInputStream; class BinaryOutputStream; 
	class Type; class HashedString;

	class AssetHandle
	{
	public:
		AssetHandle() = default;
		explicit AssetHandle(AssetEntry* entry);
		AssetHandle(const HashedString& id, const Type& type);
		~AssetHandle() = default;
		//UntypedResourceHandle(const UntypedResourceHandle& other) : entry_(other.entry_) {}

		void release() { entry_.release(); }
		void load_async(std::function<void(AssetEntry&)> on_done = [](AssetEntry&) {});
		void store_async(std::function<void(AssetEntry&)> on_done = [](AssetEntry&) {});
		void unload_async();
		std::string get_name() const;
		const HashedString& get_id() const;
		//auto operator=(const UntypedResourceHandle& other) { entry_ = other.entry_; }
		operator bool() const { return entry_ != nullptr; }

	protected:
		friend class AssetManager;
		friend BinaryInputStream& operator>>(BinaryInputStream& stream, AssetHandle& handle);
		friend BinaryOutputStream& operator<<(BinaryOutputStream& stream, const AssetHandle& handle);

		void set(const HashedString& id, const Type& type);
		template <class T>
		void set(const HashedString& id) { set(id, T::get_type_static(); }
		Asset* get() const;

		cp::RefPtr<AssetEntry> entry_;
	};

	template <class T>
	class TypedAssetHandle : public AssetHandle
	{
		using Base = AssetHandle;
	public:
		using Base::AssetHandle;
		using Base::operator=;

		TypedAssetHandle() = default;
		TypedAssetHandle(const HashedString& id) : Base(id, T::get_type_static()) {}
		/*
		ResourceHandle() = default;
		ResourceHandle(const HashedString& id);
		ResourceHandle(const ResourceHandle& other) : entry_(other.entry_) {}
		*/

		void set(const HashedString& id) { Base::set(id, T::get_type_static()); }
		//void ReloadAsync();
		T* get() const { return static_cast<T*>(Base::get()); }
		void set_resource(T* resource) { Base::set_resource(resource); }
		//auto operator=(const ResourceHandle& other) -> ResourceHandle& { entry_ = other.entry_; return *this; }

	private:
		friend class AssetManager;
		friend class AssetLoader;
	};
}
