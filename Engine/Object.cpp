#include "pch.h"
#include "Object.h"

namespace JE {
	void Object::Init()
	{
		_components.resize(static_cast<size_t>(ComponentType::MaxComponentType));

		for (std::unique_ptr<Component>& component : _components) {
			Component* comp = component.get();
			if (comp)
				comp->Init();
		}
	}
	void Object::Tick(const float& DeltaTime)
	{
		for (std::unique_ptr<Component>& component : _components) {
			Component* comp = component.get();
			if (comp)
				comp->Tick(DeltaTime);
		}
	}
	void Object::Render(IRenderer* r)
	{
		for (std::unique_ptr<Component>& component : _components) {
			Component* comp = component.get();
			if (comp)
				comp->Render(r);
		}
	}
	void Object::SetActive(bool on)
	{
		if (on)
			_state = State::Active;
		else
			_state = State::Paused;
	}
}