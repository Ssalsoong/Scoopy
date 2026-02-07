#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <CoreComponents.h>

namespace MMMEngine
{
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

        static const ObjPtr<LevelUpManager>& Get() { return instance; }
    };
}
