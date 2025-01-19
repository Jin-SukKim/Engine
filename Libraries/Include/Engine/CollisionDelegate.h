#pragma once

namespace JE {
	class Collider;

	class CollisionDelegate
	{
	public:
		template<typename T>
		void BindDelegate(T* object, void(T::* func)(Collider*, Collider*)) {
			_callBack = [object, func](Collider* comp, Collider* other) {
				(object->*func)(comp, other);
				};
		}

		void Execute(Collider* comp, Collider* other) {
			if (_callBack)
				_callBack(comp, other);
		}
	private:
		// 자기자신의 Component, 충돌한 Object, 충돌한 Object의 Component
		std::function<void(Collider*, Collider*)> _callBack;
	};
}
