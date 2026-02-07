#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"

namespace MMMEngine
{
    class Image;
    class USERSCRIPTS FadeInOutFX : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND


            int m_fadeState = 0; // 0 = idle, 1 = in, 2 = out;
        float m_internalTime = 0.0f;


    public:
        FadeInOutFX()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(FadeIn);
        REGISTER_BEHAVIOUR_MESSAGE(FadeOut);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        static ObjPtr<FadeInOutFX> Instance;
        
        USCRIPT_PROPERTY()
        ObjPtr<Image> FadeImage;

        USCRIPT_PROPERTY()
        float FadeDuration = 0.55f;

        USCRIPT_PROPERTY()
	    AnimationCurve FadeCurve;

        int GetState();

        USCRIPT_MESSAGE()
        void Awake();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
        void FadeIn();

        USCRIPT_MESSAGE()
        void FadeOut();
    };
}
