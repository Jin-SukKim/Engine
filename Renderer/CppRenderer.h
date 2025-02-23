#pragma once
#include "BitmapBuffer.h"
#include "RendererInterface.h"

namespace JE {

// Bitmap�� ����� Rendering
class CppRenderer : public BitmapBuffer, public IRenderer
{
	using Super = BitmapBuffer;
public:
	CppRenderer(HWND hwnd) : Super(hwnd) {}
	virtual ~CppRenderer() override {}

	virtual bool Init(const ScreenPoint& screenSize) override;
	virtual void Clear(const Color& bgColor) override;

	virtual void Render() override;

	virtual ScreenPoint GetScreenSize() const override { return Super::GetScreenSize(); }
	// �� �׸���
	virtual void DrawPoint(const Vector2& pos, const Color& color) override;
	virtual void DrawPoint(const ScreenPoint& pos, const Color& color) override;

	// �� �׸��� �˰���
	virtual void DrawLine(const Vector2& startPos, const Vector2& endPos, const Color& color) override;

	virtual void DrawFullVerticalLine(const int x, const Color& color) override;
	virtual void DrawFullHorizontalLine(const int y, const Color& color) override;

	// std::wstring ����
	virtual void PushStatisticText(std::wstring&& text) override;
	virtual void PushStatisticTexts(std::vector<std::wstring>&& texts) override;

	// Render ��� ���� : Normal, Wireframe etc
	virtual void SetDrawMode(DrawMode mode) { _mode = mode; }
	virtual bool IsWireframe() const { return _mode == DrawMode::Wireframe; }

private:
	// �־��� ���� ��ġ�� ȭ��� ȭ�� �� ����, 9���� ���� �߿� ��� ���� �ִ���
	int TestRegion(const Vector2& pos, const Vector2& minPos, const Vector2& maxPos);
	// ȭ�� ������ ��ȿ�� �������� �߶��ִ� �˰���
	bool CohenSutherlandLineClip(Vector2& startPos, Vector2& endPos, const Vector2& minPos, const Vector2& maxPos);
	// Brensenham's Algorihtm�� ����� �� �׸���
	void BrensenhamDrawLine(Vector2& clippedStart, Vector2& clippedEnd, const Color& color);

protected:
	DrawMode _mode = DrawMode::Normal;
};

};
