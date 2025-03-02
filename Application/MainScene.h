#pragma once
#include "Engine/Scene.h"

namespace JE {
	class SpriteActor;
	class Mesh2DComponent;
	class Camera2DComponent;

	class MainScene : public Scene
	{
		using Super = Scene;
	public:
		MainScene(const std::wstring& name);
		~MainScene() override {}

		void Init() override;
		void Tick(const float& DeltaTime) override;
		void Render(IRenderer* r) override;

		void OnEnter() override;
		void OnExit() override;

		void DrawGizmo(IRenderer* r);
	private:
		// test
		SpriteActor* actor;
		Mesh2DComponent* mesh;
		Camera2DComponent* cam;
	};
}
