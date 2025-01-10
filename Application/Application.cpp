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

		// BackBuffer�� �׸���
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

		Vector2 viewPos = Vector2(0.f, 0.f); // ���� ī�޶� ��ġ
		Vector2 extent = Vector2(screenSize.X * 0.5f, screenSize.Y * 0.5f); // ȭ�� �߾� ��ǥ

		// �׸����� ����
		int gridUnit = 10;
		int xGrid = screenSize.X / gridUnit; // x�� ���� ��
		int yGrid = screenSize.Y / gridUnit; // y�� ���� ��

		// �׸��尡 ���۵Ǵ� ���ϴ� ��ǥ �� ���
		Vector2 minPos = viewPos - extent; // ī�޶� ��ġ���� ȭ���� �� ũ�⸦ ���� ���̴� ȭ���� ���ϴ� ��ġ�� ���
		// �׸��� ������ ���� ���ϴ� ��ġ ��� (�׸��� ������ ���� ��ǥ ����)
		Vector2 minGridPos = Vector2(std::ceilf(minPos.X / (float)gridUnit), std::ceilf(minPos.Y / (float)gridUnit)) * (float)gridUnit;
		// ȭ�� ��ǥ�� ��ȯ (�׸����� ���� ��ǥ�� ī�޶� ��ġ�� �������� ������)
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