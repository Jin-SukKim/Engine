#pragma once
#include "BitmapBuffer.h"
#include "RendererInterface.h"

namespace JE {

enum class DrawMode {
	Normal = 0,
	Wireframe,
	DepthBuffer
};

// Bitmap�� ����� Rendering
class CppRenderer : public BitmapBuffer, RendererInterface
{
	using Super = BitmapBuffer;
public:
	virtual bool Init(const ScreenPoint& screenSize) override;
	virtual void Clear(const Color& bgColor) override;

	virtual void Render() override;

	// �� �׸���
	virtual void DrawPoint(const Vector2& pos, const Color& color) override;
	virtual void DrawPoint(const ScreenPoint& pos, const Color& color) override;

	// �� �׸��� �˰�����
	virtual void DrawLine(const Vector2& startPos, const Vector2& endPos, const Color& color) override;
	//virtual void DrawLine(const Vector3& startPos, const Vector3& endPos, const Color& color) override;
	//virtual void DrawLine(const Vector4& startPos, const Vector4& endPos, const Color& color) override;

	virtual void DrawFullVerticalLine(const int x, const Color& color) override;
	virtual void DrawFullHorizontalLine(const int y, const Color& color) override;

	// std::wstring ����
	virtual void PushStatisticText(std::wstring&& text) override;
	virtual void PushStatisticTexts(std::vector<std::wstring>&& texts) override;

	// ���� ����
	virtual void SetDepthBufferValue(const ScreenPoint& pos, float depth) override;
	virtual float GetDepthBufferValue(const ScreenPoint& pos) const override;

private:
	// �־��� ���� ��ġ�� ȭ��� ȭ�� �� ����, 9���� ���� �߿� ��� ���� �ִ���
	int TestRegion(const Vector2& pos, const Vector2& minPos, const Vector2& maxPos);
	// ȭ�� ������ ��ȿ�� �������� �߶��ִ� �˰�����
	bool CohenSutherlandLineClip(Vector2& startPos, Vector2& endPos, const Vector2& minPos, const Vector2& maxPos);
	// Brensenham's Algorihtm�� ����� �� �׸���
	void BrensenhamDrawLine(Vector2& clippedStart, Vector2& clippedEnd, const Color& color);

protected:
	DrawMode _mode = DrawMode::Normal;
};

};