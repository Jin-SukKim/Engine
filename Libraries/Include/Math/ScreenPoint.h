#pragma once

namespace JE {

// ȭ�� ��ǥ�� / ȭ�� ũ��
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

	// ScreenSize�� ��ī��Ʈ ��ǥ���� ��ǥ���� �޾� ȭ�� ��ǥ��� ��ȯ
	static ScreenPoint ToScreenCoordinate(const ScreenPoint& screenSize, const Vector2& pos);
	// ScreenSize�� �޾� ȭ�� ��ǥ�迡�� ��ī��Ʈ ��ǥ��� ��ȯ
	Vector2 ToCartesianCoordinate(const ScreenPoint& screenSize);


	const float GetAspectRatio() const {
		return static_cast<float>(X) / static_cast<float>(Y);
	}

	// ȭ�� ��ǥ�� [0, width - 1] x [0, height 0 1]
	int X = 0; // width or ScreenPos.x
	int Y = 0; // height or ScreenPos.y
};

}