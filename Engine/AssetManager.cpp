#include "pch.h"
#include "AssetManager.h"

namespace JE {
    std::map<std::wstring, std::unique_ptr<Resource>> AssetManager::_resources;
}
