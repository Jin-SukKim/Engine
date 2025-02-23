#include "pch.h"
#include "AssetManager.h"

namespace JE {
    std::map<std::wstring, std::unique_ptr<Resource>> AssetManager::_resources;
    
    void AssetManager::Insert(const std::wstring& key, std::unique_ptr<Resource> resource)
    {
        if (resource == nullptr || key == L"")
            return;

        _resources[key] = std::move(resource);
    }
}
