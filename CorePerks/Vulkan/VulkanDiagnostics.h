// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#ifdef CP_VULKAN
namespace cp
{
	std::string GetVulkanError(VkResult result);
}

#define CP_VULKAN_TRY(_cond_, ...) CP_SAFE_SCOPE( VkResult _result_ = _cond_; CP_TRY(_result_ == VK_SUCCESS, "{}", cp::GetVulkanError(_result_).c_str()); )
#define CP_VULKAN_CHECK(_cond_, ...) CP_SAFE_SCOPE( VkResult _result_ = _cond_; CP_CHECK(_result_ == VK_SUCCESS,"{}", cp::GetVulkanError(_result_).c_str()); )

#endif
