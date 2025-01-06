#include "pch.h"
#include "Mesh2DComponent.h"
#include "AssetManager.h"

namespace JE {
	void Mesh2DComponent::Tick(const float& DeltaTime)
	{
		// TODO: owner의 Transform Matrix를 가져와 Vertices 갱신
	}

	void Mesh2DComponent::Render()
	{
	}
	
	void Mesh2DComponent::SetMesh(const std::wstring& name)
	{
		_mesh = AssetManager::Find<Mesh2D>(name);
	}

	void Mesh2DComponent::SetTexture(const std::wstring& name)
	{
		_texture = AssetManager::Find<Texture>(name);
	}
}