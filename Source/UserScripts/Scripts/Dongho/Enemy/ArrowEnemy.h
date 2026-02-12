#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"
#include "Prefab.h"
#include "SimpleMath.h"

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
		float velocity = 1.0f;
		float attackDelay = 0.4f;
		float battledist = 2.0f;
		float checkdist = 4.0f;

        std::queue<ObjPtr<GameObject>> Arrows;
        ResPtr<Prefab> pre_arrow;
        ObjPtr<GameObject> target;
        void ArrowAttack(ObjPtr<GameObject> target, int atk);
        void LookAt(const DirectX::SimpleMath::Vector3& target);
        void ReturnArrow(ObjPtr<GameObject> obj);
    };
}
