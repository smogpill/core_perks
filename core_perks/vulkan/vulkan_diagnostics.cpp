// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/vulkan/vulkan_diagnostics.h"

#ifdef CP_VULKAN
namespace cp
{
	std::string GetVulkanError(VkResult result)
	{
		return std::format("[Vulkan][{}]", string_VkResult(result));
	}
}
#endif
