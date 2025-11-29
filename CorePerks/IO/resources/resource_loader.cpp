// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource_holder.h"
#include "core_perks/io/resources/resource_manager.h"

namespace cp
{
    ResourceLoader::ResourceLoader(ResourceHolder& holder)
        : _holder(holder)
    {
    }

    auto ResourceLoader::GetAssetPath() const -> std::string
    {
        return _holder.GetAssetPath();
    }

    bool ResourceLoader::ReadAsBinaryFile(std::vector<uint8>& content)
    {
        namespace fs = std::filesystem;
        const std::filesystem::path path = GetAssetPath();

        CP_TRY(fs::exists(path), "File does not exist: {}", path.string());
        CP_TRY(fs::is_regular_file(path), "Path is not a regular file: {}", path.string());

        // Open file in binary mode
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        CP_TRY(file.is_open(), "Failed to open file : {}", path.string());

        // Get file size
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read entire file
        content.clear();
        content.resize(size);
        CP_TRY(file.read(reinterpret_cast<char*>(content.data()), size), "Failed to read file: {}", path.string());

        return true;
    }

    bool ResourceLoader::PathExists() const
    {
        return _holder.PathExists();
    }

    auto ResourceLoader::GetId() const -> const std::string&
    {
        return _holder.GetId();
    }
}
