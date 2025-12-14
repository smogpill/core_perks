// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/base/reflection/type_manager.h"

namespace cp
{
	TypeManager::TypeManager()
	{
		for (Type* type : Type::_types)
			type->init();
	}

	std::vector<Type*> TypeManager::get_derived_types(const Type& base_type) const
	{
		std::vector<Type*> derived_types;
		for (Type* type : Type::_types)
			if (type != base_type && type->is_a(base_type))
				derived_types.push_back(base_type);
	}
}
