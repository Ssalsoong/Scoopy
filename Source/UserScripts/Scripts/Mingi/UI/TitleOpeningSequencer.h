#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"

namespace MMMEngine
{
    class TitleMenu;
    class Image;
    class USERSCRIPTS TitleOpeningSequencer : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            float m_internalTime = 0.0f;
        int m_state = 0;

        float m_playBY = 0.0f;
        float m_creditBY = 0.0f;
        float m_exitBY = 0.0f;
    public:
        TitleOpeningSequencer()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
            float titleFadeStartTime = 0.52f;

        USCRIPT_PROPERTY()
            float titleLoadFadeTime = 1.2f;

        USCRIPT_PROPERTY()
            float buttonLoadFadeTime = 0.32f;


        USCRIPT_PROPERTY()
            float buttonAnimOffsetTime = 0.65f;

        USCRIPT_PROPERTY()
            float buttonAnimYDiff = 12.0f;

        USCRIPT_PROPERTY()
            ObjPtr<Image> TitleImage;

        USCRIPT_PROPERTY()
            ObjPtr<Image> PlayImage;

        USCRIPT_PROPERTY()
            ObjPtr<Image> CreditImage;

        USCRIPT_PROPERTY()
            ObjPtr<Image> ExitImage;

        USCRIPT_PROPERTY()
            AnimationCurve TitleRotate;

        USCRIPT_PROPERTY()
            AnimationCurve TitleScaleX;

        USCRIPT_PROPERTY()
            AnimationCurve TitleScaleY;

        USCRIPT_PROPERTY()
            AnimationCurve ButtonScaleX;
        USCRIPT_PROPERTY()
            AnimationCurve ButtonScaleY;
        USCRIPT_PROPERTY()
            ObjPtr<TitleMenu> TitleMenu;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();
    };
}
