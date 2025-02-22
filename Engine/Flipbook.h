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
		// Sprite Sheet���� �ϳ��� 2D Animation
		struct Sprite {
			Vector2 LeftTop = Vector2::Zero; // ���� ��ġ
			Vector2 Offset = Vector2::Zero;  // Sprite�� root ��ġ
			float Duration = 0.f;			 // ���� sprite�� �Ѿ�� �ð�

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
		// �� Flipbook�� ������ �ִ� Actor�� ������ ����ϱ� ���� animation�� �����ϴ� Component�� �����͸� ������ �ֱ�
		Animator* _animator = nullptr;
		std::wstring _spriteName = L"";
		Texture* _spriteSheet = nullptr;
		Texture _sprite; // ������ �׷��� Texture
		Vector2 _size = Vector2::Zero;	 // Sprite ũ��
		std::vector<Sprite> _animationSheet = {};
		bool _bLoop = true;				 // �ݺ� ����
		bool _bComplete = false;		     // Animation ���� ����
		uint32 _index = -1;
		float _time = 0.f;
	};

}