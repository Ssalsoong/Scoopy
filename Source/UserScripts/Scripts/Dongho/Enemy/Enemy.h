#pragma once
#include "ScriptBehaviour.h"
#include <SimpleMath.h>
#include "rttr/type"
#include "UserScriptsCommon.h"

namespace MMMEngine {
	class Transform;
	class Player;
	class Castle;
	class USERSCRIPTS Enemy : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
	public:
		Enemy()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		USCRIPT_MESSAGE()
		void Start();
		USCRIPT_MESSAGE()
		void Update();
		enum class EnemyState
		{
			GoToBuilding,
			AttackBuilding,
			GoToCastle,
			AttackCastle,
			ChasePlayer,
			AttackPlayer,
			Dead,
			None
		};
		EnemyState state = EnemyState::GoToCastle;
		EnemyState pendingState = EnemyState::None;
		bool stateLocked = false;
		ObjPtr<GameObject> buildingTarget = nullptr;
		DirectX::SimpleMath::Vector3 buildingpos;
		bool HitByPlayer = false;
		float attackTimer = 0.0f;
		ObjPtr<Transform> tr;
		DirectX::SimpleMath::Vector3 pos;
		ObjPtr<GameObject> player;
		ObjPtr<Transform> playertr;
		DirectX::SimpleMath::Vector3 playerpos;
		ObjPtr<GameObject> castle;
		ObjPtr<Transform> castletr;
		DirectX::SimpleMath::Vector3 castlepos;
		USCRIPT_PROPERTY()
		int atk = 0;
		USCRIPT_PROPERTY()
		float velocity = 0.f;
		USCRIPT_PROPERTY()
		float attackDelay = 0.f;
		USCRIPT_PROPERTY()
		float battledist = 0.f;
		USCRIPT_PROPERTY()
		float checkdist = 0.f;

		void PlayerHitMe();
		bool MoveToTarget(const DirectX::SimpleMath::Vector3& target, float stopDist);
		bool CheckPlayer();
		bool LostPlayer();
		void ChangeState(EnemyState next);
		void RequestState(EnemyState next);
		void FinishAttackCycle();
		void LookAt(const DirectX::SimpleMath::Vector3& target);
		void GoToCastle();
		void AttackCastle();
		void ChasePlayer();
		void AttackPlayer();
		void GoToBuilding();
		void AttackBuilding();
		bool FindNearBuilding();
		void Dead();

		float snowDamageTimer = 0.0f;
		float snowDamageDelay = 0.5f;
		void CalSnowDamageDelay();
		bool ApplySnowDamage();
	};
}

