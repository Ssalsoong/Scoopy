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
        SetExecutionOrder(100);
        REGISTER_BEHAVIOUR_MESSAGE(LateUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void LateUpdate();

        USCRIPT_PROPERTY()
        ObjPtr<WorldSpaceUI> WorldSpaceUI;
    };
}
