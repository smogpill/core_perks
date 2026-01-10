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
	void ResourceHandle::release()
	{
		entry_.release();
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, ResourceHandle& handle)
	{
		ResourceID id;
		stream >> id;
		handle = ResourceManager::get().get(id);
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const ResourceHandle& handle)
	{
		stream << handle.resource_ ? handle.resource_->get_id() : "";
		return stream;
	}
}
