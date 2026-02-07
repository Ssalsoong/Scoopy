#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <SimpleMath.h>
#include "PhysxManager.h"


namespace MMMEngine
{
    class USERSCRIPTS SnowTrigger : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        SnowTrigger()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        //USCRIPT_MESSAGE()
        //void OnTriggerEnter(MMMEngine::CollisionInfo info);

        //USCRIPT_MESSAGE()
        //void OnTriggerExit(MMMEngine::CollisionInfo info);
    };
}
