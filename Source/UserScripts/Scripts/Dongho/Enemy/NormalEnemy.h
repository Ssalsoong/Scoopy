#pragma once
#include "ScriptBehaviour.h"
#include "rttr/type"
#include "UserScriptsCommon.h"

namespace MMMEngine {
	class EnemySpawner;
	class USERSCRIPTS NormalEnemy : public ScriptBehaviour
	{
	private:
		RTTR_ENABLE(ScriptBehaviour)
		RTTR_REGISTRATION_FRIEND
	public:
		NormalEnemy()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		void Start();
		void Update();
		int HP = 30;
		int atk = 2;
		float velocity = 13.0f;
		float attackDelay = 0.65f;
		float battledist = 1.7f;
		float checkdist = 12.0f;

		void ApplyStats();
	};
}
