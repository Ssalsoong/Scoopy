#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class Image;
    class Canvas;
    class USERSCRIPTS SwitchSceneFX : public ScriptBehaviour
    {
    private:
        float m_timer = 0.0f;
        int m_animState = 0; // 0 : idle, 1 : start, 2 : end;
        DirectX::SimpleMath::Vector2 m_canvasSize;
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        SwitchSceneFX()
        {
        }

        USCRIPT_MESSAGE()
        void Awake();

        static ObjPtr<SwitchSceneFX> Instance;

        USCRIPT_PROPERTY()
        ObjPtr<Image> FXImage;

        USCRIPT_PROPERTY()
        AnimationCurve FXCurve;

        USCRIPT_PROPERTY()
            ObjPtr<Canvas> ParentCanvas;

        USCRIPT_MESSAGE()
        void OnDestroy();

        USCRIPT_MESSAGE()
        void Update();

        int GetState();


        USCRIPT_MESSAGE()
            void FXStart();

        USCRIPT_MESSAGE()
            void FXEnd();
    };
}
