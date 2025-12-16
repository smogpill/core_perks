// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/base/reflection/type_manager.h"
#include "core_perks/math/numerical/hash.h"
#include "core_perks/containers/vector_extensions.h"

namespace cp
{
	Type::Type(const char* name)
		: _name(name)
		, _name_hash(hash::fast::hash32(_name))
	{
		get_types().push_back(this);
	}

	Type::~Type()
	{
		erase_unordered_first(get_types(), this);
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

	std::vector<Type*> Type::get_derived() const
	{
		std::vector<Type*> derived_types;
		for (Type* type : get_types())
			if (type != this && type->is_a(*this))
				derived_types.push_back(type);
		return derived_types;
	}

	std::vector<Type*>& Type::get_types()
	{
		static std::vector<Type*> types;
		return types;
	}
}
