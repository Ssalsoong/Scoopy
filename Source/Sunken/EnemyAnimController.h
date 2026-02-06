#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

#include "AnimResourceManager.h"
#include "Animator.h"

namespace MMMEngine
{
    class USERSCRIPTS EnemyAnimController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

        float animSpeed = 1.0f;
    public:
        EnemyAnimController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_MESSAGE()
        ObjPtr<AnimResourceManager> mAnimManager;
        USCRIPT_MESSAGE()
        ObjPtr<Animator> mAnimator;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

    };
}
