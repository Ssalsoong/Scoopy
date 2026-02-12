#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class PlayerController;
    class ParticleRenderer;
    class USERSCRIPTS ScoopParticleCheck : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ScoopParticleCheck()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_PROPERTY()
            ObjPtr<PlayerController> pc;

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> pt;
    };
}
