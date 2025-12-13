// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class StackAllocator
	{
	public:
		void* alloc(uint32 size, uint32 alignment = alignof(std::max_align_t)
#ifdef CP_DEBUG
			, const char* file = nullptr, int line = 0
#endif
		);
		void free(void* ptr);
	private:
		uint8* _memory = nullptr;
		uint64 _offset = 0;
		uint64 _capacity = 0;
	};
}
