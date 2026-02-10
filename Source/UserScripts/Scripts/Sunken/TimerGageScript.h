#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>

namespace MMMEngine
{
    class USERSCRIPTS TimerGageScript : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            ObjPtr<Gage> mTimerGage;

        bool mPrevStat = false;

    public:
        TimerGageScript()
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
