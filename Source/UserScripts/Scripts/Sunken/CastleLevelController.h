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

        USCRIPT_PROPERTY()
        ObjPtr<Castle> mCastle;

        USCRIPT_PROPERTY()
        ObjPtr<Canvas> mCanvas;
        USCRIPT_PROPERTY()
        ObjPtr<Gage> mExpGage;
        USCRIPT_PROPERTY()
        ObjPtr<Gage> mHpGage;
        USCRIPT_PROPERTY()
        ObjPtr<Image> mReadyIcon;

        USCRIPT_PROPERTY()
            ObjPtr<Image> mCastleIcon;
        USCRIPT_PROPERTY()
            ObjPtr<Image> mScoopIcon;

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
