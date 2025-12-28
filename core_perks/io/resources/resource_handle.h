// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Resource; class ResourceEntry; class BinaryInputStream; class BinaryOutputStream; 
	class Type; class HashedString;

	class ResourceHandle
	{
	public:
		ResourceHandle() = default;
		explicit ResourceHandle(ResourceEntry* entry);
		ResourceHandle(const HashedString& id, const Type& type);
		template <class T>
		explicit ResourceHandle(const HashedString& id) : ResourceHandle(id, T::get_type_static()) {}
		~ResourceHandle() = default;
		//UntypedResourceHandle(const UntypedResourceHandle& other) : entry_(other.entry_) {}

		void load_async(std::function<void(ResourceEntry&)> on_done = [](ResourceEntry&) {});
		void store_async(std::function<void(ResourceEntry&)> on_done = [](ResourceEntry&) {});
		void unload_async();
		void release() { entry_.release(); }
		std::string get_name() const;
		const HashedString& get_id() const;
		//auto operator=(const ResourceHandle& other) { entry_ = other.entry_; }
		operator bool() const { return entry_ != nullptr; }

	protected:
		friend class ResourceManager;
		friend BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle);
		friend BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle);

		void set(const HashedString& id, const Type& type);
		template <class T>
		void set(const HashedString& id) { set(id, T::get_type_static()); }
		Resource* get() const;
		
		RefPtr<ResourceEntry> entry_;
	};
}
