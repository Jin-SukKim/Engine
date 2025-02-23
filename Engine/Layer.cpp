#include "pch.h"
#include "Layer.h"
#include "Object.h"

namespace JE {
	void Layer::Init()
	{
		for (auto& object : _objects) {
			Object* obj = object.get();
			if (obj)
				obj->Init();
		}
	}
	void Layer::Tick(const float& DeltaTime)
	{
		for (auto& object : _objects) {
			Object* obj = object.get();
			if (isActive(obj))
				obj->Tick(DeltaTime);
		}
	}
	void Layer::Render(IRenderer* r)
	{
		for (auto& object : _objects) {
			Object* obj = object.get();
			if (isActive(obj))
				obj->Render(r);
		}
	}
	void Layer::Destroy()
	{
		for (ObjectIter iter = _objects.begin(); iter != _objects.end();) {
			Object* obj = iter->get();
			// object�� ���°� Dead�� Layer���� �����
			if (obj && obj->GetState() == Object::State::Dead)
				iter = _objects.erase(iter); // ����鼭 ���� iter ��ȯ
			else
				++iter;
		}

		//std::erase_if(_objects, [](const std::unique_ptr<Object>& obj) {
		//	return obj->IsDead();
		//	});
	}
	Object* Layer::AddObject(std::unique_ptr<Object>&& obj)
	{
		if (!obj)
			return nullptr;

		_objects.emplace_back(std::move(obj));
		return _objects.back().get();
	}

	void Layer::EraseObject(Object* eraseObj)
	{
		std::erase_if(_objects, [&](const std::unique_ptr<Object>& obj) {
			return obj.get() == eraseObj;
			});
	}
	
	std::unique_ptr<Object>&& Layer::MoveObject(Object* obj)
	{
		// obj ã��
		ObjectIter it = std::find_if(_objects.begin(), _objects.end(),
			[&](const std::unique_ptr<Object>& object) {
				return object.get() == obj;
			});

		if (it != _objects.end()) {
			// �������� �ӽ� ������ �̵�
			std::unique_ptr<Object>&& moveObj = std::move(*it);
			// ���� ����ִ� ��Ҹ� �����̳ʿ��� ����
			_objects.erase(it);
			// �̵��� ��ü ��ȯ
			return std::move(moveObj);
		}
		
		// nullptr ��ȯ
		static std::unique_ptr<Object> nullObj = nullptr;
		return std::move(nullObj);
	}

	bool Layer::isActive(Object* obj)
	{
		if (!obj)
			return false;

		Object::State state = obj->GetState();
		if (state == Object::State::Paused || state == Object::State::Dead)
			return false;

		return true;
	}
}