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
    class PlayerAnimController;
    class USERSCRIPTS PlayerMove : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            USCRIPT_PROPERTY()
            ObjPtr<PlayerAnimController> mPAController;
    public:
        PlayerMove()
        {
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        float basespeed = 80.0f;

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


        //수치조절
        USCRIPT_PROPERTY()
        float turnSpeed = 25.0f;

        USCRIPT_PROPERTY()
        bool is_Scoop = false;
        ObjPtr<GameObject> Snow;

        
        bool m_LookTarget = false;

        USCRIPT_MESSAGE()
        void FixedUpdate();


        DirectX::SimpleMath::Vector3 ComputeDesiredVelocity();
        void ApplyYawFromVelocity(const DirectX::SimpleMath::Vector3& v);

        float WrapPi(float a);

        float curYaw = 0.0f;

        float dt = 0.0f;

        bool test = false;

        ObjPtr<TileMap> T;

        void SetScoopMode(bool value, ObjPtr<GameObject> target);

        void SetInputDir(DirectX::SimpleMath::Vector3 vec);

        DirectX::SimpleMath::Vector3 m_InputDir;            // Update에서 저장 (정규화 전/후 상관없음)


        Vector3 m_lockForward = Vector3::Forward;
        Vector3 m_lockRight = Vector3::Right;
        bool    m_hasScoopBasis = false;

        bool makeDirty = false;
        float m_PendingYaw = 0.f;

        ObjPtr<RigidBodyComponent> m_rigid;

        float ComputeSpeed();

        USCRIPT_PROPERTY()
        float DefaultSpeed = 80.f;
        USCRIPT_PROPERTY()
        float OnSnowSpeed = 60.f;

        USCRIPT_PROPERTY()
        float MinusSpeed = 1.5f;

        USCRIPT_PROPERTY()
        float MinSpeed = 35.f;

    };
}
