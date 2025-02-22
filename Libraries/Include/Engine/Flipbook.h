#pragma once
#include "Resource.h"

namespace JE {
	class Animator;
	class Texture;

	// 2D animation
	class Flipbook : public Resource
	{
		using Super = Resource;
	public:
		// Sprite Sheet에서 하나의 2D Animation
		struct Sprite {
			Vector2 LeftTop = Vector2::Zero; // 시작 위치
			Vector2 Offset = Vector2::Zero;  // Sprite의 root 위치
			float Duration = 0.f;			 // 다음 sprite로 넘어가는 시간

			Sprite() {}
			Sprite(Vector2 leftTop, Vector2 offset, float duration) : LeftTop(leftTop), Offset(offset), Duration(duration) {}
		};

		Flipbook() : Resource(L"Flipbook", ResourceType::Flipbook), _sprite(L"Sprite") {}
		virtual ~Flipbook() override {}

		bool Load(const std::wstring& path);

		void Update(float DeltaTime);
		void Render();

		void CreateAnimation(const std::wstring& name, Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, uint32 spriteLength, float duration);
	
		void Reset();
		const std::wstring& Name() const { return _spriteName; }
		bool IsLoop() const { return _bLoop; }
		bool IsComplete() const { return _bComplete; }
		void SetAnimator(Animator* animator) { _animator = animator; }
		const Texture* GetCurrentSprite() const { return &_sprite; }
	private:
		// 이 Flipbook을 가지고 있는 Actor를 가져와 사용하기 위해 animation을 관리하는 Component의 포인터를 가지고 있기
		Animator* _animator = nullptr;
		std::wstring _spriteName = L"";
		Texture* _spriteSheet = nullptr;
		Texture _sprite; // 실제로 그려질 Texture
		Vector2 _size = Vector2::Zero;	 // Sprite 크기
		std::vector<Sprite> _animationSheet = {};
		bool _bLoop = true;				 // 반복 할지
		bool _bComplete = false;		     // Animation 종료 여부
		uint32 _index = -1;
		float _time = 0.f;
	};

}