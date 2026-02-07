#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "SimpleMath.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class USERSCRIPTS Player : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
		Player()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		USCRIPT_MESSAGE()
		void Start();
		USCRIPT_MESSAGE()
		void Update();
		USCRIPT_PROPERTY()
		int level = 0;
		USCRIPT_PROPERTY()
		int maxHP = 100;
		int GetmaxHP() const { return maxHP; }
		USCRIPT_PROPERTY()
		float battledist = 0.7f;
		USCRIPT_PROPERTY()
		int atk = 15;
		float attackDelay = 0.65f;
		USCRIPT_PROPERTY()
		int maxpoint = 10;
		int Getmaxpoint() const { return maxpoint; };
		int healHP = 10;
		float healDelay = 1.0f;
		float nonfightDelay = 10.0f;
		float damageDelay = 0.1f;
		int prevHP = 100;
		float attackTimer = 0.0f;
		float healTimer = 0.0f;
		float nonfightTimer = 0.0f;
		bool fighting = false;
		float damageTimer = 0.0f;
		bool isMoving = false;
		bool buildchance = true;
		void Setbuildchance(bool value) { buildchance = value; }

		DirectX::SimpleMath::Vector3 pos;
		void AutoHeal();
		void HandleAttack();

		void BuildOn();
		void LevelUp();

		void CalDamageDelay();
		void GetDamage(int t);
		void Dead();
    };
}
