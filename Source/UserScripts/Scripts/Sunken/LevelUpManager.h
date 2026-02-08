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
        EXP_END
    };

    class Player;
    class CastleLevelController;
    class Castle;
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

        std::vector<int> mCastleExp = { 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 };
        std::vector<int> mBuildingExp = { 0, 15, 30, 45, 60, 75 };

        static ObjPtr<LevelUpManager> instance;

        int mCastleLevel = 0;
        int mScoopLevel = 0;

    public:
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
