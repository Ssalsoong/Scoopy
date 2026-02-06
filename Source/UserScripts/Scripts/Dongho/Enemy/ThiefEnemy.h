#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class USERSCRIPTS ThiefEnemy : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ThiefEnemy()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Start();

        void Update();
		int HP = 30;
		int atk = 3;
		float velocity = 8.0f;
		float attackDelay = 0.65f;
		float battledist = 0.5f;
		float checkdist = 2.5f;

		void ApplyStats();
    };
}
