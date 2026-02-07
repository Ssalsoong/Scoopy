#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimResourceManager.h"
#include "../Dongho/Enemy/Enemy.h"

namespace MMMEngine { class ThiefEnemy; }

namespace MMMEngine { class NormalEnemy; }

namespace MMMEngine { class ArrowEnemy; }

namespace MMMEngine { class Animator; }

namespace MMMEngine
{
	class USERSCRIPTS EnemyAnimController : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
			RTTR_REGISTRATION_FRIEND
		
		AnimType mAnimType = AnimType::AT_END;
		Enemy::EnemyState prevStat = Enemy::EnemyState::Dead;

		ObjPtr<AnimResourceManager> mAnimManager;

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
			ObjPtr<Enemy> mEnemy;

		USCRIPT_PROPERTY()
		float mAnimSpeed = 1.0f;

		ObjPtr<ArrowEnemy> mArcher;
		ObjPtr<NormalEnemy> mWarrior;
		ObjPtr<ThiefEnemy> mScout;

		USCRIPT_PROPERTY()
			int AnimSize = 0;

		USCRIPT_MESSAGE()
			void Start();

		USCRIPT_MESSAGE()
			void Update();
	};
}
