// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryOutputStream
	{
	public:
		BinaryOutputStream();
		~BinaryOutputStream();
		void write(const void* buffer, uint64 size);
		template <class T> requires std::is_trivially_copyable_v<T>
		void write(const T& value);
		uint64 size() const;
		void copy_to_buffer(void* dest_buffer);

	private:
		static constexpr uint32 block_size = 64 * 1024;
		std::vector<uint8*> blocks_;
		uint32 current_block_pos_ = 0;
	};

	template <class T> requires std::is_trivially_copyable_v<T>
	CP_FORCE_INLINE void BinaryOutputStream::write(const T& value)
	{
		write(&value, sizeof(T));
	}

	template <class T> requires std::is_trivially_copyable_v<T>
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const T& value) { stream.write(value); return stream; }
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const std::string& str);

	template <class T>
	BinaryOutputStream& operator<<(BinaryOutputStream& stream, const std::vector<T>& v)
	{
		const uint32 size = (uint32)v.size();
		stream << size;
		if (size > 0)
		{
			if constexpr (std::is_trivially_copyable_v<T>)
			{
				stream.write(v.data(), size * sizeof(T));
			}
			else
			{
				for (uint32 i = 0; i < size; ++i)
					stream << v[i];
			}
		}

		return stream;
	}
}
