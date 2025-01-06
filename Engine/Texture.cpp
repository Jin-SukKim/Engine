#include "pch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace JE {
	bool Texture::Load(const std::wstring& path)
	{
		//fs::path filePath = fs::current_path().parent_path() / "Resources" / filename;
		fs::path filePath = path;
		if (!fs::exists(filePath)) // ���� ���� ���� Ȯ��
			return false;

		// �̹��� ������
		int width = 0, height = 0, channel = 0;

		// stbi_load�� ���� wstring -> string���� ��ȯ
		std::string pathStr = filePath.string();
		unsigned char* img = stbi_load(pathStr.c_str(), &width, &height, &channel, STBI_rgb_alpha); // uint8�� ����
		if (img == nullptr)
			return false;
		
		_width = static_cast<uint32>(width);
		_height = static_cast<uint32>(height);
		// image buffer �ʱ�ȭ
		_buffer.clear();
		// Color�� ������ ���� ���� (���� rgba�� ���� �����ϸ� r * b * g * aó�� ä�ε� ���������)
		_buffer.reserve(_width * _height);

		// �̹��� �ȼ� �� ����
		for (uint32 y = 0; y < _height; ++y) {
			for (uint32 x = 0; x < _width; ++x) {
				// �̹����� [0, 255] ������ ���� rgba�� ���� ���� �����ϱ⿡ channel��ŭ �����ֱ�
				uint32 idx = (y * _width + x) * _channel;
				Color32 color(img[idx], img[idx + 1], img[idx + 2], img[idx + 3]);
				_buffer.emplace_back(Color(color)); // �޸𸮸� �̸� Ȯ�������Ƿ� ���� �Ҵ���� �ʴ� �� �� ����
			}
		}

		return true;
	}
	Color Texture::SamplePoint(const Vector2& uv, bool wrapped) const
	{
		/*
			Texture�� ��ǥ�� �̹����� ũ�⿡ ���� �̹��� ��ǥ�� ��ȯ�� ���

			UV ��ǥ���� ������ [0, 1]�̰� �̹��� ��Ǧ�� �ȼ��� ����� ���� ����Ǿ� �ִٰ� �����ϱ⿡
			0.5�� Ȯ���ؼ� ��ǥ�� ��ȯ, �� �̹��� ��ǥ�� [-0.5, �̹��� ũ�� - 1 + 0.5]�� ��
		*/ 
		Vector2 imageUV = Vector2(uv.X * _width, uv.Y * _height) - Vector2(0.5f);
		// �̹��� �ȼ� ������ Index ���� ���� [0, width - 1] x [0, height - 1]�� ��ȯ
		uint32 x = static_cast<uint32>(std::roundf(imageUV.X));
		uint32 y = static_cast<uint32>(std::roundf(imageUV.Y));

		return wrapped ? GetWrapped(x, y) : GetClamp(x, y);
	}

	Color Texture::GetClamp(uint32 x, uint32 y) const
	{
		// Clamp�� ������ ����
		x = std::clamp(x, static_cast<uint32>(0), _width - 1);
		y = std::clamp(y, static_cast<uint32>(0), _height - 1);

		uint32 index = y * _width + x;

		return _buffer[index];
	}

	Color Texture::GetWrapped(uint32 x, uint32 y) const
	{
		// ������ �Ѿ�� Texture�� �ݺ�
		x %= _width;
		y %= _height;

		uint32 index = y * _width + x;
		return _buffer[index];
	}
}