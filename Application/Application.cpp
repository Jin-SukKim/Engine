#include "pch.h"
#include "Application.h"

#include "Engine/SpriteActor.h"
#include "Engine/Mesh2DComponent.h"
#include "Engine/Camera2DComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/Transform2DComponent.h"

namespace JE {

	bool Application::Init(const ScreenPoint& screenSize)
	{
		if (!GetRenderer()->Init(screenSize))
			return false;

		if (!GetEngine()->Init())
			return false;
		
		AssetManager::Load<Mesh2D>(L"SquareMesh", MeshData::SquareVertex, MeshData::SquareIndices);

		actor = new SpriteActor(L"Player");
		actor->Init();
		Transform2DComponent* tr = actor->GetComponent<Transform2DComponent>();
		tr->SetScale(30.f);
		mesh = actor->AddComponent<Mesh2DComponent>(L"PlayerMesh");
		mesh->SetMesh(L"SquareMesh");
		cam = actor->AddComponent<Camera2DComponent>(L"PlayerCamera");
		cam->AttachCamera(actor);

		return true;
	}

	void Application::Run()
	{
		GetTimer()->Begin();

		Tick();
		Render();

		GetTimer()->End();
	}

	void Application::Tick()
	{
		GetEngine()->Tick(GetTimer()->GetDeltaTime());
	}

	void Application::Render()
	{
		IRenderer* r = GetRenderer();
		r->Clear(_bgColor);

		// BackBuffer에 그리기
		//GetEngine()->Render();
		DrawGizmo();

		r->PushStatisticText(std::format(L"{:.9f} fps", GetTimer()->GetDeltaTime()));

		// Test
		const Transform2DComponent* tr = actor->GetComponent<Transform2DComponent>();
		Matrix3x3 finalMat = tr->GetTransformMatrix() * cam->GetViewMatrix();


		IRenderer2D* r2 = dynamic_cast<IRenderer2D*>(GetRenderer());
		r2->DrawMesh(mesh->GetMesh(), finalMat, nullptr);

		r->Render(); // SwapBuffer
	}

	TimeManager* Application::GetTimer()
	{
		if (GetEngine())
			return GetEngine()->GetTimeManager();
		return nullptr;
	}

	void Application::DrawGizmo()
	{
		IRenderer& r = *GetRenderer();
		const Engine& g = *GetEngine();

		Color gridColor(0.8f, 0.8f, 0.8f, 0.3f);

		ScreenPoint screenSize = r.GetScreenSize();

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
			r.DrawFullVerticalLine(gridBottomLeft.X + x * gridUnit, gridColor);
		for (int y = 0; y < yGrid; y++)
			r.DrawFullHorizontalLine(gridBottomLeft.Y - y * gridUnit, gridColor);

		ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(screenSize, -viewPos);
		r.DrawFullHorizontalLine(worldOrigin.Y, Color::Red);
		r.DrawFullVerticalLine(worldOrigin.X, Color::Blue);
	}
}