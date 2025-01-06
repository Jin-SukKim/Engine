#include "pch.h"
#include "MeshComponent.h"
#include "AssetManager.h"

namespace JE {
	void MeshComponent::Tick(const float& DeltaTime)
	{
		// TODO: owner의 Transform Matrix를 가져와 Vertices 갱신
	}
	void MeshComponent::Render()
	{
	}
	void MeshComponent::SetMesh(const std::wstring& name)
	{
		_mesh = AssetManager::Find<Mesh>(name);
	}
	void MeshComponent::SetTexture(const std::wstring& name)
	{
		_texture = AssetManager::Find<Texture>(name);
	}
}