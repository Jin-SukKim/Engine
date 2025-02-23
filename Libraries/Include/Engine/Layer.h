#pragma once
#include "Entity.h"

namespace JE {
    class Object;

    // Type ���� Object�� ������ LayerType�� ������� �׸���
    class Layer : public Entity
    {
        using Super = Entity;
        using ObjectIter = std::vector<std::unique_ptr<Object>>::iterator;
    public:
        Layer() {}
        Layer(const std::wstring& name) : Super(name) {}
        virtual ~Layer() override {}

        // Layer�� Object�� �ʱ�ȭ
        virtual void Init() override;
        // Layer�� ��� Object�� Tick ����
        virtual void Tick(const float& DeltaTime) override;
        // Layer�� ��� Object�� Render
        virtual void Render(IRenderer* r) override;
        // Layer�� Object �� state�� Dead�� object�� layer���� ����
        virtual void Destroy();

        // Layer�� Object �߰�
        Object* AddObject(std::unique_ptr<Object>&& obj);
        // Layer�� �߰��Ǿ� �ִ� Object ����Ʈ
        const std::vector<std::unique_ptr<Object>>& GetObjects() { return _objects; }
        // Layer���� Object ����
        void EraseObject(Object* obj);
        // Layer�� Object�� �̵���Ű�� ���� ���� ����Ʈ���� �����ϰ� Object�� ��ȯ
        std::unique_ptr<Object>&& MoveObject(Object* Obj);
    private:
        // Object�� Ȱ��ȭ �������� Ȯ��
        bool isActive(Object* obj);

    private:
        std::vector<std::unique_ptr<Object>> _objects;
    };

}