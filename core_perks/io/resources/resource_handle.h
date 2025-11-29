// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/io/resources/resource_holder.h"
#include "core_perks/patterns/reference.h"

namespace cp
{
	class ResourceHandleBase
	{
	public:
		ResourceHandleBase() = default;
		ResourceHandleBase(const std::string& id);
		ResourceHandleBase(const ResourceHandleBase& other) : _holder(other._holder) {}

		void SetID(const std::string& id);
		void Release() { _holder.Release(); }
		void AddLoadCallback(std::function<void(bool)> callback) { CP_ASSERT(_holder); _holder->AddLoadCallback(callback); }
		void UnloadAsync();
		void StoreAsync(std::function<void(bool)> onDone = [](bool) {});
		auto GetName() const -> std::string { return _holder ? _holder->GetName() : nullptr; }
		auto GetId() const -> const std::string& { return _holder ? _holder->GetId() : nullptr; }
		auto operator=(const ResourceHandleBase& other) { _holder = other._holder; }
		operator bool() const { return _holder != nullptr; }

	protected:
		ResourceHandleBase(ResourceHolder* holder) : _holder(holder) {}
		void LoadAsync(std::function<Resource* ()> createFunction);
		auto Get() const -> Resource* { if (_holder) return _holder->Get(); return nullptr; }
		void SetResource(Resource* resource);

		cp::RefPtr<ResourceHolder> _holder;
	};

	template <class T>
	class ResourceHandle : public ResourceHandleBase
	{
		using Base = ResourceHandleBase;
	public:
		using Base::ResourceHandleBase;
		using Base::operator=;
		/*
		ResourceHandle() = default;
		ResourceHandle(const std::string& id);
		ResourceHandle(const ResourceHandle& other) : _holder(other._holder) {}
		*/

		//void ReloadAsync();
		auto Get() const -> T* { return static_cast<T*>(Base::Get()); }
		void LoadAsync() { Base::LoadAsync([]() { return new T(); }); }
		void SetResource(T* resource) { Base::SetResource(resource); }
		auto operator=(const ResourceHandle& other) -> ResourceHandle& { _holder = other._holder; return *this; }

	private:
		friend class ResourceManager;
		friend class ResourceLoader;
	};
}
