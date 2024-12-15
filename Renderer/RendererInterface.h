#pragma once

namespace JE {

enum class DrawMode {
	Normal = 0,
	Wireframe,
	DepthBuffer
};

// 같은 Interface를 사용해 C++, DirectX, OpenGL로 확장할 수 있을 것 같다.
class RendererInterface {
public:
	virtual ~RendererInterface() = default; // interface class라도 생성자/소멸자는 = default로 선언

	virtual bool Init(const ScreenPoint& screenSize) = 0;
	virtual void Clear(const Color& bgColor) = 0;
	
	virtual void Render() = 0;

	virtual ScreenPoint GetScreenSize() const = 0;

	// 점 그리기
	virtual void DrawPoint(const Vector2& pos, const Color& color) = 0;
	virtual void DrawPoint(const ScreenPoint& pos, const Color& color) = 0;

	// 선 그리기 알고리듬
	virtual void DrawLine(const Vector2& startPos, const Vector2& endPos, const Color& color) = 0;
	//virtual void DrawLine(const Vector3& startPos, const Vector3& endPos, const Color& color) = 0;
	//virtual void DrawLine(const Vector4& startPos, const Vector4& endPos, const Color& color) = 0;

	virtual void DrawFullVerticalLine(const int x, const Color& color) = 0;
	virtual void DrawFullHorizontalLine(const int y, const Color& color) = 0;

	// std::wstring 설정
	virtual void PushStatisticText(std::wstring&& text) = 0;
	virtual void PushStatisticTexts(std::vector<std::wstring>&& texts) = 0;

	// 깊이 버퍼
	virtual float GetDepthBufferValue(const ScreenPoint& pos) const = 0;
	virtual void SetDepthBufferValue(const ScreenPoint& pos, float depth) = 0;
};

};