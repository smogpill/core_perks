// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/streams/binary_serializer.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
	ResourceEntry::ResourceEntry(const ResourceID& id)
		: id_(id)
	{
	}

	ResourceEntry::~ResourceEntry()
	{
		manager().on_entry_destroyed(*this);
	}

	void ResourceEntry::set(const RefPtr<Resource>& resource)
	{
		std::scoped_lock lock(mutex_);
		resource_ = resource;
		if (resource_)
			resource_->entry_ = this;
	}

	void ResourceEntry::load_async(std::function<void()>&& on_done)
	{
		std::scoped_lock lock(mutex_);
		if (state_ == ResourceState::READY || state_ == ResourceState::FAILED)
		{
			on_done();
			return;
		}
		on_done_funcs_.emplace_back(std::move(on_done));
		ResourceManager::LoadRequest request;
		request.handle_ = ResourceHandle(this);
		manager().push_load_request(std::move(request));
	}

	void ResourceEntry::on_all_refs_removed()
	{
		// TODO
	}

	ResourceManager& ResourceEntry::manager()
	{
		return ResourceManager::get();
	}

	void ResourceEntry::update_state()
	{
	}

	void ResourceEntry::notify_ready_to_load()
	{
		std::scoped_lock lock(mutex_);
		if (state_ == ResourceState::NONE)
		{
			state_ = ResourceState::LOADING;
			loading_resource_ = type_->create();
			JobSystem& job_system = JobSystem::get();
			job_system.enqeue([this]()
				{
					this->on_loading();
				});
		}
	}

	void ResourceEntry::on_loading()
	{
		CP_ASSERT(state_ == ResourceState::LOADING);
		BinarySerializer serializer;
		serializer.file_region_ = std::move(manager().map_resource(id_));
		if (!serializer.file_region_.is_mapped())
		{
			state_ = ResourceState::FAILED;
			return;
		}
	}
}
