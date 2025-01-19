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
		// �ڱ��ڽ��� Component, �浹�� Object, �浹�� Object�� Component
		std::function<void(Collider*, Collider*)> _callBack;
	};
}
