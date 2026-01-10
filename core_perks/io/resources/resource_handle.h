// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Resource; class BinaryInputStream; class BinaryOutputStream; class Type; class ResourceID;

	class ResourceHandle
	{
	public:
		ResourceHandle() = default;
		~ResourceHandle();

		const ResourceID& get_id() const;
		void release();
		ResourceHandle& operator=(Resource* resource);
		operator bool() const { return entry_ != nullptr; }

	protected:
		friend class ResourceManager;
		friend BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle);
		friend BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle);

		RefPtr<ResourceEntry> entry_;
	};

	template <class T>
	class ResourceHandleT : public ResourceHandle
	{
		CP_BASE(ResourceHandle);
	public:
		ResourceHandleT() = default;
		T* get() const { return static_cast<T*>(resource_.get()); }
		using operator=;
	};
}
