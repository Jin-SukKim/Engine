#pragma once
#include "Object.h"

namespace JE {
	class SpriteActor : public Object
	{
		using Super = Object;
	public:
		SpriteActor(const std::wstring& name);
		virtual ~SpriteActor() override {};

		virtual void Init() override;
		virtual void Tick(const float& DeltaTime) override;
		virtual void Render() override;

	private:
		// Transform2DCompoennt Ãß°¡
		void initTransform();
	};
}
