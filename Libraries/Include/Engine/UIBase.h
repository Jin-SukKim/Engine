#pragma once
#include "Entity.h"

namespace JE {
	class UIBase : public Entity
	{
		using Super = Entity;
	public:
		// TODO: �̰� �ƴ� InputManager�� ����ϴ°� ���ƺ���
		// �׷��� ���� Object�� UI�� Input�� �������� ���� ��Ǿ� �� �� ����.
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

		// UI�� Load �Ǿ����� �ʱ�ȭ
		virtual void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		// UI�� Ȱ��ȭ�Ǹ� ȣ��
		void Active();
		// ��Ȱ��ȭ�Ǹ� ȣ��
		void InActive();
		// UI�� ������� ȣ��
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
