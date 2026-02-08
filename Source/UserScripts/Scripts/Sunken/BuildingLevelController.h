#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>
#include "../Dongho/Building/Building.h"

namespace MMMEngine
{
    class Battlestats;
    class USERSCRIPTS BuildingLevelController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            USCRIPT_PROPERTY()
            ObjPtr<Building> mBuilding;
        USCRIPT_PROPERTY()
            ObjPtr<Battlestats> mBattleStat;


        USCRIPT_PROPERTY()
            ObjPtr<Canvas> mCanvas;
        USCRIPT_PROPERTY()
            ObjPtr<Gage> mExpGage;
        USCRIPT_PROPERTY()
            ObjPtr<Gage> mHpGage;

        int mCurrLevel = 0;
        int mReqExp = 0;
        bool isActive = false;

        void UpdateGuage();
        void LevelUp();
        void SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding);
    public:
        USCRIPT_PROPERTY()
            Vector2 mGagePosOffset;
        USCRIPT_PROPERTY()
            float mUIScale = 1.0f;
        USCRIPT_PROPERTY()
            Vector2 mPadding = { 0.0f, 10.0f };
        USCRIPT_PROPERTY()
            float mDistanceFactor = 1.0f;
    
    public:
        BuildingLevelController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerEnter);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerExit);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

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
