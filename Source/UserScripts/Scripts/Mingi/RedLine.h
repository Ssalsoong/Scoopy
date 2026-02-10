#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class Transform;
    class LineRenderer;
    class USERSCRIPTS RedLine : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            float m_internalTimer = 0.0f;
    public:
        RedLine()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();


        USCRIPT_PROPERTY()
        float CheckDistance = 0.0f;

        USCRIPT_PROPERTY()
        bool CheckYPos = false;

        USCRIPT_PROPERTY()
        bool CheckXPos = true;

        USCRIPT_PROPERTY()
        ObjPtr<Transform> PlayerTr;


        USCRIPT_PROPERTY()
        ObjPtr<LineRenderer> ReddoLine;
    };
}
