#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class GameObject;
    class RectTransform;
    class Text;
    class USERSCRIPTS PauseUI : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            ObjPtr<Text> m_P_text;
            ObjPtr<Text> m_a_text;
            ObjPtr<Text> m_u_text;
            ObjPtr<Text> m_s_text;
            ObjPtr<Text> m_e_text;

            ObjPtr<Text> m_Resume_text;
            ObjPtr<Text> m_ToTitle_text;

            float m_internalTimer = 0.0f;

            float m_buttonSelectTimer = 0.0f;

            bool m_isPause = false;

            bool m_isControlAble = false;
            int m_currentSelected = 0;
            float m_resumeFocusAlpha = 1.0f;
            float m_toTitleFocusAlpha = 0.5f;

            DirectX::SimpleMath::Vector2 m_P_pos = { 0.0f,0.0f };
            DirectX::SimpleMath::Vector2 m_a_pos = { 0.0f,0.0f };
            DirectX::SimpleMath::Vector2 m_u_pos = { 0.0f,0.0f };
            DirectX::SimpleMath::Vector2 m_s_pos = { 0.0f,0.0f };
            DirectX::SimpleMath::Vector2 m_e_pos = { 0.0f,0.0f };
            DirectX::SimpleMath::Vector2 m_Resume_pos = { 0.0f,0.0f };
            DirectX::SimpleMath::Vector2 m_ToTitle_pos = { 0.0f,0.0f };
    public:
        PauseUI()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
        ObjPtr<GameObject> PanelGO;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> P_rect;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> a_rect;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> u_rect;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> s_rect;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> e_rect;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> Resume_rect;

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> ToTitle_rect;

        USCRIPT_PROPERTY()
        AnimationCurve PosYCurve;

        USCRIPT_PROPERTY()
        AnimationCurve ButtonYCurve;

        USCRIPT_PROPERTY()
        float FadeDuration = 0.625f;

        USCRIPT_PROPERTY()
        float AnimOffestTime = 0.25f;

        USCRIPT_PROPERTY()
        int buttonAnimOffset = 10;

        USCRIPT_PROPERTY()
        float FocusFadeSpeed = 8.0f;

        USCRIPT_PROPERTY()
        AnimationCurve ButtonScaleCurve;

        USCRIPT_PROPERTY()
            float ButtonSelectTimeOffset = 0.0f;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
        void OnDestroy();
    };
}
