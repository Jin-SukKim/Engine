#pragma once

namespace JE {
	
class Application
{
public:
	Application(std::unique_ptr<Engine> engine, std::unique_ptr<RendererInterface> renderer) : _engine(std::move(engine)), _renderer(std::move(renderer)) {}
	~Application() {}
	bool Init(const ScreenPoint& screenSize);
	void Run();
	void Tick();
	void Render();

	Engine* GetEngine() { return _engine.get(); }
	TimeManager* GetTimer();
	RendererInterface* GetRenderer() { return _renderer.get(); }

	void DrawGizmo();

protected:
	std::unique_ptr<Engine> _engine;
	std::unique_ptr<RendererInterface> _renderer;
	Color _bgColor = Color::White;
};

}
