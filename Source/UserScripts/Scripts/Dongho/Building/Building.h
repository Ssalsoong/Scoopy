#pragma once
#include "ScriptBehaviour.h"
#include "rttr/type"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"
#include "SimpleMath.h"

namespace MMMEngine {
	class USERSCRIPTS Building : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
	public:
		Building()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		DirectX::SimpleMath::Vector3 pos;
		void Start();
		void Update();
		int maxHP = 50;
		int HP = 50;
		int point = 0;
		int exp = 0;
		int atk = 10;
		int level = 0;
		float attackDelay = 1.0f;
		float attackTimer = 0.0f;
		float attackdist = 10.0f;
		bool isDead = false;
		std::queue<ObjPtr<GameObject>> Buildingballs;
		ResPtr<StaticMesh> buildingballmesh;
		ObjPtr<GameObject> enemyTarget;
		void GetDamage(int t) { HP -= t; HP = std::max(HP, 0); };
		void PointUp(int t);
		void CheckDead();
		void CheckEnemy();
		void AutoAttack();
		void ReturnBall(ObjPtr<GameObject> obj);
	};
}
