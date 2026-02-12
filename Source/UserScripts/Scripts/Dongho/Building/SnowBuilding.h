#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class USERSCRIPTS SnowBuilding : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        SnowBuilding()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Start();

        void Update();
        void UpdateSnow();
        void LevelApply(int level);
        void ProvideSnow();
        int snowpoint = 0;
        int maxsnowpoint = 1;
        float chargeDelay = 2.0f;
        float chargeTimer = 0.0f;

        float provideDelay = 0.5f;
        float provideTimer = 0.0f;
        float snowProvidedist = 2.0f;

        ObjPtr<GameObject> player;
        DirectX::SimpleMath::Vector3 playerpos;
    };
}
