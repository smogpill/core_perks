// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "Precompiled.h"
#include "CorePerks/IO/FileUtils.h"

namespace cp
{
    bool ReadBinaryFile(const std::string& path, std::vector<char>& buffer)
    {
        buffer.clear();

        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            CP_ERROR("Failed to open {}", path);
            return false;
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        buffer.resize(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return true;
    }
}
