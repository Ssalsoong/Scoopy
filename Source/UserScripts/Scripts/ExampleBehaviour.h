#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <string>

namespace MMMEngine
{
    class USERSCRIPTS ExampleBehaviour : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ExampleBehaviour()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        bool isCustomBool = true;
        float customFloat = 14.0f;
        int customInt = 2.0f;
        std::string customString = "Hello, World!";

        bool isDontShowBool = false;

        void Start();
        void Update();
    };
}
