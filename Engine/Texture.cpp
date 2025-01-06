#include "pch.h"
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace JE {
	bool Texture::Load(const std::wstring& path)
	{
		//fs::path filePath = fs::current_path().parent_path() / "Resources" / filename;
		fs::path filePath = path;
		if (!fs::exists(filePath)) // 파일 존재 여부 확인
			return false;

		// 이미지 데이터
		int width = 0, height = 0, channel = 0;

		// stbi_load를 위해 wstring -> string으로 변환
		std::string pathStr = filePath.string();
		unsigned char* img = stbi_load(pathStr.c_str(), &width, &height, &channel, STBI_rgb_alpha); // uint8과 동일
		if (img == nullptr)
			return false;
		
		_width = static_cast<uint32>(width);
		_height = static_cast<uint32>(height);
		// image buffer 초기화
		_buffer.clear();
		// Color를 저장할 공간 예약 (만약 rgba로 각각 저장하면 r * b * g * a처럼 채널도 곱해줘야함)
		_buffer.reserve(_width * _height);

		// 이미지 픽셀 색 저장
		for (uint32 y = 0; y < _height; ++y) {
			for (uint32 x = 0; x < _width; ++x) {
				// 이미지는 [0, 255] 범위를 가진 rgba를 각각 따로 저장하기에 channel만큼 곱해주기
				uint32 idx = (y * _width + x) * _channel;
				Color32 color(img[idx], img[idx + 1], img[idx + 2], img[idx + 3]);
				_buffer.emplace_back(Color(color)); // 메모리를 미리 확보했으므로 새로 할당받지 않다 좀 더 빠름
			}
		}

		return true;
	}
	Color Texture::SamplePoint(const Vector2& uv, bool wrapped) const
	{
		/*
			Texture의 좌표를 이미지의 크기에 맞춘 이미지 좌표로 변환해 사용

			UV 좌표계의 범위는 [0, 1]이고 이미지 좌푠느 픽셀의 가운데에 색이 저장되어 있다고 생각하기에
			0.5씩 확장해서 좌표계 변환, 즉 이미지 좌표는 [-0.5, 이미지 크기 - 1 + 0.5]가 됨
		*/ 
		Vector2 imageUV = Vector2(uv.X * _width, uv.Y * _height) - Vector2(0.5f);
		// 이미지 픽셀 버퍼의 Index 정수 범위 [0, width - 1] x [0, height - 1]로 변환
		uint32 x = static_cast<uint32>(std::roundf(imageUV.X));
		uint32 y = static_cast<uint32>(std::roundf(imageUV.Y));

		return wrapped ? GetWrapped(x, y) : GetClamp(x, y);
	}

	Color Texture::GetClamp(uint32 x, uint32 y) const
	{
		// Clamp는 범위를 고정
		x = std::clamp(x, static_cast<uint32>(0), _width - 1);
		y = std::clamp(y, static_cast<uint32>(0), _height - 1);

		uint32 index = y * _width + x;

		return _buffer[index];
	}

	Color Texture::GetWrapped(uint32 x, uint32 y) const
	{
		// 범위를 넘어가면 Texture가 반복
		x %= _width;
		y %= _height;

		uint32 index = y * _width + x;
		return _buffer[index];
	}
}