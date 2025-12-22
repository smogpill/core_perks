// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/streams/binary_input_stream.h"

namespace cp
{
	void operator>>(BinaryInputStream& stream, std::string& str)
	{
		uint32 size = 0;
		stream.read(size);
		str.resize(size);
		if (size > 0)
			stream.read(str.data(), size);
	}
}