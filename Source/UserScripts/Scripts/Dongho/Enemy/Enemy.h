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

		bool HitByPlayer = false;
		float attackTimer = 0.0f;

		USCRIPT_PROPERTY()
		int atk = 0;
		USCRIPT_PROPERTY()
		float attackDelay = 0.f;

		void AttackTarget(ObjPtr<GameObject> target);
		void PlayerHitMe();
		void Dead();

		float snowDamageTimer = 0.0f;
		float snowDamageDelay = 0.5f;
		void CalSnowDamageDelay();
		bool ApplySnowDamage();
	};
}

