// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/base/reflection/type_manager.h"

namespace cp
{
	TypeManager::TypeManager()
	{
		for (Type* type : get_types())
			type->init();
	}

	Vector<Type*>& TypeManager::get_types()
	{
		static Vector<Type*> types;
		return types;
	}

	std::unordered_map<uint32, Type*>& TypeManager::get_id_to_type_map()
	{
		static std::unordered_map<uint32, Type*> map;
		return map;
	}
}
