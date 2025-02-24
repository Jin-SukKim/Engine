#pragma once
#include "Resource.h"

namespace JE {
	class Animator2DComponent;
	class Texture;

	// 2D animation
	class Flipbook : public Resource
	{
		using Super = Resource;
	public:
		// Sprite Sheet���� �ϳ��� 2D Animation
		struct Sprite {
			Vector2 LeftTop = Vector2::Zero; // ���� ��ġ
			float Duration = 0.f;			 // ���� sprite�� �Ѿ�� �ð�

			Sprite() {}
			Sprite(Vector2 leftTop, float duration) : LeftTop(leftTop), Duration(duration) {}
		};

		Flipbook() : Resource(L"Flipbook", ResourceType::Flipbook) {}
		virtual ~Flipbook() override {}

		// TODO:
		virtual bool Load(const std::wstring& path) { return false; }

		void Tick(float DeltaTime);

		void CreateAnimation(const std::wstring& name, Texture* spriteSheet, 
			Vector2 leftTop, Vector2 size, Vector2 offset, uint32 spriteLength, float duration);
		void SetSpriteBuffer(std::vector<Color>& buffer);
	
		void Reset();
		const std::wstring& Name() const { return _spriteName; }
		bool IsLoop() const { return _bLoop; }
		bool IsComplete() const { return _bComplete; }
		void SetAnimator(Animator2DComponent* animator) { _animator = animator; }
		Vector2 GetSize() const { return _size; }
		const uint32 GetIndex() const { return _index; }
		
	private:
		// �� Flipbook�� ������ �ִ� Actor�� ������ ����ϱ� ���� animation�� �����ϴ� Component�� �����͸� ������ �ֱ�
		Animator2DComponent* _animator = nullptr;
		std::wstring _spriteName = L"";
		Texture* _spriteSheet = nullptr;

		Vector2 _size = Vector2::Zero;	 // Sprite ũ��
		std::vector<Sprite> _animationSheet = {};
		bool _bLoop = true;				 // �ݺ� ����
		bool _bComplete = false;		     // Animation ���� ����
		uint32 _index = -1;
		float _time = 0.f;
	};

}