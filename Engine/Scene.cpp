#include "pch.h"
#include "Scene.h"
#include "Object.h"
#include "Layer.h"

namespace JE {
	Scene::Scene(const std::wstring& name) : Super(name)
	{
		createLayers();
	}
	void Scene::Init()
	{
		for (auto& layer : _layers) {
			Layer* l = layer.get();
			if (l)
				l->Init();
		}
	}
	void Scene::Tick(const float& DeltaTime)
	{
		for (auto& layer : _layers) {
			Layer* l = layer.get();
			if (l)
				l->Tick(DeltaTime);
		}
	}
	void Scene::Render(IRenderer* r)
	{
		for (auto& layer : _layers) {
			Layer* l = layer.get();
			if (l)
				l->Render(r);
		}
	}
	void Scene::Destroy()
	{
		for (auto& layer : _layers) {
			Layer* l = layer.get();
			if (l)
				l->Destroy();
		}
	}

	void Scene::OnEnter()
	{
		Init();
	}

	void Scene::OnExit()
	{
		// TODO: 보통 Scene에서 나갈때 Collision 초기화
	}
	
	Object* Scene::AddObject(std::unique_ptr<Object>&& obj, const LayerType type)
	{
		// type에 맞는 layer에 object 추가
		return _layers[static_cast<uint16>(type)]->AddObject(std::move(obj));
	}

	std::unique_ptr<Object>&& Scene::MoveObject(Object* obj)
	{
		// Object의 LayerType 가져오기
		LayerType type = obj->GetLayerType();
		// Layer에서 Object를 찾아서 옮기기
		return _layers[static_cast<uint16>(type)]->MoveObject(obj);
	}

	void Scene::EraseObject(Object* obj)
	{
		// Object에 맞는 LayerType 가져오기
		LayerType type = obj->GetLayerType();
		// Layer에서 Object를 삭제하기
		_layers[static_cast<uint16>(type)]->EraseObject(obj);
	}

	void Scene::createLayers()
	{
		// 각 LayerType별로 Object를 관리하도록 Type 크기만큼 초기화
		uint16 max = static_cast<uint16>(LayerType::MaxLayerType);
		_layers.resize(max);
		for (uint16 i = 0; i < max; ++i)
			_layers[i] = std::make_unique<Layer>(); // 각 LayerType 별로 Layer 생성
	}
}