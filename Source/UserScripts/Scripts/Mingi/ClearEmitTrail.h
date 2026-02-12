#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class USERSCRIPTS ClearEmitTrail : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ClearEmitTrail()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnDisable);

        }

        USCRIPT_MESSAGE()
        void OnDisable();
    };
}
