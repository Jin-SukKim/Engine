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

		// Spine Event 참고 (Unreal의 Montage 또는 Notify와 같은 느낌)
		struct Events {
			Event StartEvent; // 새 Animation이 시작할 때
			Event CompleteEvent; // 현재 Animation의 Loop가 끝날 때
			Event EndEvent; // Animation을 끝내고 다른 Animation으로 넘어갈 때
		};

		Animator2DComponent() : Super(L"Animator", ComponentType::Animator) {}
		virtual ~Animator2DComponent() override {}

		virtual void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		virtual void Render(IRenderer* r) override;

		// (이름, sprite sheet, sprite 시작 위치, sprite 크기, sprite의 root 위치, sprite 개수, 다음 sprite로 넘어가는 시간
		const std::wstring& CreateAnimation(const std::wstring& name, Texture* spriteSheet,
			Vector2 leftTop, Vector2 size, Vector2 offset, uint32 spriteLength, float duration);

		// sprite sheet이 아닌 각각 이미지로 분할되어 있는 애니메이션의 경우, 한 폴더의 이미지들을 모두 가져와 벡터로 사용
		const std::wstring& CreateAnimationByFolder(const std::wstring& name, const std::wstring& path, Vector2 offset, float duration);
		
		// Play할 Animation 찾기
		Flipbook* FindAnimation(const std::wstring& name);
		// Play Animation
		void PlayAnimation(const std::wstring& name);

		// Animation의 Event를 찾기
		Events* FindEvents(const std::wstring& name);
		// Animation의 Event
		// nullptr이 아닌 event만 설정
		void SetEvents(const std::wstring& name, std::function<void()> start, std::function<void()> complete, std::function<void()> end);
		Event* GetStartEvent(const std::wstring& name);
		Event* GetCompleteEvent(const std::wstring& name);
		Event* GetEndEvent(const std::wstring& name);

		// Sprite Animation의 각 Frame을 AssetManager에 저장
		const std::wstring& LoadSprite();

		bool IsComplete() { return _activeAnimation->IsComplete();  }
	private:
		std::map<std::wstring, std::unique_ptr<Flipbook>> _animations = {};
		Flipbook* _activeAnimation = nullptr; // 현재 플레이되는 animation
		
		// 각 Animation의 Event들 ( // 각 애니메이션마다 event 포이터 가지고 있기)
		std::map<std::wstring, std::unique_ptr<Events>> _events = {}; 
		Texture* _sprite = nullptr;
		std::wstring _frameKey = L"";
	};
}