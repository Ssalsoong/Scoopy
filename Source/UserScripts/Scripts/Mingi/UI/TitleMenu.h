#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"

namespace MMMEngine
{
    class Image;
    class USERSCRIPTS TitleMenu : public ScriptBehaviour
    {
    private:

        int m_selected = 0;
        
        float m_internalTimer[3] = {0,};

        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        ObjPtr<Image> GetButtonFromIdx(int idx);

    public:
        TitleMenu()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
        USCRIPT_PROPERTY()
        ObjPtr<Image> playButton;
        USCRIPT_PROPERTY()
        ObjPtr<Image> creditButton;
        USCRIPT_PROPERTY()
        ObjPtr<Image> exitButton;

        USCRIPT_PROPERTY()
            bool IsControllAble = false;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_PROPERTY()
            AnimationCurve ButtonOnScaleX;

        USCRIPT_PROPERTY()
            AnimationCurve ButtonOnScaleY;

        void TurnOffAllButton();
        void TurnOnPlayButton();
    };
}
