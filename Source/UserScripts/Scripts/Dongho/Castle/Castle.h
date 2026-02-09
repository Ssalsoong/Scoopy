#pragma once
#include "ScriptBehaviour.h"
#include "rttr/type"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"
#include "SimpleMath.h"
#include "Prefab.h"

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
		USCRIPT_MESSAGE()
		void Start();
		USCRIPT_MESSAGE()
		void Update();
		DirectX::SimpleMath::Vector3 pos;
		USCRIPT_PROPERTY()
		int level = 0;
		USCRIPT_PROPERTY()
		int maxHP = 300;
		int GetmaxHP() const { return maxHP; }
		int healHP = 10;
		float healDelay = 1.0f;
		float NonfightDelay = 5.0f;
		float attackDelay = 1.0f;
		USCRIPT_PROPERTY()
		int exp = 0;
		USCRIPT_PROPERTY()
		int atk = 10;
		USCRIPT_PROPERTY()
		int point = 0;
		USCRIPT_PROPERTY()
		ResPtr<Prefab> pre_bullet;
		USCRIPT_PROPERTY()
		float bulletSpeed = 6.0f;
		float bulletsize = 0.2f;
		int prevHP = 300;
		bool fighting = false;
		float healTimer = 0.0f;
		float NonfightTimer = 0.0f;
		float attackTimer = 0.0f;
		float attackdist = 5.0f;
		std::queue<ObjPtr<GameObject>> Castleballs;
		ObjPtr<GameObject> enemyTarget;
		void PointUp(int t);
		void CheckEnemy();
		void AutoAttack();
		void AutoHeal();
		void ReturnBall(ObjPtr<GameObject> obj);
		void Dead();
		void LevelUp();
		void Level5Apply(int value);
		void Level10Apply(int value);
	};
}