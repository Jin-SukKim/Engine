#include "pch.h"
#include "BitmapBuffer.h"

using namespace JE;

bool BitmapBuffer::Init(const ScreenPoint& screenSize)
{
    // 현재 윈도우의 DC 객체 가져오기
    _screenDC = ::GetDC(_hwnd); 
    if (_screenDC == NULL)
        return false;

    // 메모리 DC 생성 (화면과 동일한 특성을 가진 메모리, backBuffer)
    _memoryDC = ::CreateCompatibleDC(_screenDC);
    if (_memoryDC == NULL)
        return false;

    _screenSize = screenSize; // 화면 크기만큼 Bitmap 이미지 생성

    // Bitmap Setting
    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    // bitmap 크기
    bmi.bmiHeader.biWidth = _screenSize.X;
    // bitmap에 이미지가 거꾸로 저장되기에 높이를 음수로 지정해 반전 
    // (원래 bottom-up이 default이기에 음수로 반전해 top-down으로 변환)
    bmi.bmiHeader.biHeight = -_screenSize.Y; 
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // pixel 당 bit 수 (32bits BGRA)
    bmi.bmiHeader.biCompression = BI_RGB; // 압축없는 포맷

    // 메모리에 직접 접근 가능한 Bitmap 생성
    // _screenBuffer 포인터를 통해 픽셀 데이터에 직접 접근 가능 (32bit BGRA 포맷으로 각 픽셀 조작 가능)
    // _screenBuffer는 1차원 배열처럼 사용해야됨
    _memoryBitmap = ::CreateDIBSection(_memoryDC, &bmi, DIB_RGB_COLORS, (void**)&_screenBuffer, NULL, 0);
    if (_memoryBitmap == NULL)
        return false;

    // memoryDC를 memoryBitmap을 가르키도록 변경 (screenDC는 여전히 화면에 표시되는 윈도우의 Bitmap을 가르키는 중)
    _bitmap = (HBITMAP)::SelectObject(_memoryDC, _memoryBitmap); // 리소스 관리 및 메모리 누수 방지를 위해 memoryDC가 기존에 가르키면 Bitmap 저장 (추후 삭제)
    if (_bitmap == NULL) 
        return false;

    // 깊이 버퍼 생성
    CreateDepthBuffer();

    return true;
}

void BitmapBuffer::SwapBuffer()
{
    DrawStatisticTexts();
    ::BitBlt(_screenDC, 0, 0, _screenSize.X, _screenSize.Y, _memoryDC, 0, 0, SRCCOPY); // backBuffer를 frontBuffer로 복사

    _statisticTexts.clear();
}

void BitmapBuffer::Clear(const Color32& bgColor)
{
    // Pixel 버퍼 초기화
    // memcpy(_screenBuffer, &bgColor, _screenSize.X, _screenSize.y);
    Color32* dest = _screenBuffer; // screenBuffer 포인터를 통해 bitmap에 접근
    for (int i = 0; i < _screenSize.Y; ++i)
        for (int j = 0; j < _screenSize.X; ++j)
            *(dest + i * _screenSize.X + j) = bgColor;

    // 깊이 버퍼 초기화
    ClearDepthBuffer();
}

void BitmapBuffer::Release()
{
    DeleteObject(_bitmap);
    DeleteObject(_memoryBitmap);
    ReleaseDC(_hwnd, _screenDC);
    ReleaseDC(_hwnd, _memoryDC);
}

Color32* BitmapBuffer::GetScreenBuffer() const
{
    return _screenBuffer;
}

void BitmapBuffer::CreateDepthBuffer()
{
    _depthBuffer = std::vector<float>(_screenSize.X * _screenSize.Y);
}

void BitmapBuffer::ClearDepthBuffer()
{
    // depthBuffer는 깊이 테스트를 위해 가까운 거리로 업데이트하니 최대한 먼 거리로 초기화
    if (!_depthBuffer.empty()) {
        static float maxDepth = INFINITY;
        std::fill(_depthBuffer.begin(), _depthBuffer.end(), maxDepth);
    }
}

Color BitmapBuffer::GetPixel(const ScreenPoint& pos)
{
    if (!IsInScreen(pos))
        return Color(1.f, 0.f, 1.f, 1.f); // 거의 사용하지 않는 Magenta Color 반환

    Color32* dest = _screenBuffer;
    Color32 bufferColor = *(dest + GetScreenBufferIndex(pos)); // 현재 pixel 위치의 값
    return Color(bufferColor); // RGBA[0.0, 1.0]으로 변환해 반환
}

void BitmapBuffer::SetPixel(const ScreenPoint& pos, const Color& color)
{
    if (!IsInScreen(pos))
        return; 

    Color32* dest = _screenBuffer;
    *(dest + GetScreenBufferIndex(pos)) = color.ToColor32();
}

void BitmapBuffer::DrawStatisticTexts()
{
    if (_statisticTexts.size() == 0)
        return;

    HFONT hFont, fOldFont;
    hFont = (HFONT)::GetStockObject(ANSI_VAR_FONT); // Window에 정의되어 있던 Font 사용
    if (fOldFont = (HFONT)::SelectObject(_memoryDC, hFont)) {
        static const int leftPos = 600;
        static const int topPos = 10;
        static const int lineSpace = 20; // 위아래 줄 간격
        int currentPos = topPos;
        for (const std::wstring& text : _statisticTexts) {
            TextOut(_memoryDC, leftPos, currentPos, text.c_str(), static_cast<int>(text.length())); // Text 출력
            currentPos += lineSpace; 
        };

        ::SelectObject(_memoryDC, fOldFont); // 원래 font로 되돌려주기
    }
}

bool BitmapBuffer::IsInScreen(const ScreenPoint& pos)
{
    // Bitmap 캔버스 범위 밖인지 확인
    if (pos.X < 0 || pos.X >= _screenSize.X || pos.Y < 0 || pos.Y >= _screenSize.Y)
        return false;
    return true;
}

int BitmapBuffer::GetScreenBufferIndex(const ScreenPoint& pos) const
{
    // 1차원 배열을 2차원 배열처럼 사용할 때의 index 사용법
    return pos.Y * _screenSize.X + pos.X;
}
