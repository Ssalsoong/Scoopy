#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>

namespace MMMEngine { class Player; }

namespace MMMEngine
{
    class USERSCRIPTS PlayerHpGage : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            USCRIPT_PROPERTY()
            ObjPtr<Player> mPlayer;

        ObjPtr<Gage> mGage;
    public:
        PlayerHpGage()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();
    };
}
