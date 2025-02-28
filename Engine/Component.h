#pragma once
#include "Entity.h"

namespace JE {
	class Object;

	class Component : public Entity
	{
		using Super = Entity;
	public:
		Component(const std::wstring& name, ComponentType type) : Super(name), _type(type) {}
		virtual ~Component() override {}

		virtual void Init() override {};
		virtual void Tick(const float& DeltaTime) override {};
		virtual void Render(IRenderer* r) override;

		void SetOwner(Object* owner) { _owner = owner; }
		Object* GetOwner() const { return _owner; }
		ComponentType GetType() const { return _type; }

		void SetVisibility(bool show) { _show = show; }
	private:
		Object* _owner = nullptr;
		ComponentType _type;
		bool _show = true;
	};
}
