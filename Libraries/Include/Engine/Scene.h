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

		// Scene�� Object�� �ʱ�ȭ
		virtual void Init() override;
		// Scene�� ��� Object�� Tick ����
		virtual void Tick(const float& DeltaTime) override;
		// Scene�� ��� Object�� Render
		virtual void Render(IRenderer* r) override;
		// Scene�� Object �� state�� Dead�� object�� Scene���� ����
		virtual void Destroy();

		// Scene�� Load (���� Scene�� ��ġ�� ������Ʈ�� ȯ�� �ʱ�ȭ � ���)
		virtual void OnEnter();
		// ���� Scene���� ������ �ٸ� Scene���� ����
		virtual void OnExit();

		// type�� �´� layer�� object �߰�
		Object* AddObject(std::unique_ptr<Object>&& obj, const LayerType type);
		// type�� �´� layer
		Layer* GetLayer(const LayerType& type) { return _layers[static_cast<uint16>(type)].get(); }
		// Object�� �ű��
		std::unique_ptr<Object>&& MoveObject(Object* obj);
		// Object ����
		void EraseObject(Object* obj);
	private:
		// Object Ÿ�Ժ��� Layer �ʱ�ȭ
		void createLayers();

	private:
		// LayerType�� ������� �����ϰ� �׸���
		std::vector<std::unique_ptr<Layer>> _layers;
	};

}