// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/patterns/reference.h"

namespace cp
{
	class ResourceLoader;
	class Resource;

	enum class ResourceState
	{
		// Stable states
		NONE,
		READY,
		FAILED,

		// Transitions
		LOADING_DEPENDENCIES,
		LOADING_SELF,
		PROCESSING,
		SERIALIZING,
		RELEASING
	};

	template <class T>
	struct LoadResult
	{
		std::unique_ptr<T> _resource = nullptr;
		bool _result = false;
	};

	class ResourceHolder : public cp::RefCounted<ResourceHolder>
	{
		using Base = cp::RefCounted<ResourceHolder>;
	public:
		using Callback = std::function<void(bool)>;
		ResourceHolder(const std::string& id);
		virtual ~ResourceHolder();

		void AddLoadCallback(Callback callback);
		auto GetId() const -> const std::string& { return _id; }
		auto GetName() const -> std::string;
		auto GetIdHash() const -> uint64 { return _idHash; }
		void AddLoadingDependency();
		void RemoveLoadingDependency();
		void LoadAsync(std::function<Resource* ()> createFunc);
		bool PathExists() const;
		void UnloadAsync();
		void StoreAsync(Callback callback);
		auto Get() const -> Resource* { return _resource; }
		void Set(Resource* resource);
		auto GetAssetPath() const -> std::string;

	protected:
		friend class ResourceLoader;
		friend class ResourceManager;
		void OnAllRefsRemoved() override;

		std::string _id;
		uint64 _idHash = 0;
		cp::RefPtr<ResourceHolder> _loadingParent;
		std::atomic<uint32> _nbLoadingDependencies = 0;
		std::mutex _callbackMutex;
		std::atomic<ResourceState> _state = ResourceState::NONE;
		std::vector<Callback> _loadCallbacks;
		std::atomic<Resource*> _resource = nullptr;
		std::atomic<Resource*> _loadingResource = nullptr;
		bool _loadingResult = false;
	};
}
