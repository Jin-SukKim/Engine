#pragma once

namespace JE {

struct Color32;
struct Color;

class BitmapBuffer
{
public:
	BitmapBuffer(HWND hwnd) : _hwnd(hwnd) {}
	virtual ~BitmapBuffer() {}

	// bitmap 이미지를 화면으로 사용하기 위해 화면의 크기만큼 크기 설정
	bool Init(const ScreenPoint& screenSize);
	// double-buffering
	void SwapBuffer();
	void Clear(const Color32& bgColor);
	void Release();

	ScreenPoint GetScreenSize() const { return _screenSize; };
	// Bitmap의 색상 버퍼 가져오기
	Color32* GetScreenBuffer() const;

	void CreateDepthBuffer();
	void ClearDepthBuffer();
	void SetDepthBufferValue(const ScreenPoint& pos, float depth);
	float GetDepthBufferValue(const ScreenPoint& pos) const;

	Color GetPixel(const ScreenPoint& pos);
	void SetPixel(const ScreenPoint& pos, const Color& color);

	void DrawStatisticTexts();

private:
	// 그려지는 화면 안에 있는지
	bool IsInScreen(const ScreenPoint& pos) const;
	// 화면 크기와 같은 크기를 가진 버퍼의 index 값
	int GetScreenBufferIndex(const ScreenPoint& pos) const;

protected:
	ScreenPoint _screenSize; // 화면의 크기
	std::vector<std::wstring> _statisticTexts; // 화면에 출력할 texts

private:
	HWND _hwnd = 0; // 윈도우창 핸들
	HDC _screenDC = 0, _memoryDC = 0; // front_buffer, back_buffer
	HBITMAP _bitmap = 0, _memoryBitmap = 0; // 캔버스
	
	Color32* _screenBuffer = nullptr; // bitmap의 데이터 포인터 (bitmap은 32bit로 이루어진 BGRA 데이터 포맷 사용, 1차원 배열을 2차원 배열처럼 사용할 때처럼 사용)
	std::vector<float> _depthBuffer; // 깊이 테스트를 위한 깊이 버퍼 (픽셀의 수)

};

}