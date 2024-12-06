#pragma once

namespace JE {

	class BitmapBuffer
	{
	public:

	private:
		HWND _hwnd = 0; // ������â �ڵ�
		HDC _screenDC = 0, _memoryDC = 0; // front_buffer, back_buffer
		HBITMAP _bitmap = 0, _screenBitMap = 0; // ĵ����

		ScreenPoint _screenSize;
		Color32* _screenBuffer = nullptr;
		std::vector<float> _depthBuffer;

		std::vector<std::wstring> _statisticTexts;
	};

}