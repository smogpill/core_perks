// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/test/test.h"
#include "core_perks/base/reflection/type_manager.h"

namespace cp
{
	CP_DEFINE_CLASS(Test) {}

	void execute_all_tests()
	{
		TypeManager& type_manager = TypeManager::get();
		const std::vector<Type*>& types = type_manager.get_derived_types(Test::get_type_static());
		for (const Type* type : types)
		{
			Test* test = type->create<Test>();
			test->execute();
			delete test;
		}
	}
}
