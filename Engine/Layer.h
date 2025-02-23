#pragma once
#include "Entity.h"

namespace JE {
    class Object;

    // Type 별로 Object를 관리해 LayerType의 순서대로 그리기
    class Layer : public Entity
    {
        using Super = Entity;
        using ObjectIter = std::vector<std::unique_ptr<Object>>::iterator;
    public:
        Layer() {}
        Layer(const std::wstring& name) : Super(name) {}
        virtual ~Layer() override {}

        // Layer의 Object를 초기화
        virtual void Init() override;
        // Layer의 모든 Object의 Tick 실행
        virtual void Tick(const float& DeltaTime) override;
        // Layer의 모든 Object를 Render
        virtual void Render(IRenderer* r) override;
        // Layer의 Object 중 state가 Dead인 object를 layer에서 삭제
        virtual void Destroy();

        // Layer에 Object 추가
        Object* AddObject(std::unique_ptr<Object>&& obj);
        // Layer에 추가되어 있는 Object 리스트
        const std::vector<std::unique_ptr<Object>>& GetObjects() { return _objects; }
        // Layer에서 Object 삭제
        void EraseObject(Object* obj);
        // Layer의 Object를 이동시키기 위해 현재 리스트에서 삭제하고 Object를 반환
        std::unique_ptr<Object>&& MoveObject(Object* Obj);
    private:
        // Object가 활성화 상태인지 확인
        bool isActive(Object* obj);

    private:
        std::vector<std::unique_ptr<Object>> _objects;
    };

}