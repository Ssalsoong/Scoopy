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
		int HP = 20;
		int atk = 5;
		float velocity = 18.0f;
		float attackDelay = 0.5f;
		float battledist = 1.f;
		float checkdist = 12.0f;

		void ApplyStats();
    };
}
