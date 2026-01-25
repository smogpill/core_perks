// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/patterns/test.h"

namespace cp
{
	CP_DEFINE_CLASS(Test) {}

	void Test::execute_all()
	{
		for (const Type* type : TypeManager::get_types())
		{
			if (!type->is_a<Test>())
				continue;
			Test* test = type->create<Test>();
			if (test)
			{
				test->execute();
				delete test;
			}
		}
	}
}
