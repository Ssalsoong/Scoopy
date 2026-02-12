#pragma once
#include "ScriptBehaviour.h"
#include "rttr/type"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"
#include "SimpleMath.h"
#include "Prefab.h"
#include "AnimationCurve.h"
#include "CoreComponents.h"

namespace MMMEngine {
	class USERSCRIPTS Building : public ScriptBehaviour
	{
		enum ANIMTYPE : int {
			SPAWN = 0,
			LEVEL = 1,
			END
		};

	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND


		void UpdateSpawnAnim();
		void UpdateLevelAnim();
	public:
		Building()
		{
        REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
        REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		DirectX::SimpleMath::Vector3 pos;
		void Start();
		void Update();


		void OnEnable();
		void OnDisable();

		USCRIPT_PROPERTY()
			AnimationCurve mSpawnScaleCurve;
		USCRIPT_PROPERTY()
			AnimationCurve mLevelScaleCurve;

		ObjPtr<Transform> mMeshTrans;

		bool isAnimating = false;
		float mElipsedTime = 0.0f;
		float mCurveScale;
		Vector3 mInitScale;
		ANIMTYPE mAnimType = ANIMTYPE::END;

		int maxHP = 70;
		int point = 0;
		int exp = 0;
		int atk = 13;
		int level = 0;
		int prevLevel = 0;
		float attackDelay = 1.0f;
		float attackTimer = 0.0f;
		float attackdist = 3.0f;
		bool isDead = false;
		ResPtr<Prefab> pre_bullet;
		float bulletSpeed = 6.0f;
		float bulletsize = 0.2f;
		std::queue<ObjPtr<GameObject>> Buildingballs;
		ObjPtr<GameObject> enemyTarget;
		void PointUp(int t);
		void Dead();
		void CheckEnemy();
		void AutoAttack();
		void ReturnBall(ObjPtr<GameObject> obj);

		void SetAttackDist(float value) { attackdist = value; }
	};
}
