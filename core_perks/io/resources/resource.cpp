// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/streams/binary_serializer.h"
#include "core_perks/containers/vector_extensions.h"
#include "resource_folder.h"

namespace cp
{
	CP_DEFINE_CLASS(Resource)
	{
	}
	
	void Resource::add_dependency(const ResourceHandle& handle)
	{
		CP_ASSERT(handle);
		if (!contains(dependencies_, handle))
			dependencies_.push_back(handle);
	}

	void Resource::on_serialize(BinarySerializer& serializer)
	{
		serializer / dependencies_;
		/*
		if (serializer.writing_)
		{
			BinaryOutputStream& stream = serializer.ostream_;
			stream << (uint32)dependencies_.size();
			for (const ResourceHandle& dep : dependencies_)
			{
				stream << dep->get_type();
				stream << dep->get_id();
			}
		}
		else
		{
			dependencies_.clear();
			ResourceManager& manager = ResourceManager::get();
			BinaryInputStream& stream = serializer.istream_;
			uint32 count = 0;
			stream >> count;
			dependencies_.reserve(count);
			uint32 type_id = 0;
			HashedString id;
			for (uint32 i = 0; i < count; ++i)
			{
				stream >> type_id;
				stream >> id;
				const Type* type = Type::get_by_id(type_id);
				if (type)
				{
					ResourceHandle handle = manager.load_async(id, *type);
					dependencies_.emplace_back(std::move(handle));
				}
			}
		}
		*/
	}

	bool Resource::on_load()
	{
		return true;
	}

	void Resource::on_unload()
	{
		dependencies_.clear();
		dependents_.clear();
	}
}
