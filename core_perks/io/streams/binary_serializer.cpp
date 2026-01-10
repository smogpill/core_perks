// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/streams/binary_serializer.h"

namespace cp
{
	BinarySerializer::BinarySerializer(MappedFileRegion&& region, Mode mode)
		: writing_(mode == Mode::WRITE)
		, istream_(region.data(), region.size())
	{
	}
}