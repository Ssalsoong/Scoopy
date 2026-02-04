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
			Dead
		};
		EnemyState state = EnemyState::GoToCastle;
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
		int HP = 0;
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

		void GetDamage(int t) { HP -= t; HP = std::max(HP, 0); };
		void PlayerHitMe();
		bool MoveToTarget(const DirectX::SimpleMath::Vector3& target, float stopDist);
		bool CheckPlayer();
		bool LostPlayer();
		void ChangeState(EnemyState next);
		void LookAt(const DirectX::SimpleMath::Vector3& target);
		void GoToCastle();
		void AttackCastle();
		void ChasePlayer();
		void AttackPlayer();
		void GoToBuilding();
		void AttackBuilding();
		bool FindNearBuilding();
		void Dead();
	};
}

