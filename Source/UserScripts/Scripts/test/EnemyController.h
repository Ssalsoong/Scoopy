#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <SimpleMath.h>

namespace MMMEngine
{
    class EnemyMove;
    class EnemySensor;
    class TargetSlotProvider;

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

		// 슬롯 상태
		ObjPtr<TargetSlotProvider> m_SlotProvider;
		ObjPtr<GameObject> m_SlotTarget;
		bool m_hasSlot = false;
		int m_slotRing = -1;
		int m_slotIndex = -1;
		DirectX::SimpleMath::Vector3 m_effectiveTargetPos{};

		// 슬롯 재요청
		int m_slotRetryFrames = 0;
		int m_slotRetryInterval = 10; // 10프레임마다 재요청

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

        void HurtTimerOn() { HurtTimer = HurtDelay; }
        void HurtCal();
        float HurtTimer = 0.0f;
        float HurtDelay = 2.0f;

		enum class AttackPhase { Motion, Pause };
		AttackPhase m_attackPhase = AttackPhase::Motion;

        float RecoverDelay = 1.0f;
        float RecoverTimer = 0.0f;
        void DoHit();
        void MotionEnter();
        void PauseEnter();
        float debuff = 1.0f;
        void SetDebuffAttack(float value) { debuff = value; }
    private:
		void TryAcquireSlot();
	    void ReleaseSlot();

		float slotArriveRadius = 0.2f;
		bool m_usingSlotTarget = false;
    };
}
