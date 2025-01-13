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
			// object의 상태가 Dead면 Layer에서 지우기
			if (obj && obj->GetState() == Object::State::Dead)
				iter = _objects.erase(iter); // 지우면서 다음 iter 반환
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
		// obj 찾기
		ObjectIter it = std::find_if(_objects.begin(), _objects.end(),
			[&](const std::unique_ptr<Object>& object) {
				return object.get() == obj;
			});

		if (it != _objects.end()) {
			// 소유권을 임시 변수로 이동
			std::unique_ptr<Object>&& moveObj = std::move(*it);
			// 이제 비어있는 요소를 컨테이너에서 삭제
			_objects.erase(it);
			// 이동된 객체 반환
			return std::move(moveObj);
		}
		
		// nullptr 반환
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