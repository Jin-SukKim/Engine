#pragma once

namespace JE {

    class TimeManager;
    class IRenderer;
    class SceneManager;
    class CollisionManager;
    class Scene;
    class Layer;

    class Engine
    {
    public:
        static Engine& GetEngine() {
            static Engine instance;
            return instance;
        }

        virtual ~Engine() {};

        virtual bool Init();
        virtual void Tick();
        virtual void Render(IRenderer* r);

        TimeManager* GetTimeManager() { return &_timeManager; }
        InputManager* GetInputManager() { return &_inputManager; }
        SceneManager* GetSceneManager() { return &_sceneManager; }
        CollisionManager* GetCollisionManager() { return &_collisionManager; }

        void SetManager();

        template<typename T>
        static T* Instantiate(const std::wstring& name, LayerType type);

    private:
        Engine() {} // Private constructor for singleton
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        TimeManager _timeManager;
        // TODO: InputManager를 Engine이 아닌 각 Window 클래스가 가지고 있어야 함
        InputManager _inputManager;
        SceneManager _sceneManager; // 각 윈도우가 가질지 엔진에 하나의 Manager만 가질지 고민 중
        CollisionManager _collisionManager;
    };

    template<typename T>
    inline T* Engine::Instantiate(const std::wstring& name, LayerType type)
    {
        Engine& engine = Engine::GetEngine();
        std::unique_ptr<T> newObj = std::make_unique<T>(name);
        newObj->SetLayerType(type);
        Scene* activeScene = engine.GetSceneManager()->GetActiveScene();
        if (!activeScene)
            return nullptr;
        Layer* layer = activeScene->GetLayer(type);
        if (!layer)
            return nullptr;

        return dynamic_cast<T*>(layer->AddObject(std::move(newObj)));
    }

}
