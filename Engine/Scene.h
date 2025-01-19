#pragma once
#include "Entity.h"

namespace JE {
	class Object;
	class Layer;

	class Scene : public Entity
	{
		using Super = Entity;
	public:
		Scene(const std::wstring& name);
		virtual ~Scene() override {};

		// Scene의 Object를 초기화
		virtual void Init() override;
		// Scene의 모든 Object의 Tick 실행
		virtual void Tick(const float& DeltaTime) override;
		// Scene의 모든 Object를 Render
		virtual void Render(IRenderer* r) override;
		// Scene의 Object 중 state가 Dead인 object를 Scene에서 삭제
		virtual void Destroy();

		// Scene을 Load (보통 Scene에 배치된 오브젝트나 환경 초기화 등에 사용)
		virtual void OnEnter();
		// 현재 Scene에서 나가서 다른 Scene으로 들어갈떄
		virtual void OnExit();

		// type에 맞는 layer에 object 추가
		Object* AddObject(std::unique_ptr<Object>&& obj, const LayerType type);
		// type에 맞는 layer
		Layer* GetLayer(const LayerType& type) { return _layers[static_cast<uint16>(type)].get(); }
		// Object를 옮기기
		std::unique_ptr<Object>&& MoveObject(Object* obj);
		// Object 삭제
		void EraseObject(Object* obj);
	private:
		// Object 타입별로 Layer 초기화
		void createLayers();

	private:
		// LayerType의 순서대로 저장하고 그리기
		std::vector<std::unique_ptr<Layer>> _layers;
	};

}