// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/memory/stack_allocator.h"
#include "core_perks/memory/memory.h"

namespace cp
{
	namespace stack_allocator
	{
		static constexpr uint32 marker = 0x0D15EA5E;

		struct Header
		{
			uint32 _size;
			uint32 _header_alloc_size;
#ifdef CP_DEBUG
			const char* _file; // not using const char* to avoid 8-byte alignment on the struct
			uint32 _line;
			uint32 _marker;
#endif
		};

		static_assert(sizeof(Header) % alignof(Header) == 0);

		CP_FORCE_INLINE Header* get_header(void* ptr)
		{
			return (Header*)ptr - 1;
		}
	}

	void* StackAllocator::alloc(const uint32 size, const uint32 alignment
#ifdef CP_DEBUG
		, const char* file, int line
#endif
		)
	{
		if (size == 0) [[unlikely]]
			return nullptr;

		const size_t ptr_offset = align_up(_offset + sizeof(stack_allocator::Header), std::max((size_t)alignment, alignof(stack_allocator::Header)));
		const size_t header_alloc_size = ptr_offset - _offset;
		_offset = ptr_offset + size;
		if (_offset > _capacity)
			CP_FATAL("Stack allocator is full D:");

		void* ptr = _memory + ptr_offset;
		stack_allocator::Header* header = stack_allocator::get_header(ptr);
		header->_size = size;
		CP_ASSERT(header_alloc_size <= std::numeric_limits<decltype(header->_header_alloc_size)>::max());
		header->_header_alloc_size = (uint8)header_alloc_size;
#ifdef CP_DEBUG
		header->_marker = stack_allocator::marker;
		header->_file = file;
		header->_line = line;
#endif
		return ptr;
	}

	void StackAllocator::free(void* ptr)
	{
        if (!ptr)
            return;

		stack_allocator::Header* header = stack_allocator::get_header(ptr);
#ifdef CP_DEBUG
        if (header->_marker != stack_allocator::marker) [[unlikely]]
            CP_FATAL("Stack allocator corruption :S (marker mismatch)");
#endif
		CP_ASSERT((uint8*)ptr + header->_size == _memory + _offset);
        _offset = (uint64)ptr - header->_header_alloc_size;
	}
}
