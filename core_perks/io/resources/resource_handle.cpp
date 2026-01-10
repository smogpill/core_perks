// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/resources/resource_handle.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_entry.h"
#include "core_perks/io/streams/binary_output_stream.h"
#include "core_perks/io/streams/binary_input_stream.h"

namespace cp
{
	ResourceHandle::ResourceHandle(const ResourceID& id, const Type& type)
	{
		bind(id, type);
	}

	ResourceHandle::ResourceHandle(ResourceEntry* entry)
		: entry_(entry)
	{
	}

	void ResourceHandle::bind(const ResourceID& id, const Type& type)
	{
		entry_ = ResourceManager::get().get_or_create_entry(id, type);
	}

	void ResourceHandle::create()
	{
		CP_ASSERT(entry_);
		entry_->create();
	}

	const ResourceID& ResourceHandle::get_id() const
	{
		return entry_ ? entry_->get_id() : ResourceID::get_empty();
	}

	void ResourceHandle::release()
	{
		entry_.release();
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle)
	{
		ResourceID id;
		stream >> id;
		handle.bind(id);
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle)
	{
		if (handle)
		{
			stream << handle.get_type();
			stream << handle.get_id();
		}
		else
		{
			stream << (uint32)0;
		}
		return stream;
	}
}
