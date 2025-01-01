#pragma once
#include "Entity.h"
#include "Component.h"

namespace JE {
	class Object : public Entity
	{
		using Super = Entity;
	public:
		enum class State {
			Active,
			Paused,
			Dead,
			End
		};

		Object(const std::wstring& name) : Super(name) {}
		virtual ~Object() override {}

		virtual void Init() override = 0;
		virtual void Tick(const float& DeltaTime) override = 0;
		virtual void Render() override = 0;

		template<typename T>
		T* AddComponent();
		template<typename T>
		T* AddComponent(const std::wstring& name);

		template<typename T>
		T* GetComponent();

		void SetActive(bool on);

		State GetActive() const { return _state; }
		bool IsActive() const { return _state == State::Active; }
		void Death() { _state = State::Dead; }
		bool IsDead() const { return _state == State::Dead; }

		void SetLayerType(LayerType type) { _layerType = type; }
		LayerType GetLayerType() const { return _layerType; }

	private:
		std::vector<std::unique_ptr<Component>> _components; // TODO: component의 type이 많으면 unordered_map으로 저장하는 방안 검토
		State _state = State::Active;
		LayerType _layerType = LayerType::None;
	};

	template<typename T>
	inline T* Object::AddComponent()
	{
		std::unique_ptr<T> comp = std::make_unique<T>();
		comp->Init();
		comp->SetOwner(this);
		size_t idx = static_cast<size_t>(comp->GetType());
		_components[idx] = std::move(comp);
		return dynamic_cast<T*>(_components[idx].get());
	}

	template<typename T>
	inline T* Object::AddComponent(const std::wstring& name)
	{
		std::unique_ptr<T> comp = std::make_unique<T>(name);
		comp->Init();
		comp->SetOwner(this);
		size_t idx = static_cast<size_t>(comp->GetType());
		_components[idx] = std::move(comp);
		return dynamic_cast<T*>(_components[idx].get());
	}

	template<typename T>
	inline T* Object::GetComponent()
	{
		T* comp = nullptr;
		for (std::unique_ptr<Component>& component : _components) {
			comp = dynamic_cast<T*>(component.get());
			// 원하는 Component를 찾았다면
			if (comp)
				break;
		}
		return comp;
	}
}