#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"

namespace MMMEngine
{
    class USERSCRIPTS ArrowEnemy : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ArrowEnemy()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Start();
        void Update();
		int HP = 15;
		int atk = 2;
		float velocity = 8.0f;
		float attackDelay = 0.4f;
		float battledist = 2.0f;
		float checkdist = 4.0f;
        std::queue<ObjPtr<GameObject>> Arrows;
        ResPtr<StaticMesh> arrowmesh;
        ObjPtr<GameObject> target;
        void ApplyStats();
        void ArrowAttack(ObjPtr<GameObject> target);
        void ReturnArrow(ObjPtr<GameObject> obj);
    };
}
