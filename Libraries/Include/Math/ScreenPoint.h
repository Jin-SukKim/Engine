#pragma once

namespace JE {

// 화면 좌표계 / 화면 크기
struct ScreenPoint
{
	ScreenPoint() {}
	ScreenPoint(int x, int y) : X(x), Y(y) {}
	ScreenPoint(float x, float y) : X(MathUtils::FloorToInt<int>(x)), Y(MathUtils::FloorToInt<int>(y)) {}
	ScreenPoint(const Vector2& pos) : ScreenPoint(pos.X, pos.Y) {}

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
		return static_cast<float>(X) / static_cast<float>(Y);
	}

	// 화면 좌표계 [0, width - 1] x [0, height 0 1]
	int X = 0; // width or ScreenPos.x
	int Y = 0; // height or ScreenPos.y
};

}