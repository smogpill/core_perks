// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class BinaryInputStream; class BinaryOutputStream; class Type; class ResourceID;
	class ResourceEntry;

	class ResourceHandle
	{
	public:
		ResourceHandle() = default;
		ResourceHandle(const ResourceID& id, const Type& type);

		void create();
		void bind(const ResourceID& id, const Type& type);
		const ResourceID& get_id() const;
		const Type* get_type() const;
		void release();
		operator bool() const { return entry_ != nullptr; }

	protected:
		friend class ResourceManager;
		friend class ResourceEntry;
		friend BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle);
		friend BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle);

		ResourceHandle(ResourceEntry* entry);

		RefPtr<ResourceEntry> entry_;
	};

	template <class T>
	class ResourceHandleT : public ResourceHandle
	{
		CP_BASE(ResourceHandle);
	public:
		ResourceHandleT() = default;
		ResourceHandleT(const ResourceID& id) : Base(id, T::get_type_static()) {}
		ResourceHandleT(const ResourceHandleT& handle) = default;
		ResourceHandleT(const ResourceHandle& handle);
		using ResourceHandle::operator=;

		void bind(const ResourceID& id) { Base::set_entry(id, T::get_type_static()); }
		T* get() const { return entry_ ? entry_->get<T>() : nullptr; }
	};

	template <class T>
	ResourceHandleT<T>::ResourceHandleT(const ResourceHandle& handle)
	{

	}
}
