#include "pch.h"
#include "BitmapBuffer.h"

using namespace JE;

bool BitmapBuffer::Init(const ScreenPoint& screenSize)
{
    // ���� �������� DC ��ü ��������
    _screenDC = ::GetDC(_hwnd); 
    if (_screenDC == NULL)
        return false;

    // �޸� DC ���� (ȭ��� ������ Ư���� ���� �޸�, backBuffer)
    _memoryDC = ::CreateCompatibleDC(_screenDC);
    if (_memoryDC == NULL)
        return false;

    _screenSize = screenSize; // ȭ�� ũ�⸸ŭ Bitmap �̹��� ����

    // Bitmap Setting
    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    // bitmap ũ��
    bmi.bmiHeader.biWidth = _screenSize.X;
    // bitmap�� �̹����� �Ųٷ� ����Ǳ⿡ ���̸� ������ ������ ���� 
    // (���� bottom-up�� default�̱⿡ ������ ������ top-down���� ��ȯ)
    bmi.bmiHeader.biHeight = -_screenSize.Y; 
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // pixel �� bit �� (32bits BGRA)
    bmi.bmiHeader.biCompression = BI_RGB; // ������� ����

    // �޸𸮿� ���� ���� ������ Bitmap ����
    // _screenBuffer �����͸� ���� �ȼ� �����Ϳ� ���� ���� ���� (32bit BGRA �������� �� �ȼ� ���� ����)
    // _screenBuffer�� 1���� �迭ó�� ����ؾߵ�
    _memoryBitmap = ::CreateDIBSection(_memoryDC, &bmi, DIB_RGB_COLORS, (void**)&_screenBuffer, NULL, 0);
    if (_memoryBitmap == NULL)
        return false;

    // memoryDC�� memoryBitmap�� ����Ű���� ���� (screenDC�� ������ ȭ�鿡 ǥ�õǴ� �������� Bitmap�� ����Ű�� ��)
    _bitmap = (HBITMAP)::SelectObject(_memoryDC, _memoryBitmap); // ���ҽ� ���� �� �޸� ���� ������ ���� memoryDC�� ������ ����Ű�� Bitmap ���� (���� ����)
    if (_bitmap == NULL) 
        return false;

    // ���� ���� ����
    CreateDepthBuffer();

    return true;
}

void BitmapBuffer::SwapBuffer()
{
    DrawStatisticTexts();
    ::BitBlt(_screenDC, 0, 0, _screenSize.X, _screenSize.Y, _memoryDC, 0, 0, SRCCOPY); // backBuffer�� frontBuffer�� ����

    _statisticTexts.clear();
}

void BitmapBuffer::Clear(const Color32& bgColor)
{
    // Pixel ���� �ʱ�ȭ
    // memcpy(_screenBuffer, &bgColor, _screenSize.X, _screenSize.y);
    Color32* dest = _screenBuffer; // screenBuffer �����͸� ���� bitmap�� ����
    for (int i = 0; i < _screenSize.Y; ++i)
        for (int j = 0; j < _screenSize.X; ++j)
            *(dest + i * _screenSize.X + j) = bgColor;

    // ���� ���� �ʱ�ȭ
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
    // depthBuffer�� ���� �׽�Ʈ�� ���� ����� �Ÿ��� ������Ʈ�ϴ� �ִ��� �� �Ÿ��� �ʱ�ȭ
    if (!_depthBuffer.empty()) {
        static float maxDepth = INFINITY;
        std::fill(_depthBuffer.begin(), _depthBuffer.end(), maxDepth);
    }
}

Color BitmapBuffer::GetPixel(const ScreenPoint& pos)
{
    if (!IsInScreen(pos))
        return Color(1.f, 0.f, 1.f, 1.f); // ���� ������� �ʴ� Magenta Color ��ȯ

    Color32* dest = _screenBuffer;
    Color32 bufferColor = *(dest + GetScreenBufferIndex(pos)); // ���� pixel ��ġ�� ��
    return Color(bufferColor); // RGBA[0.0, 1.0]���� ��ȯ�� ��ȯ
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
    hFont = (HFONT)::GetStockObject(ANSI_VAR_FONT); // Window�� ���ǵǾ� �ִ� Font ���
    if (fOldFont = (HFONT)::SelectObject(_memoryDC, hFont)) {
        static const int leftPos = 600;
        static const int topPos = 10;
        static const int lineSpace = 20; // ���Ʒ� �� ����
        int currentPos = topPos;
        for (const std::wstring& text : _statisticTexts) {
            TextOut(_memoryDC, leftPos, currentPos, text.c_str(), static_cast<int>(text.length())); // Text ���
            currentPos += lineSpace; 
        };

        ::SelectObject(_memoryDC, fOldFont); // ���� font�� �ǵ����ֱ�
    }
}

bool BitmapBuffer::IsInScreen(const ScreenPoint& pos)
{
    // Bitmap ĵ���� ���� ������ Ȯ��
    if (pos.X < 0 || pos.X >= _screenSize.X || pos.Y < 0 || pos.Y >= _screenSize.Y)
        return false;
    return true;
}

int BitmapBuffer::GetScreenBufferIndex(const ScreenPoint& pos) const
{
    // 1���� �迭�� 2���� �迭ó�� ����� ���� index ����
    return pos.Y * _screenSize.X + pos.X;
}
