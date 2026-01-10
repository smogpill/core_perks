// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/base/resource_base.h"
#include "core_perks/io/file/mapped_file_region.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class Resource; class ResourceManager;

	class ResourceEntry : public RefCounted
	{
	public:
		ResourceEntry(const ResourceID& id);
		~ResourceEntry();

		void set(const RefPtr<Resource>& resource);
		void load_async(std::function<void()>&& on_done = [](){});
		template <class T = Resource>
		RefPtr<T> get() const { return resource_; }
		const ResourceID& get_id() const { return id_; }

	protected:
		void on_all_refs_removed() override;
		void notify_ready_to_load();

	private:
		ResourceManager& manager();
		void update_state();
		void on_loading();

		std::mutex mutex_;
		ResourceState state_ = ResourceState::NONE;
		const Type* type_ = nullptr;
		ResourceID id_;
		RefPtr<Resource> resource_;
		RefPtr<Resource> loading_resource_;
		std::vector<std::function<void()>> on_done_funcs_;
		MappedFileRegion file_mapping_;
	};
}
