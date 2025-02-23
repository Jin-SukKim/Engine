#pragma once
#include "Resource.h"

namespace JE {
	class AssetManager
	{
	public:
		template<typename T>
		static T* Find(const std::wstring& key);

		template<typename T>
		static T* Load(const std::wstring& key, const std::wstring& path);

		template<typename T>
		static T* Load(const std::wstring& key, const std::vector<Vertex2D>& vertices, const std::vector<uint32>& indices);
	
		static void Insert(const std::wstring& key, std::unique_ptr<Resource> resource);
	private:
		// TODO: wstring이 아닌 hash_key로 저장하면 메모리 사용량을 줄일 수 있지 않을까?
		static std::map<std::wstring, std::unique_ptr<Resource>> _resources;

	};

	template<typename T>
	inline T* AssetManager::Find(const std::wstring& key)
	{
		auto iter = _resources.find(key);
		if (iter == _resources.end())
			return nullptr;

		return dynamic_cast<T*>(iter->second.get());
	}

	template<typename T>
	inline T* AssetManager::Load(const std::wstring& key, const std::wstring& path)
	{
		T* resource = AssetManager::Find<T>(key);
		if (resource)
			return resource;

		std::unique_ptr<T> newResource = std::make_unique<T>(key);
		if (!newResource->Load(path)) {
			assert(false);
			return nullptr;
		}

		_resources[key] = std::move(newResource);
		return AssetManager::Find<T>(key);
	}

	template<typename T>
	inline T* AssetManager::Load(const std::wstring& key, const std::vector<Vertex2D>& vertices, const std::vector<uint32>& indices)
	{
		T* resource = AssetManager::Find<T>(key);
		if (resource)
			return resource;

		std::unique_ptr<T> newResource = std::make_unique<T>(key);
		if (!newResource->Load(vertices, indices)) {
			assert(false);
			return nullptr;
		}

		_resources[key] = std::move(newResource);
		return AssetManager::Find<T>(key);
	}

}