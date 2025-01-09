#pragma once
#include "BitmapBuffer.h"
#include "RendererInterface.h"

namespace JE {

// Bitmap을 사용한 Rendering
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
	// 점 그리기
	virtual void DrawPoint(const Vector2& pos, const Color& color) override;
	virtual void DrawPoint(const ScreenPoint& pos, const Color& color) override;

	// 선 그리기 알고리듬
	virtual void DrawLine(const Vector2& startPos, const Vector2& endPos, const Color& color) override;

	virtual void DrawFullVerticalLine(const int x, const Color& color) override;
	virtual void DrawFullHorizontalLine(const int y, const Color& color) override;

	// std::wstring 설정
	virtual void PushStatisticText(std::wstring&& text) override;
	virtual void PushStatisticTexts(std::vector<std::wstring>&& texts) override;

	// Render 모드 설정 : Normal, Wireframe etc
	virtual void SetDrawMode(DrawMode mode) { _mode = mode; }
	virtual bool IsWireframe() const { return _mode == DrawMode::Wireframe; }

private:
	// 주어진 점의 위치가 화면과 화면 밖 영역, 9개의 영역 중에 어디에 속해 있는지
	int TestRegion(const Vector2& pos, const Vector2& minPos, const Vector2& maxPos);
	// 화면 영역에 유효한 선분으로 잘라주는 알고리듬
	bool CohenSutherlandLineClip(Vector2& startPos, Vector2& endPos, const Vector2& minPos, const Vector2& maxPos);
	// Brensenham's Algorihtm을 사용한 선 그리기
	void BrensenhamDrawLine(Vector2& clippedStart, Vector2& clippedEnd, const Color& color);

protected:
	DrawMode _mode = DrawMode::Normal;
};

};
