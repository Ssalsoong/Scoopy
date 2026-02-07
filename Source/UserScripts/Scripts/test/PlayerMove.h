#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <string>
#include "PhysxManager.h"
#include <SimpleMath.h>
#include <MMMTime.h>

using namespace DirectX::SimpleMath;

namespace MMMEngine
{
    class TileMap;

    class USERSCRIPTS PlayerMove : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        PlayerMove()
        {
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
        

        USCRIPT_PROPERTY()
        float basespeed = 7.0f;

        USCRIPT_PROPERTY()
        bool isSlow = false;
        float yawRadians = 0.0f;

        float maxYawSpeed = 0.0f;

        //PID제어
        float kp = 0.0f;

        //라디안
        float alignEps;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        //수치조절
        USCRIPT_PROPERTY()
        float turnSpeed = 25.0f;

        USCRIPT_PROPERTY()
        bool is_Scoop = false;
        ObjPtr<GameObject> Snow;

        Vector3 m_InputDir;            // Update에서 저장 (정규화 전/후 상관없음)
        bool m_LookTarget = false;

        USCRIPT_MESSAGE()
        void FixedUpdate();


        Vector3 ComputeDesiredVelocity();
        void ApplyYawFromVelocity(const Vector3& v);

        float WrapPi(float a);

        float curYaw = 0.0f;
        //Vector3 forward{};
        float dt = 0.0f;

        bool test = false;

        ObjPtr<TileMap> T;

        void SetScoopMode(bool value, ObjPtr<GameObject> target);

    };
}
