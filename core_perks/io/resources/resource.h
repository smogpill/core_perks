// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/mapped_resource_data.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class BinaryOutputStream;
	class ResourceLoader;
	class MappedResourceData;

	class Resource : public RefCounted
	{
		CP_BASE(RefCounted);
		CP_CLASS(Resource);
	public:
		virtual ~Resource() = default;

		virtual bool on_load() { return true; }
		virtual void on_unload() {}
		virtual bool on_dependency_loaded(ResourceEntry& dependency) { return true; }
		virtual bool on_ready() { return true; }
		virtual void on_unready() {}
		virtual void on_store(cp::BinaryOutputStream& stream) const {}
		virtual MappedResourceData map_sub_resource(const ResourceHandle& resource) = 0 { return MappedResourceData(resource); }
		virtual void unmap_sub_resource(const ResourceHandle& resource) = 0 {}

	protected:
	private:
		uint32 version_ = 0;
		ResourceEntry* entry_ = nullptr;
		std::vector<ResourceHandle> dependencies_;
		std::vector<ResourceHandle> dependents_;
	};
}
