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
		// TODO: ���� Scene���� ������ Collision �ʱ�ȭ
	}
	
	Object* Scene::AddObject(std::unique_ptr<Object>&& obj, const LayerType type)
	{
		// type�� �´� layer�� object �߰�
		return _layers[static_cast<uint16>(type)]->AddObject(std::move(obj));
	}

	std::unique_ptr<Object>&& Scene::MoveObject(Object* obj)
	{
		// Object�� LayerType ��������
		LayerType type = obj->GetLayerType();
		// Layer���� Object�� ã�Ƽ� �ű��
		return _layers[static_cast<uint16>(type)]->MoveObject(obj);
	}

	void Scene::EraseObject(Object* obj)
	{
		// Object�� �´� LayerType ��������
		LayerType type = obj->GetLayerType();
		// Layer���� Object�� �����ϱ�
		_layers[static_cast<uint16>(type)]->EraseObject(obj);
	}

	void Scene::createLayers()
	{
		// �� LayerType���� Object�� �����ϵ��� Type ũ�⸸ŭ �ʱ�ȭ
		uint16 max = static_cast<uint16>(LayerType::MaxLayerType);
		_layers.resize(max);
		for (uint16 i = 0; i < max; ++i)
			_layers[i] = std::make_unique<Layer>(); // �� LayerType ���� Layer ����
	}
}