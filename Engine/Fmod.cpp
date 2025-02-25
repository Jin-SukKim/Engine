#include "pch.h"
#include "Fmod.h"

namespace JE {
    FMOD::Studio::System* Fmod::System = nullptr;
    FMOD::System* Fmod::CoreSystem = nullptr;

    void Fmod::Initialize()
    {
        void* extraDriverData = nullptr;

        FMOD::Studio::System::create(&System);
        System->getCoreSystem(&CoreSystem);
        CoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

        System->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
    }

    bool Fmod::CreateSound(const std::string path, FMOD::Sound** sound)
    {
        if (FMOD_OK != CoreSystem->createSound(path.c_str(), FMOD_3D, 0, sound))
            return false;
        return true;
    }

    void Fmod::SoundPlay(FMOD::Sound* sound, FMOD::Channel** channel)
    {
        CoreSystem->playSound(sound, 0, false, channel);
    }

    void Fmod::Set3DListenerAttributes(const Vector3& pos, const Vector3& vel)
    {
        // 3D 에서는 각각 pos, vel, forward, up을 parameter로 받기
        //-1 ~ 1 : parameter로 받은 pos와 다른 값의 범위를 변환해줘야함 (NDC 좌표계 범위 쓰면 될듯)
        //FMOD_VECTOR fmodPos(pos.x, pos.y, 0.3f);
        //FMOD_VECTOR fmodVel(vel.x, vel.y, 0.f);
        FMOD_VECTOR fmodPos(0.f, 0.f, 0.0f);
        FMOD_VECTOR fmodVel(0.f, 0.f, 0.0f);
        FMOD_VECTOR fmodForward(0.0f, 0.0f, 1.0f);
        FMOD_VECTOR fmodUp(0.0f, 1.0f, 0.0f);

        CoreSystem->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);
    }

    void Fmod::Release()
    {
        System->release();
        System = nullptr;
    }
}