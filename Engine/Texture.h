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

		// UV ��ǥ�� �´� Texture�� �ȼ� ��
		Color SamplePoint(const Vector2& uv, bool wrapped = true) const; // uv ��ǥ�� �´� Texture�� �ȼ� �� ��������

		// Texture�� ũ�⸦ ����� Texture �� ���� �ݺ�
		Color GetClamp(uint32 x, uint32 y) const;
		// Texture�� �ݺ������� ����� ��ó�� ǥ��
		Color GetWrapped(uint32 x, uint32 y) const;

		uint32 GetWidth() const { return _width; }
		uint32 GetHeight() const { return _height; }

	private:
		uint32 _width = 0;
		uint32 _height = 0;
		static const uint8 _channel = 4; // RGBA
		std::vector<Color> _buffer; // Texture �̹��� (1D�� 2Dó�� ���)
	};
}

