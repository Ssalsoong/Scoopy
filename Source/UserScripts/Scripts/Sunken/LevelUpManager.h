#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>

namespace MMMEngine
{
    enum EXPTYPE : int {
        EXP_CASTLE,
        EXP_END
    };

    class Player;
    class CastleLevelController;
    class USERSCRIPTS LevelUpManager : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        USCRIPT_PROPERTY()
            ObjPtr<Canvas> mCanvas;
        USCRIPT_PROPERTY()
            ObjPtr<CastleLevelController> mCastleController;

        std::vector<int> mCastleExp = { 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 };

        static ObjPtr<LevelUpManager> instance;
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

        int GetExpPoint(EXPTYPE _type, int _level);

        static const ObjPtr<LevelUpManager>& Get() { return instance; }
    };
}
