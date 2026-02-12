#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "CoreComponents.h"
#include "LevelUpManager.h"
#include "AnimationCurve.h"

namespace MMMEngine
{
    class USERSCRIPTS LevelUpBubble : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        ObjPtr<Canvas> mCanvas;
        ObjPtr<GameObject> mPlayer;
        ObjPtr<Image> mSpeechBubbleIcon;
        ObjPtr<Text> mHeadline;
        ObjPtr<Text> mScript;
        std::vector<ObjPtr<Image>> mIcons;

        USCRIPT_PROPERTY()
            AnimationCurve mBubbleSizeCurve;
		USCRIPT_PROPERTY()
			AnimationCurve mCloseSizeCurve;
		USCRIPT_PROPERTY()
			AnimationCurve mSelectRotCurve;

        float mCurveScale = 0.0f;
        float mCloseScale = 0.0f;

        float mFinalScale = 0.0f;

        float mElipsedTime = 0.0f;  // 전체 애니메이션 타임
        float mSelectTime = 0.0f;   // 아이콘 선택커브 타임
        bool isAnimating = false;

        int mSelectIdx = 0;

        bool isActive = false;
        bool isDirty = true;
        EXPTYPE mType = EXPTYPE::EXP_END;

        void SetUIActive(bool _val);
        void SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding);
        void SetIconTrans();

        void UpdateControl();
        void UpdateIcon();
    public:
        LevelUpBubble()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        void SetHeadlineText(const std::wstring& _text);
        void SetScriptText(const std::wstring& _text);
        void SetActive(bool _val);

        void SetIcons(std::vector<ObjPtr<Image>>& _vec);
        bool IsAnimating() { return isAnimating; }
    public:
		USCRIPT_PROPERTY()
			int mInputLayer = 10;
		USCRIPT_PROPERTY()
			float mUIScale = 1.0f;
		USCRIPT_PROPERTY()
			float mDistanceFactor = 0.2f;
		USCRIPT_PROPERTY()
			float mIconWidth = 10.0f;
		USCRIPT_PROPERTY()
			Vector2 mSpeechOffset;
        USCRIPT_PROPERTY()
            Vector2 mIconOffset;
		USCRIPT_PROPERTY()
			Vector2 mHeadlineOffset;
		USCRIPT_PROPERTY()
			Vector2 mScriptOffset;

		USCRIPT_PROPERTY()
			float mSelectIconSize = 1.0f;
		USCRIPT_PROPERTY()
            float mDeselectIconSize = 0.8f;
    };
}
