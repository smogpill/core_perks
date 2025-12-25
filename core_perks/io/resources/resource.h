// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/providers/mapped_resource_data.h"

namespace cp
{
	class BinaryOutputStream;
	class ResourceLoader;
	class MappedResourceData;
	class ResourceEntry;

	class Resource
	{
		CP_CLASS(Resource);
	public:
		virtual ~Resource() = default;

		virtual bool on_load() { return true; }
		virtual void on_unload() {}
		virtual bool on_dependency_loaded(ResourceEntry& dependency) { return true; }
		virtual bool on_all_dependencies_loaded() { return true; }
		virtual void on_store(cp::BinaryOutputStream& stream) const {}
		virtual MappedResourceData map_sub_resource(const ResourceHandle& resource) = 0 { return MappedResourceData(resource); }
		virtual void unmap_sub_resource(const ResourceHandle& resource) = 0 {}
		ResourceEntry& get_entry() const { return *entry_; }
		ResourceHandle get_handle() const { return ResourceHandle(entry_); }

	protected:
	private:
		friend class ResourceEntry;

		uint32 version_ = 0;
		ResourceEntry* entry_ = nullptr;
		std::vector<ResourceHandle> dependencies_;
		std::vector<ResourceHandle> dependents_;
	};
}
