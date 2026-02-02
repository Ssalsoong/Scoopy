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
		void Start();
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
		ObjPtr<Player> playercomp;
		ObjPtr<Transform> playertr;
		DirectX::SimpleMath::Vector3 playerpos;
		ObjPtr<GameObject> castle;
		ObjPtr<Castle> castlecomp;
		ObjPtr<Transform> castletr;
		DirectX::SimpleMath::Vector3 castlepos;
		struct EnemyStats
		{
			int HP = 0;
			int atk = 0;
			float velocity = 0.f;
			float attackDelay = 0.f;
			float battledist = 0.f;
			float checkdist = 0.f;
		};
		EnemyStats stats;
		float playerLostdist = 50.0f;
		void GetDamage(int t) { stats.HP -= t; stats.HP = std::max(stats.HP, 0); };
		void PlayerHitMe();
		int GetHP() { return stats.HP; }
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

