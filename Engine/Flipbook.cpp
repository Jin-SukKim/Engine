#include "pch.h"
#include "Flipbook.h"

namespace JE {
	bool Flipbook::Load(const std::wstring& path)
	{
		return false;
	}
	void Flipbook::Update(float DeltaTime)
	{
		if (_bComplete) {
			// Loop
			if (_bLoop)
				Reset();
			// ���� ������� Sprite Animation�� Loop���� �ʰ� �����ٸ�
			else
				return;
		}

		_time += DeltaTime;

		if (_animationSheet[_index].Duration < _time) {
			_time = 0.f;
			++_index;
			if (_index >= _animationSheet.size()) {
				_bComplete = true;
				--_index;
			}
		}
	}

	void Flipbook::Render()
	{
		// Sprite Sheet���� �׸� �κи� sub texture�� ������ ������ �غ�
		std::vector<Color>& buffer = _sprite.GetBuffer();
		
		uint32 width = _sprite.GetWidth();
		uint32 height = _sprite.GetHeight();

		uint32 startX = static_cast<uint32>(_animationSheet[_index].LeftTop.X);
		uint32 startY = static_cast<uint32>(_animationSheet[_index].LeftTop.Y);
		for (uint32 y = 0; y < height; ++y) {
			for (uint32 x = 0; x < width; ++x) {
				buffer[y * width + x] = _spriteSheet->GetClamp(startX + x, startY + y); 
			}
		}

	}

	void Flipbook::CreateAnimation(const std::wstring& name, Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, uint32 spriteLength, float duration)
	{
		_spriteName = name;
		_spriteSheet = spriteSheet;
		_size = size;
		// sprite sheet���� sprite length��ŭ�� sprite�� ���� ����
		for (size_t i = 0; i < spriteLength; ++i) {
			// sprite ����
			Sprite sprite = {
				Vector2(leftTop.X + (i * size.X), leftTop.Y),
				offset,
				duration
			};

			_animationSheet.emplace_back(sprite);
		}

		// Sprite Sheet���� �׸� �κи� sub texture�� ������ ������ �غ�
		_sprite.SetWidth(static_cast<uint32>(size.X));
		_sprite.SetHeight(static_cast<uint32>(size.Y));

		std::vector<Color>& buffer = _sprite.GetBuffer();
		buffer.clear();
		buffer.resize(_sprite.GetWidth() * _sprite.GetHeight());
	}
	void Flipbook::Reset()
	{
		_time = 0.f;
		_index = 0;
		_bComplete = false;
	}
}