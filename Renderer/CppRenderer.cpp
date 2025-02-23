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
    // CohenSutherland Line Clipping Algorithm을 사용해 화면을 벗어나는 선분을 잘라 최적화
    Vector2 clippedStart = startPos;
    Vector2 clippedEnd = endPos;
    // pos(0, 0)을 중심으로 하는 화면의 크기
    Vector2 screenExtend = Vector2(_screenSize.X, _screenSize.Y) * 0.5f;
    Vector2 minScreen = -screenExtend;
    Vector2 maxScreen = screenExtend;

    if (!CohenSutherlandLineClip(clippedStart, clippedEnd, minScreen, maxScreen))
        return;

    // Brensenham's Algorithm을 사용한 선 그리기
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
    // 삽입된 string이 몇 줄인지 알 수 없으므로 위에서 아래로 화면에 출력하기 위해 거꾸로 저장
    std::move(texts.begin(), texts.end(), std::back_inserter(_statisticTexts));
}

/*
    주어진 점이 어느 영역에 위치해 있는지 파악

    (화면과 바깥 영역에 부여된 이진수 값, 0000 = 화면)
    1001 1000 1010
    0001 0000 0010
    0101 0100 0110
*/
int CppRenderer::TestRegion(const Vector2& pos, const Vector2& minPos, const Vector2& maxPos)
{
    int result = 0;
    if (pos.X < minPos.X) // 화면의 왼쪽 영역 밖이면
        result |= 0b0001;
    else if (pos.X > maxPos.X) // 오른쪽 영역 밖이면
        result |= 0b0010;
    
    if (pos.Y < minPos.Y) // 아래 영역 밖
        result |= 0b0100;
    else if (pos.Y > maxPos.Y) // 위 영역 밖
        result |= 0b1000;
    return result;
}

bool CppRenderer::CohenSutherlandLineClip(Vector2& startPos, Vector2& endPos, const Vector2& minPos, const Vector2& maxPos)
{
    /*
        선분을 그릴 때 고려해야 할 상황
            1) 화면 안에 위치해서 자를 필요가 없는 경우
                - 두 영역 값이 모두 0 인 경우
            2) 화면 밖에 위치해 있어서 그릴 필요가 없는 경우
                - 두 영역을 구해 & (AND) 연산하면 항상 0보다 큰 값이 나온다.
            3) 화면을 가로질러서 화면에 유효한 영역으로 잘라내야 하는 경우
                - 두 영역을 구해 & (AND) 연사하면 항상 0이 나온다.
    */

    // 두 점의 영역
    int startRegion = TestRegion(startPos, minPos, maxPos);
    int endRegion = TestRegion(endPos, minPos, maxPos);

    float width = endPos.X - startPos.X;
    float height = endPos.Y - startPos.Y;

    while (true) {
        if (startRegion == 0 && endRegion == 0) // 두 점 모두 화면 영역일 때 (상황 1)
            return true;
        if (startRegion & endRegion) // 두 점 모두 화면 밖의 영역이면 (상황 2)
            return false;

        // 상황 3 - 한 점을 클리핑한 다음 다시 영역 테스트 진행 후 검사
        Vector2 clippedPos;
        // 시작점부터 클리핑한 후 끝점 클리핑
        int currentRegion = startRegion ? startRegion : endRegion;

        // 왼쪽/오른쪽 영역에 있는 경우
        if (currentRegion < 0b0100) {
            if (currentRegion & 0b0001) // 왼쪽
                clippedPos.X = minPos.X;
            else // 오른쪽
                clippedPos.X = maxPos.X;

            if (MathUtils::EqualsInTolerance(height, 0.f)) // 일직선 (수평)
                clippedPos.Y = startPos.Y;
            else
                // 직선의 방정식으로 수직 경계와 교점을 구하기 y = Ya + m(x - Xa), m = 기울기
                clippedPos.Y = startPos.Y + height * (clippedPos.X - startPos.X) / width;
        }
        // 위/아래 영역에 있는 경우
        else {
            if (currentRegion & 0b0100) // 아래
                clippedPos.Y = minPos.Y;
            else // 위
                clippedPos.Y = maxPos.Y;

            if (MathUtils::EqualsInTolerance(width, 0.f)) // 일직선 (수직)
                clippedPos.X = endPos.X; // 추가적인 계산 스킵
            else
                // 직선의 방정식으로 수평 경계와 교점을 구하기 x = Xa + (y - Ya) / m
                clippedPos.X = endPos.X + width * (clippedPos.Y - endPos.Y) / height;
        }

        // 클리핑한 결과로 다시 영역 테스트
        if (startRegion) { // 시작점을 클리핑했다면
            startPos = clippedPos;
            startRegion = TestRegion(startPos, minPos, maxPos);
        }
        else { // 끝점을 클리핑했다면
            endPos = clippedPos;
            endRegion = TestRegion(endPos, minPos, maxPos);
        }
    }

    return true;
}

void CppRenderer::BrensenhamDrawLine(Vector2& clippedStart, Vector2& clippedEnd, const Color& color)
{
    // 화면 좌표계로 변환
    ScreenPoint lineStart = ScreenPoint::ToScreenCoordinate(_screenSize, clippedStart);
    ScreenPoint lineEnd = ScreenPoint::ToScreenCoordinate(_screenSize, clippedEnd);

    int width = lineEnd.X - lineStart.X;
    int height = lineEnd.Y - lineStart.Y;

    // 완만한 경사인지 급격한 경사인지 파악 (완만하면 좌우(x축), 급격하면 상하(y축))
    bool isGradualSlope = std::abs(width) >= std::abs(height);

    // 팔분면의 방향
    // ex) width가 0보다 크면 오른쪽으로 선을 그리는 것, 작으면 왼쪽으로 선을 그리는 것
    int dx = width >= 0 ? 1 : -1; // x 방향
    int dy = height >= 0 ? 1 : -1; // y 방향
    int w = dx * width;
    int h = dy * height;

    // 완만한 경사이면 수평이동과 비슷하고 아니라면 수직이동과 비슷
    int f = isGradualSlope ? 2 * h - w : 2 * w - h; // 픽셀 이동 판별식 (최초 판별식)
    // 다음 픽셀 판별식을 위해 더할 값
    int nf = isGradualSlope ? 2 * h : 2 * w; // 수직/수평 이동
    int df = isGradualSlope ? 2 * (h - w) : 2 * (w - h); // 대각 이동

    int x = lineStart.X;
    int y = lineStart.Y;

    // 완만한 경사
    if (isGradualSlope) {
        // 끝점에 도달할 때까지 반복
        while (x != lineEnd.X) { 
            SetPixel({ x, y }, color);

            if (f < 0) // 수평으로 이동
                f += nf; // 다음 픽셀 판별을 위한 판별식
            else { // 한 칸 y축 이동
                f += df;
                y += dy; // 팔분면에 따라 dy 방향으로 이동
            }

            x += dx; // 팔분면에 따라 방향이 달라지는데 dx 방향으로 디오
        }
    }
    // 급격한 경사
    else {
        while (y != lineEnd.Y) {
            SetPixel({ x, y }, color);

            if (f < 0) // 수직으로 이동
                f += nf; // 다음 픽셀 판별식
            else { // 한칸 x축 이동
                f += df;
                x += dx;
            }

            y += dy;
        }
    }
}
