// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2026 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/io/streams/binary_serializer.h"

namespace cp
{
	BinarySerializer::BinarySerializer(MappedFileRegion&& region, Mode mode)
		: writing_(mode == Mode::WRITE)
		, input_stream_(region.cdata(), region.size())
		, output_memory_view_(region.data(), region.size())
		, output_stream_(output_memory_view_)
	{
	}

	bool BinarySerializer::failed() const
	{
		if (writing_)
			return output_stream_.failed();
		else
			return input_stream_.failed();
	}
}