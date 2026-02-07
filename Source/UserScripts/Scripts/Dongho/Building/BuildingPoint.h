#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class USERSCRIPTS BuildingPoint : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        BuildingPoint()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
        USCRIPT_PROPERTY()
        float checkdist = 2.f;
        bool canBuild = false;
        ObjPtr<GameObject> player;
        DirectX::SimpleMath::Vector3 playerpos;
        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        void CheckPlayer();
    };
}
