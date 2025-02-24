#include "pch.h"
#include "Flipbook.h"

namespace JE {
	void Flipbook::Tick(float DeltaTime)
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
				duration
			};

			_animationSheet.emplace_back(sprite);
		}
	}

	void Flipbook::SetSpriteBuffer(std::vector<Color>& buffer)
	{
		uint32 width = static_cast<uint32>(_size.X);
		uint32 height = static_cast<uint32>(_size.Y);

		buffer.clear();
		buffer.resize(width * height);

		// Sprite Sheet���� �׸� �κи� sub texture�� ������ ������ �غ�
		uint32 startX = static_cast<uint32>(_animationSheet[_index].LeftTop.X);
		uint32 startY = static_cast<uint32>(_animationSheet[_index].LeftTop.Y);
		for (uint32 y = 0; y < height; ++y) {
			for (uint32 x = 0; x < width; ++x) {
				buffer[y * width + x] = _spriteSheet->GetClamp(startX + x, startY + y);
			}
		}

	}

	void Flipbook::Reset()
	{
		_time = 0.f;
		_index = -1;
		_bComplete = false;
	}
}