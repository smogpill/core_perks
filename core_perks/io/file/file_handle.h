// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class FileHandle : public RefCounted
	{
	public:
		enum class Mode
		{
			READ = 1 << 0,
			WRITE = 1 << 1,
			APPEND = 1 << 2,
			CREATE = 1 << 3,
			TRUNCATE = 1 << 4,
			EXCLUSIVE = 1 << 5
		};
		FileHandle() = default;
		FileHandle(const std::string& path, Mode mode = Mode::READ);
		FileHandle(FileHandle&& other);
		~FileHandle();
		
		bool open(const std::string& path, Mode mode);
		void close();
		bool is_open() const;
		uint64 get_size() const;
		const auto& get_native_handle() const { return native_handle_; }
#ifdef CP_DEBUG_OR_DEV
		const std::string& get_debug_path() const { return debug_path_; }
#endif

	private:
#ifdef CP_WINDOWS
		HANDLE native_handle_ = INVALID_HANDLE_VALUE;
#endif
#ifdef CP_DEBUG_OR_DEV
		std::string debug_path_;
#endif
	};
}
