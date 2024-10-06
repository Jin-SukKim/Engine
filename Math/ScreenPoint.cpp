#include "pch.h"
#include "ScreenPoint.h"

using namespace JE;

ScreenPoint& ScreenPoint::operator=(const ScreenPoint& screenPos)
{
	{
		if (&screenPos == this) // �ּҰ� ������
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
	// ��ī��Ʈ ��ǥ��� ����� �����̰� ��ũ�� ��ǥ��� �»���� ����
	// ��ũ�� ��ǥ��� y���� �����Ҽ��� �Ʒ��� �������⿡ ��ī��Ʈ ��ǥ�� y�� -�� ��ȯ

	// [-halfWidth, halfWidth] x [-halfHeight, halfHeight] => [0, width] x [0, height], y�� -�� ��ȯ(�ݴ����)
	return ScreenPoint(pos.x + screenSize.x * 0.5f, -pos.y + screenSize.y * 0.5f);
}

Vector2 JE::ScreenPoint::ToCartesianCoordinate(const ScreenPoint& screenSize)
{
	// �ȼ��� ����, ���η� [0, 1] ���� ������ ������.
	// ȭ�� �ػ� ũ�Ⱑ ¦���� ��� ��¿ �� ���� ��ī��Ʈ ��ǥ���� ����(0, 0)�� 
	// ǥ���ϱ� ���� �߾��� �����ϴ� �� �ȼ� �� �ϳ��� �����ؾ� �ϴµ� 
	// �� ������ ������ �ϴ� �ȼ� ���� (+0.5, -0.5)

	// ��ũ�� ��ǥ ������ ��ǥ�ϴ� ������ �ش� ������ ������ ��� 
	// ex) �߾� �ȼ��� ��ī��Ʈ ��ǥ (0.5, -0.5), ������ �ϴ� �ȼ�
	// ��ũ�� ��ǥ��� �ȼ��� �ǹ��ϹǷ� 0.5�� ���� �ȼ� ���߾��� �ǹ��ϰ� �� �� ��ǥ�踦 ��ȯ
	return Vector2(x + 0.5f - screenSize.x * 0.5f, -(y + 0.5f) + screenSize.y * 0.5f);
}