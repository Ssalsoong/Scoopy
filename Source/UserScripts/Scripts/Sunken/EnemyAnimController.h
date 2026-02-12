#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimResourceManager.h"
#include "../test/EnemyController.h"

namespace MMMEngine { class Animator; }

namespace MMMEngine
{
	class USERSCRIPTS EnemyAnimController : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND
		
		AnimType mAnimType = AnimType::AT_END;
		EnemyController::EnemyState prevStat = EnemyController::EnemyState::Dead;

		ObjPtr<AnimResourceManager> mAnimManager;
		ObjPtr<EnemyController> mEnemy;

		void UpdateArcher();
		void UpdateWarrior();
		void UpdateScout();
	public:
		EnemyAnimController()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

		
		USCRIPT_PROPERTY()
		ObjPtr<Animator> mAnimator;


		USCRIPT_PROPERTY()
		float mAnimSpeed = 1.0f;

		USCRIPT_PROPERTY()
			int AnimSize = 0;

		USCRIPT_MESSAGE()
			void Start();

		USCRIPT_MESSAGE()
			void Update();
	};
}
