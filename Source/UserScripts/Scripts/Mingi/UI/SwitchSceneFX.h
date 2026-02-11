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
    class RectTransform;
    class USERSCRIPTS SwitchSceneFX : public ScriptBehaviour
    {
    private:
        float m_timer = 0.0f;
        int m_animState = 0; // 0 : idle, 1 : start, 2 : end;
        DirectX::SimpleMath::Vector2 m_canvasSize;
        ObjPtr<RectTransform> m_rect;
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        SwitchSceneFX()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(FXEnd);
        REGISTER_BEHAVIOUR_MESSAGE(FXStart);
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Awake();

        static ObjPtr<SwitchSceneFX> Instance;

        USCRIPT_PROPERTY()
        ObjPtr<Image> FXImage;

        USCRIPT_PROPERTY()
        AnimationCurve FXCurve;

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
