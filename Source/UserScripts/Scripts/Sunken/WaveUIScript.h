#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "CoreComponents.h"

namespace MMMEngine
{
    class USERSCRIPTS WaveUIScript : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            ObjPtr<Image> mWaveBackground;
        USCRIPT_PROPERTY()
            ObjPtr<Text> mWaveText;

        bool mPrevStat = false;
    public:
        WaveUIScript()
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
