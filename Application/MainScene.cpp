#include "pch.h"
#include "MainScene.h"
#include "Windows/Application.h"

#include "Renderer/CppRenderer2D.h"
#include "Engine/SpriteActor.h"
#include "Engine/Mesh2DComponent.h"
#include "Engine/Camera2DComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/MeshData.h"
#include "Engine/Transform2DComponent.h"
#include "Renderer/RendererInterface.h"

namespace JE {
	MainScene::MainScene(const std::wstring& name) : Super(name)
	{
		AssetManager::Load<Mesh2D>(L"SquareMesh", MeshData::SquareVertex, MeshData::SquareIndices);
	}

	void MainScene::Init()
	{
		actor = Engine::Instantiate<SpriteActor>(L"Player", LayerType::Player);
		actor->Init();
		Transform2DComponent* tr = actor->GetComponent<Transform2DComponent>();
		tr->SetScale(30.f);
		mesh = actor->AddComponent<Mesh2DComponent>(L"PlayerMesh");
		mesh->SetMesh(L"SquareMesh");
		cam = actor->AddComponent<Camera2DComponent>(L"PlayerCamera");
		cam->AttachCamera(actor);

		Super::Init();
	}

	void MainScene::Tick(const float& DeltaTime)
	{
		Super::Tick(DeltaTime);
	}

	void MainScene::Render(IRenderer* r)
	{
		Super::Render(r);

		IRenderer2D* i2 = dynamic_cast<IRenderer2D*>(r);
		if (i2)
			i2->SetViewCamera(cam);
		DrawGizmo(r);

		actor->Render(r);
	}

	void MainScene::OnEnter()
	{
		Super::OnEnter();
	}

	void MainScene::OnExit()
	{


		Super::OnExit();
	}

	void MainScene::DrawGizmo(IRenderer* r)
	{
		Color gridColor(0.8f, 0.8f, 0.8f, 0.3f);

		ScreenPoint screenSize = r->GetScreenSize();

		Vector2 viewPos = Vector2(0.f, 0.f); // 메인 카메라 위치
		Vector2 extent = Vector2(screenSize.X * 0.5f, screenSize.Y * 0.5f); // 화면 중앙 좌표

		// 그리드의 격자
		int gridUnit = 10;
		int xGrid = screenSize.X / gridUnit; // x축 격자 수
		int yGrid = screenSize.Y / gridUnit; // y축 격자 수

		// 그리드가 시작되는 좌하단 좌표 값 계산
		Vector2 minPos = viewPos - extent; // 카메라 위치에서 화면의 반 크기를 빼서 보이는 화면의 좌하단 위치를 계산
		// 그리드 단위로 맞춘 좌하단 위치 계산 (그리드 단위에 맞춰 좌표 정렬)
		Vector2 minGridPos = Vector2(std::ceilf(minPos.X / (float)gridUnit), std::ceilf(minPos.Y / (float)gridUnit)) * (float)gridUnit;
		// 화면 좌표로 변환 (그리드의 시작 좌표를 카메라 위치를 기준으로 재조정)
		ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(screenSize, minGridPos - viewPos);

		for (int x = 0; x < xGrid; x++)
			r->DrawFullVerticalLine(gridBottomLeft.X + x * gridUnit, gridColor);
		for (int y = 0; y < yGrid; y++)
			r->DrawFullHorizontalLine(gridBottomLeft.Y - y * gridUnit, gridColor);

		ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(screenSize, -viewPos);
		r->DrawFullHorizontalLine(worldOrigin.Y, Color::Red);
		r->DrawFullVerticalLine(worldOrigin.X, Color::Blue);
	}
}