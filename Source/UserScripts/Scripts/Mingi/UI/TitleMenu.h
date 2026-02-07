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
        float m_internalTimer[3] = {0,}; // 0 = ButtonSelect, 1 = ButtonExecute, 2 = PopUp
        bool m_popupOn = false;
        int m_popupstate = 0; // 0 = idle, 1 = on, 2 = off

        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        ObjPtr<Image> GetButtonFromIdx(int idx);
        int GetIdxFromButtonString(std::string name);

        void MenuControl();
        void PopupAnimation();
        void MenuAnimation();

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
        ObjPtr<Image> creditPopup;

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

        USCRIPT_PROPERTY()
            AnimationCurve ButtonExecuteScaleX;

        USCRIPT_PROPERTY()
            AnimationCurve ButtonExecuteScaleY;

        USCRIPT_PROPERTY()
            AnimationCurve ButtonExecuteRotZ;

        USCRIPT_PROPERTY()
            AnimationCurve PopUpScaleXY;

        USCRIPT_PROPERTY()
            AnimationCurve PopUpOffScaleXY;


        USCRIPT_PROPERTY()
        float popupFadeDuration = 1.2f;


        void TurnOffAllButton();
        void TurnOnPlayButton();
        void TurnOnOneButton(std::string buttonName);
    };
}
