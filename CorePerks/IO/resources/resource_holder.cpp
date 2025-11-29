// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#include "precompiled.h"
#include "core_perks/io/resources/resource_holder.h"
#include "core_perks/io/resources/resource_manager.h"
#include "core_perks/io/resources/resource_loader.h"
#include "core_perks/io/resources/resource.h"
#include "core_perks/io/streams.h"
#include "core_perks/math/hash.h"
#include "core_perks/threading/job/job_system.h"

namespace cp
{
    ResourceHolder::ResourceHolder(const std::string& id)
        : _id(id)
        , _idHash(hash::strong::hash64(id))
    {
    }

    ResourceHolder::~ResourceHolder()
    {
        delete _resource;
        delete _loadingResource;
    }

    void ResourceHolder::AddLoadingDependency()
    {
        ++_nbLoadingDependencies;
    }

    void ResourceHolder::RemoveLoadingDependency()
    {
        if (--_nbLoadingDependencies == 0)
        {
            _callbackMutex.lock();

            Resource* loadedResource = _loadingResource.exchange(nullptr);

            // Notify that all dependencies have been loaded
            if (_loadingResult)
                _loadingResult = loadedResource->OnDependenciesLoaded();

            // Swap the resource and update the state
            if (_loadingResult)
            {
                Resource* oldResource = _resource.exchange(loadedResource);
                delete oldResource;
                _state = ResourceState::READY;
            }
            else
            {
                delete loadedResource;
                if (_state != ResourceState::READY)
                    _state = ResourceState::FAILED;
            }

            // Call the callbacks
            std::vector<std::function<void(bool)>> callbacks = std::move(_loadCallbacks);
            _callbackMutex.unlock();
            for (auto& cb : callbacks)
            {
                cp::JobSystem::Get().Enqueue([callback = std::move(cb), loadingResult = _loadingResult]() { callback(loadingResult); });
            }

            // Notify the parent resource
            if (_loadingParent)
            {
                if (!_loadingResult)
                    _loadingParent->_loadingResult = false;
                _loadingParent->RemoveLoadingDependency();
                _loadingParent = nullptr;
            }
        }
    }

    void ResourceHolder::OnAllRefsRemoved()
    {
        ResourceManager::Get().DestroyHolder(*this);
        Base::OnAllRefsRemoved();
    }

    void ResourceHolder::AddLoadCallback(Callback callback)
    {
        std::scoped_lock lock(_callbackMutex);
        switch (_state)
        {
        case ResourceState::READY:
        {
            cp::JobSystem::Get().Enqueue([cb = std::move(callback)]() { cb(true); });
            break;
        }
        case ResourceState::FAILED:
        {
            cp::JobSystem::Get().Enqueue([cb = std::move(callback)]() { cb(false); });
            break;
        }
        default:
        {
            _loadCallbacks.push_back(std::move(callback));
            break;
        }
        }
    }

    void ResourceHolder::LoadAsync(std::function<Resource* ()> createFunc)
    {
        ResourceState expected = ResourceState::NONE;
        if (_state.compare_exchange_strong(expected, ResourceState::LOADING))
        {
            AddRef();
            cp::JobSystem::Get().Enqueue([this, create = std::move(createFunc)]()
                {
                    Resource* oldResource = _loadingResource.exchange(create());
                    delete oldResource;
                    ResourceLoader loader(*this);
                    AddLoadingDependency();
                    _loadingResult = _loadingResource.load()->OnLoad(loader);
                    RemoveLoadingDependency();
                    RemoveRef();
                });
        }
    }

    void ResourceHolder::UnloadAsync()
    {
        ResourceState expected = ResourceState::READY;
        if (_state.compare_exchange_strong(expected, ResourceState::RELEASING))
        {
            AddRef();
            cp::JobSystem::Get().Enqueue([this]()
                {
                    Resource* resource = _resource.exchange(nullptr);
                    delete resource;
                    RemoveRef();
                });
        }
    }

    void ResourceHolder::StoreAsync(Callback callbackFunc)
    {
        AddRef();

        cp::JobSystem::Get().Enqueue([this, callback = std::move(callbackFunc)]()
            {
                ResourceState expected = ResourceState::READY;
                while (!_state.compare_exchange_weak(expected, ResourceState::SERIALIZING))
                {
                    if (expected != ResourceState::SERIALIZING)
                    {
                        callback(false);
                        return;
                    }
                    expected = ResourceState::READY;
                }
                const Resource* resource = _resource;
                cp::BinaryOutputStream stream;
                resource->OnStore(stream);

                expected = ResourceState::SERIALIZING;
                const bool wasSerializing = _state.compare_exchange_strong(expected, ResourceState::READY);
                CP_ASSERT(wasSerializing);

                namespace fs = std::filesystem;

                const fs::path filePath = GetAssetPath();

                RemoveRef();

                {
                    auto parentPath = filePath.parent_path();
                    if (!parentPath.empty())
                        fs::create_directories(parentPath);
                }

                std::ofstream file(filePath, std::ios::binary);
                if (!file.is_open())
                {
                    CP_ERROR("Failed to open for write: {}", filePath.string());
                    callback(false);
                    return;
                }

                const std::string content = stream.str();

                file.write(content.data(), content.size());

                if (!file.good())
                {
                    CP_ERROR("Failed to write: {}", filePath.string());
                    callback(false);
                    return;
                }

                file.close();

                callback(true);
            });
    }

    bool ResourceHolder::PathExists() const
    {
        return std::filesystem::exists(GetAssetPath());
    }

    auto ResourceHolder::GetName() const -> std::string
    {
        const size_t pos = _id.find_last_of('/');
        return (pos == std::string::npos) ? _id : _id.substr(pos + 1);
    }

    void ResourceHolder::Set(Resource* resource)
    {
        Resource* oldResource = _resource.exchange(resource);
        delete oldResource;
    }

    auto ResourceHolder::GetAssetPath() const -> std::string
    {
        return (std::filesystem::path(ResourceManager::Get().GetAssetsPath()) / GetId()).string();
    }
}