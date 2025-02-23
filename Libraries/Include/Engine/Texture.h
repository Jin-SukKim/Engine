#pragma once
#include "Resource.h"

namespace JE {
	class Texture : public Resource 
	{
		using Super = Resource;
	public:
		Texture(const std::wstring& name) : Super(name, ResourceType::Texture) {}
		virtual ~Texture() override {}

		static Texture* Create(const std::wstring name, uint32 width, uint32 height);
		bool Load(const std::wstring& path) override;

		// UV ��ǥ�� �´� Texture�� �ȼ� ��
		Color SamplePoint(const Vector2& uv, bool wrapped = true) const; // uv ��ǥ�� �´� Texture�� �ȼ� �� ��������

		// Texture�� ũ�⸦ ����� Texture �� ���� �ݺ�
		Color GetClamp(uint32 x, uint32 y) const;
		// Texture�� �ݺ������� ����� ��ó�� ǥ��
		Color GetWrapped(uint32 x, uint32 y) const;

		void SetWidth(uint32 width) { _width = width; }
		uint32 GetWidth() const { return _width; }
		void SetHeight(uint32 height) { _height = height; }
		uint32 GetHeight() const { return _height; }

		std::vector<Color>& GetBuffer() { return _buffer; }
		const std::vector<Color>& GetBuffer() const { return _buffer; }
	private:
		uint32 _width = 0;
		uint32 _height = 0;
		static const uint8 _channel = 4; // RGBA
		std::vector<Color> _buffer; // Texture �̹��� (1D�� 2Dó�� ���)
	};
}

