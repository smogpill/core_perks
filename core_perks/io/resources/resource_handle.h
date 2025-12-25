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
		~ResourceHandle() = default;
		//UntypedResourceHandle(const UntypedResourceHandle& other) : entry_(other.entry_) {}

		void release() { entry_.release(); }
		void load_async(std::function<void(ResourceEntry&)> on_done = [](ResourceEntry&) {});
		void store_async(std::function<void(ResourceEntry&)> on_done = [](ResourceEntry&) {});
		void unload_async();
		std::string get_name() const;
		const HashedString& get_id() const;
		//auto operator=(const UntypedResourceHandle& other) { entry_ = other.entry_; }
		operator bool() const { return entry_ != nullptr; }

	protected:
		friend class ResourceManager;
		friend BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle);
		friend BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle);

		void set(const HashedString& id, const Type& type);
		template <class T>
		void set(const HashedString& id) { set(id, T::get_type_static()); }
		Resource* get() const;

		cp::RefPtr<ResourceEntry> entry_;
	};

	template <class T>
	class TypedResourceHandle : public ResourceHandle
	{
		using Base = ResourceHandle;
	public:
		using Base::ResourceHandle;
		using Base::operator=;

		TypedResourceHandle() = default;
		TypedResourceHandle(const HashedString& id) : Base(id, T::get_type_static()) {}
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
		friend class ResourceManager;
		friend class ResourceLoader;
	};
}
