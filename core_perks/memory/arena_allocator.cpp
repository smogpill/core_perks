// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/memory/arena_allocator.h"

namespace cp
{
	void* ArenaAllocator::alloc(size_t size, size_t alignment)
	{
		if (size == 0)
			return nullptr;

		_offset = align_up(_offset, alignment);
		const size_t start = _offset;
		_offset += size;
		if (_offset > _capacity)
			CP_FATAL("Arena allocator is full");
		return _memory + start;
	}
}
