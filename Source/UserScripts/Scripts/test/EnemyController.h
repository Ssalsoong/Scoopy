#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <SimpleMath.h>

namespace MMMEngine
{
    class EnemyMove;
    class EnemySensor;

    class USERSCRIPTS EnemyController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
        bool EnemyHurt = false;

        struct Stat
        {
            int AD;
            float AS;
            float Range;
        };

        float distance = 0.f;
    public:
        enum class EnemyState
        {
            Move,
            Attack,
            Dead
        };

        enum class EnemyType
        {
            Warrior,
            Scout,
            Archer
        };

        Stat E_state;

        EnemyState curState = EnemyState::Move;
        EnemyState prevState = EnemyState::Dead;

        EnemyType m_EnemyType = EnemyType::Warrior;

        ObjPtr<EnemySensor> m_Sensor;
        ObjPtr<EnemyMove> m_Move;
        
        USCRIPT_PROPERTY()
        ObjPtr<GameObject> m_MainTarget;
        
        ObjPtr<GameObject> m_CurTarget;

        DirectX::SimpleMath::Vector3 toTarget{};

    public:
        EnemyController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        bool CheckHurt();

        void OnHurtFlag(bool value);

        void InitEnemy(EnemyType type , DirectX::SimpleMath::Vector3 pos, int hp);
        
        //내부로직에의해 스테이트변경
        void ChangeState();
        //현재 스테이트에 따라 행동할것들
        void CheckState();

        //거리계산
        void UpdateDistance();

        void OnStateEnter(EnemyState state);

        bool UpdateTarget();

		USCRIPT_PROPERTY()
		ObjPtr<GameObject> m_SensorObj;
        

        EnemyType GetType() { return m_EnemyType; }


        void AttackTarget();
        ObjPtr<GameObject> battletarget;

        float attackTimer = 0.0f;
		float snowDamageTimer = 0.0f;
		float snowDamageDelay = 0.5f;
		void CalSnowDamageDelay();
		bool ApplySnowDamage();

        void SetType(EnemyType type) { m_EnemyType = type; }
    };
}
