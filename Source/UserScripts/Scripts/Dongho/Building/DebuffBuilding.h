#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class Enemy;
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
        float debuff = 0.8f;
        float debuffdist = 3.0f;
        std::unordered_set<ObjPtr<Enemy>> m_inside;
        void Start();
        void Update();
        void GiveDebuff();
        void LevelApply(int level);
    };
}
