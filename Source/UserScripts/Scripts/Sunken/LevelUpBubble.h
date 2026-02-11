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
            AnimationCurve mAnimCurve;

        float mElipsedTime = 0.0f;

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
    public:
		USCRIPT_PROPERTY()
			float mUIScale = 1.0f;
		USCRIPT_PROPERTY()
			float mDistanceFactor = 0.2f;
		USCRIPT_PROPERTY()
			Vector2 mSpeechOffset;
        USCRIPT_PROPERTY()
            Vector2 mIconOffset;
		USCRIPT_PROPERTY()
			Vector2 mIconPadding;
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
