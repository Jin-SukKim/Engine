#include "pch.h"
#include "MeshComponent.h"
#include "AssetManager.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "Renderer/CppRenderer3D.h"

namespace JE {
	void MeshComponent::Tick(const float& DeltaTime)
	{
	}

	void MeshComponent::Render(IRenderer* r)
	{
		IRenderer3D* r3d = dynamic_cast<IRenderer3D*>(r);
		if (r3d == nullptr)
			return;

		Actor* owner = dynamic_cast<Actor*>(this->GetOwner());
		if (owner == nullptr)
			return;

		const TransformComponent* tr = owner->GetComponent<TransformComponent>();
		if (tr == nullptr)
			return;

		r3d->DrawMesh(_mesh, tr, _texture);
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