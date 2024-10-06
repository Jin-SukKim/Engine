#pragma once

namespace JE {

// 화면 좌표계 / 화면 크기
struct ScreenPoint
{
	ScreenPoint() {}
	ScreenPoint(int x, int y) : x(static_cast<uint16>(x)), y(static_cast<uint16>(y)) {}
	ScreenPoint(uint16 x, uint16 y) : x(x), y(y) {}
	ScreenPoint(float x, float y) : x(MathUtils::FloorToInt<uint16>(x)), y(MathUtils::FloorToInt<uint16>(y)) {}
	ScreenPoint(const Vector2& pos) : ScreenPoint(pos.x, pos.y) {}

	ScreenPoint& operator=(const ScreenPoint& screenPos);
	ScreenPoint operator+(const ScreenPoint& pos) const;
	ScreenPoint operator-(const ScreenPoint& pos) const;
	ScreenPoint& operator+=(const ScreenPoint& pos);
	ScreenPoint& operator-=(const ScreenPoint& pos);
	bool operator==(const ScreenPoint& pos) const;
	bool operator!=(const ScreenPoint& pos) const;

	// ScreenSize와 데카르트 좌표계의 좌표값을 받아 화면 좌표계로 변환
	static ScreenPoint ToScreenCoordinate(const ScreenPoint& screenSize, const Vector2& pos);
	// ScreenSize를 받아 화면 좌표계에서 데카르트 좌표계로 변환
	Vector2 ToCartesianCoordinate(const ScreenPoint& screenSize);


	const float GetAspectRatio() const {
		return static_cast<float>(x) / static_cast<float>(y);
	}

	// 화면 좌표계 [0, width - 1] x [0, height 0 1]
	uint16 x = 0; // width or ScreenPos.x
	uint16 y = 0; // height or ScreenPos.y
};

}