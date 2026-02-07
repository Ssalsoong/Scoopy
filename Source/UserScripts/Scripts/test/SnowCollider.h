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
    public:
        SnowCollider()
        {
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        USCRIPT_MESSAGE()
        void Start();

        
        USCRIPT_MESSAGE()
        void FixedUpdate();
        bool On_Player = false;
        bool GetOnPlayer() const{ return On_Player; }
        ObjPtr<GameObject> m_player;
        DirectX::SimpleMath::Vector3 curPos{};
        float m_Size = 0.0f;
        void SetSize(float size);
        float GetSize() {return m_Size;}

        USCRIPT_PROPERTY()
        float m_Rolesmooth = 5.0f;
            
        bool m_hasPrevPlayerPos = false;

        void SetOnPlayer(bool value , ObjPtr<GameObject> player);

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
    };
}
