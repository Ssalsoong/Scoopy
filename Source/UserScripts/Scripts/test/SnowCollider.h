#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <SimpleMath.h>

namespace MMMEngine
{
    class USERSCRIPTS SnowCollider : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND
            DirectX::SimpleMath::Vector3 m_lastPos;
            DirectX::SimpleMath::Vector3 m_currPos;
            DirectX::SimpleMath::Quaternion m_lastRot;
            DirectX::SimpleMath::Quaternion m_currRot;

    public:


        SnowCollider()
        {
        SetExecutionOrder(5);
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();


        USCRIPT_MESSAGE()
        void Update();
        
        USCRIPT_MESSAGE()
        void FixedUpdate();

        bool On_Player = false;


        //snow가 플레이어에 붙어있는지 확인하기 위해 get하는 함수
        bool CheckOnPlayer();

        ObjPtr<GameObject> m_player;
        DirectX::SimpleMath::Vector3 curPos{};

        float m_Size = 0.0f;
        void SetSize(float size);

        USCRIPT_PROPERTY()
        float m_Rolesmooth = 5.0f;
            
        bool m_hasPrevPlayerPos = false;

        void SetOnPlayer(bool value , ObjPtr<GameObject> player);

        USCRIPT_PROPERTY()
        ObjPtr<GameObject> TriggerCollider;

        USCRIPT_PROPERTY()
        ObjPtr<GameObject> SnowManager;

        DirectX::SimpleMath::Vector3 m_prevPlayerPos;
        DirectX::SimpleMath::Quaternion m_prevPlayerRot;
        DirectX::SimpleMath::Vector3 m_localOffset;
        DirectX::SimpleMath::Quaternion m_localRotOffset;

        DirectX::SimpleMath::Quaternion ComputeRollingRotation(DirectX::SimpleMath::Vector3& delta, float dt) const;

        USCRIPT_PROPERTY()
        float m_holdDistance = 0.5f;

        float m_holdHeight = 0.0f;

        void SnowDestory();


        int scoopCount = 0;

        void SetScoopCount(int count);
        int GetScoopCount();
        void AddScoop(int delta);

        DirectX::SimpleMath::Vector3 m_prevHoldPos{};
        bool m_hasPrevHoldPos = false;
        DirectX::SimpleMath::Quaternion m_rollRot = DirectX::SimpleMath::Quaternion::Identity;

        USCRIPT_PROPERTY()
        float m_rollSpeed = 1.0f;

        DirectX::SimpleMath::Quaternion m_baseRot = DirectX::SimpleMath::Quaternion::Identity;


        float m_rollAngle = 0.f;

        DirectX::SimpleMath::Vector3 m_rollAxis = DirectX::SimpleMath::Vector3::Right;
        bool m_hasRollAxis = false;

        int MaxSnowCount = 10;

        void MaxSnow(int count);

    };
}
