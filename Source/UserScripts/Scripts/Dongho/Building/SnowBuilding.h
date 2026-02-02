#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class USERSCRIPTS SnowBuilding : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        SnowBuilding()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Start();

        void Update();
        void UpdateSnow();
        void LevelApply(int level);
        int snowpoint = 0;
        int maxsnowpoint = 15;
        float chargeDelay = 2.0f;
        float chargeTimer = 0.0f;
    };
}
