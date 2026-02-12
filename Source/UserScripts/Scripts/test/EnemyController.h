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
	class RigidBodyComponent;

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

		USCRIPT_PROPERTY()
			ObjPtr<GameObject> m_SensorObj;

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

		void InitEnemy(EnemyType type, DirectX::SimpleMath::Vector3 pos, int hp);

		//내부로직에의해 스테이트변경
		void ChangeState();
		//현재 스테이트에 따라 행동할것들
		void CheckState();

		//거리계산
		void UpdateDistance();

		void OnStateEnter(EnemyState state);

		bool UpdateTarget();

		bool IsBruiser();

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
	private:
		void TryAcquireSlot();
		void ReleaseSlot();

		float slotArriveRadius = 0.2f;
		bool m_usingSlotTarget = false;

		float m_rangedAngle = 1.0f;
		float m_rangedHoldRadius = 3.0f;

		ObjPtr<GameObject> ResolveTarget(ObjPtr<GameObject> raw);

		ObjPtr<RigidBodyComponent> m_Rigid;

		//호 이동용 변수
		bool m_orbiting = false;
		int  m_orbitDir = 1;        // +1 또는 -1
		float orbitEnterAngle = 0.6f; // 라디안 (약 57도)
		float orbitExitAngle = 0.25f; // 라디안 (약 14도)
		float orbitStepDist = 0.4f;  // 한 프레임 이동 거리

		//호 이동 시작과 끝을 위한 변수
		float orbitRadialTolerance = 0.4f;
		float orbitStartDist = 1.7f;
		float orbitExitDist = 0.2f;
		float orbitLaneOffset = 0.25f;


		//티켓만료변수
		float m_slotElapsed = 0.0f;
		float m_slotTimeout = 10.0f;     // 10초
		float m_slotReassignCooldown = 0.5f; // 재시도 쿨다운
		float m_slotReassignTimer = 0.0f;

		bool m_canExitAttack = false;
		//디버프 관련
	public:
		std::unordered_map<const void*, float> m_AttackDebuffSources;
		float m_FinalAttackMult = 1.0f;

		void AddAttackDebuffSource(const void* src, float mult);
		void RemoveAttackDebuffSource(const void* src);
		void UpdateAttackDebuffSource(const void* src, float mult);

	private:
		void RecalcAttackMult();

	};
}