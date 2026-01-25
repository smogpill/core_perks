// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/base/reflection/type_manager.h"
#include "core_perks/math/numerical/hash.h"
#include "core_perks/containers/vector_extensions.h"
#include "core_perks/io/streams/binary_input_stream.h"
#include "core_perks/io/streams/binary_output_stream.h"

namespace cp
{
	Type::Type(const char* name)
		: _name(name)
	{
		_id = hash::fast::hash32(_name);
		TypeManager::get_types().push_back(this);
		CP_ASSERT(TypeManager::get_id_to_type_map().find(_id) == TypeManager::get_id_to_type_map().end());
		TypeManager::get_id_to_type_map()[_id] = this;
	}

	Type::~Type()
	{
		TypeManager::get_id_to_type_map().erase(_id);
		TypeManager::get_types().swap_and_pop_first(this);
	}

	void Type::init(Type* type)
	{
		if (type == nullptr)
		{
			if (_initialized)
				return;
			_initialized = true;
			type = this;
		}
		if (_base)
			_base->init(type);
		if (_init)
			_init(*type);
	}

	Type* Type::get_by_id(Id id)
	{
		const auto& map = TypeManager::get_id_to_type_map();
		auto it = map.find(id);
		if (it != map.end())
			return it->second;
		return nullptr;
	}

	bool Type::is_a(const Type& type) const
	{
		const Type* it = this;
		do
		{
			if (it == &type)
				return true;
			it = it->_base;
		} while (it);
		return false;
	}

	BinaryInputStream& operator>>(BinaryInputStream& stream, Type*& type)
	{
		Type::Id id;
		stream >> id;
		type = Type::get_by_id(id);
		return stream;
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const Type* type)
	{
		stream << (type ? type->get_id() : Type::Id(0));
		return stream;
	}
}
