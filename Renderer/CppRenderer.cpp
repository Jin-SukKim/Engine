#include "pch.h"
#include "CppRenderer.h"
using namespace JE;

bool CppRenderer::Init(const ScreenPoint& screenSize)
{
    return Super::Init(screenSize);
}

void CppRenderer::Clear(const Color& bgColor)
{
    Super::Clear(bgColor.ToColor32());
}

void CppRenderer::Render()
{
    SwapBuffer();
}

void CppRenderer::DrawPoint(const Vector2& pos, const Color& color)
{
    SetPixel(ScreenPoint::ToScreenCoordinate(_screenSize, pos), color);
}

void CppRenderer::DrawPoint(const ScreenPoint& pos, const Color& color)
{
    SetPixel(pos, color);
}

void CppRenderer::DrawLine(const Vector2& startPos, const Vector2& endPos, const Color& color)
{
    // CohenSutherland Line Clipping Algorithm�� ����� ȭ���� ����� ������ �߶� ����ȭ
    Vector2 clippedStart = startPos;
    Vector2 clippedEnd = endPos;
    // pos(0, 0)�� �߽����� �ϴ� ȭ���� ũ��
    Vector2 screenExtend = Vector2(_screenSize.X, _screenSize.Y) * 0.5f;
    Vector2 minScreen = -screenExtend;
    Vector2 maxScreen = screenExtend;

    if (!CohenSutherlandLineClip(clippedStart, clippedEnd, minScreen, maxScreen))
        return;

    // Brensenham's Algorithm�� ����� �� �׸���
    BrensenhamDrawLine(clippedStart, clippedEnd, color);
}


void CppRenderer::DrawFullVerticalLine(const int x, const Color& color)
{
    if (x < 0 || x >= _screenSize.X)
        return;

    for (int y = 0; y < _screenSize.Y; ++y)
        SetPixel({ x, y }, color);
}

void CppRenderer::DrawFullHorizontalLine(const int y, const Color& color)
{
    if (y < 0 || y >= _screenSize.Y)
        return;

    for (int x = 0; x < _screenSize.X; ++x)
        SetPixel({ x, y }, color);
}

void CppRenderer::PushStatisticText(std::wstring&& text)
{
    _statisticTexts.emplace_back(text);
}

void CppRenderer::PushStatisticTexts(std::vector<std::wstring>&& texts)
{
    // ���Ե� string�� �� ������ �� �� �����Ƿ� ������ �Ʒ��� ȭ�鿡 ����ϱ� ���� �Ųٷ� ����
    std::move(texts.begin(), texts.end(), std::back_inserter(_statisticTexts));
}

/*
    �־��� ���� ��� ������ ��ġ�� �ִ��� �ľ�

    (ȭ��� �ٱ� ������ �ο��� ������ ��, 0000 = ȭ��)
    1001 1000 1010
    0001 0000 0010
    0101 0100 0110
*/
int CppRenderer::TestRegion(const Vector2& pos, const Vector2& minPos, const Vector2& maxPos)
{
    int result = 0;
    if (pos.X < minPos.X) // ȭ���� ���� ���� ���̸�
        result |= 0b0001;
    else if (pos.X > maxPos.X) // ������ ���� ���̸�
        result |= 0b0010;
    
    if (pos.Y < minPos.Y) // �Ʒ� ���� ��
        result |= 0b0100;
    else if (pos.Y > maxPos.Y) // �� ���� ��
        result |= 0b1000;
    return result;
}

bool CppRenderer::CohenSutherlandLineClip(Vector2& startPos, Vector2& endPos, const Vector2& minPos, const Vector2& maxPos)
{
    /*
        ������ �׸� �� ����ؾ� �� ��Ȳ
            1) ȭ�� �ȿ� ��ġ�ؼ� �ڸ� �ʿ䰡 ���� ���
                - �� ���� ���� ��� 0 �� ���
            2) ȭ�� �ۿ� ��ġ�� �־ �׸� �ʿ䰡 ���� ���
                - �� ������ ���� & (AND) �����ϸ� �׻� 0���� ū ���� ���´�.
            3) ȭ���� ���������� ȭ�鿡 ��ȿ�� �������� �߶󳻾� �ϴ� ���
                - �� ������ ���� & (AND) �����ϸ� �׻� 0�� ���´�.
    */

    // �� ���� ����
    int startRegion = TestRegion(startPos, minPos, maxPos);
    int endRegion = TestRegion(endPos, minPos, maxPos);

    float width = endPos.X - startPos.X;
    float height = endPos.Y - startPos.Y;

    while (true) {
        if (startRegion == 0 && endRegion == 0) // �� �� ��� ȭ�� ������ �� (��Ȳ 1)
            return true;
        if (startRegion & endRegion) // �� �� ��� ȭ�� ���� �����̸� (��Ȳ 2)
            return false;

        // ��Ȳ 3 - �� ���� Ŭ������ ���� �ٽ� ���� �׽�Ʈ ���� �� �˻�
        Vector2 clippedPos;
        // ���������� Ŭ������ �� ���� Ŭ����
        int currentRegion = startRegion ? startRegion : endRegion;

        // ����/������ ������ �ִ� ���
        if (currentRegion < 0b0100) {
            if (currentRegion & 0b0001) // ����
                clippedPos.X = minPos.X;
            else // ������
                clippedPos.X = maxPos.X;

            if (MathUtils::EqualsInTolerance(height, 0.f)) // ������ (����)
                clippedPos.Y = startPos.Y;
            else
                // ������ ���������� ���� ���� ������ ���ϱ� y = Ya + m(x - Xa), m = ����
                clippedPos.Y = startPos.Y + height * (clippedPos.X - startPos.X) / width;
        }
        // ��/�Ʒ� ������ �ִ� ���
        else {
            if (currentRegion & 0b0100) // �Ʒ�
                clippedPos.Y = minPos.Y;
            else // ��
                clippedPos.Y = maxPos.Y;

            if (MathUtils::EqualsInTolerance(width, 0.f)) // ������ (����)
                clippedPos.X = endPos.X; // �߰����� ��� ��ŵ
            else
                // ������ ���������� ���� ���� ������ ���ϱ� x = Xa + (y - Ya) / m
                clippedPos.X = endPos.X + width * (clippedPos.Y - endPos.Y) / height;
        }

        // Ŭ������ ����� �ٽ� ���� �׽�Ʈ
        if (startRegion) { // �������� Ŭ�����ߴٸ�
            startPos = clippedPos;
            startRegion = TestRegion(startPos, minPos, maxPos);
        }
        else { // ������ Ŭ�����ߴٸ�
            endPos = clippedPos;
            endRegion = TestRegion(endPos, minPos, maxPos);
        }
    }

    return true;
}

void CppRenderer::BrensenhamDrawLine(Vector2& clippedStart, Vector2& clippedEnd, const Color& color)
{
    // ȭ�� ��ǥ��� ��ȯ
    ScreenPoint lineStart = ScreenPoint::ToScreenCoordinate(_screenSize, clippedStart);
    ScreenPoint lineEnd = ScreenPoint::ToScreenCoordinate(_screenSize, clippedEnd);

    int width = lineEnd.X - lineStart.X;
    int height = lineEnd.Y - lineStart.Y;

    // �ϸ��� ������� �ް��� ������� �ľ� (�ϸ��ϸ� �¿�(x��), �ް��ϸ� ����(y��))
    bool isGradualSlope = std::abs(width) >= std::abs(height);

    // �Ⱥи��� ����
    // ex) width�� 0���� ũ�� ���������� ���� �׸��� ��, ������ �������� ���� �׸��� ��
    int dx = width >= 0 ? 1 : -1; // x ����
    int dy = height >= 0 ? 1 : -1; // y ����
    int w = dx * width;
    int h = dy * height;

    // �ϸ��� ����̸� �����̵��� ����ϰ� �ƴ϶�� �����̵��� ���
    int f = isGradualSlope ? 2 * h - w : 2 * w - h; // �ȼ� �̵� �Ǻ��� (���� �Ǻ���)
    // ���� �ȼ� �Ǻ����� ���� ���� ��
    int nf = isGradualSlope ? 2 * h : 2 * w; // ����/���� �̵�
    int df = isGradualSlope ? 2 * (h - w) : 2 * (w - h); // �밢 �̵�

    int x = lineStart.X;
    int y = lineStart.Y;

    // �ϸ��� ���
    if (isGradualSlope) {
        // ������ ������ ������ �ݺ�
        while (x != lineEnd.X) { 
            SetPixel({ x, y }, color);

            if (f < 0) // �������� �̵�
                f += nf; // ���� �ȼ� �Ǻ��� ���� �Ǻ���
            else { // �� ĭ y�� �̵�
                f += df;
                y += dy; // �Ⱥи鿡 ���� dy �������� �̵�
            }

            x += dx; // �Ⱥи鿡 ���� ������ �޶����µ� dx �������� ���
        }
    }
    // �ް��� ���
    else {
        while (y != lineEnd.Y) {
            SetPixel({ x, y }, color);

            if (f < 0) // �������� �̵�
                f += nf; // ���� �ȼ� �Ǻ���
            else { // ��ĭ x�� �̵�
                f += df;
                x += dx;
            }

            y += dy;
        }
    }
}
