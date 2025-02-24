#pragma once
#include "Component.h"
#include "Flipbook.h"

namespace JE {

	class Animator2DComponent : public Component
	{
		using Super = Component;
	public:
		struct Event {
			void operator=(std::function<void()> func) {
				Event = std::move(func);
			}

			void operator()() {
				if (Event)
					Event();
			}

			std::function<void()> Event; // void (*event)();
		};

		// Spine Event ���� (Unreal�� Montage �Ǵ� Notify�� ���� ����)
		struct Events {
			Event StartEvent; // �� Animation�� ������ ��
			Event CompleteEvent; // ���� Animation�� Loop�� ���� ��
			Event EndEvent; // Animation�� ������ �ٸ� Animation���� �Ѿ ��
		};

		Animator2DComponent() : Super(L"Animator", ComponentType::Animator) {}
		virtual ~Animator2DComponent() override {}

		virtual void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		// (�̸�, sprite sheet, sprite ���� ��ġ, sprite ũ��, sprite�� root ��ġ, sprite ����, ���� sprite�� �Ѿ�� �ð�
		const std::wstring& CreateAnimation(const std::wstring& name, Texture* spriteSheet,
			Vector2 leftTop, Vector2 size, Vector2 offset, uint32 spriteLength, float duration);

		// sprite sheet�� �ƴ� ���� �̹����� ���ҵǾ� �ִ� �ִϸ��̼��� ���, �� ������ �̹������� ��� ������ ���ͷ� ���
		const std::wstring& CreateAnimationByFolder(const std::wstring& name, const std::wstring& path, Vector2 offset, float duration);
		
		// Play�� Animation ã��
		Flipbook* FindAnimation(const std::wstring& name);
		// Play Animation
		void PlayAnimation(const std::wstring& name);

		// Animation�� Event�� ã��
		Events* FindEvents(const std::wstring& name);
		// Animation�� Event
		// nullptr�� �ƴ� event�� ����
		void SetEvents(const std::wstring& name, std::function<void()> start, std::function<void()> complete, std::function<void()> end);
		Event* GetStartEvent(const std::wstring& name);
		Event* GetCompleteEvent(const std::wstring& name);
		Event* GetEndEvent(const std::wstring& name);

		// Sprite Animation�� �� Frame�� AssetManager�� ����
		const std::wstring& LoadSprite();

		bool IsComplete() { return _activeAnimation->IsComplete();  }
	private:
		std::map<std::wstring, std::unique_ptr<Flipbook>> _animations = {};
		Flipbook* _activeAnimation = nullptr; // ���� �÷��̵Ǵ� animation
		
		// �� Animation�� Event�� ( // �� �ִϸ��̼Ǹ��� event ������ ������ �ֱ�)
		std::map<std::wstring, std::unique_ptr<Events>> _events = {}; 
		Texture* _sprite = nullptr;
		std::wstring _frameKey = L"";
	};
}