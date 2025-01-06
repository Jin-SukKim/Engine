#pragma once
#include "Resource.h"

namespace JE {
	class Texture : public Resource 
	{
		using Super = Resource;
	public:
		Texture(const std::wstring& name) : Super(name, ResourceType::Texture) {}
		virtual ~Texture() override {}

		bool Load(const std::wstring& path) override;

		// UV 좌표에 맞는 Texture의 픽셀 색
		Color SamplePoint(const Vector2& uv, bool wrapped = true) const; // uv 좌표에 맞는 Texture의 픽셀 색 가져오기

		// Texture의 크기를 벗어나면 Texture 끝 색상 반복
		Color GetClamp(uint32 x, uint32 y) const;
		// Texture가 반복적으로 연결된 것처럼 표현
		Color GetWrapped(uint32 x, uint32 y) const;

		uint32 GetWidth() const { return _width; }
		uint32 GetHeight() const { return _height; }

	private:
		uint32 _width = 0;
		uint32 _height = 0;
		static const uint8 _channel = 4; // RGBA
		std::vector<Color> _buffer; // Texture 이미지 (1D를 2D처럼 사용)
	};
}

