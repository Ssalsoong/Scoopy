#pragma once
#include "ScriptBehaviour.h"
#include "rttr/type"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"
#include "SimpleMath.h"

namespace MMMEngine {
	class USERSCRIPTS Castle : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
	public:
		Castle()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		DirectX::SimpleMath::Vector3 pos;
		int HP = 300;
		int maxHP = 300;
		int healHP = 10;
		float healDelay = 1.0f;
		float NonfightDelay = 5.0f;
		float attackDelay = 1.0f;
		int exp = 0;
		int atk = 10;
		int point = 0;
		int prevHP = 300;
		bool fighting = false;
		float healTimer = 0.0f;
		float NonfightTimer = 0.0f;
		float attackTimer = 0.0f;
		float attackdist = 10.0f;
		std::queue<ObjPtr<GameObject>> Castleballs;
		ResPtr<StaticMesh> castleballmesh;
		ObjPtr<GameObject> enemyTarget;
		void Start();
		void Update();
		void PointUp(int t);
		void GetDamage(int t) { HP -= t; HP = std::max(HP, 0); };
		void CheckEnemy();
		void AutoAttack();
		void AutoHeal();
		void ReturnBall(ObjPtr<GameObject> obj);
		bool CastleDeath() const { return HP <= 0; }
	};
}