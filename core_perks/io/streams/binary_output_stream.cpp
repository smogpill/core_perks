// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/streams/binary_output_stream.h"

namespace cp
{
	void OutputMemory::write(const void* buffer, uint64 size)
	{
		const uint8* src = static_cast<const uint8*>(buffer);
		while (size > 0) [[likely]]
		{
			if (size <= capacity_) [[likely]]
			{
				std::memcpy(ptr_, src, size);
				ptr_ += size;
				capacity_ -= size;
				return;
			}
			else if (capacity_)
			{
				std::memcpy(ptr_, src, capacity_);
				src += capacity_;
				ptr_ += capacity_;
				size -= capacity_;
				grow(size);
			}
			else
			{
				grow(size);
				if (!capacity_)
					return;
			}
		}
	}

	OutputMemoryView::OutputMemoryView(void* buffer, uint64 size)
		: buffer_(static_cast<uint8*>(buffer))
	{
		ptr_ = buffer_;
		capacity_ = size;
	}

	OutputMemoryBuffer::~OutputMemoryBuffer()
	{
		for (uint8* block : blocks_)
			delete[] block;
	}

	void OutputMemoryBuffer::grow(uint64 size)
	{
		blocks_.push_back(new uint8[block_size]);
		ptr_ = blocks_.back();
		capacity_ = block_size;
	}

	uint64 OutputMemoryBuffer::size() const
	{
		return blocks_.size() * block_size - capacity_;
	}

	void OutputMemoryBuffer::copy_to_buffer(void* dest_buffer)
	{
		if (blocks_.empty())
			return;
		uint8* dest = static_cast<uint8*>(dest_buffer);
		const uint block_count_minus_one = (uint)blocks_.size() - 1;
		for (uint block_idx = 0; block_idx < block_count_minus_one; ++block_idx)
		{
			const uint8* block = blocks_[block_idx];
			std::memcpy(dest, block, block_size);
		}

		const uint64 pos_in_block = block_size - capacity_;
		std::memcpy(dest, blocks_.back(), pos_in_block);
	}

	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const std::string& str)
	{
		stream << (uint32)str.size();
		stream.write(str.data(), str.size());
		return stream;
	}
}
