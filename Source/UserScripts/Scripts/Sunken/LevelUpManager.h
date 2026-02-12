#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>

namespace MMMEngine
{
    enum EXPTYPE : int {
        EXP_CASTLE,
        EXP_BUILD,
		EXP_SCOOPFIRST,
		EXP_SCOOPSECOND,
		EXP_CASTLEFIRST,
		EXP_CASTLESECOND,
        EXP_END
    };

    class Player;
    class CastleLevelController;
    class Castle;
    class LevelUpBubble;
    class USERSCRIPTS LevelUpManager : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        USCRIPT_PROPERTY()
            ObjPtr<Canvas> mCanvas;

        USCRIPT_PROPERTY()
            ObjPtr<Player> mPlayer;

        USCRIPT_PROPERTY()
            ObjPtr<Castle> mCastle;

		USCRIPT_PROPERTY()
			ObjPtr<LevelUpBubble> mLevelUpBubble;

        ObjPtr<GameObject> mPuller;

        EXPTYPE mPendingType = EXPTYPE::EXP_END;
        ObjPtr<GameObject> mLevelPendingObj;

        std::vector<int> mCastleExp = { 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100 };
        std::vector<int> mBuildingExp = { 150, 300, 450, 600, 750 };

        std::unordered_map<EXPTYPE, std::vector<std::wstring>> mHeadlineMap;
        std::unordered_map<EXPTYPE, std::vector<std::wstring>> mScriptMap;

        static ObjPtr<LevelUpManager> instance;

        int mCastleLevel = 0;
        int mScoopLevel = 0;

        int mCastleSpecial = 0;
        int mScoopSpecial = 0;

        USCRIPT_PROPERTY()
            int mScoopFirst = 5;
		USCRIPT_PROPERTY()
			int mScoopSecond = 10;
		USCRIPT_PROPERTY()
			int mCastleFirst = 5;
		USCRIPT_PROPERTY()
			int mCastleSecond = 10;

    public:
        // 풀링객체 (HP는 이제 안씀)
        USCRIPT_PROPERTY()
            ObjPtr<Gage> mExpGage;
        USCRIPT_PROPERTY()
            ObjPtr<Gage> mHpGage;
        USCRIPT_PROPERTY()
            ObjPtr<Image> mReadyIcon;

        // 캐슬관련
        USCRIPT_PROPERTY()
            ObjPtr<Image> mCastleIcon;
        USCRIPT_PROPERTY()
            ObjPtr<Image> mScoopIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mCastleRangeIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mCastleDoubleIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mCastleExpIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mCastleShieldIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mScoopRangeIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mScoopCritIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mScoopReflectIcon;

        // 빌딩관련
		USCRIPT_PROPERTY()
			ObjPtr<Image> mHPIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mBuffIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mDeBuffIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Image> mSnowIcon;

        // 버블관련
		USCRIPT_PROPERTY()
			ObjPtr<Image> mSpeechBubbleIcon;
		USCRIPT_PROPERTY()
			ObjPtr<Text> mHeadlineText;
		USCRIPT_PROPERTY()
			ObjPtr<Text> mScriptText;

        // 공용 프리팹
		USCRIPT_PROPERTY()
			ResPtr<Prefab> mReadyPrefab;
		USCRIPT_PROPERTY()
			ResPtr<Prefab> mCountPrefab;
		USCRIPT_PROPERTY()
            ResPtr<Prefab> mHPGagePrefab;
		USCRIPT_PROPERTY()
			ResPtr<Prefab> mUnivGagePrefab;

        bool ValidateReferences();
    public:
        LevelUpManager()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
            void Awake();

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        Vector2 GetCanvasPos(const Vector3& _worldPos);

        ObjPtr<Canvas> GetCanvas() { return mCanvas; }

        ObjPtr<Castle> GetCastle();

        void AddCastleLevel(int _val = 1);

        void AddScoopLevel(int _val = 1);

        int GetExpPoint(EXPTYPE _type, int _level);

        int GetMaxLevel(EXPTYPE _type);

        void SetUIPuller(ObjPtr<GameObject> _puller) { mPuller = _puller; }
        void RemoveUIPuller();
        ObjPtr<GameObject> GetUIPuller();

        void SetBubble(EXPTYPE _type, ObjPtr<GameObject> _target, std::vector<ObjPtr<Image>>& _icons);
        void RemoveBubble();
        void SetSelection(int _selectionIdx);
        ObjPtr<GameObject> GetBubbleTarget();
        bool IsBubbling();

        const std::wstring GetHeadline(int _idx);
        const std::wstring GetScripts(int _idx);


        static const ObjPtr<LevelUpManager>& Get() { return instance; }

    public:
        template <typename T>
        bool CheckValid(const ObjPtr<T>& obj, const std::string& name)
        {
            if (!obj.IsValid()) {
                std::cerr << "LevelManager::" << name << " Not Found!!" << std::endl;
                return false;
            }
            return true;
        }
    };
}
