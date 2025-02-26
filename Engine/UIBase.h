#pragma once
#include "Entity.h"

namespace JE {
	class UIBase : public Entity
	{
		using Super = Entity;
	public:
		// TODO: 이게 아닌 InputManager를 사용하는게 좋아보임
		// 그런데 기존 Object과 UI의 Input은 연관없이 각각 운영되야 될 것 같다.
		struct Event {
			void operator=(std::function<void()> func) { _event = std::move(func); }
			void operator()() {
				if (_event)
					_event();
			}
			std::function<void()> _event;
		};

		UIBase(UIType type) : Super(L"UI"), _type(type) {}
		virtual ~UIBase() override {}

		// UI가 Load 되었을때 초기화
		virtual void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		// UI가 활성화되면 호출
		void Active();
		// 비활성화되면 호출
		void InActive();
		// UI가 사라질때 호출
		void UIClear();

		virtual void OnInit() {}
		virtual void OnActive() {}
		virtual void OnInActive() {}
		virtual void OnTick(const float& DeltaTime) {}
		virtual void OnRender(IRenderer* r) {}
		virtual void OnClear() {}

		void SetUIType(UIType type) { _type = type; }
		UIType GetUIType() { return _type; }
		void SetFullScreen(bool enable) { _bFullScreen = enable; }
		bool IsFullScreen() const { return _bFullScreen; }
		void SetPos(Vector2 pos) { _pos = pos; }
		Vector2 GetPos() const { return _pos; }
		void SetSize(Vector2 size) { _size = size; }
		Vector2 GetSize() const { return _size; }

	protected:
		Vector2 _pos = Vector2::Zero;
		Vector2 _size = Vector2::One;
		bool _bMouseOn = false;

	private:
		UIType _type;
		bool _bFullScreen = false;
		bool _bEnabled = false;

		// std::vector<UIBase*> _childs;
		UIBase* _parent;
	};
}
