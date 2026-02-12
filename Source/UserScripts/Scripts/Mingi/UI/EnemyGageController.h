#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class Battlestats;
    class Gage;
    class USERSCRIPTS EnemyGageController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        EnemyGageController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
        REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
            ObjPtr<Battlestats> battlestats;

        ObjPtr<Gage> hpUI;

        enum class EnemyType { Warrior, Ranger, Assassin };

        EnemyType type;

        USCRIPT_MESSAGE()
        void OnEnable();

        USCRIPT_MESSAGE()
        void OnDisable();

        USCRIPT_MESSAGE()
        void Update();
    };
}
