#include "pch.h"
#include "MeshComponent.h"
#include "AssetManager.h"

namespace JE {
	void MeshComponent::Tick(const float& DeltaTime)
	{
		// TODO: owner�� Transform Matrix�� ������ Vertices ����
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