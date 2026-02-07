#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class Transform;
    class USERSCRIPTS CameraMove : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
        DirectX::SimpleMath::Vector3 m_currentPos;
    public:
        CameraMove()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
        DirectX::SimpleMath::Vector3 Offset;

        USCRIPT_PROPERTY()
        ObjPtr<Transform> Target;

        USCRIPT_PROPERTY()
        float ChasingSpeed = 12.0f;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();
    };
}
