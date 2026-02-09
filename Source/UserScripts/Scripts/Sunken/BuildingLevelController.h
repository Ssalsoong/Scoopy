#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>
#include "../Dongho/Building/Building.h"

namespace MMMEngine
{
    class Battlestats;
    class BuildingManager;
    class USERSCRIPTS BuildingLevelController : public ScriptBehaviour
    {
    private:
		enum BUILDTYPE : int {
            NORMAL = 0,
            HEALTH,
            BUFF,
            DEBUFF,
            SNOW,
            END
		};

    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        USCRIPT_PROPERTY()
            ObjPtr<Building> mBuilding;
        USCRIPT_PROPERTY()
            ObjPtr<GameObject> mPlayer;
        USCRIPT_PROPERTY()
            ObjPtr<Battlestats> mBattleStat;

        BUILDTYPE mType = BUILDTYPE::END;

            ObjPtr<Canvas> mCanvas;
            ObjPtr<Gage> mExpGage;
            ObjPtr<Gage> mHpGage;
			ObjPtr<Image> mReadyIcon;
			ObjPtr<Image> mHPIcon;
			ObjPtr<Image> mBuffIcon;
			ObjPtr<Image> mDeBuffIcon;
			ObjPtr<Image> mSnowIcon;

        int mCurrLevel = 0;
        int mReqExp = 0;
        int mSelectIndex = 0;
        int mUpPending = 0;
        bool isActive = false;
        bool isReady = false;

        void UpdateGuage();
        void LevelUp();
        void UpdateReadyIcon();
        void UpdateSelectIcon();
        void SetUITrans(ObjPtr<RectTransform> _rectTrans, Vector2& _offset, Vector2& _mPadding);
        void ControlSelection();
        void SetLVManager(int _idx);

        void SetActiveIcon();
    public:
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
        void SetLevelSelection(int _idx);

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
