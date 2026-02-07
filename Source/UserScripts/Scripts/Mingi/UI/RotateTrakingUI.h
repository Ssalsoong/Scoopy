#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "Canvas.h"

namespace MMMEngine
{
    class WorldSpaceUI;
    class USERSCRIPTS RotateTrakingUI : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            ObjPtr<Canvas> m_canvas;
    public:
        RotateTrakingUI()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_PROPERTY()
        ObjPtr<WorldSpaceUI> WorldSpaceUI;
    };
}
