// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/singleton.h"
#include "core_perks/containers/vector.h"

namespace cp
{
	class TypeManager : public Singleton<TypeManager>
	{
	public:
		TypeManager();

		static Vector<Type*>& get_types();
		static std::unordered_map<uint32, Type*>& get_id_to_type_map();

	private:

	};
}
