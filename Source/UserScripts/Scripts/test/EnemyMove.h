#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include <SimpleMath.h>
#include "UserScriptsCommon.h"


namespace MMMEngine
{
    class USERSCRIPTS EnemyMove : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        EnemyMove()
        {
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        USCRIPT_MESSAGE()
        void Start();
        USCRIPT_MESSAGE()
        void FixedUpdate();

        USCRIPT_PROPERTY()
        float movespeed = 1.0f;

        USCRIPT_PROPERTY()
        DirectX::SimpleMath::Vector3 Target{};

        USCRIPT_PROPERTY()
        ObjPtr<GameObject> Obj_target;

        DirectX::SimpleMath::Vector3 myPos{};

        
        ObjPtr<GameObject> m_GO;


        DirectX::SimpleMath::Vector3 ComputeChaseVelocity();



        //새로운 로직 변수
        DirectX::SimpleMath::Vector3 pendingDir = { 0,0,0 };
        bool hasPendingDir = false;

        // “정면 막힘일 때 어느 쪽으로 돌아갈지” 고정용

        void FaceVelocityYaw(const DirectX::SimpleMath::Vector3& vel);

        DirectX::SimpleMath::Vector3 curVel = { 0,0,0 };


        //trigger 변수
        DirectX::SimpleMath::Vector3 avoidDir = { 0,0,0 };   // 회피 방향 누적/저장

        std::vector<ObjPtr<GameObject>> SnowObjPtr;

        // tuning
        float avoidRadius = 2.0f; // 트리거 반경 근사값(트리거 크기에 맞춰 튜닝)
        float maxAccel = 20.f;    // 가속 제한(반응성)
        float yawLerpA = 0.2f;    // 회전 관성 계수 (고정)
        
        
        void AddObjPtr(ObjPtr<GameObject> snow);

        void Remove(ObjPtr<GameObject> snow);


        USCRIPT_PROPERTY()
        float sweepRadius = 1.5f;      // 적 콜라이더 반경(대략)
        USCRIPT_PROPERTY()
        float sweepAhead = 0.8f;       // 전방 감지 거리(속도 빠르면 더 키움)
        USCRIPT_PROPERTY()
        float avoidGain = 1.0f;        // 회피 강도(슬라이드+탄젠트면 1.0 근처면 충분)

        int avoidSide = +1;
        bool hasAvoidSide = false;

        void ChangeTarget(ObjPtr<GameObject> target);

        void MoveTriggerSet(bool value);
        bool is_move = true;
    };
}
