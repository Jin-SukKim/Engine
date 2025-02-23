#pragma once

namespace JE {

struct Color32;
struct Color;

class BitmapBuffer
{
public:
	BitmapBuffer(HWND hwnd) : _hwnd(hwnd) {}
	virtual ~BitmapBuffer() {}

	// bitmap �̹����� ȭ������ ����ϱ� ���� ȭ���� ũ�⸸ŭ ũ�� ����
	bool Init(const ScreenPoint& screenSize);
	// double-buffering
	void SwapBuffer();
	void Clear(const Color32& bgColor);
	void Release();

	ScreenPoint GetScreenSize() const { return _screenSize; };
	// Bitmap�� ���� ���� ��������
	Color32* GetScreenBuffer() const;

	void CreateDepthBuffer();
	void ClearDepthBuffer();
	void SetDepthBufferValue(const ScreenPoint& pos, float depth);
	float GetDepthBufferValue(const ScreenPoint& pos) const;

	Color GetPixel(const ScreenPoint& pos);
	void SetPixel(const ScreenPoint& pos, const Color& color);

	void DrawStatisticTexts();

private:
	// �׷����� ȭ�� �ȿ� �ִ���
	bool IsInScreen(const ScreenPoint& pos) const;
	// ȭ�� ũ��� ���� ũ�⸦ ���� ������ index ��
	int GetScreenBufferIndex(const ScreenPoint& pos) const;

protected:
	ScreenPoint _screenSize; // ȭ���� ũ��
	std::vector<std::wstring> _statisticTexts; // ȭ�鿡 ����� texts

private:
	HWND _hwnd = 0; // ������â �ڵ�
	HDC _screenDC = 0, _memoryDC = 0; // front_buffer, back_buffer
	HBITMAP _bitmap = 0, _memoryBitmap = 0; // ĵ����
	
	Color32* _screenBuffer = nullptr; // bitmap�� ������ ������ (bitmap�� 32bit�� �̷���� BGRA ������ ���� ���, 1���� �迭�� 2���� �迭ó�� ����� ��ó�� ���)
	std::vector<float> _depthBuffer; // ���� �׽�Ʈ�� ���� ���� ���� (�ȼ��� ��)

};

}