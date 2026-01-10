// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class FileHandle;

	class MappedFileRegion
	{
	public:
		enum class Access
		{
			READ_ONLY,
			READ_WRITE
		};
		MappedFileRegion(FileHandle& file, Access access);
		MappedFileRegion(FileHandle& file, Access access, uint64 offset, uint64 size);
		~MappedFileRegion();

		void* data() { return data_; }
		uint64 size() const { return size_; }
		std::span<uint8> span() { return std::span<uint8>(static_cast<uint8*>(data_), size_); }
		bool is_mapped() const { return data_ != nullptr; }
		uint8* begin() { return static_cast<uint8*>(data_); }
		uint8* end() { return static_cast<uint8*>(data_) + size_; }

	private:
		friend class FileHandle;

		void* data_ = nullptr;
		uint64 size_ = 0;
		RefPtr<FileHandle> file_;
#ifdef CP_WINDOWS
		HANDLE mapping_ = INVALID_HANDLE_VALUE;
#endif
	};
}
