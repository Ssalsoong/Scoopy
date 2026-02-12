#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class Text;
    class WorldSpaceUI;
    class USERSCRIPTS MarkUIInfo : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        MarkUIInfo()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
            ObjPtr<WorldSpaceUI> worldSpaceUI;

        USCRIPT_PROPERTY()
            ObjPtr<Text> countText;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();
    };
}
