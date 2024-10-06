#include "pch.h"
#include "ScreenPoint.h"

using namespace JE;

ScreenPoint& ScreenPoint::operator=(const ScreenPoint& screenPos)
{
	{
		if (&screenPos == this) // 주소가 같으면
			return *this;

		x = screenPos.x;
		y = screenPos.y;

		return *this;
	}
}

ScreenPoint JE::ScreenPoint::operator+(const ScreenPoint& pos) const
{
	return ScreenPoint(x + pos.x, y + pos.y);
}

ScreenPoint JE::ScreenPoint::operator-(const ScreenPoint& pos) const
{
	return ScreenPoint(x - pos.x, y - pos.y);
}

ScreenPoint& JE::ScreenPoint::operator+=(const ScreenPoint& pos)
{
	*this = *this + pos;
	return *this;
}

ScreenPoint& JE::ScreenPoint::operator-=(const ScreenPoint& pos)
{
	*this = *this - pos;
	return *this;
}

bool JE::ScreenPoint::operator==(const ScreenPoint& pos) const
{
	return x == pos.x && y == pos.y;
}

bool JE::ScreenPoint::operator!=(const ScreenPoint& pos) const
{
	return x != pos.x || y != pos.y;
}

ScreenPoint JE::ScreenPoint::ToScreenCoordinate(const ScreenPoint& screenSize, const Vector2& pos)
{
	// 데카르트 좌표계는 가운데가 중점이고 스크린 좌표계는 좌상단이 중점
	// 스크린 좌표계는 y값이 증가할수록 아래로 내려가기에 데카르트 좌표계 y를 -로 변환

	// [-halfWidth, halfWidth] x [-halfHeight, halfHeight] => [0, width] x [0, height], y를 -로 변환(반대방향)
	return ScreenPoint(pos.x + screenSize.x * 0.5f, -pos.y + screenSize.y * 0.5f);
}

Vector2 JE::ScreenPoint::ToCartesianCoordinate(const ScreenPoint& screenSize)
{
	// 픽셀이 가로, 세로로 [0, 1] 범위 영역을 가진다.
	// 화면 해상도 크기가 짝수인 경우 어쩔 수 없이 데카르트 좌표계의 원점(0, 0)을 
	// 표현하기 위해 중앙을 공유하는 네 픽셀 중 하나를 선택해야 하는데 
	// 이 엔진은 오른쪽 하단 픽셀 선택 (+0.5, -0.5)

	// 스크린 좌표 영역을 대표하는 값으로 해당 영역의 중점을 사용 
	// ex) 중앙 픽셀의 데카르트 좌표 (0.5, -0.5), 오른쪽 하단 픽셀
	// 스크린 좌표계는 픽셀을 의미하므로 0.5를 더해 픽셀 정중앙을 의미하게 한 뒤 좌표계를 변환
	return Vector2(x + 0.5f - screenSize.x * 0.5f, -(y + 0.5f) + screenSize.y * 0.5f);
}