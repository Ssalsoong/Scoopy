#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimationCurve.h"

namespace MMMEngine
{
    class RectTransform;
    class Text;
    class Gage;
    class Image;
    class USERSCRIPTS TimerUI : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            int m_waveCount = 0;
        int m_maxWaveNum = 10;
        float m_animationTime = 0.0f;
        int m_animState = 0;
        bool m_isWaveMode = false;
        float m_initPosY = 0.0f;
        float m_initPosX = 0.0f;

        bool m_textChanged = false;
        float m_maximumAnimTime = 0.0f;
        bool m_playingAnimation = false;
        ObjPtr<RectTransform> m_rect = nullptr;
    public:
        TimerUI()
        {
        REGISTER_BEHAVIOUR_MESSAGE(ForceSwitchTimer);
        REGISTER_BEHAVIOUR_MESSAGE(ForceSwitchWave);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(SwitchTimer);
        REGISTER_BEHAVIOUR_MESSAGE(SwitchWave);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_PROPERTY()
        ObjPtr<Text> WaveText = nullptr;

        USCRIPT_PROPERTY()
        ObjPtr<Gage> TimerGage = nullptr;


        USCRIPT_PROPERTY()
        ObjPtr<Image> WaveBack = nullptr;

        USCRIPT_PROPERTY()
            float changeTextTime = 0.0f;

        void SetWaveCount(int count);
        void SetMaxWaveNum(int num);

        USCRIPT_MESSAGE()
        void SwitchWave();

        USCRIPT_MESSAGE()
        void SwitchTimer();


        USCRIPT_MESSAGE()
            void ForceSwitchWave();

        USCRIPT_MESSAGE()
            void ForceSwitchTimer();

        USCRIPT_PROPERTY()
            AnimationCurve scaleCurve;

        USCRIPT_PROPERTY()
            AnimationCurve rotCurve;

        USCRIPT_PROPERTY()
            AnimationCurve posCurve;

        USCRIPT_PROPERTY()
            AnimationCurve alphaCurve;

        bool IsPlayingAnimation();

        USCRIPT_PROPERTY()
            AnimationCurve gageCurve;
    };
}
