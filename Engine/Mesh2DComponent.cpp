#include "pch.h"
#include "Mesh2DComponent.h"
#include "AssetManager.h"
#include "SpriteActor.h"
#include "Transform2DComponent.h"
#include "Renderer/CppRenderer2D.h"

namespace JE {
	void Mesh2DComponent::Tick(const float& DeltaTime)
	{
	}

	void Mesh2DComponent::Render(IRenderer* r)
	{
		IRenderer2D* r2d = dynamic_cast<IRenderer2D*>(r);
		if (r2d == nullptr)
			return;

		SpriteActor* owner = dynamic_cast<SpriteActor*>(this->GetOwner());
		if (!owner)
			return;

		const Transform2DComponent* tr = owner->GetComponent<Transform2DComponent>();
		if (!tr)
			return;

		r2d->DrawMesh(_mesh, tr, _texture);

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