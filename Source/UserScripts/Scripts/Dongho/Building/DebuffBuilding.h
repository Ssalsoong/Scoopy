#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class USERSCRIPTS DebuffBuilding : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        DebuffBuilding()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
        float speedDebuff = 0.8f;
        float attackDebuff = 1.1f;
        float debuffdist = 2.0f;
        std::unordered_set<ObjPtr<GameObject>> m_inside;
        void Start();
        void Update();
        void GiveDebuff();
        void LevelApply(int level);
    };
}
