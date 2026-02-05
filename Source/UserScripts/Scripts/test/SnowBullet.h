#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <SimpleMath.h>
#include "PhysxManager.h"

namespace MMMEngine
{
    class USERSCRIPTS SnowBullet : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        SnowBullet()
        {
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerEnter);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        USCRIPT_MESSAGE()
        void Start();

        
        DirectX::SimpleMath::Vector3 Bezier2(const DirectX::SimpleMath::Vector3& p0, const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2, float t);
        DirectX::SimpleMath::Vector3 SnowBullet::Bezier2Deriv(
            const DirectX::SimpleMath::Vector3& p0,
            const DirectX::SimpleMath::Vector3& p1,
            const DirectX::SimpleMath::Vector3& p2,
            float t);

        USCRIPT_MESSAGE()
        void FixedUpdate();


        //시작위치
        DirectX::SimpleMath::Vector3 startPos;
        float flightTime = 0.8f;   // 전체 비행 시간(튜닝)
        float peakHeight = 4.f;    // 포물선 높이(튜닝)
        float maxSpeed = 30.f;     // 속도 상한(필수에 가까움)

        USCRIPT_MESSAGE()
        void OnTriggerEnter(MMMEngine::CollisionInfo info);

        
        USCRIPT_PROPERTY()
        float speed = 5.0f;


        USCRIPT_PROPERTY()
        ObjPtr<GameObject> target;

        float m_t = 0.f;

        void ReSet();

        DirectX::SimpleMath::Vector3 F_Pos;

        void SetSnowTarget(ObjPtr<GameObject> Enemy);
    };
}
