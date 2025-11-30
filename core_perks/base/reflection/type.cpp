// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/base/reflection/type.h"
#include "core_perks/base/reflection/type_manager.h"
#include "core_perks/math/hash.h"

CP_DEFINE_TYPE(bool) {}
CP_DEFINE_TYPE(int8) {}
CP_DEFINE_TYPE(int16) {}
CP_DEFINE_TYPE(int32) {}
CP_DEFINE_TYPE(int64) {}
CP_DEFINE_TYPE(uint8) {}
CP_DEFINE_TYPE(uint16) {}
CP_DEFINE_TYPE(uint32) {}
CP_DEFINE_TYPE(uint64) {}
CP_DEFINE_TYPE(float) {}
CP_DEFINE_TYPE(double) {}

namespace cp
{
	Type::Type(const char* name)
		: _name(name)
		, _name_hash(hash::fast::hash32(_name))
	{
		_types.push_back(this);
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
}
