#pragma once

namespace JE {

enum class DrawMode {
	Normal = 0,
	Wireframe,
	DepthBuffer
};

// ���� Interface�� ����� C++, DirectX, OpenGL�� Ȯ���� �� ���� �� ����.
class RendererInterface {
public:
	virtual ~RendererInterface() = default; // interface class�� ������/�Ҹ��ڴ� = default�� ����

	virtual bool Init(const ScreenPoint& screenSize) = 0;
	virtual void Clear(const Color& bgColor) = 0;
	
	virtual void Render() = 0;

	virtual ScreenPoint GetScreenSize() const = 0;

	// �� �׸���
	virtual void DrawPoint(const Vector2& pos, const Color& color) = 0;
	virtual void DrawPoint(const ScreenPoint& pos, const Color& color) = 0;

	// �� �׸��� �˰���
	virtual void DrawLine(const Vector2& startPos, const Vector2& endPos, const Color& color) = 0;
	//virtual void DrawLine(const Vector3& startPos, const Vector3& endPos, const Color& color) = 0;
	//virtual void DrawLine(const Vector4& startPos, const Vector4& endPos, const Color& color) = 0;

	virtual void DrawFullVerticalLine(const int x, const Color& color) = 0;
	virtual void DrawFullHorizontalLine(const int y, const Color& color) = 0;

	// std::wstring ����
	virtual void PushStatisticText(std::wstring&& text) = 0;
	virtual void PushStatisticTexts(std::vector<std::wstring>&& texts) = 0;

	// ���� ����
	virtual float GetDepthBufferValue(const ScreenPoint& pos) const = 0;
	virtual void SetDepthBufferValue(const ScreenPoint& pos, float depth) = 0;
};

};