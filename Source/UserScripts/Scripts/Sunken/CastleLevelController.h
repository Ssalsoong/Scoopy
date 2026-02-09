#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>

namespace MMMEngine
{
    class Castle;
    class Gage;
    class USERSCRIPTS CastleLevelController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        bool isActive = false;

        ObjPtr<Castle> mCastle;
        ObjPtr<Canvas> mCanvas;
        ObjPtr<Gage> mExpGage;
        ObjPtr<Gage> mHpGage;
        ObjPtr<Image> mReadyIcon;

        ObjPtr<Image> mCastleIcon;
        ObjPtr<Image> mScoopIcon;
        ObjPtr<Image> mCountIcon;

        int mCurrLevel = 0;
        int mReqExp = 0;
        int mUpPending = 0;

        int mSelectIndex = 0;
        
        void UpLevel();
        void SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding);
        void SetLVManager(int _upIndex);

        void UpdateGuage();
        void UpdateReadyIcon();
        void UpdateSelectIcon();
    public:
        CastleLevelController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerEnter);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerExit);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);
        }

        USCRIPT_PROPERTY()
            Vector2 mGagePosOffset;
        USCRIPT_PROPERTY()
            Vector2 mReadyPosOffset;
        USCRIPT_PROPERTY()
            Vector2 mSelectPosOffset;
        USCRIPT_PROPERTY()
            Vector2 mCountPosOffset;
        USCRIPT_PROPERTY()
            float mUIScale = 1.0f;
        USCRIPT_PROPERTY()
            Vector2 mPadding = { 0.0f, 10.0f };
        USCRIPT_PROPERTY()
            Vector2 mSelectPadding = { 10.0f, 0.0f };
        USCRIPT_PROPERTY()
            float mDistanceFactor = 1.0f;

    public:
        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
            void OnTriggerEnter(MMMEngine::CollisionInfo info);

        USCRIPT_MESSAGE()
            void OnTriggerExit(MMMEngine::CollisionInfo info);
    };
}
