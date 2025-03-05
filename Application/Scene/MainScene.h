#pragma once
#include "Engine/Scene.h"

namespace JE {
	class SpriteActor;
	class Mesh2DComponent;
	class Camera2DComponent;
	class Animator2DComponent;
	class Collider;

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

		void GiveWater();
		void HitWall(Collider* comp, Collider* other);
	private:
		// test
		SpriteActor* actor;
		Mesh2DComponent* mesh;
		Camera2DComponent* cam;
		Camera2DComponent* mainCam;
		Animator2DComponent* anim;
	};
}
