#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class Image;
    class USERSCRIPTS GameClearSequencer : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
        float m_internalTimer = 0.0f;
        int m_animState = 0;
    public:
        GameClearSequencer()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
        std::string NextSceneName = "TitleScene";

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_PROPERTY()
            ObjPtr<Image> GameClearImg;

        USCRIPT_PROPERTY()
        ObjPtr<Image> GameClearBGPanel;


        USCRIPT_PROPERTY()
            AnimationCurve GameClearBGAlphaCV;

        USCRIPT_PROPERTY()
        AnimationCurve GameClearScaleXCV;

        USCRIPT_PROPERTY()
        AnimationCurve GameClearScaleYCV;

        USCRIPT_PROPERTY()
        AnimationCurve GameClearAlphaCV;

        USCRIPT_PROPERTY()
        AnimationCurve GameClearRotZCV;

        void StartGameClear();
    };
}
