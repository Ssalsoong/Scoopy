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
		int HP = 45;
		int atk = 4;
		float velocity = 8.0f;
		float attackDelay = 0.65f;
		float battledist = 0.5f;
		float checkdist = 2.5f;

		void ApplyStats();
	};
}
