// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "pch.h"
#include "core_perks/extern/public_externals.h"
#include "core_perks/extern/private_externals.h"

#ifdef CP_XXHASH
#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION
#include <xxHash/xxhash.h>
#endif

#ifdef CP_VMA
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#endif

#ifdef CP_TINYOBJLOADER
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#endif
