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
        bool isUpgrade = false;

        USCRIPT_PROPERTY()
        ObjPtr<Castle> mCastle;
        USCRIPT_PROPERTY()
        ObjPtr<Gage> mExpGage;
        USCRIPT_PROPERTY()
        ObjPtr<Gage> mHpGage;
        USCRIPT_PROPERTY()
        ObjPtr<Canvas> mCanvas;

        int mCurrLevel = 0;
        int mReqExp = 0;

        void UpLevel();
        void UpdateGuage();
    public:
        CastleLevelController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerEnter);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerExit);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
            Vector2 mUIPosOffset;
        USCRIPT_PROPERTY()
            float mUIScale = 1.0f;
        USCRIPT_PROPERTY()
            float mPadding = 10.0f;
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
