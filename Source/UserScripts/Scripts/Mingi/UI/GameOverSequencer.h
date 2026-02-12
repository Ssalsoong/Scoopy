#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class Camera;
    class RectTransform;
    class Text;
    class Image;
    class USERSCRIPTS GameOverSequencer : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            float m_focusFadeSpeed = 15.000f;
            float m_internalTimer = 0.0f;
            float m_sceneChangeTime = 1.250f;
            int m_animState = 0;
            int m_currentSelected = 0;

            float m_replayFocusAlpha = 0.0f;
            float m_totitleFocusAlpha = 0.0f;

            float m_state1_maximumTime = 0.0f;
            float m_state3_maximumTime = 0.0f;

            bool m_startGameOverSequence = false;

            bool m_isControlAble = false;

        DirectX::SimpleMath::Vector3 m_firstCamPos = {};
        DirectX::SimpleMath::Vector3 m_firstCamRot = {};
        DirectX::SimpleMath::Vector2 m_firstGameOverPos = {};
        DirectX::SimpleMath::Vector3 m_firstGameOverRot = {};
        DirectX::SimpleMath::Vector2 m_firstReplayPos = {};
        DirectX::SimpleMath::Vector2 m_firstToTitlePos = {};

        ObjPtr<Camera> m_mainCam;

        ObjPtr<RectTransform> m_gameoverRect;
        ObjPtr<RectTransform> m_replayRect;
        ObjPtr<RectTransform> m_totitleRect;
    public:
        GameOverSequencer()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(StartGameOver);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
        float ButtonOffsetTime = 0.08f;

        USCRIPT_PROPERTY()
            ObjPtr<Image> GameOverBGPanel;

        USCRIPT_PROPERTY()
            ObjPtr<Text> GameOverText;


        USCRIPT_PROPERTY()
            ObjPtr<Text> ReplayText;

        USCRIPT_PROPERTY()
            ObjPtr<Text> ToTitleText;

        USCRIPT_PROPERTY()
        AnimationCurve PanelCV;

        USCRIPT_PROPERTY()
        AnimationCurve GameOverPosXCV;

        USCRIPT_PROPERTY()
        std::string TitleSceneName = "TitleScene";

        USCRIPT_PROPERTY()
            std::string GameSceneName = "GameScene";

        USCRIPT_PROPERTY()
        AnimationCurve GameOverPosYCV;

        USCRIPT_PROPERTY()
        AnimationCurve GameOverRotZCV;

        USCRIPT_PROPERTY()
            AnimationCurve ButtonYPosCV;


        USCRIPT_PROPERTY()
            AnimationCurve ButtonScaleCV;

        USCRIPT_PROPERTY()
        AnimationCurve CameraPosZCV;

        USCRIPT_PROPERTY()
        AnimationCurve CameraPosYCV;

        USCRIPT_PROPERTY()
        AnimationCurve CameraRotXCV;


        USCRIPT_PROPERTY()
            AnimationCurve CameraRotYCV;

        USCRIPT_PROPERTY()
            AnimationCurve CameraRotZCV;

        USCRIPT_MESSAGE()
            void StartGameOver();


        bool IsSequenceEnable();
    };
}
