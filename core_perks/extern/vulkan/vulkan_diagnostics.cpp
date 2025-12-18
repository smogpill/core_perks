// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/extern/vulkan/vulkan_diagnostics.h"

#ifdef CP_VULKAN
namespace cp
{
	std::string get_vulkan_error(VkResult result)
	{
		return std::format("[Vulkan][{}]", string_VkResult(result));
	}
}
#endif
