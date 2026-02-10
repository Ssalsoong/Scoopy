#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class RedLineSin;
    class Transform;
    class LineRenderer;
    class USERSCRIPTS RedLine : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            float m_internalTimer = 0.0f;
        DirectX::SimpleMath::Color m_baseColor;
    public:
        RedLine()
        {
        SetExecutionOrder(20);
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
        bool CheckZPos = false;

        USCRIPT_PROPERTY()
        bool CheckXPos = true;

        USCRIPT_PROPERTY()
        ObjPtr<Transform> PlayerTr;

        USCRIPT_PROPERTY()
        ObjPtr<LineRenderer> ReddoLine;
    };
}
