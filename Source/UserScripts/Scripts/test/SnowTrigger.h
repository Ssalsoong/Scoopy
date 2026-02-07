#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <SimpleMath.h>
#include "PhysxManager.h"


namespace MMMEngine
{
    class USERSCRIPTS SnowTrigger : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        SnowTrigger()
        {
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerEnter);
        REGISTER_BEHAVIOUR_MESSAGE(OnTriggerExit);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        void SetTriggerSize(float size);

        ObjPtr<ColliderComponent> ColObj;

        USCRIPT_MESSAGE()
        void OnTriggerEnter(MMMEngine::CollisionInfo info);

        USCRIPT_MESSAGE()
        void OnTriggerExit(MMMEngine::CollisionInfo info);

        ObjPtr<GameObject> Parent_Obj;

        bool is_player = false;
        ObjPtr<GameObject> main_Player;

        void SetParentPtr(ObjPtr<GameObject> mamaoooh);

        void DestoryTrigger();
    };
}
