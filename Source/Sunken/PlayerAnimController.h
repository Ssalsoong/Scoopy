#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    enum PlayerStatus : int {
        P_IDLE = 0,
        P_MOVE,
        P_ATTACK,
        P_SCOOPIDLE,
        P_SCOOPMOVE,
        P_SCOOPATTACK,
        P_END
    };

    class Animator;
    class USERSCRIPTS PlayerAnimController : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        PlayerAnimController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(PlayAnimation);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

		USCRIPT_PROPERTY()
			ObjPtr<Animator> mAnimator;

		USCRIPT_PROPERTY()
			int animCount = 0;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();

        USCRIPT_MESSAGE()
        void PlayAnimation(PlayerStatus _status);
    };
}
