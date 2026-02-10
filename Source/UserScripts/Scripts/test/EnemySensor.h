#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "ColliderComponent.h"
#include "PhysxManager.h"

namespace MMMEngine
{
    class SphereColliderComponent;
    class USERSCRIPTS EnemySensor : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        EnemySensor()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerEnter);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerExit);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        USCRIPT_MESSAGE()
        void Start();


        USCRIPT_MESSAGE()
        void OnTriggerEnter(MMMEngine::CollisionInfo info);


        USCRIPT_MESSAGE()
        void OnTriggerExit(MMMEngine::CollisionInfo info);

        
        ObjPtr<SphereColliderComponent> ColObj;

        std::unordered_set<ObjPtr<GameObject>> TargetList;

        ObjPtr<GameObject> MainTarget;

        ObjPtr<GameObject> GetTarget();

        int GetPriority(const ObjPtr<GameObject> obj);
    };
}
