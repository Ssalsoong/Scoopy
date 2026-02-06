#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "AnimatorController.h"

namespace MMMEngine { class AnimResourceManager; }

namespace MMMEngine { class Animator; }

namespace MMMEngine
{
    class USERSCRIPTS PlayerAnimController : public ScriptBehaviour
    {
    private:
        enum PSTAT : int {
            IDLE = 0,
            MOVE,
            ATTACK,
            MOVEATTACK,
            SCOOPIDLE,
            SCOOPMOVE,
            END
        };

    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND

        ObjPtr<AnimResourceManager> mAnimManager;
        
        float mAtkElipsedTime = 0.0f;
        bool isAtking = false;
        float mMoveSpeed = 0.0f;

        PSTAT mCurrStat = PSTAT::IDLE;

        void CreateAnimNode();
        void CreateParams();
        void CreateTrans();
    public:
        PlayerAnimController()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        USCRIPT_PROPERTY()
            float mIdleSpeed = 0.01f;

        USCRIPT_PROPERTY()
            float mAnimSpeed = 1.0f;

		USCRIPT_PROPERTY()
			int mAnimSize = 0;

		USCRIPT_PROPERTY()
			ObjPtr<Animator> mAnimator;
		USCRIPT_PROPERTY()
			ObjPtr<AnimatorController> mAnimController;

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_MESSAGE()
        void Update();
        
        // 애니메이션 속도
        USCRIPT_PROPERTY()
            void SetAnimSpeed(float _speed);

        // 현재 Velocity 입력 (Update로 입력)
		USCRIPT_PROPERTY()
			void SetMoveSpeed(float _speed);

        // 현재 공격상태 입력
        USCRIPT_PROPERTY()
            void SetAttack(bool _isAttacking);

        // 공격재생 (분리형)
		USCRIPT_PROPERTY()
			void PlayAttack();
    };
}
