#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class USERSCRIPTS BuffBuilding : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        BuffBuilding()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
        float speedbuff = 10.0f;
        int attackbuff = 5;
        float buffdist = 2.0f;
        
        ObjPtr<GameObject> player;
        DirectX::SimpleMath::Vector3 playerpos;
        bool prevInRange = false;
        void Start();
        void Update();
        void GiveBuff();
        void LevelApply(int level);
    };
}
